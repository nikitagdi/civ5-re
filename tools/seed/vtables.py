"""Virtuals: which virtual functions each dynamic class declares, in which order.

A slot of class C at subobject offset X maps to the slot of the direct base D that holds X (the primary base
for X = 0); a slot is inherited when its target equals D's target, otherwise C declares an override with the
name and signature of the slot family (the introducing class's slot). New entries of the primary group are
declared in slot order. Bases without `_ZTV` get their slots from the derived classes (`base_slots_by_prefix`).
"""
from collections import defaultdict

from seed import mangle as M
from seed.model import Fn, Inst, path_of

PURE = "__cxa_pure_virtual"


class Slot:
    __slots__ = ("kind", "symbols", "target")

    def __init__(self, kind, symbols, target):
        self.kind, self.symbols, self.target = kind, list(symbols or ()), target


class VClass:
    def __init__(self, name):
        self.name = name             # binfacts class spelling
        self.groups = {}             # offset -> [Slot]
        self.real = False            # has _ZTV
        self.bases = []              # [(name, offset)]
        self.mname = None            # mangle Name
        self.derived = []            # [(VClass, offset of this base in it)]


def _parse_class(symbol):
    return M.parse(symbol).type.name


class Engine:
    def __init__(self, model, facts, existing, foreign=None):
        self.m, self.f, self.ex, self.fo = model, facts, existing, foreign
        self.cls = {}
        self.fam_names = defaultdict(set)   # family -> {(Part, params, variadic, const)}
        self.fam_ret = {}                   # family introduced by an existing class -> its return spelling
        self.fam_pure = set()

    # -- inputs ----------------------------------------------------------------------------------------
    def load(self):
        for name, t in self.f.typeinfo.items():
            if self.f.groups.get(("typeinfo", name)) != "firaxis":
                continue
            c = self.get(name)
            try:
                c.mname = _parse_class(t["symbol"])
            except M.MangleError:
                continue
            for b in t.get("bases") or ():
                c.bases.append((b["class"], b["offset"]))
                self.get(b["class"]).derived.append((c, b["offset"]))
                if self.get(b["class"]).mname is None and b.get("typeinfo"):
                    try:
                        self.get(b["class"]).mname = _parse_class(b["typeinfo"])
                    except M.MangleError:
                        pass
        for name, v in self.f.vtables.items():
            if self.f.groups.get(("vtable", name)) not in ("firaxis",) and name not in self.cls:
                continue
            c = self.get(name)
            c.real = True
            for g in v["groups"]:
                c.groups[-g["offset_to_top"]] = [Slot(s["kind"], s.get("symbols"), s["target"]) for s in g["slots"]]
        # existing (SDK) classes without _ZTV: their declared slots, from typedb
        fn_addr, fn_pure = {}, set()
        for fn in self.ex.db.get("functions", []):
            mk = fn.get("markers") or {}
            for k in (fn.get("mangled"), fn.get("key")):
                if k:
                    fn_addr[k] = mk.get("addr") or mk.get("base")
                    if fn.get("pure"):
                        fn_pure.add(k)
        by_path = {}
        for c in self.cls.values():
            if c.mname is not None and c.mname.last.targs is None:
                by_path[path_of(c.mname)] = c
        for path, r in self.ex.all_records.items():
            c = by_path.get(path)
            if c is None or c.real or not r.get("vtable"):
                continue
            for g in r["vtable"]["groups"]:
                slots = []
                for s in g["slots"]:
                    sym = s.get("symbol")
                    if s.get("kind") == "pure" or (s.get("function") or sym) in fn_pure:
                        slots.append(Slot("pure", [PURE], None))
                    else:
                        a = fn_addr.get(s.get("function") or sym)
                        slots.append(Slot("func" if a else "unknown", [sym] if sym else [], a))
                c.groups[-g["offset_to_top"]] = slots
        # third-party bases with a _ZTV of their own
        for name, c in self.cls.items():
            if not c.real and name in self.f.vtables:
                c.real = True
                for g in self.f.vtables[name]["groups"]:
                    c.groups[-g["offset_to_top"]] = [Slot(s["kind"], s.get("symbols"), s["target"])
                                                     for s in g["slots"]]

    def get(self, name):
        c = self.cls.get(name)
        if c is None:
            c = self.cls[name] = VClass(name)
        return c

    # -- structure -------------------------------------------------------------------------------------
    def holder(self, c, x):
        """Direct base of c whose subobject holds offset x (x = 0: the primary base) -> (VClass, offset)."""
        best = None
        for bname, off in c.bases:
            if off <= x and (best is None or off >= best[1]):
                b = self.cls.get(bname)
                if b is not None and self.dynamic(b):
                    best = (b, off)
        if best is not None and x == 0 and best[1] != 0:
            return None
        return best

    _dyn = {}

    def dynamic(self, c):
        if c.real or any(c.groups.values()):
            return True
        if c.name in self._dyn:
            return self._dyn[c.name]
        self._dyn[c.name] = False
        r = any(self.dynamic(self.cls[b]) for b, _ in c.bases if b in self.cls)
        r = r or any(off != 0 and d.real and off in d.groups for d, off in c.derived)
        # a non-empty first base of a dynamic class with more bases holds the vptr: it is the primary base
        r = r or any(off == 0 and d.real and any(o > 0 for _, o in d.bases) for d, off in c.derived)
        if not r:
            r = bool(self.synthesize(c, 0, probe=True))
        self._dyn[c.name] = r
        return r

    def group(self, c, x):
        """Slots of c's group at offset x; synthesized for a class without _ZTV."""
        if x in c.groups:
            return c.groups[x]
        if c.real or (c.name, x) in self._busy:
            return None
        self._busy.add((c.name, x))
        try:
            return self.synthesize(c, x)
        finally:
            self._busy.discard((c.name, x))

    _busy = set()

    def real_groups(self, c, x, seen=()):
        """Groups at offset x of the classes with a _ZTV that derive from c (through classes without one)."""
        out = []
        for d, off in c.derived:
            if d.name == c.name or d.name in seen:
                continue
            if d.real:
                out.append(d.groups.get(off + x))
            else:
                out += self.real_groups(d, off + x, seen + (c.name,))
        return out

    def synthesize(self, c, x, probe=False):
        rows, exact, erows = [], [], []
        for d, off in c.derived:
            if d.name == c.name:
                continue
            if d.real:
                gs = [d.groups.get(off + x)]
            elif probe:
                gs = [None]
            else:
                g = self.group(d, off + x)
                # d is waiting for this very class (it is being synthesized): use the groups of d's own
                # real descendants instead of d's unfinished one
                gs = self.real_groups(d, off + x) if g is None else [g]
            for g in gs:
                if g:
                    rows.append(g)
                    if off + x != 0:
                        exact.append(len(g))
                        erows.append(g)
        if not rows:
            if not probe:
                c.groups[x] = []
            return []
        h = self.holder(c, x)
        n_base = 0
        if h is not None:
            if (h[0].name, x - h[1]) in self._busy:
                # the holder is being synthesized right now (it asked for this class as its own derived one):
                # its slot count is unknown yet, so no group is cached for this class
                return None
            hg = self.group(h[0], x - h[1])
            n_base = len(hg) if hg is not None else 0
        limit = min(len(r) for r in rows)
        if x != 0 or exact:
            n = min(exact) if exact else limit
            if probe:
                return [None] * n
        else:
            n = n_base
            own = self.scope_names(c)
            has_dtor = self.has_dtor_symbol(c)
            for i in range(n_base, limit):
                col = [r[i] for r in rows]
                if all(s.kind == "pure" for s in col) or any(
                        self.sym_scope(n_) in own for s in col for n_ in s.symbols):
                    n = max(n, i + 1)
                if i == n_base and has_dtor and self.is_dtor(rows[0], i) and i + 1 < limit:
                    n = max(n, i + 2)
            if n == n_base and not probe and h is None and self.dynamic(c):
                n = min(limit, 2 if self.is_dtor(rows[0], 0) else 1)
            if probe:
                return [None] * n
            if n > n_base:
                self.m.decisions["base_slots_by_prefix"].append(f"{c.name}: {n_base}..{n}")
        slots = []
        for i in range(n):
            col = [r[i] for r in rows if i < len(r)]
            ecol = [r[i] for r in erows if i < len(r) and r[i].kind == "func"]
            own = [s for s in col if any(self.sym_scope(n_) == self.scope_key(c) for n_ in s.symbols)]
            if own:
                s = own[0]
                slots.append(Slot(s.kind, [n_ for n_ in s.symbols if self.sym_scope(n_) == self.scope_key(c)],
                                  s.target))
            elif h is not None and i < n_base:
                slots.append(Slot("inherit", [], None))
            elif ecol:
                slots.append(Slot("func", [], ecol[0].target))
            elif all(s.kind == "pure" for s in col):
                slots.append(Slot("pure", [PURE], col[0].target))
            elif self.is_dtor(rows[0], i):
                slots.append(Slot("unknown-dtor", [], None))
            else:
                slots.append(Slot("unknown", [], None))
        c.groups[x] = slots
        return slots

    def has_dtor_symbol(self, c):
        key = self.scope_key(c)
        sc = self.m.scopes.get(key)
        if sc is not None and any(f.kind == "dtor" for f in sc.fns):
            return True
        r = self.ex.records.get(key)
        return bool(r and r.get("dynamic"))

    def depth(self, name, seen=()):
        """Inheritance depth: bases are declared before the classes derived from them (names given to slots)."""
        c = self.cls.get(name)
        if c is None or name in seen:
            return 0
        return max((self.depth(b, seen + (name,)) + 1 for b, _ in c.bases), default=0)

    def scope_key(self, c):
        return path_of(M.strip_targs(c.mname)) if c.mname else (c.name,)

    def scope_names(self, c, seen=None):
        out = {self.scope_key(c)}
        for b, _ in c.bases:
            if b in self.cls:
                out |= self.scope_names(self.cls[b])
        return out

    def sym_scope(self, mangled):
        y = self._parse(mangled)
        if y is None or y.name is None:
            return None
        return path_of(M.strip_targs(y.name.scope()))

    _pcache = {}

    def _parse(self, mangled):
        if mangled in self._pcache:
            return self._pcache[mangled]
        try:
            y = M.parse(mangled) if mangled.startswith("_Z") else None
        except M.MangleError:
            y = None
        self._pcache[mangled] = y
        return y

    def is_dtor(self, slots, i):
        s = slots[i]
        return any((self._parse(n) is not None and self._parse(n).name is not None
                    and self._parse(n).name.last.kind == "dtor") for n in s.symbols)

    # -- families --------------------------------------------------------------------------------------
    def family(self, c, x, i, depth=0):
        h = self.holder(c, x)
        if h is not None and depth < 64:
            g = self.group(h[0], x - h[1])
            if g is not None and i < len(g):
                return self.family(h[0], x - h[1], i, depth + 1)
        return (c.name, x, i)

    def base_target(self, c, x, i):
        h = self.holder(c, x)
        if h is None:
            return None, False
        g = self.group(h[0], x - h[1])
        if g is None or i >= len(g):
            return None, False
        s = g[i]
        if s.kind == "inherit":
            return self.base_target(h[0], x - h[1], i)
        return s, True

    def collect_names(self):
        by_path = {self.scope_key(c): c for c in self.cls.values() if c.mname is not None
                   and c.mname.last.targs is None}
        fns = {}
        for fn in self.ex.db.get("functions", []):
            if fn.get("mangled"):
                fns[fn["mangled"]] = fn
            if fn.get("key"):
                fns.setdefault(fn["key"], fn)
        for path, r in self.ex.records.items():
            c = by_path.get(path)
            if c is None or not r.get("vtable"):
                continue
            for g in r["vtable"]["groups"]:
                x = -g["offset_to_top"]
                for i, s in enumerate(g["slots"]):
                    fsym = s.get("function") or s.get("symbol")
                    sig = self.signature(fsym) if fsym else None
                    if sig is not None and (x not in c.groups or i < len(c.groups[x])):
                        fam = self.family(c, x, i)
                        self.fam_names[fam].add(sig)
                        fn = fns.get(fsym)
                        if fn is not None and fn.get("return") and sig != ("dtor",):
                            self.fam_ret.setdefault(fam, fn["return"])
        for c in self.cls.values():
            for x, g in list(c.groups.items()):
                for i, s in enumerate(g):
                    fam = self.family(c, x, i)
                    if s.kind == "pure":
                        self.fam_pure.add(fam)
                    for n in s.symbols:
                        sig = self.signature(n)
                        if sig is not None:
                            self.fam_names[fam].add(sig)
            self.foreign_names(c)

    def foreign_names(self, c):
        """Slots introduced by a third-party base (its header is in third_party): names, parameters, return type.
        A destructor takes two slots (complete, deleting); the other virtuals one each."""
        decl = self.fo.virtuals.get(self.scope_key(c)) if self.fo is not None and c.mname is not None else None
        g = c.groups.get(0)
        if not decl or not g or self.holder(c, 0) is not None:
            return
        if sum(2 if d[0].startswith("~") else 1 for d in decl) != len(g):
            return      # the header and the binary disagree on the slot count: no names
        i = 0
        for name, ret, params, const in decl:
            fam = (c.name, 0, i)
            if name.startswith("~"):
                self.fam_names[fam].add(("dtor",))
                self.fam_names[(c.name, 0, i + 1)].add(("dtor",))
                i += 2
                continue
            self.fam_names[fam].add((M.Part(name), tuple(M.Builtin(p) for p in params), False, const))
            self.fam_ret.setdefault(fam, ret)
            i += 1

    def signature(self, mangled):
        y = self._parse(mangled)
        if y is None or y.kind not in ("function", "thunk"):
            return None
        last = y.name.last
        if last.kind == "dtor":
            return ("dtor",)
        return (M.Part(last.name, last.kind, None, last.conv), y.params, y.variadic, y.const)

    def fam_signature(self, fam):
        sigs = sorted(self.fam_names.get(fam, ()), key=repr)
        if not sigs:
            return None
        if any(s == ("dtor",) for s in sigs):
            return ("dtor",)
        if len(sigs) > 1:
            self.m.decisions["slot_name_by_hierarchy"].append(f"{fam[0]}[{fam[2]}]: {len(sigs)} names")
        return sigs[0]


def run(model, facts, existing, foreign=None):
    """Add the virtual functions of the model's classes. Returns the engine (report counters)."""
    e = Engine(model, facts, existing, foreign)
    e.code = getattr(facts, "code", None)
    e.load()
    for c in list(e.cls.values()):
        if not c.real:
            e.group(c, 0)
    e.collect_names()
    index = {}
    for sc in model.scopes.values():
        for f in sc.fns:
            for i in f.insts:
                if i.mangled:
                    index[i.mangled] = (f, i)
                if i.addr:
                    index.setdefault(("addr", i.addr, sc.path), (f, i))
    stats = defaultdict(int)
    for name in sorted(e.cls, key=lambda n: (e.depth(n), n)):
        c = e.cls[name]
        if not c.real or c.mname is None or e.f.groups.get(("vtable", name)) != "firaxis":
            continue
        path = path_of(M.strip_targs(c.mname))
        sc = model.scopes.get(path)
        if sc is None or sc.kind != "class":
            continue
        spec = M.Speller().name(c.mname) if c.mname.last.targs is not None else None
        _declare(e, model, sc, c, spec, index, stats)
    for sc in model.scopes.values():
        if sc.kind == "class" and not sc.dynamic and sc.typeinfo:
            ve = e.cls.get(_ti_name(facts, sc.typeinfo))
            if ve is not None and e.dynamic(ve):
                sc.dynamic = True
                if sc.tparams is None and sc.vtable is None:
                    sc.vtable = "?"
                    _declare_synth(e, model, sc, ve, index, stats)
    return e, stats


def _ti_name(facts, symbol):
    for name, t in facts.typeinfo.items():
        if t["symbol"] == symbol:
            return name
    return None


def _method(e, model, sc, c, spec, key, sig, index, fam=None):
    """The Fn of class sc for a slot: an existing one by mangled name, or a new one."""
    if isinstance(key, str) and key in index:
        f, inst = index[key]
        if f.scope == sc.path:
            if fam is not None and fam in e.fam_ret and f.kind == "method":
                f.ret_text = e.fam_ret[fam]
            return f, inst
    for f in sc.fns:
        same_sig = sig is not None and sig != ("dtor",) and not f.virtual and f.kind == "method" and \
            (f.part, f.params, f.variadic, f.const) == tuple(sig) and any(i.spec == spec for i in f.insts)
        if (sig == ("dtor",) and f.kind == "dtor") or (f.virtual and getattr(f, "_vkey", None) == key) or same_sig:
            inst = next((i for i in f.insts if i.spec == spec), None)
            if inst is None:
                inst = Inst(spec)
                f.insts.append(inst)
            return f, inst
    if sig == ("dtor",):
        f = Fn(sc.path, M.Part("", "dtor"), "dtor", ())
    elif sig is None:
        f = Fn(sc.path, M.Part(f"vf_{fam[2]:02d}") if fam else None, "method")
        f._vf = True
    else:
        part, params, variadic, const = sig
        ret_text = "int" if params == (M.Ptr(M.Named(M.Name((M.Part("lua_State"),)))),) else None
        f = Fn(sc.path, part, "method", params, variadic, const, ret_text=ret_text)
    f._vkey = key
    if fam is not None and fam in e.fam_ret and f.kind == "method":
        f.ret_text = e.fam_ret[fam]
    inst = Inst(spec)
    f.insts.append(inst)
    sc.fns.append(f)
    if isinstance(key, str):
        index[key] = (f, inst)
    return f, inst


def _dtor_slot(e, c, prim, i):
    """Slot i of the primary group is a destructor entry (by its symbol, or by the family it belongs to)."""
    return e.is_dtor(prim, i) or e.fam_signature(e.family(c, 0, i)) == ("dtor",)


def _declare(e, model, sc, c, spec, index, stats):
    sc.dynamic = True
    prim = c.groups.get(0, [])
    by_target = {}
    pending, fresh, by_slot, thunk_fns = [], [], {}, []
    # primary group first (new entries get declaration order), then secondary groups
    for x in sorted(c.groups):
        for i, s in enumerate(c.groups[x]):
            bs, inherited = e.base_target(c, x, i)
            fam = e.family(c, x, i)
            new = not inherited
            own = [n for n in s.symbols if e.sym_scope(n) == path_of(M.strip_targs(c.mname))]
            if inherited and bs is not None and bs.kind not in ("unknown", "unknown-dtor") \
                    and bs.target == s.target and s.kind == bs.kind and not own:
                continue
            if inherited and bs is not None and s.kind == "pure" and bs.kind == "pure":
                continue
            if s.kind == "thunk":
                tnames = [n for n in s.symbols if n.startswith("_ZTh")]
                if not tnames:
                    continue
                tgt = "_Z" + tnames[0][tnames[0].index("_", 4) + 1:]
                sig = e.signature(tgt)
                f, inst = _method(e, model, sc, c, spec, tgt, sig or e.fam_signature(fam), index, fam)
                if s.target not in inst.thunks:
                    inst.thunks.append(s.target)
                f.virtual = True
                stats["thunk"] += 1
                thunk_fns.append((f, inst, s))
                continue
            if x != 0:
                pending.append((x, i, s, fam))
                continue
            is_dtor = e.is_dtor(prim, i) or (e.fam_signature(fam) == ("dtor",))
            if is_dtor:
                key = ("dtor",)
                f, inst = _method(e, model, sc, c, spec, own[0] if own else key, ("dtor",), index)
                f.virtual = True
                if s.kind == "pure":
                    # pure virtual destructor: `= 0`; its body (D1/D2/D0 symbols, if exported) keeps its markers
                    f.pure = True
                    if new and f.slot is None:
                        f.slot = i
                    by_slot[i] = f
                    continue
                d0 = i > 0 and e.is_dtor(prim, i - 1) and prim[i - 1].target != s.target
                # the complete destructor of this class is declared (slot i - 1): this entry is the deleting one
                d0 = d0 or (inst.addr is not None and i > 0 and _dtor_slot(e, c, prim, i - 1)
                            and prim[i - 1].target != s.target)
                d0 = d0 or any(n.endswith("D0Ev") for n in s.symbols)
                if d0:
                    inst.deleting = s.target
                elif inst.addr is None and inst.base != s.target:
                    # the complete destructor may be an alias of the base's (spec: still declared, own markers)
                    inst.addr = s.target
                if new and f.slot is None:
                    f.slot = i
                by_slot[i] = f
                continue
            if s.kind == "pure":
                sig = e.fam_signature(fam)
                f, inst = _method(e, model, sc, c, spec, ("pure", fam), sig, index, fam)
                f.virtual = f.pure = True
                f.slot = i if new else f.slot
                stats["pure"] += 1
                continue
            key = own[0] if own else ("addr", s.target)
            sig = e.signature(own[0]) if own else e.fam_signature(fam)
            if not own and sig is None:
                # the family has no name anywhere: vf_<NN> of the introducing class
                stats["vf"] += 1
            f, inst = _method(e, model, sc, c, spec, key, sig, index, fam)
            f.virtual = True
            if f.part is None:
                f.part = M.Part(f"vf_{fam[2]:02d}")
                f._vf = True
            if inst.addr is None:
                inst.addr = s.target
                if not own:
                    inst.fastcc = e.f.fastcc(s.target)
                    if inst.fastcc:
                        stats["fastcc"] += 1
            if new and f.slot is None:
                f.slot = i
                if not own and getattr(f, "_vf", False):
                    f._pfam = fam
                    fresh.append(f)
            by_target[s.target] = f
            by_slot[i] = f
    # an exported thunk of an unexported method: the method is the unnamed primary entry the thunk calls
    for tf, tinst, s in thunk_fns:
        if tinst.addr is not None or tinst.base is not None:
            continue
        pf = by_slot.get(_called_slot(e, prim, s.target))
        if pf is None or pf is tf or not getattr(pf, "_vf", False) or tf.kind != "method":
            continue
        _absorb(e, sc, spec, tf, tinst, pf, fresh)
        model.decisions["thunk_names_entry"].append(f"{c.name}[{pf.slot}]")
    # an exported thunk whose method has no code link to any primary entry (its body is inlined into the thunk):
    # clang gives every method that overrides only a secondary base's virtual a new primary slot of its own, so
    # the method must take one of the still unnamed primary entries. Both lists are in declaration order; the
    # extra unnamed entries are taken to be the first ones (decision thunk_names_by_order)
    named, seen = [], set()
    for tf, tinst, s in thunk_fns:
        if tf.kind == "method" and tf in sc.fns and tf.slot is None and id(tf) not in seen \
                and tinst.addr is None and tinst.base is None:
            seen.add(id(tf))
            named.append((tf, tinst))
    free = sorted((f for f in fresh if getattr(f, "_vf", False)), key=lambda f: f.slot)
    if named and len(free) >= len(named):
        for (tf, tinst), pf in zip(named, free[len(free) - len(named):]):
            _absorb(e, sc, spec, tf, tinst, pf, fresh)
            model.decisions["thunk_names_by_order"].append(f"{c.name}[{pf.slot}]")
    # unexported thunks look like plain functions in secondary groups: link each to the primary slot its code
    # calls (decision thunk_by_code), the rest in order when the counts agree (thunk_by_order)
    rest = []
    for x, i, s, fam in pending:
        k = _called_slot(e, prim, s.target)
        f = by_slot.get(k) if k is not None else None
        if f is None:
            rest.append((x, i, s, fam))
            continue
        _link(e, model, f, spec, s, fam, fresh, "thunk_by_code", f"{c.name}+{x}[{i}]")
    fresh = [f for f in fresh if not getattr(f, "_linked", False)]
    if rest and len(rest) == len(fresh):
        for (x, i, s, fam), f in zip(rest, sorted(fresh, key=lambda f: f.slot)):
            _link(e, model, f, spec, s, fam, fresh, "thunk_by_order", f"{c.name}+{x}[{i}]")
    elif rest:
        stats["secondary_unpaired"] += len(rest)
        model.decisions["thunk_unpaired"].extend(f"{c.name}+{x}[{i}]" for x, i, _, _ in rest)
    stats["classes"] += 1


def _absorb(e, sc, spec, tf, tinst, pf, fresh):
    """The unnamed primary entry pf is the method tf: it takes tf's name, signature and thunks."""
    pinst = next(k for k in pf.insts if k.spec == spec)
    pinst.thunks.extend(t for t in tinst.thunks if t not in pinst.thunks)
    pf.part, pf.params, pf.variadic, pf.const = tf.part, tf.params, tf.variadic, tf.const
    pf._vf = False
    tf.insts.remove(tinst)
    if not tf.insts and tf in sc.fns:
        sc.fns.remove(tf)
    if pf in fresh:
        fresh.remove(pf)
    pfam = getattr(pf, "_pfam", None)
    if pfam is not None:
        e.fam_names[pfam].add((pf.part, pf.params, pf.variadic, pf.const))


def _declare_synth(e, model, sc, c, index, stats):
    """A dynamic class without _ZTV: the slots it introduces (synthesized from the derived classes)."""
    g = c.groups.get(0) or []
    for i, s in enumerate(g):
        bs, inherited = e.base_target(c, 0, i)
        if inherited:
            continue
        fam = e.family(c, 0, i)
        if s.kind in ("unknown-dtor",) or e.fam_signature(fam) == ("dtor",):
            f, inst = _method(e, model, sc, c, None, ("dtor",), ("dtor",), index)
            f.virtual = True
            if f.slot is None:
                f.slot = i
            if s.symbols:
                pass
            f._synth_dtor = True
            continue
        sig = e.signature(s.symbols[0]) if s.symbols and s.kind == "func" else e.fam_signature(fam)
        key = s.symbols[0] if s.symbols and s.kind == "func" else ("synth", i)
        f, inst = _method(e, model, sc, c, None, key, sig, index, fam)
        f.virtual = True
        if f.part is None:
            f.part = M.Part(f"vf_{fam[2]:02d}")
        if f.slot is None:
            f.slot = i
        if s.kind == "func" and s.target and inst.addr is None:
            inst.addr = s.target
        elif s.kind != "func":
            f.pure = True
            if s.kind == "unknown":
                model.decisions["base_slots_by_prefix"].append(f"{c.name}[{i}] pure")
    stats["synth_classes"] += 1


def _called_slot(e, prim, target):
    if e.code is None:
        return None
    direct, indirect = e.code.calls(int(target, 16))
    cands = {k for k, ps in enumerate(prim) if ps.target and int(ps.target, 16) in direct}
    if not cands:
        cands = {d // 4 for d in indirect if d % 4 == 0 and d // 4 < len(prim)}
    return cands.pop() if len(cands) == 1 else None


def _link(e, model, f, spec, s, fam, fresh, decision, label):
    # the thunk itself is not exported, so it gets no `thunk` marker (A4); the link only names the override
    if f in fresh:
        sig = e.fam_signature(fam)
        if sig is not None and sig != ("dtor",):
            f.part, f.params, f.variadic, f.const = sig
        else:
            f.part = M.Part(f"vf_{fam[2]:02d}")
            f.params = None
        # the derived classes override this slot under the name it got here, not under its own index
        pfam = getattr(f, "_pfam", None)
        if pfam is not None:
            e.fam_names[pfam].add((f.part, f.params, f.variadic, f.const))
    f._linked = True
    model.decisions[decision].append(label)
