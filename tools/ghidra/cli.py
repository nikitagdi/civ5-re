"""civ5re ghidra rebuild|apply|verify|functions|decomp|paramid."""

import json
import os
import re
import shutil
import subprocess
import sys
import time
from datetime import datetime
from pathlib import Path

from civ5re import binlock, config

from . import decomp as decompmod, gui, headless, plan as planmod, verify as verifymod

EXIT = {"ok": 0, "skipped": 0, "conflicts": 1, "failed": 2}
TARGET_SUFFIXES = (".h", ".inl")


class InputError(Exception):
    """Something the command needs is missing or stale: nothing was applied (exit code 2)."""

    def __init__(self, message, hint=None):
        super().__init__(message)
        self.hint = hint

    def full(self):
        return f"{self.args[0]}: {self.hint}" if self.hint else str(self.args[0])


def iso_now():
    return datetime.now().astimezone().isoformat(timespec="seconds")


def _git(repo, *args):
    try:
        p = subprocess.run(["git", "-C", str(repo), *args], capture_output=True, text=True, timeout=30)
    except (OSError, subprocess.SubprocessError):
        return None
    return p.stdout.strip() if p.returncode == 0 else None


def git_head(repo):
    return _git(repo, "rev-parse", "--short=12", "HEAD")


def git_dirty(repo):
    names = _git(repo, "diff", "--name-only") or ""
    return any(n.strip().endswith(TARGET_SUFFIXES) for n in names.splitlines())


def rebase_in_progress(repo):
    gd = _git(repo, "rev-parse", "--git-dir")
    if not gd:
        return False
    gd = Path(gd) if os.path.isabs(gd) else Path(repo) / gd
    return (gd / "rebase-merge").exists() or (gd / "rebase-apply").exists()


class Ctx:
    def __init__(self, cfg, headless_factory=None, ensure_inputs=None, gui_mod=None):
        self.cfg = cfg
        self.repo = Path(cfg.root)
        self.build = self.repo / "build"
        self.gdir = self.build / "ghidra"
        self.typedb_json = self.build / "typedb.json"
        self.binfacts_json = self.build / "binfacts.json"
        self.functions_txt = self.build / "ghidra-functions.txt"
        self.plan_json = self.gdir / "plan.json"
        self.state_json = self.gdir / "state.json"
        self.report_json = self.gdir / "apply-report.json"
        self.dry_report_json = self.gdir / "apply-report.dry.json"
        self.last_apply = self.gdir / "last-apply.json"
        self.logs = self.gdir / "logs"
        self.decomp_dir = self.build / "decomp"
        self.ensure_inputs = ensure_inputs or default_ensure_inputs
        self._headless_factory = headless_factory
        self.gui = gui_mod or gui

    @property
    def project_dir(self):
        p = self.cfg.path("ghidra_project_dir")
        if p is None:
            raise InputError("ghidra_project_dir is not set", "set it in civ5re.local.cfg")
        return p

    @property
    def ghidra_dir(self):
        p = self.cfg.path("ghidra_dir")
        if p is None:
            raise InputError("ghidra_dir is not set", "set it in civ5re.local.cfg")
        return p

    def headless(self):
        if self._headless_factory:
            return self._headless_factory(self)
        return headless.Headless(self.ghidra_dir, self.cfg.get("ghidra", "heap"), self.repo, self.logs)

    def target(self):
        try:
            return binlock.target(binlock.load(self.repo / "bins.lock"))
        except (binlock.BinlockError, OSError) as e:
            raise InputError(f"bins.lock: {e}") from None

    def binary_path(self):
        bin_dir = self.cfg.path("bin_dir")
        if bin_dir is None:
            raise InputError("bin_dir is not set", "set it in civ5re.local.cfg")
        return bin_dir / self.target().path

    def script(self, name):
        return headless.script_path(self.repo, name)


def default_ensure_inputs(ctx):
    try:
        from binfacts import api as bf_api
    except ImportError:
        raise InputError("the binfacts module is not available") from None
    try:
        bf_api.load()
    except Exception as e:
        raise InputError(f"binfacts: {e}") from None
    try:
        from typedb import api as tdb_api
    except ImportError:
        raise InputError("the typedb module is not available") from None
    try:
        tdb_api.build_db(ctx.repo / "Civ5Expansion2", ctx.build, markers="strict")
    except Exception as e:
        raise InputError(f"typedb: {e}") from None
    check_typedb_ok(ctx)


def check_typedb_ok(ctx):
    if not ctx.typedb_json.is_file() or not ctx.binfacts_json.is_file():
        raise InputError("build/typedb.json or build/binfacts.json is missing")
    db = json.loads(ctx.typedb_json.read_text(encoding="utf-8"))
    if not db.get("ok", True):
        errs = [d for d in db.get("diagnostics", []) if d.get("severity") == "error"]
        first = errs[0] if errs else {}
        raise InputError(f"typedb has {len(errs)} error(s), first: {first.get('file')}:{first.get('line')} "
                         f"{first.get('code')}", "fix them: civ5re typedb")


def script_constant(path, name):
    try:
        m = re.search(rf"\b{name}\s*=\s*\"?([^\";\s]+)\"?\s*;", Path(path).read_text(encoding="utf-8"))
    except OSError:
        return None
    return m.group(1) if m else None


def check_apply_script(ctx):
    p = ctx.script("Civ5reApply.java")
    if not p.is_file():
        raise InputError(f"{p} not found")
    v = script_constant(p, "SCRIPT_VERSION")
    if v is not None and v != str(planmod.SCRIPT_VERSION):
        raise InputError(f"Civ5reApply.java SCRIPT_VERSION {v} != plan.py {planmod.SCRIPT_VERSION}")


def make_plan(ctx):
    if not ctx.functions_txt.is_file():
        ensure_functions(ctx)
    try:
        plan = planmod.build_from_files(ctx.typedb_json, ctx.binfacts_json, ctx.functions_txt,
                                        ctx.target().sha256)
        planmod.assert_valid(plan)
    except planmod.PlanError as e:
        raise InputError(str(e)) from None
    planmod.write(plan, ctx.plan_json)
    return plan


def read_json(path):
    try:
        return json.loads(Path(path).read_text(encoding="utf-8"))
    except (OSError, json.JSONDecodeError):
        return None


def write_json(path, obj):
    path = Path(path)
    path.parent.mkdir(parents=True, exist_ok=True)
    tmp = path.with_name(path.name + ".tmp")
    tmp.write_text(json.dumps(obj, indent=1, sort_keys=True, ensure_ascii=False) + "\n", encoding="utf-8",
                   newline="\n")
    os.replace(tmp, path)


# ---- ghidra-functions.txt -------------------------------------------------------------------------------------

def export_version(ctx):
    return script_constant(ctx.script("Civ5reExport.java"), "VERSION")


def ensure_functions(ctx):
    """build/ghidra-functions.txt from the working project's cache; re-exports when the list is old."""
    meta = headless.read_meta(ctx.project_dir)
    if not meta or not meta.get("cache_key"):
        raise InputError("no working project (civ5re.json missing)", "run: civ5re ghidra rebuild")
    k12 = meta["cache_key"][:12]
    src = headless.cache_functions(ctx.project_dir, k12)
    want = export_version(ctx)
    have = None
    if src.is_file():
        with open(src, encoding="utf-8") as f:
            f.readline()
            m = re.search(r"\bexport=(\S+)", f.readline())
            have = m.group(1) if m else None
    if not src.is_file() or (want is not None and have != want):
        if not headless.cache_exists(ctx.project_dir, k12):
            raise InputError(f"the analysis cache pristine-{k12} is missing", "run: civ5re ghidra rebuild")
        res = ctx.headless().export_functions(ctx.project_dir, k12)
        if not res.ok or not src.is_file():
            raise InputError(f"function list export failed (exit {res.code})", f"see {res.log}")
    ctx.functions_txt.parent.mkdir(parents=True, exist_ok=True)
    tmp = ctx.functions_txt.with_name(ctx.functions_txt.name + ".tmp")
    shutil.copyfile(src, tmp)
    os.replace(tmp, ctx.functions_txt)
    return ctx.functions_txt


# ---- apply --------------------------------------------------------------------------------------------------

def _record(ctx, dry_run, hook):
    last = read_json(ctx.last_apply) or {}
    return {"time": iso_now(), "trigger": "hook" if hook else "manual", "commit": git_head(ctx.repo),
            "dirty": git_dirty(ctx.repo), "route": None, "status": "failed", "plan_sha256": None,
            "changes": 0, "conflicts": 0, "divergences": 0, "seconds": 0.0, "message": "", "dry_run": dry_run,
            "applied_plan_sha256": last.get("applied_plan_sha256")}


def collect(ctx, plan, dry_run, rec, res_log=None):
    """Reads the report (and state.json after a real apply) into rec. Returns the divergence list."""
    sha = plan["header"]["plan_sha256"]
    rp = ctx.dry_report_json if dry_run else ctx.report_json
    report = read_json(rp)
    if report is None:
        hint = f" (see {res_log})" if res_log else ""
        rec.update(status="failed", message=f"Ghidra produced no {rp.name}{hint}")
        return None
    try:
        changes, rconf, aborted = verifymod.check_report(report, sha)
    except verifymod.VerifyError as e:
        rec.update(status="failed", message=str(e))
        return None
    rec["changes"] = changes
    rec["conflicts"] = rconf + len(plan["conflicts"])
    if aborted:
        rec.update(status="failed", message=f"apply aborted, nothing changed: {aborted}")
        return None
    divs = []
    if not dry_run:
        state = read_json(ctx.state_json)
        if state is None:
            rec.update(status="failed", message="state.json missing after apply")
            return None
        try:
            divs = verifymod.verify(plan, state)
        except verifymod.VerifyError as e:
            rec.update(status="failed", message=str(e))
            return None
    rec["divergences"] = len(divs)
    if rec["conflicts"] or divs:
        rec["status"] = "conflicts"
        rec["message"] = f"{rec['conflicts']} conflicts, {len(divs)} divergences"
    else:
        rec["status"] = "ok"
        rec["message"] = ("dry run: %d changes would be made" % changes) if dry_run else \
            f"applied, {changes} changes, verified"
    return divs


def run_apply(ctx, dry_run=False, prune_all=False, hook=False, out=print):
    t0 = time.monotonic()
    rec = _record(ctx, dry_run, hook)
    divs = []
    try:
        if hook and rebase_in_progress(ctx.repo):
            rec.update(status="skipped", message="a rebase is in progress")
            return _finish(ctx, rec, divs, t0, out)
        ctx.ensure_inputs(ctx)
        check_typedb_ok(ctx)
        plan = make_plan(ctx)
        rec["plan_sha256"] = plan["header"]["plan_sha256"]
        if hook and not dry_run and rec["applied_plan_sha256"] == rec["plan_sha256"]:
            rec.update(status="skipped", message="the plan is unchanged since the last successful apply")
            return _finish(ctx, rec, divs, t0, out)
        check_apply_script(ctx)
        project_dir = ctx.project_dir
        if headless.is_open(project_dir):
            rec["route"] = "gui"
            port = ctx.gui.find_instance(int(ctx.cfg.get("ghidra", "mcp_port")), str(project_dir))
            if port is None:
                raise gui.GuiError("the project is open (civ5re.lock) but no plugin instance serves Civ5XP "
                                   "(a stale lock after a crash?)", gui.HINT_NO_INSTANCE)
            _drop_reports(ctx, dry_run)
            ctx.gui.run_script(port, ctx.script("Civ5reApply.java"), ctx.repo, dry_run, prune_all)
            divs = collect(ctx, plan, dry_run, rec)
        else:
            rec["route"] = "headless"
            if not headless.project_exists(project_dir):
                raise InputError("no working project", "run: civ5re ghidra rebuild")
            headless.check_ghidra(ctx.ghidra_dir)
            _drop_reports(ctx, dry_run)
            res = ctx.headless().apply(project_dir, dry_run=dry_run, prune_all=prune_all)
            divs = collect(ctx, plan, dry_run, rec, res.log)
            if divs is None and res.code != 0:
                rec["message"] += f"; headless exit {res.code}"
    except (InputError, gui.GuiError, headless.HeadlessError, config.ConfigError) as e:
        rec.update(status="failed", message=e.full() if hasattr(e, "full") else str(e))
    return _finish(ctx, rec, divs or [], t0, out)


def _drop_reports(ctx, dry_run):
    p = ctx.dry_report_json if dry_run else ctx.report_json
    if p.exists():
        p.unlink()


def _finish(ctx, rec, divs, t0, out):
    rec["seconds"] = round(time.monotonic() - t0, 1)
    if rec["status"] == "ok" and not rec["dry_run"]:
        rec["applied_plan_sha256"] = rec["plan_sha256"]
    write_json(ctx.last_apply, rec)
    out(f"ghidra apply{' --dry-run' if rec['dry_run'] else ''}: {rec['status']} ({rec['route'] or '-'}) "
        f"{rec['message']} [{rec['seconds']} s]")
    for line in verifymod.format_divergences(divs or [], 20):
        out("  " + line)
    return rec


def cmd_apply(args, ctx=None):
    ctx = ctx or Ctx(config.load())
    rec = run_apply(ctx, args.dry_run, args.prune_all, args.hook)
    return EXIT[rec["status"]]


# ---- verify -------------------------------------------------------------------------------------------------

def cmd_verify(args, ctx=None, out=print):
    ctx = ctx or Ctx(config.load())
    plan, state = read_json(ctx.plan_json), read_json(ctx.state_json)
    if plan is None or state is None:
        out("ghidra verify: build/ghidra/plan.json or state.json is missing: run civ5re ghidra apply")
        return 2
    try:
        divs = verifymod.verify(plan, state)
    except verifymod.VerifyError as e:
        out(f"ghidra verify: {e}")
        return 2
    out(f"ghidra verify: {len(divs)} divergences")
    for k, n in verifymod.summary(divs).items():
        out(f"  {k}: {n}")
    for line in verifymod.format_divergences(divs):
        out("  " + line)
    return 1 if divs else 0


# ---- functions / paramid ---------------------------------------------------------------------------------------

def cmd_functions(args, ctx=None, out=print):
    ctx = ctx or Ctx(config.load())
    try:
        p = ensure_functions(ctx)
    except (InputError, headless.HeadlessError) as e:
        out(f"ghidra functions: {e.full() if hasattr(e, 'full') else e}")
        return 2
    out(f"ghidra functions: {p}")
    return 0


def cmd_paramid(args, ctx=None, out=print):
    ctx = ctx or Ctx(config.load())
    try:
        d = ctx.project_dir
        if not headless.project_exists(d):
            raise InputError("no working project", "run: civ5re ghidra rebuild")
        if headless.is_open(d):
            raise InputError("the working project is open (civ5re.lock)", "close it in Ghidra")
        headless.check_ghidra(ctx.ghidra_dir)
    except (InputError, headless.HeadlessError) as e:
        out(f"ghidra paramid: {e.full() if hasattr(e, 'full') else e}")
        return 2
    out("ghidra paramid: Decompiler Parameter ID on the whole program, this is slow")
    res = ctx.headless().paramid(d)
    out(f"ghidra paramid: exit {res.code} in {res.seconds:.0f} s, log {res.log}")
    return 0 if res.ok else 2


# ---- decomp -------------------------------------------------------------------------------------------------

LOCK_WAIT = 60.0


def wait_unlocked(project_dir, timeout, sleep=time.sleep, step=1.0):
    """Waits for civ5re.lock to go away (a running headless apply); seconds waited, None on timeout."""
    waited = 0.0
    while headless.is_open(project_dir):
        if waited >= timeout:
            return None
        sleep(step)
        waited += step
    return waited


def run_decomp(ctx, selectors, classes, force=False, out=print, lock_wait=LOCK_WAIT, sleep=time.sleep):
    t0 = time.monotonic()
    if not selectors and not classes:
        raise InputError("no functions selected", "give addresses or names, --list FILE or --class NAME")
    facts = read_json(ctx.binfacts_json)
    if facts is None:
        raise InputError("build/binfacts.json is missing", "run civ5re binfacts")
    try:
        addrs = decompmod.select(facts["symbols"], selectors, classes)
    except decompmod.SelectError as e:
        raise InputError(str(e)) from None
    applied = (read_json(ctx.last_apply) or {}).get("applied_plan_sha256")
    out_dir = ctx.decomp_dir
    todo, fresh = decompmod.split_fresh(out_dir, addrs, applied, force)
    dumped = 0
    if todo:
        d = ctx.project_dir
        if not headless.project_exists(d):
            raise InputError("no working project", "run: civ5re ghidra rebuild")
        hl = ctx.headless()
        try:
            held = hl.lock()
            held.__enter__()
        except headless.projlock.LockTimeout as e:
            raise InputError(f"the headless lock is busy: {e}", "wait for the other civ5re ghidra run") from None
        try:
            waited = wait_unlocked(d, lock_wait, sleep)
            if waited is None:
                raise InputError(f"the working project is locked (civ5re.lock) for over {lock_wait:.0f} s: "
                                 "Ghidra holds it", "close it, or use the MCP tools")
            headless.check_ghidra(ctx.ghidra_dir)
            req = out_dir / ".request.txt"
            decompmod.write_request(req, todo, decompmod.symbol_sizes(facts["symbols"]))
            run_file = out_dir / decompmod.RUN_FILE
            if run_file.exists():
                run_file.unlink()
            res = hl.decomp(d, req, out_dir, applied)
            rows = decompmod.read_run(out_dir)
        finally:
            held.__exit__(None, None, None)
        if rows is None or not res.ok:
            raise InputError(f"decomp failed (exit {res.code}) {res.script_error or ''}".strip(), f"see {res.log}")
        dumped, missing = decompmod.merge_run(out_dir, rows, applied)
        if missing:
            out("decomp: not a function in Ghidra: " + ", ".join("0x" + a for a in missing))
    out(f"decomp: {dumped} dumped, {len(fresh)} fresh [{time.monotonic() - t0:.1f} s] -> {out_dir}")
    return 0


def cmd_decomp(args, ctx=None, out=print):
    ctx = ctx or Ctx(config.load())
    try:
        sels = list(args.selectors)
        if args.list:
            sels += decompmod.read_list(args.list)
        return run_decomp(ctx, sels, args.cls or [], args.force, out)
    except (InputError, headless.HeadlessError, config.ConfigError, OSError) as e:
        out(f"ghidra decomp: {e.full() if hasattr(e, 'full') else e}")
        return 2


# ---- rebuild ------------------------------------------------------------------------------------------------

class Stages:
    def __init__(self, ctx, out):
        self.out = out
        ctx.logs.mkdir(parents=True, exist_ok=True)
        self.path = ctx.logs / (time.strftime("%Y%m%d-%H%M%S") + "-rebuild.log")
        self.times = []

    def run(self, name, fn):
        t0 = time.monotonic()
        self.out(f"== {name}")
        try:
            return fn()
        finally:
            dt = time.monotonic() - t0
            self.times.append((name, dt))
            with open(self.path, "a", encoding="utf-8") as f:
                f.write(f"stage {name}: {dt:.1f} s\n")
            self.out(f"   {name}: {dt:.1f} s")


def run_rebuild(ctx, yes=False, no_cache=False, out=print):
    st = Stages(ctx, out)
    rec = _record(ctx, False, False)
    rec["route"] = "headless"
    t0 = time.monotonic()
    divs = []
    replaced = False
    try:
        d = ctx.project_dir
        info = headless.check_ghidra(ctx.ghidra_dir)
        if headless.is_open(d):
            raise InputError("the working project is open (civ5re.lock)", "close it in Ghidra")
        if headless.project_exists(d) and not yes:
            raise InputError("the working project exists; rebuild would replace it (the old one is kept as "
                             "civ5re.prev)", "re-run with --yes")
        target = ctx.target()
        binary = ctx.binary_path()
        sha = st.run("verify binary", lambda: binlock.sha256_of(binary) if binary.is_file() else None)
        if sha != target.sha256:
            raise InputError(f"{binary} is missing or differs from bins.lock")
        if not ctx.binfacts_json.is_file():
            try:
                ctx.ensure_inputs(ctx)
            except InputError:
                pass
        if not ctx.binfacts_json.is_file():
            raise InputError("build/binfacts.json is missing (needed for the function seeds)",
                             "run civ5re binfacts")
        seeds = headless.function_seeds(json.loads(ctx.binfacts_json.read_text(encoding="utf-8")))
        key = headless.cache_key(sha, ctx.ghidra_dir, ctx.repo, seeds)
        k12 = headless.key12(key)
        hl = ctx.headless()
        if no_cache or not headless.cache_exists(d, k12):
            res = st.run("cache (import + seeds + full analysis + Param ID)",
                         lambda: hl.build_cache(d, k12, binary, force=no_cache, seeds_text=seeds))
            if not res.ok or not headless.cache_exists(d, k12) or not headless.cache_functions(d, k12).is_file():
                raise InputError(f"cache build failed (exit {res.code}) {res.script_error or ''}".strip(),
                                 f"see {res.log}")
        else:
            out(f"== cache pristine-{k12} found, no import")
        if not headless.cache_functions(d, k12).is_file():
            res = st.run("function list export", lambda: hl.export_functions(d, k12))
            if not res.ok:
                raise InputError(f"function list export failed (exit {res.code})", f"see {res.log}")
        ctx.functions_txt.parent.mkdir(parents=True, exist_ok=True)
        shutil.copyfile(headless.cache_functions(d, k12), ctx.functions_txt)
        try:
            st.run("inputs", lambda: ctx.ensure_inputs(ctx))
            check_typedb_ok(ctx)
        except InputError as e:
            out(f"   {e.full()}")
            rec.update(status="failed", plan_sha256=None,
                       message="typedb not available: cache ready, plan/apply skipped")
            return _finish(ctx, rec, [], t0, out)
        plan = st.run("plan", lambda: make_plan(ctx))
        rec["plan_sha256"] = plan["header"]["plan_sha256"]
        check_apply_script(ctx)
        rb = d / headless.REBUILD_DIR
        if rb.exists():
            shutil.rmtree(rb)
        st.run("copy cache", lambda: headless.copy_project(headless.cache_dir(d), headless.cache_name(k12),
                                                          rb, headless.PROJECT))
        _drop_reports(ctx, False)
        _drop_reports(ctx, True)
        res = st.run("apply + dry run", lambda: hl.apply(rb, both=True, stage="rebuild-apply"))
        divs = collect(ctx, plan, False, rec, res.log)
        if divs is not None:
            drec = dict(rec)
            dd = collect(ctx, plan, True, drec, res.log)
            if dd is None:
                rec.update(status="failed", message="dry run: " + drec["message"])
            elif drec["changes"]:
                rec.update(status="failed", message=f"the repeated dry run still makes {drec['changes']} changes "
                           "(apply is not idempotent)")
            elif not divs:
                st.run("replace project", lambda: headless.replace_project(d, {
                    "cache_key": key, "key12": k12, "binary_sha256": sha, "ghidra": info,
                    "plan_sha256": rec["plan_sha256"], "time": iso_now()}))
                replaced = True
                rec["applied_plan_sha256"] = rec["plan_sha256"]
                if rec["status"] == "ok":
                    rec["message"] = f"rebuilt from cache {k12}: {rec['changes']} changes, 0 divergences"
        if not replaced:
            out(f"the working project is untouched; {rb} is kept for analysis")
    except (InputError, gui.GuiError, headless.HeadlessError, config.ConfigError) as e:
        rec.update(status="failed", message=e.full() if hasattr(e, "full") else str(e))
    return _finish(ctx, rec, divs or [], t0, out)


def cmd_rebuild(args, ctx=None):
    ctx = ctx or Ctx(config.load())
    rec = run_rebuild(ctx, args.yes, args.no_cache)
    return EXIT[rec["status"]]


# ---- registration --------------------------------------------------------------------------------------------

def register(subparsers):
    p = subparsers.add_parser("ghidra", help="Ghidra project: rebuild, apply, verify, functions, paramid")
    sub = p.add_subparsers(dest="ghidra_command", metavar="<action>")

    r = sub.add_parser("rebuild", help="cache -> function list -> plan -> apply -> dry run -> verify -> replace")
    r.add_argument("--yes", action="store_true", help="replace an existing working project")
    r.add_argument("--no-cache", action="store_true", help="redo the full analysis")
    r.set_defaults(func=cmd_rebuild)

    a = sub.add_parser("apply", help="plan -> open Ghidra (plugin HTTP) or headless -> verify")
    a.add_argument("--dry-run", action="store_true")
    a.add_argument("--prune-all", action="store_true", help="allow deleting more stale items than the limit")
    a.add_argument("--hook", action="store_true", help="post-commit mode: skip an unchanged plan and rebases")
    a.set_defaults(func=cmd_apply)

    v = sub.add_parser("verify", help="state.json against plan.json")
    v.set_defaults(func=cmd_verify)

    f = sub.add_parser("functions", help="build/ghidra-functions.txt from the working project's cache")
    f.set_defaults(func=cmd_functions)

    d = sub.add_parser("decomp", help="asm + decompiler C of chosen functions to build/decomp/<addr>.txt (headless)")
    d.add_argument("selectors", nargs="*", metavar="ADDR_OR_NAME", help="0x... address, exact name or regex")
    d.add_argument("--list", metavar="FILE", help="file with one selector per line")
    d.add_argument("--class", dest="cls", action="append", metavar="NAME", help="all functions of Class::")
    d.add_argument("--force", action="store_true", help="dump again even when fresh")
    d.set_defaults(func=cmd_decomp)

    pi = sub.add_parser("paramid",help="Decompiler Parameter ID again on the working project (slow)")
    pi.set_defaults(func=cmd_paramid)
