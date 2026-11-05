"""Runs rules: lazy inputs, per-rule try/except, staged scoping, waivers, exit code."""
import time
import traceback
from pathlib import Path

from . import plugins, staged
from .api import ERROR, WARN, Finding, InputError, Rule, RuleSkip
from .context import Context

NOT_AFFECTED = "not affected by the change"


class Result:
    def __init__(self, mode):
        self.mode = mode
        self.exit = 0
        self.seconds = 0.0
        self.inputs = {"binfacts_sha256": None, "typedb": None, "ghidra_functions": None,
                       "clang": None, "plugins": []}
        self.selftest = {"status": "skipped", "fixtures": 0, "failed": []}
        self.rules = []
        self.findings = []
        self.counts = {"error": 0, "warn": 0, "waived": 0, "waivers": 0}
        self.matched = 0
        self.timings = {}
        self.n_builtin = 0
        self.n_plugin = 0
        self.notes = []

    def fatal(self, rule, message, detail=None):
        self.findings.append(Finding(rule=rule, message=message, detail=detail or [], fatal=True,
                                     key=f"{rule}:{message}"))

    def recount(self):
        self.findings.sort(key=lambda f: (f.path or "", f.line or 0, f.rule, f.message))
        self.counts["error"] = sum(1 for f in self.findings if f.severity == ERROR and not f.waived)
        self.counts["warn"] = sum(1 for f in self.findings if f.severity == WARN and not f.waived)
        self.counts["waived"] = sum(1 for f in self.findings if f.waived)

    @property
    def has_fatal(self):
        return any(f.fatal for f in self.findings)

    def to_json(self):
        return {
            "schema": "civ5re.check/1", "mode": self.mode, "exit": self.exit,
            "seconds": round(self.seconds, 3), "inputs": self.inputs, "selftest": self.selftest,
            "rules": self.rules, "findings": [f.to_json() for f in self.findings], "counts": self.counts,
        }


def order(rules):
    return sorted(rules, key=plugins.sort_key)


def choose(rules, only, skip, res):
    ids = {r.id for r in rules}
    for name in list(only or []) + list(skip or []):
        if name not in ids:
            res.fatal("tool", f"unknown rule id {name} (see: civ5re check --list)")
    only, skip = set(only or []), set(skip or [])
    return [r for r in order(rules) if (not only or r.id in only) and r.id not in skip]


def _normalize(rule, out, show):
    kept = []
    for f in out:
        if not isinstance(f, Finding):
            raise TypeError(f"rule returned {type(f).__name__}, expected Finding")
        f.rule = rule.id
        if f.severity not in (ERROR, WARN):
            raise ValueError(f"bad severity {f.severity!r}")
        if f.path is not None:
            f.path = str(f.path).replace("\\", "/")
            if f.line is None or f.line < 1:
                f.line = 1
            if show is not None and f.path not in show:
                continue
        else:
            f.line = None
        kept.append(f)
    return kept


def assign_keys(findings):
    for f in findings:
        if not f.key:
            f.key = f"{f.rule}:{f.path or ''}:{f.mangled or f.entity or ''}"
    seen = {}
    for f in sorted(findings, key=lambda f: (f.path or "", f.line or 0, f.rule, f.message)):
        n = seen[f.key] = seen.get(f.key, 0) + 1
        if n > 1:
            f.key = f"{f.key}#{n}"


def _make_plan(ctx, settings):
    if settings.mode == "staged":
        changes = staged.index_changes(settings.repo)
        tracked = staged.tracked(settings.repo)
    else:
        changes, tracked = [], None
        for f in settings.files:
            p = Path(f)
            cands = [p] if p.is_absolute() else [Path.cwd() / p, settings.repo / p]
            for c in cands:
                if c.is_file():
                    try:
                        changes.append(staged.Change("M", c.resolve().relative_to(settings.repo.resolve()).as_posix()))
                    except ValueError:
                        raise InputError("--files", f"{f} is outside the repository") from None
                    break
            else:
                raise InputError("--files", f"{f}: no such file")
    return staged.plan(changes, ctx.includes, tracked)


def run_rules(settings, rules, *, plugin_modules=(), load_errors=()):
    t0 = time.perf_counter()
    res = Result(settings.mode)
    res.inputs["plugins"] = sorted(plugin_modules)
    ctx = Context(settings)
    for e in load_errors:
        res.fatal("plugin", e)
    res.n_builtin = sum(1 for r in rules if not r.plugin)
    res.n_plugin = sum(1 for r in rules if r.plugin)
    chosen = choose(rules, settings.only, settings.skip, res)
    plan = None
    if settings.mode in ("staged", "files"):
        try:
            plan = _make_plan(ctx, settings)
        except (staged.StagedError, InputError) as e:
            res.fatal("tool" if isinstance(e, staged.StagedError) else "input", str(e))
            return _finish(res, ctx, chosen, t0, [])
        if plan.full:
            res.notes.append(f"full run: {plan.reason}")

    findings, reported_inputs, ran = [], set(), set()

    def execute(rule):
        entry = {"id": rule.id, "title": rule.title, "plugin": rule.plugin, "status": "ran",
                 "reason": None, "seconds": 0.0, "error": 0, "warn": 0}
        res.rules.append(entry)
        show = None
        if plan is not None:
            sc = plan.scope(rule)
            if sc is None:
                entry.update(status="skipped", reason=NOT_AFFECTED)
                return
            ctx.scope, show = sc.run, sc.show
        else:
            ctx.scope = None
        t = time.perf_counter()
        try:
            for n in rule.needs:
                ctx.require(n)
            if rule.deep:
                diags = (ctx.typedb.get("diagnostics") or [])
                if any(d.get("code") == "clang-error" for d in diags):
                    raise RuleSkip("typedb has no layout/vtable data (clang-error); fix M0 findings first")
            out = _normalize(rule, list(rule.run(ctx) or []), show)
            findings.extend(out)
            ran.add(rule.id)
            entry["error"] = sum(1 for f in out if f.severity == ERROR)
            entry["warn"] = sum(1 for f in out if f.severity == WARN)
        except RuleSkip as e:
            entry.update(status="skipped", reason=str(e) or "skipped by the rule")
        except InputError as e:
            entry.update(status="skipped", reason=f"input {e.name} unavailable: {e.reason}")
            if e.name not in reported_inputs:
                reported_inputs.add(e.name)
                res.fatal("input", f"{e.name}: {e.reason}")
        except Exception:
            tb = traceback.format_exc().rstrip().splitlines()
            f = rule.finding(None, None, "rule crashed", detail=tb, fatal=True,
                             key=f"{rule.id}:crashed")
            findings.append(f)
            ran.add(rule.id)
            entry.update(reason="rule crashed", error=1)
        entry["seconds"] = round(time.perf_counter() - t, 3)

    body = [r for r in chosen if r.id != "W0"]
    tail = [r for r in chosen if r.id == "W0"]
    for r in body:
        execute(r)
    assign_keys(findings)
    try:
        waivers = ctx.waivers
    except InputError as e:
        res.fatal("input", f"{e.name}: {e.reason}")
        waivers = []
    valid = {(w.rule, w.key): w for w in waivers if w.valid}
    for f in findings:
        if not f.fatal and (f.rule, f.key) in valid:
            f.waived = True
            valid[(f.rule, f.key)].matched = True
    for w in waivers:
        w.stale = w.valid and not w.matched and w.rule in ran and settings.mode in ("full", "root")
    for r in tail:
        execute(r)
    assign_keys(findings)
    res.matched = sum(1 for w in waivers if w.matched)
    res.counts["waivers"] = len(waivers)
    return _finish(res, ctx, chosen, t0, findings)


def _finish(res, ctx, chosen, t0, findings):
    res.findings += findings
    res.recount()
    res.inputs.update({k: v for k, v in ctx.inputs_info(res.inputs["plugins"]).items() if k != "plugins"})
    res.timings = dict(ctx.timings)
    res.exit = 2 if res.has_fatal else (1 if res.counts["error"] else 0)
    res.seconds = time.perf_counter() - t0
    return res


def run_check(settings):
    """Selftest (cached) + rules; returns Result."""
    from . import selftest
    loaded = plugins.load_all(settings.repo)
    res_self, tool_errors = {"status": "skipped", "fixtures": 0, "failed": []}, []
    if getattr(settings, "selftest", True):
        res_self, tool_errors = selftest.ensure(settings, loaded)
    res = run_rules(settings, loaded.rules, plugin_modules=loaded.modules, load_errors=loaded.errors)
    res.selftest = res_self
    for msg in tool_errors:
        res.fatal("input", msg)
    res.recount()
    res.exit = exit_code(res.has_fatal, res_self["status"] == "failed", res.counts["error"] > 0)
    return res


def exit_code(tool_failed, selftest_failed, has_errors):
    """2 (tool could not run) > 3 (selftest failed) > 1 (errors) > 0."""
    return 2 if tool_failed else 3 if selftest_failed else 1 if has_errors else 0
