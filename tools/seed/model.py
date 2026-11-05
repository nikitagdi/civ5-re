"""The class model of the Firaxis denominator from binfacts.

Scopes (namespaces, classes, class template families), functions, data and the unproven decisions. Virtual
slots are added by `vtables.py`, class template generalization by `templates.py`.
"""
import re
from collections import defaultdict
from dataclasses import dataclass, field

from seed import luaentry
from seed import mangle as M

LUA_STATE = M.Ptr(M.Named(M.Name((M.Part("lua_State"),))))
STD = ("std",)
ENUM_NAME = re.compile(r"^(?:\w*Types|E[A-Z]\w*|e[A-Z]\w*|\w*_TYPE|[A-Z][A-Z0-9_]+|\w*Mode)$")


class ModelError(Exception):
    pass


@dataclass
class Inst:
    """Addresses of one instantiation (`spec` None for a non-template entity)."""
    spec: str = None
    addr: str = None
    base: str = None
    deleting: str = None
    thunks: list = field(default_factory=list)
    fastcc: bool = False
    mangled: str = None
    targs: tuple = None          # function template: this instantiation's arguments


@dataclass
class Fn:
    scope: tuple                 # path of the scope (names without template args)
    part: M.Part                 # name (ctor/dtor/op/cv/name), template args stripped
    kind: str                    # method | ctor | dtor | free | c
    params: tuple = None         # None -> UNK_PARAMS
    variadic: bool = False
    const: bool = False
    ret: object = None           # mangle type; None -> UNK_RET (or nothing for ctor/dtor/cv)
    ret_text: str = None         # fixed return spelling (int, void*, void)
    static: bool = False
    virtual: bool = False
    pure: bool = False
    ftemplate: tuple = None      # function template: template args pattern (for parameter kinds)
    insts: list = field(default_factory=list)
    lua: bool = False
    order: tuple = ()
    slot: int = None             # primary-vtable index of a new virtual entry (declaration order)

    def key(self):
        return (self.scope, self.part, self.params, self.variadic, self.const, self.ftemplate is not None,
                self.ret if self.ftemplate is not None else None)


@dataclass
class Var:
    scope: tuple
    name: str
    size: int
    member: bool                 # static data member of a class
    c: bool = False              # extern "C"
    insts: list = field(default_factory=list)
    order: tuple = ()


@dataclass
class Scope:
    path: tuple
    kind: str = "namespace"      # namespace | class | enum | existing | foreign | std
    tparams: list = None         # [(kind, type)] kind: class | value; type: mangle type of a value param
    insts: dict = field(default_factory=dict)   # spec text -> template args
    fns: list = field(default_factory=list)
    vars: list = field(default_factory=list)
    bases: list = field(default_factory=list)   # [(path, targs, offset, public)]
    dynamic: bool = False
    vtable: str = None           # address, "?" or None
    vtables: dict = field(default_factory=dict)  # spec -> address (templates)
    typeinfo: str = None
    pad: tuple = None            # (offset, bytes) padding placeholder so that base offsets match RTTI
    file: str = None             # existing: file that declares it; generated: target file
    origin: str = None
    lua_class: bool = False
    addr_min: int = None
    evidence: list = field(default_factory=list)  # class evidence kinds
    nested_types: set = field(default_factory=set)
    inst_bases: dict = field(default_factory=dict)
    handwritten: bool = False    # existing class outside the SDK copies: completed per section 6
    in_template: tuple = None    # class nested in this class template (its markers carry SPEC)
    tt_header: str = None        # `template <class>`: used only as a template template argument


def path_of(name):
    return tuple(p.name for p in name.parts)


def is_std(path):
    return bool(path) and path[0] == "std"


class Model:
    def __init__(self):
        self.scopes = {}
        self.decisions = defaultdict(list)
        self.skipped = []         # [{"entity", "mangled", "reason"}]
        self.params_unknown = 0
        self.lit_types = set()     # paths used as the type of a literal template argument
        self.type_refs = set()     # paths used as a type (not only as a prefix)
        self.thunks = {}           # target mangled -> [thunk addr]
        self.symbols = {}          # mangled -> binfacts symbol
        self.tnames = set()        # referenced names with template args

    def scope(self, path):
        s = self.scopes.get(path)
        if s is None:
            s = self.scopes[path] = Scope(path)
            if len(path) > 1:
                self.scope(path[:-1])
        return s

    def skip(self, s, reason):
        self.skipped.append({"entity": s.get("demangled") or s["mangled"], "mangled": s["mangled"], "reason": reason})


def fmt_addr(a):
    return "0x%08x" % (int(a, 16) if isinstance(a, str) else a)


class Facts:
    """binfacts plus ghidra-functions, as the model reads them."""

    def __init__(self, bf, gfuncs):
        self.bf = bf
        self.kinds = set((bf["stats"]["denominator"]["by_kind"]))
        self.firaxis = [s for s in bf["symbols"] if s.get("group") == "firaxis"]
        self.den = [s for s in self.firaxis if s["kind"] in self.kinds]
        self.by_mangled = {s["mangled"]: s for s in bf["symbols"]}
        self.by_addr = defaultdict(list)
        for s in bf["symbols"]:
            if s["type"] == "func":
                self.by_addr[int(s["addr"], 16)].append(s)
        self.gfuncs = gfuncs      # addr int -> flags
        self.lua = luaentry.Index(bf)
        self.typeinfo = {t["class"]: t for t in bf["typeinfo"]}
        self.vtables = {v["class"]: v for v in bf["vtables"] if not v.get("construction")}
        self.groups = {}
        vt_sym = {s["addr"]: s for s in bf["symbols"] if s["kind"] == "vtable"}
        ti_sym = {s["addr"]: s for s in bf["symbols"] if s["kind"] == "typeinfo"}
        for v in bf["vtables"]:
            s = vt_sym.get(v["addr"])
            self.groups[("vtable", v["class"])] = s["group"] if s else None
        for t in bf["typeinfo"]:
            s = ti_sym.get(t["addr"])
            self.groups[("typeinfo", t["class"])] = s["group"] if s else None
        # Aspyr classes that are bases of Firaxis classes are declared (not studied): they take part in the
        # class model like Firaxis classes, but none of their symbols is in the denominator
        self.aspyr_bases = set()
        todo = [b["class"] for t in bf["typeinfo"] if self.groups.get(("typeinfo", t["class"])) == "firaxis"
                for b in t.get("bases") or ()]
        while todo:
            b = todo.pop()
            if self.groups.get(("typeinfo", b)) == "aspyr" and b not in self.aspyr_bases:
                self.aspyr_bases.add(b)
                self.groups[("typeinfo", b)] = "firaxis"
                if ("vtable", b) in self.groups:
                    self.groups[("vtable", b)] = "firaxis"
                todo.extend(x["class"] for x in (self.typeinfo.get(b) or {}).get("bases") or ())

    def fastcc(self, addr):
        flags = self.gfuncs.get(int(addr, 16))
        return bool(flags and flags.startswith("r="))

    def is_function_start(self, addr):
        return int(addr, 16) in self.gfuncs


def load_gfuncs(path):
    """build/ghidra-functions.txt v2 -> {start: flags}."""
    out = {}
    with open(path, encoding="utf-8") as f:
        for line in f:
            if line.startswith("#") or not line.strip():
                continue
            cols = line.rstrip("\n").split("\t")
            if len(cols) < 4:
                cols = line.split()
            out[int(cols[0], 16)] = cols[3] if len(cols) > 3 else "-"
    return out


# -- names and kinds ---------------------------------------------------------------------------------------
def scope_split(name):
    """Name of an entity -> (scope Name, template part index or None). Raises ModelError for nested templates."""
    sc = name.scope()
    idx = [i for i, p in enumerate(sc.parts) if p.targs is not None]
    if len(idx) > 1:
        raise ModelError("nested template scopes")
    return sc, (idx[0] if idx else None)


def _tspec(m, sc, sc_name, tpos):
    """SPEC of the instantiation that holds an entity: `Foo<X>` or, for a class nested in a class template,
    `Foo<X>::Inner` (typedb SPEC grammar); the template arguments are those of the enclosing template."""
    if tpos == len(sc_name.parts) - 1:
        return _template(m, sc, sc_name.last.targs, sc_name)
    outer = M.Name(sc_name.parts[:tpos + 1])
    _template(m, m.scope(path_of(outer)), outer.last.targs, outer)
    spec = M.Speller().name(sc_name)
    sc.insts[spec] = outer.last.targs
    sc.in_template = path_of(outer)
    return spec


def collect_refs(model, obj):
    for n in M.names_in(obj):
        p = path_of(n)
        model.type_refs.add(p)
        if any(part.targs for part in n.parts):
            model.tnames.add(n)
            # `Foo<X>::iterator`: Foo is a class template, not a namespace
            for k, part in enumerate(n.parts[:-1]):
                if part.targs is not None:
                    model.type_refs.add(p[:k + 1])
        for part in n.parts:
            for a in part.targs or ():
                if isinstance(a, M.Lit) and isinstance(a.type, M.Named):
                    model.lit_types.add(path_of(a.type.name))


def is_lua_sole(params):
    return params == (LUA_STATE,)


def ret_for(sym, part):
    """(ret type, fixed text)."""
    if part.kind in ("ctor", "dtor", "cv"):
        return None, ""
    if part.kind == "op" and part.name in ("new", "new[]"):
        return None, "void*"
    if part.kind == "op" and part.name in ("delete", "delete[]"):
        return None, "void"
    if sym.ret is not None:
        return sym.ret, None
    if is_lua_sole(sym.params):
        return None, "int"
    return None, None


def build(facts, existing):
    """Model of everything in the denominator that the existing tree does not declare."""
    m = Model()
    m.symbols = facts.by_mangled
    for s in facts.den:
        if s["kind"] == "thunk":
            try:
                y = M.parse(s["mangled"])
            except M.MangleError as e:
                m.skip(s, f"demangle: {e}")
                continue
            tgt = "_Z" + s["mangled"][s["mangled"].index("_", 4) + 1:]
            m.thunks.setdefault(tgt, []).append(s["addr"])
            m.thunks.setdefault(("sig",) + _sig_key(y), []).append(s["addr"])
    for s in sorted(facts.den, key=lambda s: (s["addr"], s["mangled"])):
        k = s["kind"]
        if k in ("thunk", "vtable"):
            continue
        if existing.declares_symbol(s["mangled"]):
            continue
        if k == "c":
            _add_c(m, s)
            continue
        try:
            y = M.parse(s["mangled"])
        except M.MangleError as e:
            m.skip(s, f"demangle: {e}")
            continue
        try:
            sc, tpos = scope_split(y.name)
        except ModelError as e:
            m.skip(s, str(e))
            continue
        path = path_of(sc)
        if is_std(path):
            m.skip(s, "std scope")
            continue
        if y.kind == "data":
            _add_data(m, s, y, sc, tpos)
        elif y.kind == "function":
            _add_function(m, facts, s, y, sc, tpos)
        else:
            m.skip(s, f"symbol kind {y.kind}")
    for v in facts.bf["vtables"]:
        if facts.groups.get(("vtable", v["class"])) != "firaxis" or v.get("construction"):
            continue
        try:
            y = M.parse("_ZTV" + _ti_suffix(facts, v))
        except (M.MangleError, KeyError) as e:
            m.skip({"mangled": v["class"], "demangled": v["class"]}, f"vtable: {e}")
            continue
        n = y.type.name
        sc = m.scope(path_of(n))
        sc.dynamic = True
        sc.evidence.append("vtable")
        if n.last.targs is not None:
            try:
                sc.vtables[_template(m, sc, n.last.targs, n)] = v["addr"]
            except ModelError as e:
                m.skip({"mangled": v["class"], "demangled": v["class"]}, str(e))
        else:
            sc.vtable = v["addr"]
    for t in facts.bf["typeinfo"]:
        if facts.groups.get(("typeinfo", t["class"])) != "firaxis":
            continue
        try:
            y = M.parse(t["symbol"])
        except M.MangleError as e:
            m.skip({"mangled": t["symbol"], "demangled": t["class"]}, f"typeinfo: {e}")
            continue
        n = y.type.name
        sc = m.scope(path_of(n))
        sc.evidence.append("typeinfo")
        sc.typeinfo = t["symbol"]
        bases = []
        for b in t.get("bases") or ():
            try:
                by = M.parse(b["typeinfo"])
            except (M.MangleError, TypeError):
                bases = None
                break
            bn = by.type.name
            collect_refs(m, by.type)
            bases.append((path_of(bn), bn, b["offset"], b["public"], b["virtual"]))
        if n.last.targs is not None:
            try:
                spec = _template(m, sc, n.last.targs, n)
            except ModelError as e:
                m.skip({"mangled": t["symbol"], "demangled": t["class"]}, str(e))
                continue
            if bases is not None:
                sc.inst_bases[spec] = bases
            continue
        if bases is None:
            m.decisions["bases_unknown"].append(t["class"])
        else:
            sc.bases = bases
    return m


def _ti_suffix(facts, v):
    for s in facts.bf["symbols"]:
        if s["kind"] == "vtable" and s["addr"] == v["addr"]:
            return s["mangled"][4:]
    raise KeyError(v["class"])


def _sig_key(y):
    return (y.name, y.params, y.const)


def _template(m, sc, targs, n):
    spec = M.Speller().name(n)
    kinds = [("value", a.type) if isinstance(a, M.Lit) else ("class", None) for a in targs]
    if sc.tparams is None:
        sc.tparams = kinds
    elif [k for k, _ in sc.tparams] != [k for k, _ in kinds]:
        raise ModelError(f"template parameter kinds differ: {spec}")
    sc.insts[spec] = targs
    for a in targs:
        collect_refs(m, a)
    return spec


def _add_c(m, s):
    sc = m.scope(())
    if s["type"] != "func":
        sc.vars.append(Var((), s["name"], s["size"], False, True, [Inst(None, s["addr"], mangled=s["mangled"])],
                           (int(s["addr"], 16),)))
    else:
        f = Fn((), M.Part(s["name"]), "c", None, insts=[Inst(None, s["addr"], mangled=s["mangled"])],
               order=(int(s["addr"], 16),))
        m.params_unknown += 1
        sc.fns.append(f)


def _add_data(m, s, y, sc_name, tpos):
    path = path_of(sc_name)
    sc = m.scope(path)
    spec = None
    if tpos is not None:
        try:
            spec = _tspec(m, sc, sc_name, tpos)
        except ModelError as e:
            m.skip(s, str(e))
            return
    member = bool(s.get("scope_is_class")) or tpos is not None
    name = y.name.last.name
    for v in sc.vars:
        if v.name == name:
            v.insts.append(Inst(spec, s["addr"], mangled=s["mangled"]))
            v.size = max(v.size, s["size"])
            return
    sc.vars.append(Var(path, name, s["size"], member, False, [Inst(spec, s["addr"], mangled=s["mangled"])],
                       (int(s["addr"], 16),)))


def _add_function(m, facts, s, y, sc_name, tpos):
    path = path_of(sc_name)
    sc = m.scope(path)
    spec = None
    if tpos is not None:
        try:
            spec = _tspec(m, sc, sc_name, tpos)
        except ModelError as e:
            m.skip(s, str(e))
            return
    part = y.name.last
    ftemplate = None
    if part.targs is not None and part.kind not in ("ctor", "dtor"):
        if tpos is not None:
            m.skip(s, "member template of a class template")
            return
        if part.kind == "cv":
            m.skip(s, "conversion operator template")
            return
        ftemplate = part.targs
        spec = M.Speller().part(part, None)
        part = M.Part(part.name, part.kind, None, part.conv)
    for p in y.params:
        collect_refs(m, p)
    if y.ret is not None:
        collect_refs(m, y.ret)
    if ftemplate:
        for a in ftemplate:
            collect_refs(m, a)
    if part.conv is not None:
        collect_refs(m, part.conv)
    kind = part.kind if part.kind in ("ctor", "dtor") else "method"
    if part.kind in ("ctor", "dtor"):
        sc.evidence.append(part.kind)
    if part.kind == "cv" or (part.kind == "op" and part.name in ("()", "[]", "=", "->")):
        sc.evidence.append("member operator")
    if s.get("scope_is_class"):
        sc.evidence.append("binfacts")
    if y.const:
        sc.evidence.append("const")
    ret, ret_text = ret_for(y, part)
    f = Fn(path, part, kind, y.params, y.variadic, y.const, ret, ret_text, ftemplate=ftemplate,
           order=(int(s["addr"], 16),))
    variant = s.get("variant")
    key = f.key()
    for g in sc.fns:
        if g.key() == key:
            f = g
            break
    else:
        sc.fns.append(f)
    inst = next((i for i in f.insts if i.spec == spec), None)
    if inst is None:
        inst = Inst(spec, targs=y.name.last.targs if ftemplate else None)
        f.insts.append(inst)
    if variant in ("C2", "D2"):
        inst.base = s["addr"]
    elif variant == "D0":
        inst.deleting = s["addr"]
    elif variant in ("C1", "D1") or variant is None:
        inst.addr = s["addr"]
        inst.mangled = s["mangled"]
    else:
        m.skip(s, f"variant {variant}")
    if variant in ("C2", "D2") and inst.mangled is None:
        inst.mangled = s["mangled"]
    thunks = m.thunks.get(s["mangled"])
    if thunks:
        for t in thunks:
            if t not in inst.thunks:
                inst.thunks.append(t)
    if facts.lua.is_entry(s["mangled"], luaentry._strip_args(s["scope"]) if s.get("scope") else None, s["name"]):
        f.lua = True


def finish_structors(model):
    """C1 = C2 / D1 = D2 by address: `/// 0x` only; C2/D2 alone: a lone `base` (OD3)."""
    for sc in model.scopes.values():
        for f in sc.fns:
            if f.kind not in ("ctor", "dtor"):
                continue
            for i in f.insts:
                if i.addr is not None and i.base == i.addr:
                    i.base = None


# -- kinds of scopes ---------------------------------------------------------------------------------------
def classify(model, existing, foreign):
    """Kind of each path: existing | foreign | std | class | namespace | enum; decisions go to the report."""
    for p in sorted(model.type_refs):
        if p and not is_std(p):
            model.scope(p)
    for path in sorted(model.scopes, key=len):
        sc = model.scopes[path]
        if not path:
            sc.kind = "namespace"
            continue
        if is_std(path):
            sc.kind = "std"
            continue
        ek = existing.kind(path)
        if ek == "namespace":
            sc.kind = "namespace"
            continue
        if ek is not None:
            sc.kind = "existing"
            sc.handwritten = existing.handwritten(path)
            sc.file = existing.file(path)
            sc.evidence.append(f"existing:{ek}")
            continue
        fk = foreign.kind(path)
        if fk is not None:
            sc.kind = "foreign"
            sc.file = foreign.header(path)
            continue
        parent = model.scopes.get(path[:-1])
        parent_class = parent is not None and parent.kind in ("class", "existing", "foreign") and len(path) > 1
        has_members = bool(sc.fns or sc.vars) or any(q[:-1] == path for q in model.scopes if len(q) == len(path) + 1)
        if path in existing.fwd_names:
            sc.evidence.append("tree forward declaration")
        if sc.evidence or sc.tparams is not None or sc.dynamic:
            sc.kind = "class"
        elif path in model.lit_types:
            sc.kind = "enum"
        elif path in model.type_refs and not has_members:
            if ENUM_NAME.match(path[-1]):
                sc.kind = "enum"
                model.decisions["enum_by_name"].append("::".join(path))
            else:
                sc.kind = "class"
        elif path in model.type_refs:
            sc.kind = "class"
        elif parent_class:
            sc.kind = "class"
        else:
            sc.kind = "namespace"
            if sc.fns or sc.vars:
                model.decisions["namespace"].append("::".join(path))
    # a class makes everything under it a class (namespaces cannot nest in classes)
    for path in sorted(model.scopes, key=len):
        sc = model.scopes[path]
        par = model.scopes.get(path[:-1]) if len(path) > 1 else None
        if par is not None and par.kind in ("class", "enum") and sc.kind == "namespace":
            sc.kind = "class"


def finish_members(model):
    """static vs member (OD22) and the counters."""
    for sc in model.scopes.values():
        cls = sc.kind == "class" or sc.handwritten
        for f in sc.fns:
            if not cls:
                f.kind = "free" if f.kind == "method" else f.kind
                continue
            if f.kind != "method" or f.virtual:
                continue
            if f.part.kind == "op" and f.part.name in ("new", "new[]", "delete", "delete[]"):
                continue
            if f.const:
                continue
            if is_lua_sole(f.params) and f.ftemplate is None:
                f.static = True
                model.decisions["static_lua"].append(_label(sc, f))
            else:
                model.decisions["member_default"].append(_label(sc, f))
        for v in sc.vars:
            v.member = cls


def _label(sc, f):
    return "::".join(sc.path + (M.Speller().part(f.part, sc.path[-1] if sc.path else None),))


def template_template_params(model):
    """A bare class template name as an argument: that parameter is a template template parameter."""
    uses = defaultdict(list)
    for n in model.tnames:
        for k in range(1, len(n.parts) + 1):
            if n.parts[k - 1].targs is not None:
                uses[path_of(n)[:k]].append(n.parts[k - 1].targs)
    for sc in model.scopes.values():
        if sc.kind != "class" or sc.tparams is None:
            continue
        for targs in list(sc.insts.values()) + uses.get(sc.path, []):
            for k, a in enumerate(targs):
                if k < len(sc.tparams) and isinstance(a, M.Named) and a.name.last.targs is None:
                    t = model.scopes.get(path_of(a.name))
                    if t is not None and t.kind == "class" and t.tparams is not None:
                        sc.tparams[k] = ("template", path_of(a.name))


def apply_template_refs(model, existing):
    """Type-only class templates get their parameter kinds from the arguments they are used with; the argument
    of an existing template's template template parameter is a class template (forward declaration)."""
    for n in sorted(model.tnames, key=repr):
        p = path_of(n)
        for k in range(1, len(p) + 1):
            part = n.parts[k - 1]
            if part.targs is None:
                continue
            q = p[:k]
            sc = model.scopes.get(q)
            if sc is None:
                continue
            if sc.kind == "class" and sc.tparams is None:
                sc.tparams = [("value", a.type) if isinstance(a, M.Lit) else ("class", None) for a in part.targs]
            elif sc.kind == "existing":
                heads = existing.template_params(q)
                for j, h in enumerate(heads or ()):
                    if h.startswith("template") and j < len(part.targs) and isinstance(part.targs[j], M.Named):
                        tq = path_of(part.targs[j].name)
                        tsc = model.scopes.get(tq)
                        if tsc is not None and tsc.kind == "class":
                            tsc.tt_header = h[:h.rindex(" class ")] if " class " in h else h


def drop_undeclarable(model):
    """Functions whose types name a nested type of an existing class that the class does not declare."""
    bad = set()
    for p, sc in model.scopes.items():
        if sc.kind in ("existing", "foreign", "std", "namespace") or not p:
            continue
        for k in range(1, len(p)):
            anc = model.scopes.get(p[:k])
            if anc is not None and anc.kind == "existing":
                if not anc.handwritten:
                    bad.add(p)
                break
    if not bad:
        return
    for sc in model.scopes.values():
        keep = []
        for f in sc.fns:
            ts = list(f.params or ()) + ([f.ret] if f.ret is not None else [])
            hit = [path_of(n) for t in ts for n in M.names_in(t) if path_of(n) in bad]
            if hit:
                for i in f.insts:
                    model.skipped.append({"entity": "::".join(sc.path) + "::" + (f.part.name if f.part else "?"),
                                          "mangled": i.mangled or i.addr or "", "reason":
                                          "uses a nested type the existing class does not declare: "
                                          + "::".join(hit[0])})
                continue
            keep.append(f)
        sc.fns = keep
    for p in bad:
        model.scopes[p].kind = "undeclarable"
