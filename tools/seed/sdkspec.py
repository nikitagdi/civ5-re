"""SPEC lines for instantiations of SDK class templates whose arguments only the tree step declares.

`seed env` marks an instantiation only when every name of its SPEC is declared (sdkmarkers.spec_resolvable);
the rest (`template_global_unresolved`, members without SPEC) are marked here, after the scaffolds exist: a
`/// 0x… SPEC` line per binary instantiation above the member (or above the out-of-class definition of a static
data member). A no-code marker (`inline`, `absent`) of that member gives way to the addresses.
"""
import re
from collections import defaultdict

from seed import mangle as M
from seed.existing import _plain
from seed.model import path_of

TREE = "Civ5Expansion2/"
# families whose member bodies do not compile when instantiated (FNEW gets an int pool where the SDK operator new
# takes eMPoolType): marking an instantiation makes typedb instantiate them; needs an sdk_port.patch fix first
SKIP = {("FObjectPool",)}
NO_CODE = re.compile(r"^\s*/// (inline|absent)\s*$")


def _nparams(f):
    return len(f.get("params") or ())


def edits(db, facts, existing, declared):
    """{file: {line: [marker lines]}}, {file: set(lines to drop)}, count."""
    add = defaultdict(lambda: defaultdict(list))
    drop = defaultdict(set)
    by_scope = defaultdict(list)
    for s in facts.den:
        if declared(s["mangled"]) or s["kind"] not in ("data", "function", "method", "ctor", "dtor"):
            continue
        sc = s.get("scope") or ""
        if "<" not in sc:
            continue
        try:
            y = M.parse(s["mangled"])
        except M.MangleError:
            continue
        scn = y.name.scope()
        if scn.last.targs is None or any(p.targs for p in scn.parts[:-1]):
            continue
        if path_of(scn) in SKIP:
            continue
        by_scope[path_of(scn)].append((s, y, M.Speller().name(scn)))
    n = 0
    for g in db.get("globals", []):
        if g.get("mangled") or not g["file"].startswith(TREE) or "::" not in g["qualified"]:
            continue
        scope, _, name = g["qualified"].rpartition("::")
        path = _plain(scope)
        place = (g.get("redecls") or [{"file": g["file"], "line": g["line"]}])[0]
        for s, y, spec in sorted(by_scope.get(path, ()), key=lambda r: r[0]["addr"]):
            if s["kind"] == "data" and y.name.last.name == name:
                add[place["file"]][place["line"]].append(f"/// {s['addr']} {spec}")
                n += 1
    for f in db.get("functions", []):
        if not f["file"].startswith(TREE) or not f.get("scope") or f.get("mangled"):
            continue
        path = _plain(f["scope"])
        rows = by_scope.get(path)
        if not rows:
            continue
        lines = []
        groups = defaultdict(dict)
        for s, y, spec in rows:
            last = y.name.last
            if f["kind"] in ("ctor", "dtor"):
                if last.kind != f["kind"]:
                    continue
            elif last.kind != "name" or last.name != f["name"] or y.name.last.targs is not None:
                continue
            if len(y.params) != _nparams(f) or bool(y.const) != bool(f.get("const")):
                continue
            groups[spec][s.get("variant") or "-"] = s["addr"]
        for spec in sorted(groups):
            g = groups[spec]
            if f["kind"] in ("ctor", "dtor"):
                c1 = g.get("C1") or g.get("D1")
                c2 = g.get("C2") or g.get("D2")
                if c1:
                    lines.append(f"/// {c1} {spec}")
                if c2 and c2 != c1:
                    lines.append(f"/// base {c2} {spec}")
                if g.get("D0"):
                    lines.append(f"/// deleting {g['D0']} {spec}")
            elif g.get("-"):
                lines.append(f"/// {g['-']} {spec}")
        if lines:
            add[f["file"]][f["line"]].extend(lines)
            n += sum(1 for ln in lines if not ln.startswith(("/// base", "/// deleting")))
    return add, n


def apply(text, by_line):
    """Insert marker lines above the given lines (with their indentation); no-code markers right above go."""
    lines = text.split("\n")
    out = []
    for i, ln in enumerate(lines, 1):
        new = by_line.get(i)
        if new:
            ind = re.match(r"^\s*", ln).group(0)
            # the block of markers right above the declaration: a no-code marker gives way
            k = len(out)
            while k > 0 and out[k - 1].lstrip().startswith("///") and not out[k - 1].lstrip().startswith("///<"):
                k -= 1
            block = out[k:]
            block = [b for b in block if not NO_CODE.match(b)]
            have = {b.strip() for b in block}
            block += [ind + x for x in new if x not in have]
            out[k:] = block
        out.append(ln)
    return "\n".join(out)
