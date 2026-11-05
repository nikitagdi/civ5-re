"""Scaffold headers from the model (includes, forward declarations, classes, markers)."""
import re
from collections import defaultdict

from seed import mangle as M
from seed.model import is_std, path_of

TREE = "Civ5Expansion2/"
IND = "    "
STD_HEADERS = {   # libc++ name (after std::__1::) -> header; checks compile each header alone, without the pch
    "list": "list", "__list_node": "list", "__list_iterator": "list", "__list_const_iterator": "list",
    "vector": "vector", "__wrap_iter": "vector", "__bit_iterator": "vector",
    "map": "map", "multimap": "map", "__map_iterator": "map", "__map_const_iterator": "map",
    "__value_type": "map", "__tree_node": "map", "__tree_iterator": "map", "__tree_const_iterator": "map",
    "__tree_end_node": "map", "set": "set", "multiset": "set",
    "basic_string": "string", "char_traits": "string", "string": "string", "allocator": "memory",
    "unordered_map": "unordered_map", "unordered_multimap": "unordered_map", "__hash_node": "unordered_map",
    "__hash_iterator": "unordered_map", "__hash_const_iterator": "unordered_map",
    "__hash_map_iterator": "unordered_map", "__hash_map_const_iterator": "unordered_map",
    "__hash_value_type": "unordered_map", "unordered_set": "unordered_set", "pair": "utility",
    "deque": "deque", "__deque_iterator": "deque", "basic_istream": "istream", "basic_ostream": "ostream",
    "basic_iostream": "istream", "basic_ios": "ios", "basic_ifstream": "fstream", "basic_ofstream": "fstream",
    "basic_fstream": "fstream", "basic_filebuf": "fstream", "basic_stringstream": "sstream",
    "basic_ostringstream": "sstream", "basic_istringstream": "sstream", "function": "functional",
    "less": "functional", "equal_to": "functional", "hash": "functional", "bitset": "bitset",
    "shared_ptr": "memory", "auto_ptr": "memory", "unique_ptr": "memory", "default_delete": "memory",
}


class EmitError(Exception):
    pass


def hexs(n):
    return "0x%x" % n


def tparam_decl(kinds, sp, model=None, names=True):
    out = []
    for k, (kind, t) in enumerate(kinds):
        n = f" T{k}" if names else ""
        if kind == "class":
            out.append("class" + n)
        elif kind == "template":
            inner = model.scopes[t].tparams if model is not None else []
            out.append(tparam_decl(inner, sp, model, names=False) + " class" + n)
        else:
            out.append(sp.type(t) + n)
    return "template <" + ", ".join(out) + ">"


class Unit:
    """A top-level generated class (or a namespace-level holder of free entities)."""

    def __init__(self, path):
        self.path = path
        self.file = None
        self.origin = None


class Emitter:
    def __init__(self, model, existing, foreign, report):
        self.m, self.ex, self.fo, self.report = model, existing, foreign, report
        self.sp = M.Speller()
        self.file_of = {}            # top-level class path -> file
        self.free = defaultdict(lambda: {"fns": [], "vars": []})   # file -> namespace path -> entities

    # -- structure ----------------------------------------------------------------------------------------
    def kind(self, path):
        sc = self.m.scopes.get(path)
        return sc.kind if sc is not None else None

    def top_class(self, path):
        """Outermost class of a path whose parents are namespaces: the unit that defines it."""
        for k in range(1, len(path) + 1):
            if self.kind(path[:k]) in ("class", "enum", "existing", "foreign"):
                return path[:k]
        return None

    def children(self, path):
        return sorted(p for p in self.m.scopes if len(p) == len(path) + 1 and p[:-1] == path)

    def needs_unit(self, sc):
        if sc.kind != "class":
            return False
        if sc.fns or sc.vars or sc.dynamic or sc.typeinfo or sc.bases or sc.tparams is not None:
            return True
        return any(self.kind(c) in ("class", "enum") for c in self.children(sc.path))

    def units(self):
        out = []
        for path, sc in sorted(self.m.scopes.items()):
            if not path or sc.kind != "class":
                continue
            par = path[:-1]
            if par and self.kind(par) != "namespace":
                continue
            if self.needs_unit(sc) or any(self.needs_unit(self.m.scopes[c]) for c in self._desc(path)):
                out.append(path)
        return out

    def _desc(self, path):
        return [p for p in self.m.scopes if len(p) > len(path) and p[:len(path)] == path]

    # -- references -----------------------------------------------------------------------------------------
    def fn_types(self, f):
        out = list(f.params or ())
        if f.ret is not None:
            out.append(f.ret)
        if f.part is not None and f.part.conv is not None:
            out.append(f.part.conv)
        if f.ftemplate:
            out.extend(a.type if isinstance(a, M.Lit) else a for a in f.ftemplate)
            for i in f.insts:
                out.extend(a.type if isinstance(a, M.Lit) else a for a in i.targs or ())
        return out

    def refs_of_class(self, path, deep=True):
        """(names used, base names) by the class at path and its nested classes."""
        names, bases = [], []
        todo = [path] + (sorted(self._desc(path)) if deep else [])
        for p in todo:
            sc = self.m.scopes[p]
            for f in sc.fns:
                for t in self.fn_types(f):
                    names.extend(M.names_in(t))
            for spec, targs in sc.insts.items():
                for a in targs:
                    names.extend(M.names_in(a.type if isinstance(a, M.Lit) else a))
            if sc.tparams:
                for kind, t in sc.tparams:
                    if t is not None:
                        names.extend(M.names_in(t))
            for _, bn, *_ in sc.bases:
                bases.append(bn)
                names.extend(M.names_in(bn))
            for b in getattr(sc, "tbases", None) or ():
                names.extend(M.names_in(b[0]))
                if isinstance(b[0], M.Named):
                    bases.append(b[0].name)
        return names, bases

    # -- placement of free entities ------------------------------------------------------------------------
    # done by the caller (tree.py) through `self.free`

    # -- file text -----------------------------------------------------------------------------------------
    def file_text(self, rel, origin, unit_paths, free):
        """Text of one generated file."""
        includes, fwd = self._collect(TREE + rel, unit_paths, free)
        lines = [f"/// origin {origin}", "#pragma once"]
        for inc in sorted(includes):
            lines.append(f"#include {inc}")
        fl = self._fwd_lines(fwd)
        if fl:
            lines.append("")
            lines.extend(fl)
        for up in self._order(unit_paths):
            lines.append("")
            lines.extend(self._wrap_ns(up[:-1], self.class_lines(up, 0)))
        for ns in sorted(free):
            ents = free[ns]
            body = self.free_lines(ns, ents)
            if body:
                lines.append("")
                lines.extend(self._wrap_ns(ns, body) if ns else body)
        return "\n".join(lines) + "\n"

    def _collect(self, here, unit_paths, free, owner=None):
        includes, fwd = set(), defaultdict(set)    # namespace path -> {decl}
        defined = set()
        for up in unit_paths:
            defined.add(up)
            defined.update(self._desc(up))
        used, base_names = [], []
        for up in unit_paths:
            n, b = self.refs_of_class(up)
            used.extend(n)
            base_names.extend(b)
        for ns, ents in sorted(free.items()):
            for f in ents["fns"]:
                for t in self.fn_types(f):
                    used.extend(M.names_in(t))
        base_paths = {path_of(b) for b in base_names}
        problems = []
        for n in used:
            p = path_of(n)
            if is_std(p):
                key = p[2] if len(p) > 2 and p[1] == "__1" else (p[1] if len(p) > 1 else "")
                includes.add(f"<{STD_HEADERS.get(key, 'memory')}>")
                continue
            if not p or (owner is not None and p[:len(owner)] == owner):
                continue
            self._ref(p, n, here, defined, base_paths, includes, fwd, problems)
        for b in base_names:
            p = path_of(b)
            if is_std(p) or (owner is not None and p[:len(owner)] == owner):
                continue
            self._ref(p, b, here, defined, base_paths, includes, fwd, problems, need_complete=True)
        for pr in problems:
            self.report["problems"].append(f"{here}: {pr}")
        return includes, fwd

    def _ref(self, p, n, here, defined, base_paths, includes, fwd, problems, need_complete=False):
        sc = self.m.scopes.get(p)
        if sc is None:
            problems.append(f"unknown type {'::'.join(p)}")
            return
        top = self.top_class(p)
        if sc.kind == "existing" or (top is not None and self.kind(top) == "existing"):
            owner = p if sc.kind == "existing" else top
            f = self.ex.file(owner) or self.ex.file(p)
            if f is None:
                problems.append(f"existing {'::'.join(p)} without a file")
            elif f != here:
                includes.add(f'"{f[len(TREE):]}"')
            if sc.kind != "existing" and not self.m.scopes[owner].handwritten:
                problems.append(f"nested in an existing class: {'::'.join(p)}")
            return
        if sc.kind == "foreign" or (top is not None and self.kind(top) == "foreign"):
            inc = self.fo.include(top or p)
            if inc:
                includes.add(inc)
            return
        if sc.kind == "namespace":
            problems.append(f"namespace used as a type: {'::'.join(p)}")
            return
        if p in defined and not need_complete:
            fwd_decl = self._fwd(p)
            if fwd_decl and top == p:
                fwd[p[:-1]].add(fwd_decl)
            return
        if p in defined:
            return
        if top is not None and top != p:
            # nested type: the outer class must be complete
            f = self.file_of.get(top)
            if f is None:
                problems.append(f"no file for {'::'.join(top)}")
            elif TREE + f != here:
                includes.add(f'"{f}"')
            return
        if need_complete:
            f = self.file_of.get(p)
            if f is None:
                problems.append(f"no file for base {'::'.join(p)}")
            elif TREE + f != here:
                includes.add(f'"{f}"')
            return
        d = self._fwd(p)
        if d:
            fwd[p[:-1]].add(d)

    def _fwd(self, p):
        sc = self.m.scopes[p]
        if sc.kind == "enum":
            return f"enum {p[-1]} : int;"
        if sc.kind != "class":
            return None
        if sc.tt_header and sc.tparams is None:
            return f"{sc.tt_header} class {p[-1]};"
        if sc.tparams is not None:
            return f"{tparam_decl(sc.tparams, self.sp, self.m)} class {p[-1]};"
        return f"class {p[-1]};"

    def _fwd_lines(self, fwd):
        out = []
        for ns in sorted(fwd):
            decls = sorted(fwd[ns])
            out.extend(self._wrap_ns(ns, decls))
        return out

    @staticmethod
    def _wrap_ns(ns, body):
        if not ns:
            return list(body)
        head = " ".join(f"namespace {n} {{" for n in ns)
        tail = "}" * len(ns)
        return [head] + list(body) + [tail]

    def _order(self, units):
        """Units of one file: bases (and outer classes of used nested types) first."""
        units = sorted(units)
        deps = {}
        for u in units:
            _, bases = self.refs_of_class(u)
            names, _ = self.refs_of_class(u)
            d = set()
            for b in bases:
                t = self.top_class(path_of(b))
                if t in units and t != u:
                    d.add(t)
            for n in names:
                p = path_of(n)
                t = self.top_class(p)
                if t in units and t != u and t != p:
                    d.add(t)
            deps[u] = d
        out, seen, busy = [], set(), set()

        def visit(u):
            if u in seen or u in busy:
                return
            busy.add(u)
            for d in sorted(deps[u]):
                visit(d)
            busy.discard(u)
            seen.add(u)
            out.append(u)
        for u in units:
            visit(u)
        return out

    # -- classes ---------------------------------------------------------------------------------------
    def class_lines(self, path, depth):
        sc = self.m.scopes[path]
        ind = IND * depth
        if sc.kind == "enum":
            return [f"{ind}enum {path[-1]} : int;"]
        if not self.needs_unit(sc) and not any(self.needs_unit(self.m.scopes[c]) for c in self._desc(path)):
            if sc.tparams is not None:
                return [f"{ind}{tparam_decl(sc.tparams, self.sp, self.m)} class {path[-1]};"]
            return [f"{ind}class {path[-1]};"]
        out = []
        out.append(f"{ind}/// size ?")
        if sc.tparams is not None:
            for spec in sorted(sc.vtables):
                out.append(f"{ind}/// vtable {sc.vtables[spec]} {spec}")
            if sc.dynamic and not sc.vtables:
                out.append(f"{ind}/// vtable ?")
        elif sc.dynamic or sc.vtable:
            out.append(f"{ind}/// vtable {sc.vtable or '?'}")
        if sc.lua_class:
            out.append(f"{ind}/// lua ?")
        head = f"class {path[-1]}"
        bases = self._bases(sc)
        if bases:
            head += " : " + ", ".join(bases)
        if sc.tparams is not None:
            out.append(f"{ind}{tparam_decl(sc.tparams, self.sp, self.m)}")
        out.append(f"{ind}{head} {{")
        out.append(f"{ind}public:")
        body = self.body_lines(path, depth)
        out.extend(body)
        out.append(f"{ind}}};")
        # out-of-class definitions of static data members of a class template (their SPEC markers)
        if sc.tparams is not None:
            for v in sorted(sc.vars, key=lambda v: v.name):
                out.extend(self.tvar_def(sc, v, ind))
        return out

    def body_lines(self, path, depth):
        """Members of the class at path (nested classes first), indented for a class body at `depth`."""
        sc = self.m.scopes[path]
        ind = IND * depth
        body = []
        kids = [c for c in self.children(path) if self.kind(c) in ("class", "enum")]
        for c in kids:
            d = self._fwd(c)
            if d and self.kind(c) == "class" and (self.needs_unit(self.m.scopes[c]) or self._desc(c)):
                body.append(ind + IND + d)
        for c in self._order_nested(kids):
            body.extend(self.class_lines(c, depth + 1))
        if sc.pad:
            off, n = sc.pad
            body.append(f"{ind}{IND}char unk_{off:x}[{hexs(n)}];    ///< +{hexs(off)}")
        for f in self._fn_order(sc):
            body.extend(self.fn_lines(sc, f, depth + 1))
        for v in sorted(sc.vars, key=lambda v: v.name):
            body.extend(self.var_lines(sc, v, depth + 1, member=True))
        return body

    def amend_text(self, rel, text, path, line):
        """A hand-written class lacking binary members gets their declarations appended
        before its end (and the includes or forward declarations they need after `#pragma once`)."""
        lines = text.split("\n")
        depth, end, seen = 0, None, False
        for i in range(line - 1, len(lines)):
            code = lines[i].split("//")[0]
            for ch in code:
                if ch == "{":
                    depth += 1
                    seen = True
                elif ch == "}":
                    depth -= 1
            if seen and depth == 0:
                end = i
                break
        if end is None:
            self.report["problems"].append(f"{rel}: end of {'::'.join(path)} not found")
            return text
        cind = re.match(r"^\s*", lines[end]).group(0)
        depth_n = len(cind.expandtabs(4)) // 4
        body = self.body_lines(path, depth_n)
        if not body:
            return text
        includes, fwd = self._collect(TREE + rel, [path], {}, owner=path)
        head = [f"#include {inc}" for inc in sorted(includes) if f"#include {inc}" not in lines]
        head += self._fwd_lines(fwd)
        lines[end:end] = body
        k = next((i for i, ln in enumerate(lines) if ln.strip() == "#pragma once"), 0)
        lines[k + 1:k + 1] = head
        return "\n".join(lines)

    def _order_nested(self, kids):
        kids = sorted(kids)
        deps = {}
        for k in kids:
            sc = self.m.scopes[k]
            deps[k] = {path_of(bn) for _, bn, *_ in sc.bases if path_of(bn) in kids}
        out, seen = [], set()

        def visit(k):
            if k in seen:
                return
            seen.add(k)
            for d in sorted(deps[k]):
                visit(d)
            out.append(k)
        for k in kids:
            visit(k)
        return out

    def _bases(self, sc):
        out = []
        if sc.tparams is not None:
            for t, off, pub, virt in getattr(sc, "tbases", None) or ():
                out.append(("virtual " if virt else "") + ("public " if pub else "private ") + self.sp.type(t))
            return out
        for _, bn, off, pub, virt in sc.bases:
            out.append(("virtual " if virt else "") + ("public " if pub else "private ") + self.sp.name(bn))
        return out

    def _fn_order(self, sc):
        def key(f):
            # new virtual entries (the destructor too) in slot order: the declaration order makes the slot order
            if f.virtual and f.slot is not None:
                return (2, 0, f.slot, repr(f.part))
            if f.virtual:
                return (1, 0, 0, repr(f.part))
            k = {"ctor": 0, "dtor": 1}.get(f.kind, 3)
            return (k, 1, f.order, repr((f.part, f.params)))
        fns = sorted(sc.fns, key=key)
        # virtual new entries must keep slot order relative to each other; others may interleave
        return fns

    # -- functions -------------------------------------------------------------------------------------
    def fn_markers(self, sc, f):
        out = []
        tmpl = sc.tparams is not None or sc.in_template is not None or f.ftemplate is not None
        any_code = False
        for i in sorted(f.insts, key=lambda i: (i.spec or "", i.addr or "", i.base or "")):
            suf = f" {i.spec}" if (tmpl and i.spec) else ""
            if i.addr:
                out.append(f"/// {i.addr}{' fastcc' if i.fastcc else ''}{suf}")
                any_code = True
            if i.base:
                out.append(f"/// base {i.base}{suf}")
                any_code = True
            if i.deleting:
                out.append(f"/// deleting {i.deleting}{suf}")
            for t in sorted(i.thunks):
                out.append(f"/// thunk {t}{suf}")
        if f.pure and not any_code:
            out = ["/// pure"]
        elif not any_code:
            out = ["/// absent"]
            self.report["decisions"]["absent_generated"].append(self._qual(sc, f))
        if f.lua:
            out.append("/// lua ?")
        return out

    def _qual(self, sc, f):
        return "::".join(sc.path + (self.fn_name(sc, f),))

    def fn_name(self, sc, f):
        p = f.part
        if p.kind == "ctor":
            return sc.path[-1]
        if p.kind == "dtor":
            return "~" + sc.path[-1]
        return self.sp.part(p, sc.path[-1] if sc.path else None)

    def fn_decl(self, sc, f, member):
        name = self.fn_name(sc, f)
        tp = tuple(f"T{k}" for k in range(len(f.ftemplate))) if f.ftemplate else None
        sp = M.Speller(tparams=tp) if tp else self.sp
        if f.params is None:
            params = "UNK_PARAMS"
        else:
            params = sp.params(f.params, f.variadic, [f"a{k + 1}" for k in range(len(f.params))])
        const = " const" if f.const else ""
        if f.kind in ("ctor", "dtor") or f.part.kind == "cv":
            decl = f"{name}({params}){const}"
        elif f.ret_text:
            decl = f"{f.ret_text} {name}({params}){const}"
        elif f.ret is not None:
            decl = sp.type(f.ret, f" {name}({params}){const}")
        else:
            decl = f"UNK_RET {name}({params}){const}"
        pre = ""
        if member and f.static:
            pre = "static "
        if member and f.virtual:
            pre = "virtual "
        tail = " = 0" if (f.pure and member) else ""
        return pre + decl + tail + ";"

    def fn_lines(self, sc, f, depth):
        ind = IND * depth
        out = [ind + m for m in self.fn_markers(sc, f)]
        if f.ftemplate is not None:
            out.append(ind + tparam_decl([("value", a.type) if isinstance(a, M.Lit) else ("class", None)
                                          for a in f.ftemplate], self.sp))
        out.append(ind + self.fn_decl(sc, f, member=(sc.kind == "class" or sc.handwritten)))
        return out

    # -- data ------------------------------------------------------------------------------------------
    def var_lines(self, sc, v, depth, member):
        ind = IND * depth
        out = []
        if sc.tparams is None and sc.in_template is None:
            for i in v.insts:
                out.append(f"{ind}/// {i.addr}")
        kw = "static " if member else "extern "
        out.append(f"{ind}{kw}UNK_DATA {v.name}[{hexs(v.size)}];")
        return out

    def tvar_def(self, sc, v, ind):
        out = [f"{ind}/// {i.addr} {i.spec}" for i in sorted(v.insts, key=lambda i: i.spec or "")]
        args = ", ".join(f"T{k}" for k in range(len(sc.tparams)))
        out.append(f"{ind}{tparam_decl(sc.tparams, self.sp, self.m)}")
        out.append(f"{ind}UNK_DATA {sc.path[-1]}<{args}>::{v.name}[{hexs(v.size)}];")
        return out

    def free_lines(self, ns, ents):
        out = []
        cfns = [f for f in ents["fns"] if f.kind == "c"]
        cvars = [v for v in ents["vars"] if v.c]
        sc = self.m.scopes[ns]
        for f in sorted((f for f in ents["fns"] if f.kind != "c"), key=lambda f: (f.order, repr(f.part))):
            out.extend(self.fn_lines(sc, f, 0))
        for v in sorted((v for v in ents["vars"] if not v.c), key=lambda v: v.name):
            out.extend(self.var_lines(sc, v, 0, member=False))
        if cfns or cvars:
            out.append('extern "C" {')
            for f in sorted(cfns, key=lambda f: f.part.name):
                out.extend(self.fn_lines(sc, f, 1))
            for v in sorted(cvars, key=lambda v: v.name):
                out.extend(self.var_lines(sc, v, 1, member=False))
            out.append("}")
        return out
