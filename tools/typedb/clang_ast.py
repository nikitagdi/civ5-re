"""JSON AST of clang: pruned loading, delta-encoded `file`/`line`, `expansionLoc`, declaration model."""
from __future__ import annotations

import json
import os
import re

from typedb.markers import args_start as mk_args_start, normalize_spec, split_top

_PRUNE_SUFFIX = ("Stmt", "Expr", "Literal", "Operator", "CXXCtorInitializer", "Cleanups")
_KEEP_STMT = ("CompoundStmt", "CXXTryStmt")


def _prunable(kind: str) -> bool:
    return kind.endswith(_PRUNE_SUFFIX) or kind == "CXXCtorInitializer"


def _effect(o, st):
    """Apply the location-state changes of `o` (document order) to st = [file, line]."""
    if isinstance(o, dict):
        e = o.get("__eff")
        if e is not None:
            if e[0] is not None:
                st[0] = e[0]
            if e[1] is not None:
                st[1] = e[1]
            return
        if "offset" in o:
            f = o.get("file")
            if f is not None:
                st[0] = f
            ln = o.get("line")
            if ln is not None:
                st[1] = ln
            return
        for k, v in o.items():
            if k != "includedFrom" and isinstance(v, (dict, list)):
                _effect(v, st)
    elif isinstance(o, list):
        for v in o:
            if isinstance(v, (dict, list)):
                _effect(v, st)


def _hook(d):
    kind = d.get("kind")
    if kind is not None and _prunable(kind):
        st = [None, None]
        _effect(d, st)
        stub = {"kind": kind, "__eff": (st[0], st[1])}
        if kind == "ConstantExpr" and "value" in d:
            stub["value"] = d["value"]
        if kind in _KEEP_STMT:
            stub["body"] = True
        if "inner" in d:
            for c in d["inner"]:
                if "value" in c and c.get("kind") == "ConstantExpr":
                    stub.setdefault("value", c["value"])
        return stub
    return d


def load_json(path):
    with open(path, "rb") as f:
        return json.load(f, object_hook=_hook)


def loads_json(text):
    return json.loads(text, object_hook=_hook)


class Loc:
    __slots__ = ("file", "line", "col", "macro", "sfile", "sline")

    def __init__(self, file, line, col, macro, sfile, sline):
        self.file, self.line, self.col, self.macro, self.sfile, self.sline = file, line, col, macro, sfile, sline

    def __repr__(self):
        return f"Loc({self.file}:{self.line}:{self.col}{' macro' if self.macro else ''})"


def _bare(d, st):
    if not d or "offset" not in d:
        return None
    f = d.get("file")
    if f is not None:
        st[0] = f
    ln = d.get("line")
    if ln is not None:
        st[1] = ln
    return st[0], st[1], d.get("col")


def decode_loc(d, st):
    if not d:
        return None
    if "spellingLoc" in d:
        s = _bare(d["spellingLoc"], st)
        e = _bare(d.get("expansionLoc"), st)
        if e is None:
            return None
        return Loc(e[0], e[1], e[2], True, s[0] if s else None, s[1] if s else None)
    b = _bare(d, st)
    if b is None:
        return None
    return Loc(b[0], b[1], b[2], False, b[0], b[1])


def decode(root):
    """Decode delta-encoded locations in place: kept nodes get `_loc`, `_begin`, `_end`."""
    st = [None, None]
    stack = [(root, False)]
    while stack:
        node, done = stack.pop()
        if done:
            continue
        if "__eff" in node:
            _effect(node, st)
            continue
        inner = None
        for k, v in list(node.items()):
            if k == "loc":
                node["_loc"] = decode_loc(v, st)
            elif k == "range":
                node["_begin"] = decode_loc(v.get("begin"), st)
                node["_end"] = decode_loc(v.get("end"), st)
            elif k == "inner":
                inner = v
            elif isinstance(v, (dict, list)) and not k.startswith("_"):
                _effect(v, st)
        if inner:
            for c in reversed(inner):
                stack.append((c, False))
    return root


# ---------------------------------------------------------------------------------------------
# Declaration model

class Decl:
    """One declaration of the tree, as typedb needs it (no clang ids leave this module)."""

    def __init__(self, **kw):
        self.ent = None
        self.ast_kind = None
        self.name = None
        self.qualified = None
        self.scope = ""
        self.scope_kind = "namespace"
        self.file = None
        self.begin = None
        self.end = None
        self.line = None
        self.end_line = None
        self.macro = False
        self.parent = None
        self.node = None
        self.template = None
        self.in_pattern = False
        self.anon_ns = False
        self.extern_c = False
        self.access = None
        self.primary = None
        self.redecls = []
        self.children = []
        self.instantiations = []
        self.pattern_name = None
        self.template_args = None
        self.spec_args = None
        self.__dict__.update(kw)

    def __repr__(self):
        return f"Decl({self.ent} {self.qualified} {self.file}:{self.line})"


NONMARKABLE_KINDS = {
    "NamespaceDecl", "LinkageSpecDecl", "UsingDirectiveDecl", "UsingDecl", "UsingShadowDecl",
    "StaticAssertDecl", "NamespaceAliasDecl", "FriendDecl", "EmptyDecl", "AccessSpecDecl",
    "UsingEnumDecl", "UnresolvedUsingValueDecl", "UnresolvedUsingTypenameDecl",
}
FUNC_KINDS = {"FunctionDecl", "CXXMethodDecl", "CXXConstructorDecl", "CXXDestructorDecl", "CXXConversionDecl"}
TPARM_KINDS = {"TemplateTypeParmDecl", "NonTypeTemplateParmDecl", "TemplateTemplateParmDecl"}
RECORD_KINDS = {"CXXRecordDecl", "RecordDecl"}


def template_arg_text(node):
    if "type" in node:
        return node["type"].get("qualType", "")
    if "value" in node:
        return str(node["value"])
    for c in node.get("inner", ()):
        if c.get("kind") == "ConstantExpr" and "value" in c:
            return str(c["value"])
        if "value" in c:
            return str(c["value"])
    return "?"


def cast_nt_args(spec, nt_types, enums):
    """`FArr<char,3>` -> `FArr<char,(eMPoolType)3>`: clang names a non-type enum argument with the cast, and a bare
    integer does not convert to an enum. Both spellings of a SPEC are accepted; this is the canonical one."""
    i = mk_args_start(spec)
    if i < 0 or not spec.endswith(">"):
        return spec
    args = split_top(spec[i + 1:-1])
    if len(args) != len(nt_types):
        return spec
    out = []
    for a, t in zip(args, nt_types):
        a = a.strip()
        if t and re.fullmatch(r"-?\d+", a):
            base = re.sub(r"(const|volatile)|[&*]", "", t).strip()
            q = enums.get(base.split("::")[-1])
            if q and (len(q) == 1 or base in q):
                a = f"({base if base in q else next(iter(q))}){a}"
        out.append(a)
    return spec[:i + 1] + ",".join(out) + ">"


class Model:
    def __init__(self):
        self.decls = []
        self.by_id = {}
        self.files_seen = set()


_PARAM = re.compile(r"type-parameter-(\d+)-(\d+)")


class Extractor:
    def __init__(self, path_map, source=None):
        self.path_map = path_map
        self.source = source  # rel path -> file text, for template arguments as written
        self._paths = {}
        self.m = Model()
        self.scopes = {}  # DeclContext id -> semantic scope [(name, kind)]
        self._pending_insts = []  # (pattern id, instantiation name)

    def _written_args(self, node, name):
        """`<…>` after the name of a partial specialization, as written (normalized); None if not found."""
        loc = node.get("_loc")
        if self.source is None or loc is None or loc.macro or loc.col is None:
            return None
        rel = self.rel(loc)
        text = self.source(rel[0]) if rel is not None else None
        if not text:
            return None
        lines = text.split("\n")
        if not 0 < loc.line <= len(lines):
            return None
        rest = "\n".join(lines[loc.line - 1:loc.line + 40])
        i = loc.col - 1
        if rest[i:i + len(name)] != name:
            return None
        i += len(name)
        while i < len(rest) and rest[i].isspace():
            i += 1
        if i >= len(rest) or rest[i] != "<":
            return None
        depth, j = 0, i
        while j < len(rest):
            c = rest[j]
            if c in "(<":
                depth += 1
            elif c in ")>":
                depth -= 1
                if depth == 0:
                    return normalize_spec(rest[i:j + 1])
            elif c in ";{":
                return None
            j += 1
        return None

    def _partial_args(self, node, name):
        written = self._written_args(node, name)
        if written is not None:
            return written
        params = {}
        for c in node.get("inner", ()):
            if c.get("kind", "").endswith("TemplateParmDecl") and c.get("name"):
                params[(c.get("depth", 0), c.get("index", 0))] = c["name"]
        args = [template_arg_text(c) for c in node.get("inner", ()) if c.get("kind") == "TemplateArgument"]
        args = [_PARAM.sub(lambda m: params.get((int(m[1]), int(m[2])), m[0]), a) for a in args]
        return normalize_spec(f"<{', '.join(args)}>")

    def rel(self, loc):
        if loc is None or loc.file is None:
            return None
        r = self._paths.get(loc.file)
        if r is None and loc.file not in self._paths:
            r = self.path_map(loc.file)
            self._paths[loc.file] = r
        return r

    def run(self, root):
        decode(root)
        self._children(root.get("inner", ()), [], None, {})
        for pid, inst in self._pending_insts:
            d = self.m.by_id.get(pid)
            if d is not None and d.ent == "record_template":
                d.instantiations.append(inst)
        enums = {}
        for d in self.m.decls:
            if d.ent == "enum" and d.name:
                enums.setdefault(d.name, set()).add(d.qualified)
        for d in self.m.decls:
            if getattr(d, "nt_types", None) and any(d.nt_types):
                d.enum_names = enums
                d.instantiations = [cast_nt_args(i, d.nt_types, enums) for i in d.instantiations]
        return self.m

    @staticmethod
    def _qual(scope, name):
        parts = [p for p, _k in scope]
        return "::".join(parts + [name]) if name is not None else "::".join(parts)

    def _new(self, node, ent, name, scope, parent, ctx, begin=None):
        begin = begin or node.get("_begin") or node.get("_loc")
        end = node.get("_end") or begin
        rel = self.rel(begin)
        if rel is None or rel[1] != "tree":
            return None
        # An out-of-line declaration (`template<> void C::f<int>()`) lives in its semantic scope.
        sem = self.scopes.get(node.get("parentDeclContextId"))
        if sem is not None:
            scope = sem
        d = Decl(ent=ent, ast_kind=node.get("kind"), name=name, node=node, parent=parent)
        d.file = rel[0]
        d.begin = begin
        d.end = end
        d.line = begin.line
        d.end_line = end.line if end is not None else begin.line
        d.macro = begin.macro
        d.qualified = self._qual(scope, name) if name is not None else self._qual(scope, None)
        d.scope = self._qual(scope, None)
        d.scope_kind = "class" if scope and scope[-1][1] == "class" else "namespace"
        d.in_pattern = ctx.get("pattern", False)
        d.anon_ns = ctx.get("anon_ns", False)
        d.extern_c = ctx.get("extern_c", False)
        d.access = ctx.get("access")
        d.scope_parts = list(scope)
        self.m.decls.append(d)
        if "id" in node:
            self.m.by_id[node["id"]] = d
        if parent is not None:
            parent.children.append(d)
        return d

    def _children(self, inner, scope, parent, ctx):
        ctx = dict(ctx)
        for n in inner:
            k = n.get("kind")
            if k is None or "__eff" in n:
                continue
            if k == "AccessSpecDecl":
                ctx["access"] = n.get("access")
                self._new(n, "nonmarkable", None, scope, parent, ctx)
                continue
            self._decl(n, k, scope, parent, ctx)

    def _record_name(self, n):
        name = n.get("name")
        if name:
            return name
        b = n.get("_loc") or n.get("_begin")
        return f"(anon {n.get('tagUsed', 'struct')} L{b.line if b else 0})"

    def _decl(self, n, k, scope, parent, ctx, begin=None, template=None):
        if n.get("isImplicit"):
            return
        if k == "NamespaceDecl":
            name = n.get("name") or "(anonymous namespace)"
            self._new(n, "nonmarkable", None, scope, parent, ctx)
            c2 = dict(ctx)
            if not n.get("name"):
                c2["anon_ns"] = True
            kind = "ns_inline" if n.get("isInline") else ("ns_anon" if not n.get("name") else "ns")
            if "id" in n:
                self.scopes[n["id"]] = scope + [(name, kind)]
            self._children(n.get("inner", ()), scope + [(name, kind)], None, c2)
            return
        if k == "LinkageSpecDecl":
            self._new(n, "nonmarkable", None, scope, parent, ctx)
            c2 = dict(ctx)
            c2["extern_c"] = n.get("language") == "C"
            self._children(n.get("inner", ()), scope, parent, c2)
            return
        if k in RECORD_KINDS or k in ("ClassTemplateSpecializationDecl", "ClassTemplatePartialSpecializationDecl"):
            if k == "ClassTemplateSpecializationDecl" and ("TemplateInstantiationPattern" in n or template == "inst"):
                return
            if not n.get("completeDefinition"):
                if k in RECORD_KINDS and template is None:
                    self._new(n, "nonmarkable", None, scope, parent, ctx, begin)
                return
            name = self._record_name(n)
            ent = "record"
            tmpl = template
            args = None
            if k == "ClassTemplateSpecializationDecl":
                args = [template_arg_text(c) for c in n.get("inner", ()) if c.get("kind") == "TemplateArgument"]
                tmpl = "explicit"
                pattern = name
                name = normalize_spec(f"{name}<{', '.join(args)}>")
            elif k == "ClassTemplatePartialSpecializationDecl":
                # Its own identity: the primary's name plus the arguments as written (`FAuto<E*,B>`).
                tmpl = "partial"
                ent = "record_template"
                pattern = name
                name = normalize_spec(name + self._partial_args(n, name))
            if template == "pattern":
                ent = "record_template"
            elif ctx.get("pattern"):
                ent = "record_template"
                tmpl = "nested"
            c2 = dict(ctx)
            if ent == "record_template":
                c2["pattern"] = True
            d = self._new(n, ent, name, scope, parent, c2, begin)
            if d is None:
                return
            d.template = tmpl
            if tmpl in ("explicit", "partial"):
                d.pattern_name = self._qual(d.scope_parts, pattern)
                d.template_args = args
            d.anonymous = not n.get("name")
            d.tag = n.get("tagUsed", "struct")
            c2["access"] = "public" if d.tag in ("struct", "union") else "private"
            if "id" in n:
                self.scopes[n["id"]] = d.scope_parts + [(name, "class")]
            self._children(n.get("inner", ()), d.scope_parts + [(name, "class")], d, c2)
            return
        if k == "ClassTemplateDecl":
            pat = None
            insts = []
            for c in n.get("inner", ()):
                ck = c.get("kind")
                if ck in RECORD_KINDS and not c.get("isImplicit"):
                    pat = c
                elif ck == "ClassTemplateSpecializationDecl" and "TemplateInstantiationPattern" in c \
                        and c.get("completeDefinition"):
                    insts.append(c)
            if pat is None or not pat.get("completeDefinition"):
                self._new(n, "nonmarkable", None, scope, parent, ctx)
                return
            if ctx.get("pattern"):
                self._new(n, "unsupported", n.get("name"), scope, parent, ctx)
                return
            before = len(self.m.decls)
            self._decl(pat, pat.get("kind"), scope, parent, ctx, begin=n.get("_begin"), template="pattern")
            if len(self.m.decls) > before:
                d = self.m.decls[before]
                d.node_template = n
                d.tparams = [(c.get("name"), bool(c.get("isParameterPack")), c.get("kind") == "TemplateTypeParmDecl")
                             for c in n.get("inner", ()) if c.get("kind") in TPARM_KINDS]
                d.nt_types = [(c.get("type") or {}).get("qualType") if c.get("kind") == "NonTypeTemplateParmDecl" else None
                              for c in n.get("inner", ()) if c.get("kind") in TPARM_KINDS]
                for c in insts:
                    args = [template_arg_text(a) for a in c.get("inner", ()) if a.get("kind") == "TemplateArgument"]
                    inst = normalize_spec(f"{d.qualified}<{', '.join(args)}>")
                    if "id" in c:
                        self.scopes[c["id"]] = d.scope_parts + [(normalize_spec(f"{d.name}<{', '.join(args)}>"), "class")]
                    src = c.get("TemplateInstantiationPattern")
                    if src is None or src == pat.get("id"):
                        d.instantiations.append(inst)
                    else:  # instantiated from a partial specialization (a later top-level node)
                        self._pending_insts.append((src, inst))
            return
        if k == "FieldDecl":
            ent = "field" if n.get("name") else "nonmarkable"
            d = self._new(n, ent, n.get("name"), scope, parent, ctx, begin)
            if d is not None:
                d.bitfield = bool(n.get("isBitfield"))
            return
        if k == "IndirectFieldDecl":
            return
        if k in ("VarDecl",) or k in FUNC_KINDS:
            if "TemplateInstantiationPattern" in n:
                return  # an instantiation of a template member's out-of-line definition, not a declaration
        if k == "VarDecl":
            d = self._new(n, "global", n.get("name"), scope, parent, ctx, begin)
            if d is not None:
                d.template = template
                self._link_redecl(d, n)
            return
        if k in FUNC_KINDS:
            ent = "function"
            if n.get("explicitlyDeleted"):
                ent = "nonmarkable"
            d = self._new(n, ent, n.get("name"), scope, parent, ctx, begin)
            if d is None:
                return
            d.template = template
            d.has_body = any(c.get("body") for c in n.get("inner", ()) if "__eff" in c)
            if template is None:
                targs = [template_arg_text(c) for c in n.get("inner", ()) if c.get("kind") == "TemplateArgument"]
                if targs:  # explicit specialization `template<> f<int>()`: canonical arguments from clang
                    d.spec_args = targs
            self._link_redecl(d, n)
            return
        if k == "FunctionTemplateDecl":
            fn = None
            for c in n.get("inner", ()):
                if c.get("kind") in FUNC_KINDS:
                    fn = c
                    break
            if fn is None:
                return
            d = self._new(fn, "function", fn.get("name"), scope, parent, ctx, n.get("_begin"))
            if d is None:
                return
            d.template = "function"
            d.tparams = [(c.get("name"), bool(c.get("isParameterPack")), c.get("kind") == "TemplateTypeParmDecl")
                         for c in n.get("inner", ()) if c.get("kind") in TPARM_KINDS]
            d.has_body = any(c.get("body") for c in fn.get("inner", ()) if "__eff" in c)
            if "id" in n:
                self.m.by_id[n["id"]] = d
            self._link_redecl(d, fn)
            return
        if k == "FriendDecl":
            self._new(n, "nonmarkable", None, scope, parent, ctx, begin)
            return
        if k == "EnumDecl":
            name = n.get("name") or f"(anon enum L{(n.get('_loc') or n.get('_begin')).line})"
            ent = "enum" if any(c.get("kind") == "EnumConstantDecl" for c in n.get("inner", ())) \
                or n.get("fixedUnderlyingType") or n.get("scopedEnumTag") else "nonmarkable"
            d = self._new(n, ent, name, scope, parent, ctx, begin)
            if d is None:
                return
            d.values = []
            nxt = 0
            for c in n.get("inner", ()):
                if c.get("kind") != "EnumConstantDecl":
                    continue
                val = None
                for e in c.get("inner", ()):
                    if "value" in e:
                        val = int(e["value"])
                if val is None:
                    val = nxt
                d.values.append({"name": c.get("name"), "value": val})
                nxt = val + 1
            return
        if k in ("TypedefDecl", "TypeAliasDecl", "TypeAliasTemplateDecl"):
            d = self._new(n, "typedef", n.get("name"), scope, parent, ctx, begin)
            if d is not None:
                d.template = "alias" if k == "TypeAliasTemplateDecl" else template
            return
        if k in NONMARKABLE_KINDS or k.endswith("Decl"):
            self._new(n, "nonmarkable", None, scope, parent, ctx, begin)

    def _link_redecl(self, d, n):
        prev = n.get("previousDecl")
        if prev is None:
            return
        p = self.m.by_id.get(prev)
        if p is None:
            return
        while p.primary is not None:
            p = p.primary
        d.primary = p
        p.redecls.append(d)
        if p.ent == "unsupported":
            d.ent = "unsupported"  # the definition of a member template of a class template: no markers either
        if d.qualified != p.qualified:
            d.qualified, d.scope, d.scope_kind = p.qualified, p.scope, p.scope_kind


def extract(root, path_map, source=None):
    return Extractor(path_map, source).run(root)


_ERR = re.compile(r"^(?P<file>.+?):(?P<line>\d+):(?P<col>\d+): (?P<sev>error|fatal error|warning|note): (?P<msg>.*)$")


def parse_diagnostics(stderr: str):
    """clang stderr → [(file, line, col, severity, message)]."""
    out = []
    for raw in stderr.splitlines():
        m = _ERR.match(raw.strip())
        if m:
            out.append((m["file"], int(m["line"]), int(m["col"]), m["sev"], m["msg"]))
    return out


def norm_path(p: str) -> str:
    return os.path.normcase(os.path.normpath(p))
