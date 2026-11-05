"""`seed env`: third_party/ and the SDK copies.

Everything is assembled in a temporary folder under build/ first (steps 1-5, libc closure, typedb pass,
markers); only then are the files compared with the real tree and written (write if missing, skip if
identical, error if different).
"""
import hashlib
import os
import re
import shutil
import subprocess
from collections import defaultdict
from concurrent.futures import ThreadPoolExecutor
from pathlib import Path

from . import inputs, sdk, sdkmarkers

TP = "third_party"
TREE = "Civ5Expansion2"
INCLUDE_DIRS = (
    "compat", "fwd", "lua-5.1.4/include", "libcxx/v1", "<clang>",
    "libc/include/x86_64-linux-gnu", "libc/include",
)
LIBC_ARCH = "libc/include/x86_64-linux-gnu"
LIBC_ROOT = "libc/include"
LIBCXX_ROOT = "libcxx/v1"
LUA_ROOT = "lua-5.1.4/include"
VERSIONS = {"libcxx": "18", "libc": "2.39", "lua": "5.1.4"}
KINDS = {"class", "struct", "enum", "union"}


JOBS = min(24, os.cpu_count() or 8)


class EnvError(Exception):
    exit_code = 1


def read_all(paths):
    """File contents in the order of `paths`; reads run in parallel (the sources may sit on a slow share)."""
    with ThreadPoolExecutor(JOBS) as ex:
        return list(ex.map(lambda p: Path(p).read_bytes(), paths))


def posix(p):
    return str(p).replace("\\", "/")


def sha256(data):
    return hashlib.sha256(data).hexdigest()


def norm_nl(data):
    return data.replace(b"\r\n", b"\n")


# third_party sources

def libcxx_files(src):
    src = Path(src)
    if not src.is_dir():
        raise inputs.InputsError(f"libcxx_src not found: {src}")
    paths = []
    for root, dirs, files in os.walk(src):
        dirs.sort()
        paths.extend(Path(root) / name for name in sorted(files))
    out = {f"{TP}/{LIBCXX_ROOT}/{p.relative_to(src).as_posix()}": data for p, data in zip(paths, read_all(paths))}
    if f"{TP}/{LIBCXX_ROOT}/vector" not in out:
        raise inputs.InputsError(f"libcxx_src has no <vector>: {src}")
    return out


def lua_blobs(lekmod_dir, lock):
    commit = lock.commits[inputs.LUA_TREE[0]]
    out = {}
    for path in inputs.lua_files(lekmod_dir, lock):
        data = inputs.read_blob(lekmod_dir, commit, path)
        out[f"{TP}/{LUA_ROOT}/{Path(path).name}"] = data.replace(b"\r\n", b"\n")
    return out


def load_foreign(path=None):
    path = Path(path) if path else inputs.DATA / "foreign_types.txt"
    out = []
    for n, line in enumerate(path.read_text(encoding="utf-8").splitlines(), 1):
        if not line.strip() or line.startswith("#"):
            continue
        f = line.split("\t")
        if len(f) != 3 or f[1] not in KINDS:
            raise EnvError(f"{path.name}:{n}: bad line")
        out.append((f[0], f[1], f[2]))
    return out


def fwd_files(foreign=None):
    """Forward declarations for the names whose header is under fwd/."""
    foreign = foreign if foreign is not None else load_foreign()
    by_header = {}
    for name, kind, header in foreign:
        if header.startswith("fwd/"):
            by_header.setdefault(header, []).append((name, kind))
    out = {}
    for header, items in sorted(by_header.items()):
        flat = [n for n, k in sorted(items) if "::" not in n]
        spaced = {}
        for n, k in sorted(items):
            if "::" in n:
                ns, _, base = n.rpartition("::")
                spaced.setdefault(ns, []).append((base, k))
        lines = ["#pragma once", ""]
        kinds = dict(items)
        for n in flat:
            lines.append(decl(n, kinds[n]))
        for ns, members in sorted(spaced.items()):
            open_ = " ".join(f"namespace {p} {{" for p in ns.split("::"))
            close = " ".join("}" for _ in ns.split("::"))
            lines.append(open_)
            for base, k in members:
                lines.append(decl(base, k))
            lines.append(close)
        out[f"{TP}/{header}"] = ("\n".join(lines) + "\n").encode("utf-8")
    return out


def decl(name, kind):
    """`name<params>` (foreign_types.txt) declares a class template: `template <params> kind name;`."""
    if "<" in name:
        base, _, params = name.partition("<")
        return f"template <{params.rstrip('>')}> {kind} {base};"
    if kind == "enum":
        return f"enum {name} : int;"
    return f"{kind} {name};"


def pch_text(libcxx_includes):
    lines = ['#include "compat/civ5re_prelude.h"', '#include "compat/Windows.h"']
    lines += [f"#include <{n}>" for n in sorted(libcxx_includes)]
    return "\n".join(lines) + "\n"


_ANGLE = re.compile(r"^[ \t]*#[ \t]*include[ \t]*<([^>\n]+)>", re.M)


def sdk_system_includes(sdk_files, libcxx):
    """Angle includes of the SDK copies that are libc++ files."""
    names = set()
    for text in sdk_files.values():
        for m in _ANGLE.finditer(text):
            n = m[1].strip()
            if f"{TP}/{LIBCXX_ROOT}/{n}" in libcxx:
                names.add(n)
    return names


def include_dirs_text():
    return "\n".join(INCLUDE_DIRS) + "\n"


# temporary environment

def write_files(root, files):
    root = Path(root)
    dirs = {(root / rel).parent for rel in files}
    for d in sorted(dirs):
        d.mkdir(parents=True, exist_ok=True)

    def one(item):
        rel, data = item
        (root / rel).write_bytes(data if isinstance(data, bytes) else data.encode("utf-8"))
    with ThreadPoolExecutor(JOBS) as ex:  # thousands of small files: file creation dominates
        list(ex.map(one, files.items()))


def parse_deps(text):
    text = text.replace("\\\r\n", " ").replace("\\\n", " ")
    _, _, deps = text.partition(": ")
    out = []
    for tok in re.split(r"(?<!\\)\s+", deps.strip()):
        if tok:
            out.append(tok.replace("\\ ", " "))
    return out


def _norm(p):
    return posix(p).rstrip("/")


def _under(path, root):
    path, root = _norm(path).lower(), _norm(root).lower()
    return path == root or path.startswith(root + "/")


def _rel(path, root):
    return _norm(path)[len(_norm(root)) + 1:]


def libc_args(args, tp, libc_src):
    """The -isystem entries of the two libc folders point at the WSL sources while the closure is computed."""
    src = _norm(libc_src)
    swap = {_norm(Path(tp) / LIBC_ARCH): f"{src}/x86_64-linux-gnu", _norm(Path(tp) / LIBC_ROOT): src}
    return [swap.get(_norm(a), a) if i and args[i - 1] == "-isystem" else a for i, a in enumerate(args)]


def libc_closure(clang, args, roots, cwd):
    """`clang -M` dependencies of all root files, from one translation unit that includes them (one clang run
    instead of one per root: same set; the per-header `alone` compile checks the result)."""
    tu = Path(cwd) / "closure_all.cpp"
    tu.write_text("".join(f'#include "{r}"\n' for r in roots), encoding="utf-8", newline="\n")
    r = subprocess.run([str(clang), *args, "-x", "c++", "-M", posix(tu)], cwd=cwd, capture_output=True)
    if r.returncode != 0:
        err = r.stderr.decode("utf-8", "replace")[-1500:]
        raise EnvError(f"clang -M failed:\n{err}")
    deps = set(parse_deps(r.stdout.decode("utf-8", "replace")))
    deps.discard(posix(tu))
    return sorted(d for d in deps if _norm(d) != posix(tu))


def classify_libc(deps, tp, libc_src, tree, resource):
    """Maps closure entries that live in the libc source to third_party paths; rejects strays."""
    src = _norm(libc_src)
    arch = f"{src}/x86_64-linux-gnu"
    found, strays = {}, []
    for d in deps:
        if _under(d, Path(tp).parent) or _under(d, resource):
            continue
        if _under(d, arch):
            found[f"{TP}/{LIBC_ARCH}/{_rel(d, arch)}"] = Path(d)
        elif _under(d, src):
            found[f"{TP}/{LIBC_ROOT}/{_rel(d, src)}"] = Path(d)
        else:
            strays.append(d)
    if strays:
        raise EnvError("closure entries outside the known folders:\n  " + "\n  ".join(strays[:20]))
    return found


def foreign_libc_roots(foreign):
    out = []
    for _, _, header in foreign:
        if header.startswith(LIBC_ARCH + "/"):
            out.append(header[len(LIBC_ARCH) + 1:])
        elif header.startswith(LIBC_ROOT + "/"):
            out.append(header[len(LIBC_ROOT) + 1:])
    return sorted(set(out))


def sdk_roots(tree):
    return sorted(posix(p) for p in Path(tree).rglob("*.h"))


def closure_files(clang, tp, tree, build, libc_src, sdk_headers, extra_includes, resource, cfg_args):
    """Libc files needed by compat, fwd, Lua, libc++ and the SDK copies."""
    args = libc_args(cfg_args, tp, libc_src)
    work = Path(build) / "closure"
    work.mkdir(parents=True, exist_ok=True)
    roots = list(sdk_headers)
    extra = work / "closure_extra.cpp"
    extra.write_text(
        f'#include "{posix(Path(tp) / "pch.h")}"\n'
        + "".join(f"#include <{n}>\n" for n in extra_includes), encoding="utf-8")
    roots.append(posix(extra))
    deps = libc_closure(clang, args, roots, work)
    return classify_libc(deps, tp, libc_src, tree, resource)


def read_src(p):
    return Path(p).read_bytes()


def typedb_db(st, markers, tree=None):
    import json
    from typedb import api as typedb_api
    out = typedb_api.build_db(tree or st.tree, st.build, markers=markers, third_party=st.tp)
    return json.loads(Path(out).read_text(encoding="utf-8"))


def blocking(db, probes=False):
    """Error diagnostics; `probe-failed` only counts when `probes` (the marked tree must have none)."""
    return [d for d in db["diagnostics"] if d["severity"] == "error" and (probes or d["code"] != "probe-failed")]


def fmt_diags(diags, limit=40):
    rows = [f"{d['file']}:{d['line']}: {d['code']}: {d['message']}" for d in diags[:limit]]
    if len(diags) > limit:
        rows.append(f"... {len(diags) - limit} more")
    return "\n".join(rows)


class LayoutProbe:
    """Dummy instantiations of SDK class templates, compiled against the whole SDK (precompiled unity)."""

    def __init__(self, st):
        from typedb import api as typedb_api, compile as typedb_cc
        self.clang = typedb_cc.load_env(st.tp).clang
        self.work = st.build / "probes"
        self.work.mkdir(parents=True, exist_ok=True)
        unity = st.build / "typedb" / "unity.cpp"
        pch = st.build / "typedb" / "probe_sdk.pch"
        args = typedb_api.clang_args("tree", tree=st.tree, third_party=st.tp, build=st.build)
        r = subprocess.run([str(self.clang), *args, "-x", "c++-header", str(unity), "-o", str(pch)],
                           capture_output=True)
        if r.returncode != 0:
            raise EnvError("probe precompile failed:\n" + r.stderr.decode("utf-8", "replace")[-1500:])
        self.args = typedb_api.clang_args("tree", tree=st.tree, third_party=st.tp, build=st.build, pch=pch)
        self.args += ["-fno-access-control", "-fsyntax-only"]
        self.count = 0

    def run(self, exprs, dump=False):
        self.count += 1
        src = self.work / f"probe{self.count}.cpp"
        src.write_text(sdkmarkers.render_probe(exprs), encoding="utf-8", newline="\n")
        extra = ["-Xclang", "-fdump-record-layouts-complete"] if dump else []
        return subprocess.run([str(self.clang), *self.args, *extra, str(src)], capture_output=True)

    def compiling(self, exprs):
        with ThreadPoolExecutor(JOBS) as ex:
            ok =list(ex.map(lambda e: self.run([e]).returncode == 0, exprs))
        return [e for e, good in zip(exprs, ok) if good]

    def layouts(self, exprs):
        p = self.run(exprs, dump=True)
        if p.returncode != 0:
            raise EnvError("layout probes failed:\n" + p.stderr.decode("utf-8", "replace")[-1500:])
        return p.stdout.decode("utf-8", "replace")


def probe_layouts(probe, texts, db, log):
    """Field offsets of class templates from dummy instantiations (clang record-layout dump)."""
    exprs = sdkmarkers.probe_expressions(texts, db)
    if not exprs:
        return {}
    good = probe.compiling([e for e, _ in exprs])
    log(f"layout probes: {len(good)} of {len(exprs)} instantiations compile")
    return sdkmarkers.parse_layout_dump(probe.layouts(good))


def shadowed_offsets(st, marked, diags, report):
    """Offsets of fields that typedb does not list (a template and its partial specializations share a record)."""
    add = defaultdict(dict)
    probe = None
    for d in diags:
        m = re.fullmatch(r"field (\w+) has no ///<", d["message"]) if d["code"] == "marker-missing" else None
        if not m:
            continue
        probe = probe or LayoutProbe(st)
        dest, name = sdkmarkers.dest_of(d["file"]), m.group(1)
        expr = sdkmarkers.class_expression(marked[dest], d["line"])
        if expr is None or not probe.compiling([expr]):
            report.setdefault("field_offset_unknown", []).append(f"{dest}:{d['line']} {name}")
            continue
        blocks = sdkmarkers.parse_layout_blocks(probe.layouts([expr]).replace("\r", ""))
        want = sdkmarkers.strip_args(expr)
        cands = [b for b in blocks if sdkmarkers.strip_args(b["raw"]) == want and name in b["fields"]]
        flat = expr.replace(" ", "")
        same = [b for b in cands if b["raw"].replace(" ", "") == flat] or cands
        if not same:
            report.setdefault("field_offset_unknown", []).append(f"{dest}:{d['line']} {name}")
            continue
        end = sdkmarkers.decl_end_line(marked[dest].split("\n"), d["line"])
        add[dest][end] = f"///< +0x{same[0]['fields'][name]:x}"
        report.setdefault("field_offset_dummy", []).append(f"{dest}:{d['line']} {name}")
    for dest, by_line in add.items():
        marked[dest] = sdkmarkers.append_trailing(marked[dest], by_line)
    return bool(add)


def mark_environment(st, bf, log=lambda m: None):
    """Step 6: markers on the SDK texts; returns (marked texts, report, strict db)."""
    origins = {e.dest: e.origin for e in inputs.sdk_entries()}
    texts = dict(st.sdk)
    db0 = typedb_db(st, "off")
    errs = blocking(db0)
    if errs:
        raise EnvError("typedb (markers off) on the SDK copies:\n" + fmt_diags(errs))
    probe = LayoutProbe(st)
    layouts = probe_layouts(probe, texts, db0, log)
    marked, report = sdkmarkers.mark(texts, db0, bf, origins, layouts)
    done = set()
    for _ in range(6):
        sdk.write_sdk(marked, st.tree)
        db1 = typedb_db(st, "strict")
        report.pop("field_offset_unknown", None)
        if shadowed_offsets(st, marked, db1["diagnostics"], report):
            continue
        fix = defaultdict(lambda: defaultdict(list))
        for d in db1["diagnostics"]:
            marker = repair_marker(d)
            if marker is None:
                continue
            dest = sdkmarkers.dest_of(d["file"])
            if (dest, d["line"], marker) not in done:
                done.add((dest, d["line"], marker))
                fix[dest][d["line"]].append(marker)
        if not fix:
            break
        for dest, by_line in fix.items():
            for n in by_line:
                by_line[n].sort(key=lambda m: REPAIR_ORDER.index(m))
            marked[dest] = sdkmarkers.insert_above(marked[dest], by_line)
            report.setdefault("repaired", []).extend(f"{dest}:{n} {m}" for n, ms in sorted(by_line.items()) for m in ms)
    bad = sdkmarkers.vtable_mismatches(db1, bf)
    if bad:
        rows = [f"{c}: slot {i}: Civ5XP {want}, SDK {got}" for c, i, want, got in bad]
        raise EnvError("SDK virtual order differs from _ZTV:\n  " + "\n  ".join(rows))
    marked = sdkmarkers.append_exe_members(marked, db1, bf, report)
    sdk.write_sdk(marked, st.tree)
    with ThreadPoolExecutor(1) as ex:  # the per-header compile and the last typedb run read the same final tree
        st.alone = ex.submit(compile_alone, st)
        db2 = typedb_db(st, "strict")
    left = sdkmarkers.exe_only(db2, bf)
    if left:
        report["exe_only_unmatched"] = sorted(f"{s}::{e['name']}" for s, es in left.items() for e in es)
    return marked, report, db2


REPAIR_ORDER = ["/// size ?", "/// vtable ?", "/// inline"]


def repair_marker(d):
    """The marker that fixes a declaration typedb reports but does not list in its tables, or None."""
    if d["code"] != "marker-missing":
        return None
    msg = d["message"]
    if msg.startswith("a class template ") and msg.endswith(" has no /// size"):
        return "/// size ?"
    if msg.startswith("a class template ") and msg.endswith(" has no /// vtable"):
        return "/// vtable ?"
    if msg.startswith("a function template ") and msg.endswith(" has no main marker"):
        return "/// inline"
    if msg.startswith("definition of ") and msg.endswith("repeats no marker from the header"):
        return "/// inline"
    return None


class Staged:
    """The temporary environment: third_party files (bytes) and SDK texts, laid out under `work`."""

    def __init__(self, work, third, sdk_files, lock):
        self.work = Path(work)
        self.third = third
        self.sdk = sdk_files
        self.lock = lock
        self.alone = None  # Future of compile_alone, set by mark_environment

    @property
    def tp(self):
        return self.work / TP

    @property
    def tree(self):
        return self.work / TREE

    @property
    def build(self):
        return self.work / "build"


def compile_alone(st, jobs=JOBS):
    """Every SDK header compiles by itself (typedb's `alone` flags); returns the failures [(header, stderr)]."""
    from typedb import api as typedb_api, compile as typedb_cc
    clang = typedb_cc.load_env(st.tp).clang
    args = typedb_api.clang_args("alone", tree=st.tree, third_party=st.tp, build=st.build)
    headers = [e.dest for e in inputs.sdk_entries() if e.dest.endswith(".h")]
    stubs = st.build / "alone"
    stubs.mkdir(parents=True, exist_ok=True)

    def one(dest):
        stub = stubs / (dest.replace("/", "_") + ".cpp")
        stub.write_text(f'#include "{dest}"\n', encoding="utf-8", newline="\n")
        r = subprocess.run([str(clang), *args, "-fsyntax-only", str(stub)], capture_output=True)
        return dest, r.returncode, r.stderr.decode("utf-8", "replace")
    with ThreadPoolExecutor(jobs) as ex:
        return [(d, err) for d, rc, err in ex.map(one, headers) if rc != 0]


def outputs(st, marked):
    """{repo-relative path: bytes} for third_party/ and the SDK copies."""
    files = dict(st.third)
    for e in inputs.sdk_entries():
        files[f"{TREE}/{e.dest}"] = marked[e.dest].encode("utf-8")
    return files


def plan(root, files):
    """(to write, identical, different) as sorted path lists: write if missing, skip if identical, error if different."""
    writes, same, diff = [], [], []
    for rel, data in sorted(files.items()):
        p = Path(root) / rel
        if not p.exists():
            writes.append(rel)
        elif norm_nl(p.read_bytes()) == norm_nl(data) or tree_amended(norm_nl(p.read_bytes()), norm_nl(data)):
            same.append(rel)
        else:
            diff.append(rel)
    return writes, same, diff


_TREE_SPEC = re.compile(rb"^\s*/// (?:(?:base|deleting) )?0x[0-9a-f]{8} \S.*>\s*$")
_NO_CODE = re.compile(rb"^\s*/// (?:inline|absent)\s*$")


def tree_amended(have, want):
    """The file differs only by what `seed tree` adds to SDK copies (sdkspec.py): `/// 0x… SPEC` lines, and
    the no-code marker they replaced."""
    import difflib
    a, b = want.split(b"\n"), have.split(b"\n")
    for op, i1, i2, j1, j2 in difflib.SequenceMatcher(None, a, b, autojunk=False).get_opcodes():
        if op == "equal":
            continue
        if any(not _NO_CODE.match(x) for x in a[i1:i2]) or any(not _TREE_SPEC.match(x) for x in b[j1:j2]):
            return False
    return True


_DECL = re.compile(r"^\s*(?:class|struct|union|enum(?:\s+class)?)\s+(\w+)\s*(?::|\{|$)", re.M)


def redeclared(root, files):
    """Classes of existing non-SDK files in Civ5Expansion2/ that an SDK copy declares too: [(file, name)]."""
    sdk_names = set()
    for rel, data in files.items():
        if rel.startswith(TREE + "/"):
            sdk_names.update(_DECL.findall(data.decode("utf-8", "replace")))
    out = []
    base = Path(root) / TREE
    if not base.is_dir():
        return out
    for p in sorted(base.rglob("*")):
        rel = p.relative_to(root).as_posix()
        if not p.is_file() or rel in files or p.suffix not in (".h", ".inl", ".cpp"):
            continue
        for name in _DECL.findall(p.read_text(encoding="utf-8", errors="replace")):
            if name in sdk_names:
                out.append((rel, name))
    return out


def report_lines(report):
    c = report.get("counts", {})
    lines = ["markers: " + ", ".join(f"{k} {v}" for k, v in sorted(c.items()))]
    for key in ("size_unverified", "vtable_unknown", "unsupported", "template_global_unresolved", "repaired", "field_offset_dummy",
                "field_offset_unknown", "forward_declared", "exe_only", "exe_only_unmatched"):
        if key in report:
            lines.append(f"{key}: {len(report[key])}")
    return lines


def run(root, lekmod, libcxx_src, libc_src, relock=False, out=print):
    """seed env. Returns the exit code (0 ok, 1 model error or conflicts, 2 inputs missing or mismatched)."""
    import json
    from binfacts import api as binfacts_api
    unset = [n for n, v in (("lekmod_dir", lekmod), ("libcxx_src", libcxx_src), ("libc_src", libc_src)) if not v]
    if unset:
        out("seed env: not set: " + ", ".join(f"[seed] {n} (env CIV5RE_SEED_{n.upper()})" for n in unset)
            + "; put them in civ5re.local.cfg (see civ5re.local.cfg.example)")
        return 2
    root = Path(root)
    work = root / "build" / "seed-env"
    try:
        st = stage_environment(lekmod, libcxx_src, libc_src, work, log=out)
        bf = binfacts_api.load()
        marked, report, db = mark_environment(st, bf, log=out)
    except inputs.InputsError as e:
        out(f"seed env: {e}")
        return e.exit_code
    except (EnvError, sdkmarkers.MarkError) as e:
        out(f"seed env: {e}")
        return 1
    errors = blocking(db, probes=True)
    if errors:
        out("seed env: typedb (strict) on the marked SDK copies:\n" + fmt_diags(errors))
        return 1
    failed = st.alone.result()
    if failed:
        out("seed env: headers that do not compile alone:\n" + "\n".join(f"{d}:\n{err[:800]}" for d, err in failed[:5]))
        return 1
    out(f"alone: {sum(e.dest.endswith('.h') for e in inputs.sdk_entries())} SDK headers compile")
    files = outputs(st, marked)
    clash = redeclared(root, files)
    if clash:
        out("seed env: SDK entities are declared in other files:\n  " + "\n  ".join(f"{f}: {n}" for f, n in clash))
        return 1
    writes, same, diff = plan(root, files)
    summary = {"files": len(files), "written": len(writes), "identical": len(same), "different": len(diff),
               "report": report, "typedb_probe_failed": [d for d in db["diagnostics"] if d["code"] == "probe-failed"]}
    (root / "build").mkdir(exist_ok=True)
    (root / "build" / "seed-env-report.json").write_text(json.dumps(summary, indent=1, sort_keys=True) + "\n",
                                                         encoding="utf-8", newline="\n")
    if diff:
        out("seed env: files differ from the generated ones, nothing written:\n  " + "\n  ".join(diff[:50])
            + (f"\n  ... {len(diff) - 50} more" if len(diff) > 50 else ""))
        return 1
    for rel in writes:
        p = root / rel
        p.parent.mkdir(parents=True, exist_ok=True)
        p.write_bytes(files[rel])
    for line in report_lines(report):
        out(line)
    n_probe = len(summary["typedb_probe_failed"])
    if n_probe:
        out(f"typedb probe-failed diagnostics (not blocking): {n_probe}")
    out(f"seed env: {len(writes)} written, {len(same)} identical")
    if relock:
        keep = {rel: inputs.text_sha256(data) for rel, data in files.items() if rel.startswith(TP + "/")}
        lock = inputs.compute_lock(lekmod, root, keep_files=keep)
        inputs.LOCK.write_text(inputs.format_lock(lock), encoding="utf-8", newline="\n")
        out("seed env: inputs.lock recalculated")
    return 0


def stage_environment(lekmod, libcxx_src, libc_src, work, log=lambda m: None):
    """Compat, fwd, libc++, Lua, SDK steps 1-5 and the libc closure under `work`."""
    from typedb import api as typedb_api, compile as typedb_cc
    lock = inputs.load_lock()
    inputs.verify(lekmod, lock=lock)
    work = Path(work)
    if work.exists():
        shutil.rmtree(work)
    work.mkdir(parents=True)
    foreign = load_foreign()
    with ThreadPoolExecutor(1) as ex:  # the libc++ sources are read from a slow share while the SDK is built
        pending = ex.submit(libcxx_files, libcxx_src)
        sdk_files = sdk.build_sdk(lekmod, lock)
        log(f"sdk steps 1-5: {len(sdk_files)} files")
        libcxx = pending.result()
    log(f"libc++: {len(libcxx)} files")
    third = {f"{TP}/{d}": t.encode("utf-8") for d, t in inputs.compat_files().items()}
    third.update(fwd_files(foreign))
    third.update(libcxx)
    third.update(lua_blobs(lekmod, lock))
    third[f"{TP}/include_dirs.txt"] = include_dirs_text().encode("utf-8")
    third[f"{TP}/pch.h"] = pch_text(sdk_system_includes(sdk_files, libcxx)).encode("utf-8")
    st = Staged(work, third, sdk_files, lock)
    write_files(work, third)
    for d in (LIBC_ARCH, LIBC_ROOT):
        (st.tp / d).mkdir(parents=True, exist_ok=True)
    sdk.write_sdk(sdk_files, st.tree)
    cfg_env = typedb_cc.load_env(st.tp)
    args = typedb_api.clang_args("alone", tree=st.tree, third_party=st.tp, build=st.build)
    extra = ["sys/stat.h"] + foreign_libc_roots(foreign)
    libc = closure_files(cfg_env.clang, st.tp, st.tree, st.build, libc_src, sdk_roots(st.tree),
                         sorted(set(extra)), cfg_env.resource_include, args)
    log(f"libc closure: {len(libc)} files")
    items = sorted(libc.items())
    libc_data = {rel: data for (rel, _src), data in zip(items, read_all([s for _r, s in items]))}
    third.update(libc_data)
    write_files(work, libc_data)
    third[f"{TP}/MANIFEST.txt"] = manifest_text(third, lock).encode("utf-8")
    write_files(work, {f"{TP}/MANIFEST.txt": third[f"{TP}/MANIFEST.txt"]})
    return st


def manifest_text(files, lock, libc_src=""):
    """Tab-separated: path, sha256, source, version, original location."""
    lines = ["# path\tsha256\tsource\tversion\toriginal location"]
    for rel in sorted(files):
        if rel == f"{TP}/MANIFEST.txt":
            continue
        sub = rel[len(TP) + 1:]
        if sub.startswith(LIBCXX_ROOT + "/"):
            row = ("WSL libc++", VERSIONS["libcxx"], "usr/lib/llvm-18/include/c++/v1/" + sub[len(LIBCXX_ROOT) + 1:])
        elif sub.startswith(LIBC_ARCH + "/"):
            row = ("WSL glibc", VERSIONS["libc"], "usr/include/x86_64-linux-gnu/" + sub[len(LIBC_ARCH) + 1:])
        elif sub.startswith(LIBC_ROOT + "/"):
            row = ("WSL glibc", VERSIONS["libc"], "usr/include/" + sub[len(LIBC_ROOT) + 1:])
        elif sub.startswith(LUA_ROOT + "/"):
            row = (f"Lekmod {lock.commits['sdk'][:7]}", VERSIONS["lua"], inputs.LUA_ORIGINAL_LOCATION + sub[len(LUA_ROOT) + 1:])
        elif sub.startswith("compat/"):
            row = ("seed template", "-", "-")
        else:
            row = ("seed generated", "-", "-")
        lines.append("\t".join((sub, sha256(files[rel]), *row)))
    return "\n".join(lines) + "\n"
