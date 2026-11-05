"""P1/P2: the .cpp side agrees with the .h side."""
from checks.api import Rule

from .m_markers import DiagRule


class P2(DiagRule):
    id = "P2"
    title = "cpp-undeclared: a body in .cpp without a declaration in the tree's .h"
    scope = "entity"


def _primary(m, with_inline):
    """(kind, address, fastcc) of the marker that belongs above a .cpp body: address, lone base or inline."""
    if m.get("addr"):
        return ("addr", m["addr"], bool(m.get("fastcc")))
    if m.get("base"):
        return ("base", m["base"], False)
    if with_inline and m.get("inline"):
        return ("inline", None, False)
    return None


def _describe(p):
    if p is None:
        return "no marker"
    kind, value, fast = p
    return {"addr": value, "base": f"base {value}", "inline": "inline"}[kind] + (" fastcc" if fast else "")


class P1(Rule):
    id = "P1"
    title = "the primary marker above a .cpp body equals the declaration marker in the .h (matched by mangled name)"
    needs = ("typedb",)
    scope = "entity"
    deep = True

    def run(self, ctx):
        db = ctx.typedb
        funcs = {f["key"]: f for f in db.get("functions", ())}
        globs = {g["mangled"]: g for g in db.get("globals", ()) if g.get("mangled")}
        out = []
        for c in db.get("cpp_defs", ()):
            path = c["file"]
            if not ctx.in_scope(path):
                continue
            f = funcs.get(c.get("function")) if c.get("function") else None
            if f is not None:
                decl, where = f["markers"], f"{f['file']}:{f['line']}"
            elif c.get("mangled") in globs:
                g = globs[c["mangled"]]
                decl = {"addr": g.get("addr_marker"), "inline": bool(g.get("inline_marker"))}
                where = f"{g['file']}:{g['line']}"
            else:
                continue
            pairs = [(c["markers"], decl, None)]
            decl_inst = {i["spec"]: i for i in (f or {}).get("instances", ())}
            for ci in c.get("instances") or ():
                pairs.append((ci, decl_inst.get(ci["spec"]) or {}, ci["spec"]))
            for cm, hm, spec in pairs:
                a, b = _primary(cm, spec is None), _primary(hm, spec is None)
                if a is None or b is None or a == b:
                    continue
                what = f" for {spec}" if spec else ""
                out.append(self.finding(
                    path, c["line"], f"{_describe(a)}, in .h {_describe(b)}{what} ({where})",
                    entity=c.get("qualified"), mangled=c.get("mangled"), expected=_describe(b), actual=_describe(a),
                    key=f"P1:{c.get('mangled')}:{spec or ''}"))
        return out

