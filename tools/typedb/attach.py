"""Blocks ↔ declarations (by clang ranges), macros, obligations, diagnostic code table."""
from __future__ import annotations

import re
from collections import defaultdict

from typedb import markers as mk

# §10: every code typedb emits (all are `error`; `warning` is reserved in the schema).
CODES = (
    "marker-syntax", "near-marker", "marker-orphan", "marker-ambiguous", "marker-misplaced",
    "marker-duplicate", "marker-missing", "marker-conflict", "marker-unsupported", "marker-macro",
    "cpp-undeclared", "file-unreached", "probe-failed", "clang-error", "internal",
)
MARKER_CODES = tuple(c for c in CODES if c.startswith("marker-")) + ("near-marker",)

MAIN = ("addr", "inline", "pure", "absent")
RECORD_KINDS = ("size", "vtable")


class Diags:
    def __init__(self):
        self.items = []

    def add(self, code, file, line, message):
        assert code in CODES, code
        self.items.append({"severity": "error", "code": code, "file": file, "line": line, "message": message})


def describe(m):
    parts = ["///"]
    if m.kind == "offset":
        return f"///< +{m.value}"
    if m.kind == "addr":
        parts.append(m.value)
        if m.fastcc:
            parts.append("fastcc")
    else:
        parts.append(m.kind)
        if m.value is not None:
            parts.append(m.value)
    if m.spec:
        parts.append(m.spec)
    return " ".join(parts)


def etype(d):
    if d.ent == "function":
        if d.primary is not None:
            return "redecl"
        if d.template == "function" or d.in_pattern:
            return "ftemplate"
        return "function"
    if d.ent == "global":
        if d.primary is not None:
            return "gredecl"
        return "gtemplate" if d.in_pattern else "global"
    return d.ent


def is_markable(d):
    return d.ent not in ("nonmarkable",)


def is_structor(d):
    return d.ast_kind in ("CXXConstructorDecl", "CXXDestructorDecl")


def is_dtor(d):
    return d.ast_kind == "CXXDestructorDecl"


def allows(d, kind):
    t = etype(d)
    if t in ("record", "record_template"):
        return kind in ("size", "vtable", "lua")
    if t in ("function", "ftemplate"):
        return kind in ("addr", "base", "deleting", "thunk", "inline", "pure", "absent", "lua")
    if t in ("redecl",):
        return kind in ("addr", "base", "inline")
    if t in ("global", "gtemplate", "gredecl"):
        return kind in ("addr", "inline")
    if t in ("enum", "typedef"):
        return kind == "lua"
    if t == "unsupported":
        return True
    return False


def _head(spec):
    return mk.strip_args(spec).split("::")[-1]


def spec_chain(spec):
    """`ns::Outer<int>::Nested<char>::K` -> [`ns`, `Outer`, `Nested`, `K`]: the components without arguments."""
    return [mk.strip_args(c) for c in mk.split_top(spec, "::")]


def class_chain(x):
    """Names from the class template that encloses `x` (or is `x`) down to `x`'s own class: what the end of a SPEC
    must spell. `x` is a record, or a member of a record."""
    names = []
    p = x if x.ent == "record_template" or x.ent == "record" else x.parent
    while p is not None:
        names.append(_head(p.pattern_name) if p.template == "partial" else (p.qualified or "").split("::")[-1])
        if p.template in ("pattern", "partial"):
            break
        p = p.parent
    return names[::-1]


def const_with_init(d):
    n = d.node
    if "init" not in n:
        return False
    if n.get("constexpr"):
        return True
    qt = n.get("type", {}).get("qualType", "")
    if qt.endswith("const"):
        return True
    return qt.startswith("const ") and not re.search(r"[*&(]", qt)


class Attacher:
    def __init__(self, decls_by_file, scans, diags: Diags, strict=True):
        self.by_file = decls_by_file
        self.scans = scans
        self.d = diags
        self.strict = strict
        self.macro_bad = set()

    def run(self):
        for file in sorted(self.scans):
            scan = self.scans[file]
            decls = self.by_file.get(file, [])
            for x in decls:
                x.markers = []
                x.trailing = None
                x.block = None
            if not self.strict:
                continue
            self._file(file, scan, decls)

    # -- binding ----------------------------------------------------------------------------
    def _file(self, file, scan, decls):
        starts = defaultdict(list)
        ends = defaultdict(list)
        for x in decls:
            starts[x.line].append(x)
            ends[x.end_line].append(x)
        macro_groups = defaultdict(list)
        for x in decls:
            if x.macro and is_markable(x) and x.ent != "unsupported":
                macro_groups[x.line].append(x)
        reported = set()
        for line, group in macro_groups.items():
            if len(group) >= 2:
                for x in group:
                    self.macro_bad.add(id(x))
                names = ", ".join(sorted({x.qualified or x.name or "?" for x in group}))
                self.d.add("marker-macro", file, line,
                           f"macro expands to {len(group)} markable entities ({names}); expand it by hand")
                reported.add(line)
        for b in scan.blocks:
            line = b.last_line + 1
            cands = [x for x in starts.get(line, []) if not x.node.get("isImplicit")]
            if not cands:
                self.d.add("marker-orphan", file, b.first_line,
                           f"no declaration starts on line {line}, right below the block")
                continue
            if line in reported:
                continue
            mk = [x for x in cands if is_markable(x)]
            macro_mk = [x for x in mk if x.macro]
            if any(x.macro for x in cands) and not mk:
                self.d.add("marker-macro", file, line, "macro expansion yields no markable entity")
                continue
            if macro_mk and len(macro_mk) >= 2:
                continue
            for m in b.markers:
                self._assign(file, m, cands, b)
        for line, m in sorted(scan.trailing.items()):
            fields = [x for x in ends.get(line, []) if x.ent == "field"]
            if len(fields) == 1:
                if fields[0].trailing is not None:
                    self.d.add("marker-duplicate", file, line, "second '///<' for one field")
                fields[0].trailing = m
            elif len(fields) > 1:
                self.d.add("marker-ambiguous", file, line,
                           f"{len(fields)} fields end on this line ({', '.join(f.name for f in fields)}); one field per line")
            elif ends.get(line):
                self.d.add("marker-misplaced", file, line, "'///<' is only for a field")
            else:
                self.d.add("marker-orphan", file, line, "'///<' after no declaration")


    def _assign(self, file, m, cands, block):
        if m.kind in RECORD_KINDS:
            pool = [x for x in cands if x.ent in ("record", "record_template")]
        else:
            pool = [x for x in cands if allows(x, m.kind)]
            if len(pool) > 1:
                fv = [x for x in pool if x.ent in ("function", "global", "field")]
                if fv:
                    pool = fv
        if len(pool) == 1:
            x = pool[0]
            x.markers.append(m)
            x.block = block
            return
        if len(pool) > 1:
            self.d.add("marker-ambiguous", file, m.line,
                       f"'{describe(m)}' fits {len(pool)} entities on line {block.last_line + 1}")
            return
        target = cands[0]
        if target.ent == "unsupported":
            self.d.add("marker-unsupported", file, m.line, "markers on a member template of a class template")
            return
        self.d.add("marker-misplaced", file, m.line, f"'{describe(m)}' is not allowed on {_what(target)}")

    # -- obligations ------------------------------------------------------------------------
    def obligations(self, decls, scans, dynamic_of, has_body, instantiations):
        if not self.strict:
            for x in decls:
                x.markers = []
            return
        for x in decls:
            if x.file not in scans:
                continue
            if id(x) in self.macro_bad:
                x.markers = []
                continue
            t = etype(x)
            ms = self._dedupe(x, x.markers)
            if t == "unsupported":
                if ms:
                    self._err("marker-unsupported", x, ms[0], "markers on a member template of a class template")
                x.markers = []
                continue
            if t in ("record", "record_template"):
                x.markers = self._record(x, ms, dynamic_of(x), instantiations.get(id(x), ()))
            elif t == "field":
                if x.trailing is None and self.strict:
                    self._err("marker-missing", x, None, f"field {x.name} has no ///<")
                x.markers = []
            elif t in ("function", "ftemplate"):
                x.markers = self._function(x, ms, t == "ftemplate", has_body(x))
            elif t == "redecl":
                p = x.primary
                if not ms and p.template == "function" and p.in_pattern and not p.markers:
                    x.markers = []  # a member template of a class template without markers: none to repeat
                else:
                    x.markers = self.definition(x.file, x.line, ms, x.qualified, p.template == "function"
                                                or p.in_pattern)
            elif t in ("global", "gtemplate", "gredecl"):
                x.markers = self._global(x, ms, t)
            elif t in ("enum", "typedef"):
                x.markers = [m for m in ms if m.kind == "lua"]
            else:
                x.markers = []

    def _err(self, code, x, m, msg):
        self.d.add(code, x.file, m.line if m is not None else x.line, msg)

    def _dedupe(self, x, ms):
        seen, out = {}, []
        for m in ms:
            if not allows(x, m.kind):
                if etype(x) == "unsupported":
                    out.append(m)
                    continue
                self._err("marker-misplaced", x, m, f"'{describe(m)}' is not allowed on {_what(x)}")
                continue
            key = (m.kind, m.spec)
            if m.kind not in ("thunk", "lua") and key in seen:
                self._err("marker-duplicate", x, m, f"repeated '{m.kind}'" + (f" for {m.spec}" if m.spec else ""))
                continue
            if m.kind == "thunk" and (m.kind, m.spec, m.value) in seen:
                self._err("marker-duplicate", x, m, f"repeated thunk {m.value}")
                continue
            seen[key] = m
            seen[(m.kind, m.spec, m.value)] = m
            out.append(m)
        return out

    def _spec_ok(self, x, m, head):
        if m.spec is None:
            return True
        chain = head if isinstance(head, list) else [head]
        if spec_chain(m.spec)[-len(chain):] != chain:
            self._err("marker-misplaced", x, m, f"SPEC {m.spec} is not an instantiation of {'::'.join(chain)}")
            return False
        return True

    def _record(self, x, ms, dynamic, insts):
        out = []
        templ = x.ent == "record_template"
        head = class_chain(x) if templ else None
        for m in ms:
            if m.kind == "vtable" and dynamic is False:
                self._err("marker-misplaced", x, m, f"vtable on a non-dynamic {x.tag if hasattr(x, 'tag') else 'record'}")
                continue
            if m.spec is not None and not templ:
                self._err("marker-misplaced", x, m, "SPEC on a record that is not a template")
                continue
            if templ and not self._spec_ok(x, m, head):
                continue
            out.append(m)
        for kind in RECORD_KINDS:
            plain = [m for m in out if m.kind == kind and m.spec is None]
            spec = [m for m in out if m.kind == kind and m.spec is not None]
            if plain and spec:
                self._err("marker-conflict", x, spec[0], f"'{kind}' with and without SPEC together")
            required = kind == "size" or dynamic
            if not required:
                continue
            if not plain and not spec:
                self._err("marker-missing", x, None, f"{_what(x)} has no /// {kind}")
            elif spec and not plain and templ and x.template in ("pattern", "partial"):
                have = {m.spec for m in spec}
                for inst in sorted(insts):
                    if inst not in have and inst.split("::")[-1] not in have:
                        self._err("marker-missing", x, None, f"no /// {kind} for instantiation {inst}")
        return out

    def _function(self, x, ms, templ, body):
        out = []
        virtual = bool(getattr(x, "virtual", False))
        pure = bool(x.node.get("pure"))
        head = (class_chain(x) + [x.name] if x.in_pattern else x.name) if x.template == "function" else class_chain(x)
        for m in ms:
            k = m.kind
            if k == "base" and m.value is None:
                self._err("marker-syntax", x, m, "'base' needs an address")
                continue
            if k == "base" and not is_structor(x):
                self._err("marker-misplaced", x, m, f"base on {_what(x)}; only for a constructor or destructor")
                continue
            if k == "deleting" and not (is_dtor(x) and virtual):
                self._err("marker-misplaced", x, m, "deleting on a non-virtual destructor" if is_dtor(x)
                          else f"deleting on {_what(x)}")
                continue
            if k == "thunk" and not virtual:
                self._err("marker-misplaced", x, m, f"thunk on a non-virtual {_what(x)}")
                continue
            if k == "pure" and not pure:
                self._err("marker-misplaced", x, m, "pure on a method that is not '= 0'")
                continue

            if templ:
                if k in ("addr", "base", "deleting", "thunk") and m.spec is None:
                    self._err("marker-misplaced", x, m, f"'{describe(m)}' on a template needs SPEC")
                    continue
                if not self._spec_ok(x, m, head):
                    continue
            elif m.spec is not None:
                self._err("marker-misplaced", x, m, "SPEC on a function that is not a template")
                continue
            out.append(m)
        kinds = {m.kind for m in out}
        no_code = [k for k in ("inline", "pure", "absent") if k in kinds]
        if templ:
            specs = defaultdict(set)
            for m in out:
                if m.spec is not None:
                    specs[m.spec].add(m.kind)
            for s, ks in sorted(specs.items()):
                if not ({"addr", "base"} & ks):
                    m = next(mm for mm in out if mm.spec == s)
                    self._err("marker-missing", x, m, f"no address or base for {s}")
            if no_code and specs:
                m = next(mm for mm in out if mm.spec is not None)
                self._err("marker-conflict", x, m, f"'{no_code[0]}' together with instantiation markers")
            if len(no_code) > 1:
                self._err("marker-conflict", x, None, f"two main markers: {', '.join(no_code)}")
            optional = x.template == "function" and x.in_pattern  # member template of a class template
            if not specs and not no_code and not optional:
                self._err("marker-missing", x, None, f"{_what(x)} has no main marker")
        else:
            main = [k for k in MAIN if k in kinds]
            if len(main) > 1:
                self._err("marker-conflict", x, None, f"two main markers: {', '.join(main)}")
            extra = [k for k in ("base", "deleting", "thunk") if k in kinds]
            if extra and no_code:
                self._err("marker-conflict", x, None, f"{extra[0]} together with {no_code[0]}")
            if not main and "base" not in kinds:
                self._err("marker-missing", x, None, f"{_what(x)} has no main marker (address, base, inline, pure or absent)")
        for k in ("pure", "absent"):
            if k in kinds and body:
                self._err("marker-conflict", x, None, f"{k} on a function with a body")
        return out

    def _global(self, x, ms, t):
        out = []
        for m in ms:
            if m.kind == "addr" and m.fastcc:
                self._err("marker-misplaced", x, m, "fastcc is only for functions")
                continue
            if m.kind == "inline" and not const_with_init(x):
                self._err("marker-misplaced", x, m, "inline on a global is only for a const with an initializer")
                continue
            if m.spec is not None and t != "gtemplate" and not (t == "gredecl" and x.primary.in_pattern):
                self._err("marker-misplaced", x, m, "SPEC on a variable that is not a template member")
                continue
            out.append(m)
        kinds = {m.kind for m in out}
        if "addr" in kinds and "inline" in kinds:
            self._err("marker-conflict", x, None, "address together with inline")
        if not kinds and t == "global":
            self._err("marker-missing", x, None, f"{_what(x)} has no address marker")
        return out

    def definition(self, file, line, ms, what, templ):
        """Markers above an out-of-class definition (.h/.inl redecl or .cpp body)."""
        out, seen = [], set()
        for m in ms:
            ok = m.kind == "inline" or m.kind == "addr" or (m.kind == "base" and m.value is not None)
            if not ok:
                self.d.add("marker-misplaced", file, m.line,
                           f"'{describe(m)}' above a definition; only the main marker line is repeated")
                continue
            key = (m.kind, m.spec)
            if key in seen:
                self.d.add("marker-duplicate", file, m.line, f"repeated '{m.kind}'")
                continue
            seen.add(key)
            out.append(m)
        if not out:
            self.d.add("marker-missing", file, line, f"definition of {what} repeats no marker from the header")
        return out


def _what(x):
    t = etype(x)
    names = {"record": "a record", "record_template": "a class template", "field": "a field",
             "function": "a function", "ftemplate": "a function template", "global": "a variable",
             "gtemplate": "a template static member", "enum": "an enum", "typedef": "a typedef",
             "redecl": "a definition", "gredecl": "a variable definition"}
    base = names.get(t, "this declaration")
    if x.name and t != "nonmarkable":
        return f"{base} {x.qualified or x.name}"
    if t == "nonmarkable":
        return f"this declaration ({x.ast_kind})"
    return base
