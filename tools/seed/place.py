"""The file of each generated top-level class (and of free functions and globals)."""
import fnmatch
from bisect import bisect_left
from collections import defaultdict
from pathlib import Path

DATA = Path(__file__).resolve().parent / "data"
SDK_ROOT = "d:/firaxis_oldserver/civ5expansion2/"
HEADER_EXT = (".h", ".inl")
LIBRARY_BLOCKS = (   # (start, end, folder): libraries whose sources have no path evidence
    (0x08b94000, 0x08c59000, "Civ5/Src/CvGameDatabase"),
    (0x08c5a000, 0x08c6a000, "Civ5/Src/CvLocalization"),
    (0x08c99000, 0x08cae000, "Civ5/Src/App/CvWorldBuilderMap"),
)


def load_evidence(data=DATA):
    """basename (lower, no extension) -> sorted [(path relative to Civ5Expansion2/, ext)]; proven set (lower)."""
    by = defaultdict(set)
    for ln in (data / "source_paths_unique.txt").read_text(encoding="utf-8").splitlines():
        f = ln.split("\t")
        if len(f) < 2 or not f[0].startswith("ROOT/") or "<unresolved>" in f[0]:
            continue
        tags = [t for t in f[1].strip().split(",") if t and "?" not in t and "~" not in t]
        if tags:
            _add(by, f[0][len("ROOT/"):])
    for ln in (data / "sdk_lib_paths.txt").read_text(encoding="utf-8").splitlines():
        if ln.startswith("#") or not ln.strip():
            continue
        f = ln.split("\t")
        low = f[0].strip()
        if low.startswith("sdks/") or not low.endswith(HEADER_EXT + (".cpp",)):
            continue
        orig = f[1].strip() if len(f) > 1 else low
        rel = orig[len(SDK_ROOT):] if orig.lower().startswith(SDK_ROOT) else low
        _add(by, rel)
    return {k: sorted(v) for k, v in by.items()}


def _add(by, rel):
    p = rel.rsplit("/", 1)
    name = p[-1]
    stem, _, ext = name.rpartition(".")
    by[stem.lower()].add((rel, "." + ext.lower()))


def load_rules(data=DATA):
    out = []
    for ln in (data / "module_rules.txt").read_text(encoding="utf-8").splitlines():
        if ln.startswith("#") or not ln.strip():
            continue
        pat, folder = ln.split("\t")
        out.append((pat.strip(), folder.strip().rstrip("/")))
    return out


def _dir(rel):
    return rel.rsplit("/", 1)[0] if "/" in rel else ""


def evidence_place(name, evidence):
    """(path, origin) from path evidence, or None."""
    rows = evidence.get(name.lower())
    if not rows:
        return None
    hs = [r for r, e in rows if e in HEADER_EXT]
    if hs:
        h = sorted(hs, key=lambda r: (not r.endswith(".h"), r))[0]
        return h, "binary"
    cpps = [r for r, e in rows if e == ".cpp"]
    d = _dir(sorted(cpps)[0])
    if d.endswith("FLua/Source"):
        d = d[:-len("Source")] + "Include"
    return f"{d}/{name}.h", "guess"


def rule_folder(qual, rules):
    for pat, folder in rules:
        if fnmatch.fnmatchcase(qual, pat):
            return folder
    return "Civ5/Src/App"


def canon_case(rel, name, dirs):
    """Folder case from the existing tree (or the first spelling seen); a lower-case file name takes the class
    name's case (W and S evidence compare ignoring case)."""
    parts = rel.split("/")
    cur = ""
    for i, p in enumerate(parts[:-1]):
        key = (cur + "/" + p if cur else p).lower()
        if key not in dirs:
            dirs[key] = cur + "/" + p if cur else p
        cur = dirs[key]
    fname = parts[-1]
    stem, _, ext = fname.rpartition(".")
    if stem.lower() == name.lower() and stem == stem.lower():
        fname = name + "." + ext
    return (cur + "/" + fname) if cur else fname


def tree_dirs(tree_root):
    out = {}
    root = Path(tree_root)
    if root.exists():
        for p in sorted(root.rglob("*")):
            if p.is_dir():
                rel = p.relative_to(root).as_posix()
                out.setdefault(rel.lower(), rel)
    return out


def place(units, evidence, rules, decisions, dirs=None):
    """units: [{key, name, qual, addr (int|None)}] -> {key: (path, origin)}; decisions get the method used."""
    dirs = {} if dirs is None else dirs
    out = {}
    anchors = []
    for u in units:
        p = evidence_place(u["name"], evidence)
        if p is not None:
            p = (canon_case(p[0], u["name"], dirs), p[1])
            out[u["key"]] = p
            if u["addr"] is not None:
                anchors.append((u["addr"], _dir(p[0])))
    anchors.sort()
    addrs = [a for a, _ in anchors]
    for u in units:
        if u["key"] in out:
            continue
        folder = None
        a = u["addr"]
        if a is not None and anchors:
            k = bisect_left(addrs, a)
            left = anchors[k - 1][1] if k > 0 else None
            right = anchors[k][1] if k < len(anchors) else None
            if left is not None and left == right:
                folder = left
                decisions["place_neighbors"].append(u["qual"])
            if folder is None:
                for s, e, f in LIBRARY_BLOCKS:
                    if s <= a < e:
                        folder = f
                        decisions["place_library"].append(u["qual"])
                        break
        if folder is None:
            folder = rule_folder(u["qual"], rules)
            decisions["place_rule"].append(u["qual"])
        out[u["key"]] = (canon_case(f"{folder}/{u['name']}.h", u["name"], dirs), "guess")
    # names in one folder that match ignoring case share one file
    first = {}
    for key in sorted(out, key=lambda k: (out[k][0].lower(), out[k][0], repr(k))):
        path, origin = out[key]
        low = path.lower()
        if low in first:
            out[key] = out[first[low]]
        else:
            first[low] = key
    return out
