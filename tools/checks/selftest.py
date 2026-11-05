"""Fixture runner: every rule (built-in and plugin) must catch its own defect and pass its clean case."""
import copy
import hashlib
import json
import os
import re
import tempfile
from concurrent.futures import ThreadPoolExecutor
from pathlib import Path

from . import includes, plugins, runner, staged
from .api import InputError
from .context import Context, Settings

CHECKS_DIR = Path(__file__).resolve().parent
WORLD = CHECKS_DIR / "fixtures" / "_world"
EXPECT_RE = re.compile(r"^(\S+):(\d+)(?: (.*))?$")
EXPECT_REPO_RE = re.compile(r"^civ5re:(?: (.*))?$")      # a finding without a file
ADDR_RE = re.compile(r"^0x[0-9a-f]{8}$")
HELPERS = (("includes", includes.verify), ("staged", staged.verify), ("plugins", plugins.verify))

TOP_KEYS = ("schema", "tool", "binary", "sections", "symbols", "functions_eh", "plt", "typeinfo", "vtables", "sizes", "stats")
SYMBOL_KEYS = ("mangled", "demangled", "addr", "size", "type", "bind", "section", "kind", "variant", "scope",
               "scope_is_class", "name", "params", "cv", "group")
SORT_KEYS = ("symbols", "functions_eh", "plt", "typeinfo", "vtables")


def _walk_files(base):
    for d, dirs, names in os.walk(base):
        dirs[:] = sorted(x for x in dirs if x != "__pycache__")
        for n in sorted(names):
            if not n.endswith(".pyc"):
                yield Path(d, n)


def tool_hash(settings, loaded, clang_version):
    h = hashlib.sha256(f"clang:{clang_version}\n".encode())
    roots = [CHECKS_DIR, settings.repo / "tools" / "typedb"]
    files = [settings.repo / "tools" / "civ5re" / "placeholders.py"]
    for m in loaded.modules:
        files.append(settings.repo / "tools" / m / plugins.PLUGIN_FILE)
        roots.append(settings.repo / "tools" / m / "fixtures")
    for r in roots:
        if r.is_dir():
            files += list(_walk_files(r))
    for p in sorted(set(files)):
        if p.is_file():
            rel = p.relative_to(settings.repo).as_posix() if settings.repo in p.parents else p.as_posix()
            h.update(rel.encode() + b"\0" + hashlib.sha256(p.read_bytes()).digest())
    return h.hexdigest()


def _cache_file(settings):
    return settings.out / "cache" / "selftest.json"


def _clang_version(settings):
    try:
        return Context(settings).clang[1]
    except InputError:
        return "none"


def ensure(settings, loaded):
    """(selftest dict, tool error messages): from cache when nothing relevant changed."""
    digest = tool_hash(settings, loaded, _clang_version(settings))
    try:
        rec = json.loads(_cache_file(settings).read_text(encoding="utf-8"))
    except (OSError, ValueError):
        rec = {}
    if rec.get("hash") == digest:
        return {"status": "cached", "fixtures": rec.get("fixtures", 0), "failed": []}, []
    res, errors = run(settings, loaded)
    if res["status"] == "ok" and not errors and not loaded.errors:
        _cache_file(settings).parent.mkdir(parents=True, exist_ok=True)
        _cache_file(settings).write_text(json.dumps({"hash": digest, "fixtures": res["fixtures"]}), encoding="utf-8")
    return res, errors


def apply_patch(base, patch):
    out = copy.deepcopy(base)
    _merge(out, patch)
    for k in SORT_KEYS:
        if isinstance(out.get(k), list):
            out[k].sort(key=lambda e: (e.get("addr", ""), e.get("mangled") or e.get("symbol") or e.get("class") or ""))
    return out


def _merge(dst, patch):
    for k, v in patch.items():
        if k.endswith("+") and isinstance(v, list):
            dst.setdefault(k[:-1], []).extend(v)
        elif k.endswith("-") and isinstance(v, list):
            dst[k[:-1]] = [e for e in dst.get(k[:-1], [])
                           if not ({e.get("mangled"), e.get("symbol"), e.get("class")} & set(v))]
        elif isinstance(v, dict) and isinstance(dst.get(k), dict):
            _merge(dst[k], v)
        else:
            dst[k] = copy.deepcopy(v)


def check_world(world):
    """Problems of the synthetic binfacts.json against the current binfacts schema; [] when it matches."""
    try:
        from binfacts import api
        want = api.SCHEMA
    except (ImportError, AttributeError) as e:
        raise InputError("binfacts", f"cannot read binfacts.api.SCHEMA ({e})") from None
    errs = []
    if world.get("schema") != want:
        errs.append(f"schema is {world.get('schema')!r}, binfacts has {want!r}: regenerate _world/binfacts.json")
    for k in TOP_KEYS:
        if k not in world:
            errs.append(f"missing top-level key {k}")
    for i, s in enumerate(world.get("symbols", [])):
        miss = [k for k in SYMBOL_KEYS if k not in s]
        if miss:
            errs.append(f"symbols[{i}] ({s.get('mangled')}) lacks {','.join(miss)}")
        for k in ("addr",):
            if not ADDR_RE.match(str(s.get(k))):
                errs.append(f"symbols[{i}].{k} must look like 0x%08x")
    for key, fields in (("sections", ("name", "addr", "size", "flags")), ("functions_eh", ("addr", "size")),
                        ("plt", ("addr", "name"))):
        for i, e in enumerate(world.get(key, [])):
            miss = [k for k in fields if k not in e]
            if miss:
                errs.append(f"{key}[{i}] lacks {','.join(miss)}")
    for i, v in enumerate(world.get("vtables", [])):
        miss = [k for k in ("symbol", "class", "addr", "size", "construction", "groups") if k not in v]
        if miss:
            errs.append(f"vtables[{i}] lacks {','.join(miss)}")
        for g in v.get("groups", []):
            miss = [k for k in ("offset_to_top", "vbase_offsets", "typeinfo", "address_point", "slots") if k not in g]
            if miss:
                errs.append(f"vtables[{i}] group lacks {','.join(miss)}")
            for sl in g.get("slots", []):
                if not {"target", "symbols", "kind"} <= set(sl):
                    errs.append(f"vtables[{i}] slot lacks target/symbols/kind")
    for i, t in enumerate(world.get("typeinfo", [])):
        miss = [k for k in ("symbol", "addr", "class", "kind", "name", "flags", "bases") if k not in t]
        if miss:
            errs.append(f"typeinfo[{i}] lacks {','.join(miss)}")
    for cls, e in (world.get("sizes") or {}).items():
        if not isinstance(e.get("min"), int) or not isinstance(e.get("from"), list):
            errs.append(f"sizes[{cls}] must be {{min: int, from: [addr]}}")
    st = world.get("stats", {})
    for k in ("groups", "kinds", "variants", "aliases", "denominator"):
        if k not in st:
            errs.append(f"stats lacks {k}")
    den = st.get("denominator", {})
    for k in ("by_kind", "total", "covered_by_class"):
        if k not in den:
            errs.append(f"stats.denominator lacks {k}")
    if isinstance(den.get("by_kind"), dict) and den.get("total") != sum(den["by_kind"].values()):
        errs.append("stats.denominator.total is not the sum of by_kind")
    return errs


def discover_cases(fixdir):
    if not fixdir.is_dir():
        return []
    return sorted(d for d in fixdir.iterdir() if d.is_dir() and re.match(r"^(ok|bad)-", d.name))


def parse_expect(text):
    out = []
    for n, line in enumerate(text.splitlines(), 1):
        line = line.strip()
        if not line or line.startswith("#"):
            continue
        r = EXPECT_REPO_RE.match(line)
        if r:
            out.append((None, None, r.group(1) or ""))
            continue
        m = EXPECT_RE.match(line)
        if not m:
            raise ValueError(f"expect.txt line {n}: expected '<path>:<line> <substring>'")
        out.append((m.group(1), int(m.group(2)), m.group(3) or ""))
    return out


def _where(path, line):
    return f"{path}:{line}" if path else "civ5re:"


def compare(findings, expected):
    """Problems when findings and expect.txt lines do not match one to one."""
    left, problems = list(findings), []
    for path, line, sub in expected:
        for f in left:
            if f.path == path and f.line == line and sub in f"{f.rule} {f.message}":
                left.remove(f)
                break
        else:
            problems.append(f"expected {_where(path, line)} {sub!r} was not reported")
    for f in left:
        problems.append(f"unexpected {_where(f.path, f.line)}: {f.rule} {f.message}")
    return problems


def _case_settings(base, rule, case, tmp, patched):
    tree = case / "tree"
    world = {"binfacts": patched, "ghidra": WORLD / "ghidra-functions.txt",
             "source_paths": WORLD / "source_paths.txt", "sdk_lib_paths": WORLD / "sdk_lib_paths.txt",
             "sdk_map": WORLD / "sdk_map.txt"}
    if (case / "ghidra-functions.txt").is_file():
        world["ghidra"] = case / "ghidra-functions.txt"
    if (case / "no-ghidra-list").exists():
        world["ghidra"] = False
    cfg = {}
    if (case / "config.json").is_file():
        cfg = json.loads((case / "config.json").read_text(encoding="utf-8"))
    wf = tree / "tools" / "checks" / "waivers.json"
    return Settings(
        repo=base.repo, cfg=base.cfg, mode="root", root=tree, only=[rule.id, *rule.fixture_with], jobs=2,
        build=tmp / "build", world=world, waivers=wf if wf.is_file() else None,
        export_coverage=cfg.get("export_coverage", "report"), clang_prefix=base.clang_prefix,
        write_last=False, selftest=False)


def _run_case(base, loaded_rules, rule, case, tmp, world):
    """(problems, tool_errors)"""
    patch = case / "binfacts.json"
    data = apply_patch(world, json.loads(patch.read_text(encoding="utf-8"))) if patch.is_file() else world
    tmp.mkdir(parents=True, exist_ok=True)
    patched = tmp / "binfacts.json"
    patched.write_text(json.dumps(data), encoding="utf-8")
    try:
        expected = parse_expect((case / "expect.txt").read_text(encoding="utf-8")) \
            if (case / "expect.txt").is_file() else []
    except ValueError as e:
        return [str(e)], []
    problems = []
    if case.name.startswith("ok-") and expected:
        problems.append("ok fixture must have an empty expect.txt")
    if case.name.startswith("bad-") and not expected:
        problems.append("bad fixture needs at least one line in expect.txt")
    res = runner.run_rules(_case_settings(base, rule, case, tmp, patched), loaded_rules)
    tool = [f.message for f in res.findings if f.fatal and f.rule == "input"]
    if tool:
        return problems, tool
    crashed = [f for f in res.findings if f.fatal and f.rule == rule.id]
    if crashed:
        return problems + ["rule crashed: " + (crashed[0].detail[-1] if crashed[0].detail else "")], []
    own = [f for f in res.findings if f.rule == rule.id]
    return problems + compare(own, expected), []


def run(settings, loaded, only=None):
    """(selftest dict, tool error messages). Always without the result cache."""
    failed, tool_errors = [], []
    rules = [r for r in loaded.rules if not only or r.id in only]
    for name in only or []:
        if name not in {r.id for r in loaded.rules}:
            tool_errors.append(f"selftest: unknown rule id {name}")
    for e in loaded.errors:
        failed.append({"id": "plugins", "case": "load", "message": e})
    tasks = []
    for r in rules:
        fixdir = plugins.fixture_dir(r, settings.repo, CHECKS_DIR)
        cases = discover_cases(fixdir)
        if not any(c.name.startswith("ok-") for c in cases):
            failed.append({"id": r.id, "case": "-", "message": f"no ok-* fixture in {_rel(fixdir, settings.repo)}"})
        if not any(c.name.startswith("bad-") for c in cases):
            failed.append({"id": r.id, "case": "-", "message": f"no bad-* fixture in {_rel(fixdir, settings.repo)}"})
        tasks += [(r, c) for c in cases]
    world = None
    if tasks or not only:
        try:
            world = json.loads((WORLD / "binfacts.json").read_text(encoding="utf-8"))
        except (OSError, ValueError) as e:
            failed.append({"id": "_world", "case": "binfacts.json", "message": f"cannot read: {e}"})
    if world is not None:
        try:
            for msg in check_world(world):
                failed.append({"id": "_world", "case": "binfacts.json", "message": msg})
        except InputError as e:
            tool_errors.append(f"{e.name}: {e.reason}")
    n = len(tasks)
    if world is not None and tasks:
        with tempfile.TemporaryDirectory(prefix="civ5re-selftest-") as d:
            def one(i):
                r, c = tasks[i]
                try:
                    return i, _run_case(settings, loaded.rules, r, c, Path(d) / str(i), world)
                except Exception as e:
                    return i, ([f"fixture runner failed: {type(e).__name__}: {e}"], [])
            with ThreadPoolExecutor(max_workers=max(1, min(len(tasks), settings.jobs or os.cpu_count() or 1))) as pool:
                for i, (problems, tool) in pool.map(one, range(len(tasks))):
                    r, c = tasks[i]
                    failed += [{"id": r.id, "case": c.name, "message": p} for p in problems]
                    tool_errors += [t for t in tool if t not in tool_errors]
    if not only:
        for name, fn in HELPERS:
            n += 1
            failed += [{"id": name, "case": "verify", "message": m} for m in fn()]
    status = "failed" if failed else "ok"
    return {"status": status, "fixtures": n, "failed": failed}, tool_errors


def _rel(p, base):
    try:
        return Path(p).relative_to(base).as_posix()
    except ValueError:
        return Path(p).as_posix()
