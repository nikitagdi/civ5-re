"""O1-O4: file origin is present, proven by path evidence (binary) or a guess named after a class."""
import re

from checks import includes
from checks.api import Rule

from .m_markers import DiagRule

TREE = includes.TREE
ORIGIN_RE = re.compile(r"^/// origin (binary|guess)[ \t]*$")
SDK_EXT = (".h", ".inl", ".cpp")


class O1(DiagRule):
    id = "O1"
    title = "the first line is '/// origin binary|guess'"

    def text(self, diag):
        return "no /// origin on the first line"


def _lines(ctx, name):
    return [ln for ln in ctx.evidence(name) if ln.strip() and not ln.startswith("#")]


def _evidence(ctx):
    """(exact, folded): relative path -> proving tags. Exact: any tag without ?/~. Folded (lower case): W*/S tags only."""
    def load():
        exact, folded = {}, {}
        for ln in _lines(ctx, "source_paths"):
            f = ln.split("\t")
            if len(f) < 2 or not f[0].startswith("ROOT/") or "<unresolved>" in f[0]:
                continue
            rel = f[0][len("ROOT/"):]
            proving = [t for t in f[1].strip().split(",") if t and "?" not in t and "~" not in t]
            for t in proving:
                if t not in exact.setdefault(rel, []):
                    exact[rel].append(t)
                if t.startswith("W") and t not in folded.setdefault(rel.lower(), []):
                    folded[rel.lower()].append(t)
        for ln in _lines(ctx, "sdk_lib_paths"):
            low = ln.split("\t")[0].strip()
            if low.startswith("sdks/") or not low.endswith(SDK_EXT):
                continue
            folded.setdefault(low.lower(), [])
            if "S" not in folded[low.lower()]:
                folded[low.lower()].append("S")
        return exact, folded
    return ctx._lazy("evidence:paths", load)


def proof(ctx, rel):
    """Proving tags of a path relative to Civ5Expansion2/; [] when the path is not proven. L is exact-case."""
    exact, folded = _evidence(ctx)
    tags = list(exact.get(rel, ()))
    for t in folded.get(rel.lower(), ()):
        if t not in tags:
            tags.append(t)
    return tags


def origin_of(ctx, path):
    first = ctx.read(path).split("\n", 1)[0].rstrip("\r")
    m = ORIGIN_RE.match(first)
    return m.group(1) if m else None


class O2(Rule):
    id = "O2"
    title = "origin binary: the file's own path is proven by evidence (source paths, SDK .lib paths)"
    needs = ("files",)

    def run(self, ctx):
        out = []
        for p in ctx.scoped(includes.is_tree_source):
            if origin_of(ctx, p) == "binary" and not proof(ctx, p[len(TREE):]):
                out.append(self.finding(p, 1, "origin binary, but the path is not proven by evidence", key=f"O2:{p}"))
        return out


class O3(Rule):
    id = "O3"
    title = "origin guess: the path must not be proven by evidence"
    needs = ("files",)

    def run(self, ctx):
        out = []
        for p in ctx.scoped(includes.is_tree_source):
            if origin_of(ctx, p) == "guess":
                tags = proof(ctx, p[len(TREE):])
                if tags:
                    out.append(self.finding(p, 1, f"path is in evidence ({','.join(tags)}) - origin binary needed",
                                            key=f"O3:{p}"))
        return out


def _strip_args(name):
    depth, out = 0, []
    for ch in name:
        if ch == "<":
            depth += 1
        elif ch == ">":
            depth = max(0, depth - 1)
        elif depth == 0:
            out.append(ch)
    return "".join(out)


def top_level_names(db):
    """file -> lower-case names of the top-level classes and template families declared in it."""
    recs = [r for r in db.get("records", ()) if r.get("origin") == "tree"]
    plain = {_strip_args(r["name"]) for r in recs}
    out = {}
    for r in recs:
        name = _strip_args(r["name"])
        head, _, last = name.rpartition("::")
        if "(" in last or not last or (head and head in plain):
            continue
        out.setdefault(r["file"], set()).add(last.lower())
    return out


class O4(Rule):
    id = "O4"
    title = "origin guess: the file name equals a top-level class or template family of the file (.cpp: of its .h)"
    needs = ("typedb", "files")

    def run(self, ctx):
        db = ctx.typedb
        names = top_level_names(db)
        exempt = {f[1].strip().lower() for f in (ln.split("\t") for ln in _lines(ctx, "sdk_map")) if len(f) > 1}
        out = []
        for f in db.get("files", ()):
            p = f["path"]
            if f.get("origin") != "guess" or f.get("kind") not in ("h", "cpp") or not p.startswith(TREE) \
                    or not ctx.in_scope(p) or p[len(TREE):].lower() in exempt:
                continue
            stem = p.rsplit("/", 1)[-1].rsplit(".", 1)[0]
            target = p if f["kind"] == "h" else p[:-len(".cpp")] + ".h"
            if target[len(TREE):].lower() in exempt:
                continue    # the bodies of an SDK copy keep the SDK's file name

            if stem.lower() not in names.get(target, ()):
                where = "the file" if target == p else target.rsplit("/", 1)[-1]
                out.append(self.finding(p, 1, f"origin guess, but there is no class {stem} in {where}", key=f"O4:{p}"))
        return out
