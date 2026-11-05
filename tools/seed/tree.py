"""`civ5re seed tree`: scaffolds of the exe classes through staging."""
import json
import os
import shutil
import subprocess
import sys
import time
from bisect import bisect_right
from collections import defaultdict
from pathlib import Path

from seed import emit as E
from seed import existing as X
from seed import model as MD
from seed import place as PL
from seed import report as RP
from seed import sdkspec as SS
from seed import templates as TP
from seed import vtables as VT

TREE = "Civ5Expansion2"
NL = "\n"
STAGING = "build/seed-staging"


class TreeError(Exception):
    def __init__(self, msg, code=1):
        super().__init__(msg)
        self.exit_code = code


def compute_pads(model, existing):
    """Padding placeholders so that the second and later bases sit at their RTTI offsets (V3)."""
    need = defaultdict(int)
    lone = []       # bases that are the only base of a class and sit behind the vptr or own data
    for sc in model.scopes.values():
        if sc.kind != "class":
            continue
        bs = sorted(sc.bases, key=lambda b: b[2])
        if len(bs) == 1 and bs[0][2] > 0:
            lone.append(bs[0][0])
        if len(bs) < 2:
            continue
        for (p1, _, o1, *_), (p2, _, o2, *_) in zip(bs, bs[1:]):
            need[p1] = max(need[p1], o2 - o1)
    def end(path, ends, depth=0):
        """Data size of a class as declared (vptr, bases, padding); an empty base takes one byte."""
        if path in ends:
            return ends[path]
        sc = model.scopes.get(path)
        if sc is None or depth > 32:
            return None
        if sc.kind == "existing":
            r = existing.records.get(path)
            return r.get("dsize") if r else None
        if sc.kind != "class":
            return None
        e = 4 if sc.dynamic else 0
        for bp, _, off, *_ in sc.bases:
            be = end(bp, ends, depth + 1)
            if be is None:
                return None
            e = max(e, off + max(be, 1))
        if sc.pad:
            e = max(e, sc.pad[0] + sc.pad[1])
        ends[path] = e
        return e

    def depth(path, d=0):
        sc = model.scopes.get(path)
        if sc is None or d > 32 or not sc.bases:
            return d
        return max(depth(bp, d + 1) for bp, *_ in sc.bases)

    for path in sorted(need, key=lambda p: (depth(p), p)):
        sc = model.scopes.get(path)
        e = end(path, {})
        if sc is None or sc.kind != "class" or e is None or sc.tparams is not None:
            if need[path] and (e is None or e != need[path]):
                model.decisions["base_offset_unmatched"].append("::".join(path))
            continue
        if need[path] > e:
            e4 = (e + 3) & ~3
            if need[path] > e4:
                sc.pad = (e4, need[path] - e4)
                model.decisions["padding_for_base_offset"].append(
                    f"{'::'.join(path)} +0x{e4:x}[0x{need[path] - e4:x}]")
        elif need[path] < e:
            model.decisions["base_offset_unmatched"].append("::".join(path))

    def has_data(path, d=0):
        sc = model.scopes.get(path)
        if sc is None or d > 32:
            return True
        if sc.kind == "existing":
            r = existing.records.get(path)
            return r is None or (r.get("dsize") or 0) > 1
        return sc.kind != "class" or bool(sc.dynamic or sc.pad or any(has_data(bp, d + 1) for bp, *_ in sc.bases))

    # an only base behind the vptr: a base without data (all the seed knows) would be laid out at +0 (empty base
    # optimization), not at its RTTI offset; one placeholder gives it a body
    for path in sorted(set(lone)):
        sc = model.scopes.get(path)
        if sc is not None and sc.kind == "class" and sc.tparams is None and not has_data(path):
            sc.pad = (0, 4)
            model.decisions["padding_for_base_offset"].append(f"{'::'.join(path)} +0x0[0x4]")


def unit_addr(model, path):
    best = None
    for p in [path] + [q for q in model.scopes if len(q) > len(path) and q[:len(path)] == path]:
        for f in model.scopes[p].fns:
            for i in f.insts:
                a = i.addr or i.base
                if a and not i.spec:
                    v = int(a, 16)
                    best = v if best is None else min(best, v)
    return best


def unit_data(model, path):
    out = []
    sc = model.scopes[path]
    for v in sc.vars:
        out.extend(int(i.addr, 16) for i in v.insts if i.addr)
    if sc.vtable and sc.vtable != "?":
        out.append(int(sc.vtable, 16))
    return out


def generate(root, bf, gfuncs, db, code=None):
    """-> (files {rel under Civ5Expansion2/: text}, report dict)."""
    t0 = time.time()
    facts = MD.Facts(bf, gfuncs)
    facts.code = code
    ex = X.Existing(db)
    ex.scan_forward(root)
    fo = X.Foreign(Path(root) / "third_party")
    m = MD.build(facts, ex)
    MD.finish_structors(m)
    MD.classify(m, ex, fo)
    MD.apply_template_refs(m, ex)
    MD.template_template_params(m)
    engine, vstats = VT.run(m, facts, ex, fo)
    TP.generalize(m)
    MD.finish_members(m)
    MD.drop_undeclarable(m)
    compute_pads(m, ex)
    rep = RP.new_report()
    em = E.Emitter(m, ex, fo, rep)
    units = em.units()
    evidence = PL.load_evidence()
    rules = PL.load_rules()
    pu = [{"key": u, "name": u[-1], "qual": "::".join(u), "addr": unit_addr(m, u)} for u in units]
    placed = PL.place(pu, evidence, rules, m.decisions, PL.tree_dirs(Path(root) / TREE))
    by_file = defaultdict(list)
    origin = {}
    for u in units:
        rel, org = placed[u]
        em.file_of[u] = rel
        by_file[rel].append(u)
        origin[rel] = org
    # free functions and namespace data: the file of the nearest unit in the same kind of section
    text_idx = sorted((unit_addr(m, u), u) for u in units if unit_addr(m, u) is not None)
    data_idx = sorted((a, u) for u in units for a in unit_data(m, u))
    free = defaultdict(lambda: defaultdict(lambda: {"fns": [], "vars": []}))
    for path, sc in sorted(m.scopes.items()):
        if sc.kind != "namespace":
            continue
        for f in sc.fns:
            a = min((int(i.addr or i.base, 16) for i in f.insts if i.addr or i.base), default=None)
            u = _nearest(text_idx, a) or _nearest(data_idx, a)
            free[placed[u][0]][path]["fns"].append(f)
        for v in sc.vars:
            a = min((int(i.addr, 16) for i in v.insts if i.addr), default=None)
            u = _nearest(data_idx, a) or _nearest(text_idx, a)
            free[placed[u][0]][path]["vars"].append(v)
    files = {}
    occupied = []
    for rel in sorted(set(by_file) | set(free)):
        if (Path(root) / TREE / rel).exists():
            occupied.append(rel)
            continue
        files[rel] = em.file_text(rel, origin.get(rel, "guess"), by_file.get(rel, []), free.get(rel, {}))
    # section 6: hand-written classes (the pilot) lacking binary members are completed in place
    pilot = defaultdict(list)
    for path, sc in sorted(m.scopes.items()):
        if sc.kind == "existing" and sc.handwritten and (sc.fns or sc.vars or any(
                em.kind(c) in ("class", "enum") for c in em.children(path))):
            r = ex.records[path]
            pilot[r["file"]].append((r["line"], path))
    completed = {}
    for fpath, items in sorted(pilot.items()):
        rel = fpath[len(TREE) + 1:]
        text = (Path(root) / fpath).read_text(encoding="utf-8")
        for line, path in sorted(items, reverse=True):
            text = em.amend_text(rel, text, path, line)
        completed[rel] = text
        rep["decisions"]["pilot_completed"].extend("::".join(p) for _, p in items)
    files.update(completed)
    # SPEC lines for instantiations of SDK templates (sdkspec.py): the SDK copies get marker lines only
    add, nspec = SS.edits(db, facts, ex, ex.declares_symbol)
    amended = {}
    for fpath, by_line in sorted(add.items()):
        rel = fpath[len(TREE) + 1:]
        text = (Path(root) / fpath).read_text(encoding="utf-8")
        new = SS.apply(text, by_line)
        if new != text:
            amended[rel] = new
    RP.fill(rep, m, facts, files, occupied, vstats, units, time.time() - t0, ex)
    rep["sdk_amended"] = {"files": sorted(amended), "spec_lines": nspec}
    rep["pilot_completed"] = sorted(completed)
    files.update(amended)
    return files, rep


def _nearest(idx, a):
    if not idx or a is None:
        return idx[0][1] if idx else None
    k = bisect_right(idx, (a, (chr(0x10FFFF),)))
    if k == 0:
        return idx[0][1]
    return idx[k - 1][1]


def load_inputs(root):
    from binfacts import api as binfacts_api
    from typedb import api as typedb_api
    g = Path(root) / "build" / "ghidra-functions.txt"
    if not g.exists():
        raise TreeError("build/ghidra-functions.txt is missing (hint: civ5re ghidra functions)", 2)
    bf = binfacts_api.load()
    out = typedb_api.build_db(Path(root) / TREE, Path(root) / "build", markers="strict")
    db = json.loads(Path(out).read_text(encoding="utf-8"))
    return bf, MD.load_gfuncs(g), db


def load_code(root):
    from civ5re import binlock, config
    from seed import elfcode
    try:
        cfg = config.load()
        entry = binlock.target(binlock.load(Path(root) / "bins.lock"))
        p = Path(cfg.path("", "bin_dir")) / entry.path
        return elfcode.Code(p) if p.exists() else None
    except Exception:
        return None


def write_files(base, files):
    for rel, text in files.items():
        p = Path(base) / rel
        p.parent.mkdir(parents=True, exist_ok=True)
        p.write_bytes(text.encode("utf-8"))


def stage(root, files):
    st = Path(root) / STAGING
    if st.exists():
        shutil.rmtree(st)
    shutil.copytree(Path(root) / TREE, st / TREE)
    write_files(st / TREE, files)
    return st


def run_check(root, staging, required=True):
    env = dict(os.environ, CIV5RE_CHECKS_EXPORT_COVERAGE="required" if required else "report")
    cmd = [sys.executable, "-m", "civ5re", "check", "--root", str(staging), "--json"]
    t = time.time()
    p = subprocess.run(cmd, cwd=root, env=env, capture_output=True, text=True, encoding="utf-8")
    return p.returncode, p.stdout, p.stderr, time.time() - t


def set_export_coverage(root):
    """`[checks] export_coverage = required` in civ5re.cfg; the rest of the text is unchanged."""
    cfg = Path(root) / "civ5re.cfg"
    text = cfg.read_text(encoding="utf-8")
    lines = text.split(NL)
    start = next((i for i, ln in enumerate(lines) if ln.strip() == "[checks]"), None)
    if start is None:
        while lines and lines[-1] == "":
            lines.pop()
        lines += ["", "[checks]", "export_coverage = required", ""]
    else:
        end = next((i for i in range(start + 1, len(lines)) if lines[i].strip().startswith("[")), len(lines))
        key = [i for i in range(start + 1, end) if lines[i].split("=")[0].strip() == "export_coverage"]
        if key:
            lines[key[0]] = "export_coverage = required"
        else:
            last = max([i for i in range(start, end) if lines[i].strip()], default=start)
            lines.insert(last + 1, "export_coverage = required")
    new = NL.join(lines)
    if new != text:
        cfg.write_bytes(new.encode("utf-8"))


def run(args, out=print):
    from civ5re import config
    root = Path(config.ROOT)
    try:
        bf, gfuncs, db = load_inputs(root)
    except TreeError as e:
        out(f"seed tree: {e}")
        return e.exit_code
    t = time.time()
    files, rep = generate(root, bf, gfuncs, db, load_code(root))
    rep["timings"]["generate_s"] = round(time.time() - t, 1)
    RP.write(root, rep)
    if rep["occupied"]:
        out("seed tree: target paths are occupied by files without the required class:\n  "
            + "\n  ".join(rep["occupied"][:50]))
        return 1
    n_old = len(rep["sdk_amended"]["files"]) + len(rep["pilot_completed"])
    out(f"seed tree: {len(files) - n_old} new files, {len(rep['sdk_amended']['files'])} SDK copies get SPEC "
        f"lines, {len(rep['pilot_completed'])} hand-written files completed")
    if args.dry_run:
        return 0
    if args.out:
        write_files(Path(args.out) / TREE, files)
        out(f"seed tree: written to {args.out}")
        return 0
    if not files:
        out("seed tree: nothing to write")
        return 0
    required = getattr(args, "coverage", "required") == "required"
    st = stage(root, files)
    code, so, se, dt = run_check(root, st, required)
    rep["timings"]["check_s"] = round(dt, 1)
    RP.write(root, rep)
    if code != 0:
        out(f"seed tree: civ5re check --root {STAGING} failed (exit {code}); the tree is unchanged, "
            f"staging kept\n{se[-2000:]}")
        return 1
    write_files(root / TREE, files)
    if not required:
        out(f"seed tree: {len(files)} files written; export coverage stays a report")
        return 0
    set_export_coverage(root)
    out(f"seed tree: {len(files)} files written; [checks] export_coverage = required")
    return 0
