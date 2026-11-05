"""`probes.cpp`: one probe per line; mapping "probe line → entity"."""
from __future__ import annotations

import re

from typedb.markers import args_start, split_top

HEADER = ("template <class T> using __typedb_id = T;"
          " template <class T, class = void> struct __typedb_cpl { enum { v = 0 }; };"
          " template <class T> struct __typedb_cpl<T, decltype((void)sizeof(T))> { enum { v = 1 }; };")


class Probe:
    __slots__ = ("kind", "n", "key", "decl", "spec", "line", "text")

    def __init__(self, kind, n, key, decl, spec, text):
        self.kind, self.n, self.key, self.decl, self.spec, self.text = kind, n, key, decl, spec, text
        self.line = None


def _open_ns(parts):
    """Leading namespace parts of a scope → (open text, close text, remaining class parts)."""
    opens, i = [], 0
    while i < len(parts) and parts[i][1].startswith("ns"):
        name, kind = parts[i]
        if kind == "ns_anon":
            opens.append("namespace {")
        elif kind == "ns_inline":
            opens.append(f"inline namespace {name} {{")
        else:
            opens.append(f"namespace {name} {{")
        i += 1
    return " ".join(opens), " ".join("}" for _ in opens), [p for p, _k in parts[i:]]


def wrap(parts, body):
    o, c, _rest = _open_ns(parts)
    return f"{o} {body} {c}".strip()


_TAIL = re.compile(r"\s*(noexcept(\(.*\))?|throw\(.*?\)|__attribute__\(\(.*\)\))\s*$")


def fn_type(qual: str) -> str:
    prev = None
    while prev != qual:
        prev = qual
        qual = _TAIL.sub("", qual)
    return qual


def _q(name):
    return "::" + name


def record_probes(n, name):
    return (f"{_q(name)}* __typedb_r{n} = 0; unsigned __typedb_s{n}[3] = "
            f"{{sizeof({_q(name)}), alignof({_q(name)}), __datasizeof({_q(name)})}};")


def derived_probe(n, name, tag="P"):
    return (f"struct __typedb_{tag}{n} : {_q(name)} {{ virtual void __typedb_k(); }}; "
            f"void __typedb_{tag}{n}::__typedb_k() {{}}")


def function_probe(n, d):
    """Probe of a non-template function, inside its namespaces (`wrap`).

    An explicit specialization is named with its template arguments (clang's canonical spelling), since
    `&f` alone names the whole overload set. A member one has no DI in its class, and its type is spelled
    in class scope (`void (short *, EEndianness)`), so the probe takes its address untyped: there is no
    other overload with the same template arguments.
    """
    _o, _c, rest = _open_ns(d.scope_parts)
    name = d.name + ((" " if d.name.endswith("<") else "") + f"<{', '.join(d.spec_args)}>" if d.spec_args else "")
    if d.scope_kind == "class":
        ref = "::".join(rest + [name])
        return f"__attribute__((used)) auto __typedb_f{n} = &{ref};"
    qt = fn_type(d.node.get("type", {}).get("qualType", ""))
    return f"__attribute__((used)) __typedb_id<{qt}>* __typedb_f{n} = &{name};"


def last_component(spec):
    """`A::B<X::Y>::f<Z::W>` -> `f<Z::W>`: the last `::` component outside the template arguments."""
    return split_top(spec, "::")[-1]


def _overload_type(d, local, spec=None):
    """Pointer type of one overload of a function template instance, from the declared type with the
    template parameters replaced by the arguments of `local` (None when that cannot be done safely).

    A member template of a class template (`spec` is the whole SPEC, `Class<CArgs>::name<MArgs>`) also replaces the
    parameters of the class by the arguments of the class part, and a non-static one is `R (Class<CArgs>::*)(P)`."""
    tp = list(getattr(d, "tparams", None) or [])
    qt = d.node.get("type", {}).get("qualType", "")
    if not tp or args_start(local) < 0 or any(t[1] for t in tp):
        return None
    args = [a.strip() for a in split_top(local[args_start(local) + 1:local.rindex(">")])]
    if len(args) != len(tp) or any(not t[0] for t in tp):
        return None
    pairs = list(zip(tp, args))
    cls = "::" + d.scope
    if spec is not None:
        comps = split_top(spec, "::")
        k = next((i for i, c in enumerate(comps[:-1]) if args_start(c) >= 0), None)
        root = d.parent
        while root is not None and root.template not in ("pattern", "partial"):
            root = root.parent
        ctp = list(getattr(root, "tparams", None) or [])
        if k is None or not ctp or any(t[1] for t in ctp):
            return None
        cargs = [a.strip() for a in split_top(comps[k][args_start(comps[k]) + 1:comps[k].rindex(">")])]
        if len(cargs) != len(ctp):
            return None
        pairs += list(zip(ctp, cargs))
        cls = "::" + "::".join(comps[:-1])
    # a type argument goes in as `__typedb_id<A>` so that `const T &` stays const-ref-to-T for T = `char *`
    sub = {t[0]: (f"__typedb_id<{a}>" if t[2] else a) for t, a in pairs if t[0]}
    qt = re.sub(r"\b[A-Za-z_]\w*\b", lambda m: sub.get(m[0], m[0]), fn_type(qt))
    m = re.match(r"^(.*?)(\((?:[^()]|\([^()]*\))*\))\s*((?:const|volatile|&&|&|\s)*)$", qt)
    if not m or re.search(r"type-parameter|\btypename\b|\bdecltype\b|\bauto\b", qt):
        return None
    ret, params, quals = m.group(1).strip(), m.group(2), m.group(3).strip()
    if d.scope_kind == "class" and not d.node.get("storageClass") == "static":
        return f"{ret} ({cls}::*){params}{' ' + quals if quals else ''}"
    return f"{ret} (*){params}"


def complete_probe(n, name):
    """Completes a class template specialization that a signature of the tree passes by value.

    Only a declaration of it exists until something needs its size; the SFINAE helper asks for the size
    (instantiating it) without an error when it cannot be had.
    """
    return (f"unsigned __typedb_d{n} = __typedb_cpl<{_q(name)}>::v; {_q(name)}* __typedb_c{n} = 0;")


class Builder:
    def __init__(self):
        self.probes = []

    def add(self, kind, key, decl, spec, text):
        p = Probe(kind, None, key, decl, spec, text)
        self.probes.append(p)
        return p

    def render(self, unity_includes, skip=frozenset()):
        lines = list(unity_includes) + [HEADER]
        by_line = {}
        for p in self.probes:
            if id(p) in skip:
                p.line = None
                continue
            lines.append(p.text)
            p.line = len(lines)
            by_line[p.line] = p
        return "\n".join(lines) + "\n", by_line


def plan(model, records_dynamic_hint, include_lines):
    """Build the probe list from the AST model, numbered after sorting."""
    b = Builder()
    recs, gl, fns, insts, ftis, gis, tds = [], [], [], [], [], [], []
    for d in model.decls:
        if d.primary is not None:
            continue
        if d.ent == "record" and d.template in (None, "explicit"):
            if d.anon_ns or getattr(d, "anonymous", False) or "(anon " in d.qualified or d.in_pattern:
                continue
            recs.append(d)
        elif d.ent == "record_template" and d.template in ("pattern", "partial", "nested"):
            for spec in sorted(set(d.instantiations) | set(getattr(d, "spec_names", ()))):
                insts.append((spec, d))
        elif d.ent == "global" and not d.in_pattern and "(anon " not in d.qualified:
            gl.append(d)
        elif d.ent == "global" and d.in_pattern and getattr(d, "spec_names", ()):
            for spec in d.spec_names:
                gis.append((spec, d))
        elif d.ent == "function" and d.template is None and d.scope_kind == "namespace" and not d.in_pattern:
            fns.append(d)
        elif d.ent == "function" and d.template is None and d.spec_args and not d.in_pattern \
                and not d.anon_ns and d.ast_kind == "CXXMethodDecl":
            fns.append(d)  # explicit specialization of a member template: no DI in its class
        elif d.ent == "function" and d.template == "function":
            for spec in sorted(getattr(d, "spec_names", ())):
                ftis.append((spec, d))
        elif d.ent == "typedef" and d.template is None and d.scope_kind == "class" and not d.in_pattern                 and not d.anon_ns and "(anon " not in d.qualified:
            tds.append(d)  # a typedef of a class has no debug info unless something uses it
    recs.sort(key=lambda d: d.qualified)
    for i, d in enumerate(recs, 1):
        b.add("record", d.qualified, d, None, record_probes(i, d.qualified))
        if records_dynamic_hint(d):
            b.add("dynamic", d.qualified, d, None, derived_probe(i, d.qualified))
    gl.sort(key=lambda d: (d.qualified, d.file, d.line))
    for i, d in enumerate(gl, 1):
        _o, _c, rest = _open_ns(d.scope_parts)
        ref = "::".join(rest + [d.name])
        b.add("global", d.qualified, d, None, wrap(d.scope_parts, f"__attribute__((used)) decltype({ref})* __typedb_g{i} = &{ref};"))
    fns.sort(key=lambda d: (d.qualified, d.file, d.line))
    for i, d in enumerate(fns, 1):
        b.add("function", d.qualified, d, None, wrap(d.scope_parts, function_probe(i, d)))
    insts.sort(key=lambda x: (x[0], x[1].qualified))
    for i, (spec, d) in enumerate(insts, 1):
        q = _q(spec)
        b.add("instance", spec, d, spec, f"static_assert(sizeof({q}) > 0, \"\"); {q}* __typedb_i{i} = 0; "
                                         f"unsigned __typedb_j{i}[3] = {{sizeof({q}), alignof({q}), __datasizeof({q})}};")
        if records_dynamic_hint(d):
            b.add("instance_dynamic", spec, d, spec, derived_probe(i, spec, "Q"))
    ftis.sort(key=lambda x: (x[0], x[1].qualified))
    for i, (spec, d) in enumerate(ftis, 1):
        local = last_component(spec)
        if d.in_pattern:
            # member template of a class template: SPEC is `[ns::]Class<Args>::name<MArgs>`, named from the root
            cast = _overload_type(d, local, spec)
            ref = _q(spec)
            init = f"static_cast<{cast}>(&{ref})" if cast else f"&{ref}"
            b.add("function_instance", spec, d, spec, f"__attribute__((used)) auto __typedb_t{i} = {init};")
            continue
        _o, _c, rest = _open_ns(d.scope_parts)
        ref = "::".join(rest + [local])
        cast = _overload_type(d, local)
        init = f"static_cast<{cast}>(&{ref})" if cast else f"&{ref}"
        b.add("function_instance", spec, d, spec, wrap(d.scope_parts, f"__attribute__((used)) auto __typedb_t{i} = {init};"))
    # static data member of a class template: the symbol of each marked instantiation
    gis.sort(key=lambda x: (x[0], x[1].qualified))
    for i, (spec, d) in enumerate(gis, 1):
        b.add("global_instance", f"{spec}::{d.name}", d, spec,
              f"__attribute__((used)) auto* __typedb_u{i} = &{_q(spec)}::{d.name};")
    tds.sort(key=lambda d: (d.qualified, d.file, d.line))
    for i, d in enumerate(tds, 1):
        b.add("typedef", d.qualified, d, None, f"{_q(d.qualified)}* __typedb_y{i} = 0;")
    for p in b.probes:
        p.n = int(re.search(r"__typedb_[rPgfijQtuy](\d+)", p.text).group(1))
    return b


_ERR = re.compile(r"probes\.cpp:(\d+):\d+: (error|note|fatal error)")


def failed_lines(stderr: str):
    out = set()
    for m in _ERR.finditer(stderr):
        out.add(int(m.group(1)))
    return out
