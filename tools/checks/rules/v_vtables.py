"""V1-V3: declared vtables, virtual slots and bases against the _ZTV and _ZTI data of the binary."""
import re

from checks.api import Rule
from checks.rules.a_symbols import base_dtors, bin_index, cached, ents, fmt, hexa


def tree_records(ctx):
    return [r for r in ctx.typedb.get("records", []) if r.get("origin") == "tree"]


class V1(Rule):
    id = "V1"
    title = "'/// vtable' is the address of the class's _ZTV symbol"
    needs = ("typedb", "binfacts")
    scope = "entity"
    deep = True

    def run(self, ctx):
        b = bin_index(ctx)
        out = []
        for r in tree_records(ctx):
            sym = r.get("vtable_symbol")
            if not sym:
                continue
            v = b.vtables.get(sym)
            va = hexa(v["addr"]) if v else None
            marker = r.get("vtable_marker")

            def emit(msg):
                out.append(self.finding(r["file"], r["line"], msg, entity=r["name"], mangled=sym,
                                        expected=fmt(va) if va is not None else None, actual=marker,
                                        key=f"V1:{sym}"))

            if marker is None:
                if v:
                    emit(f"{r['name']} has no '/// vtable', but {sym} = {fmt(va)}")
            elif marker == "?":
                if v:
                    emit(f"vtable ?, but {sym} = {fmt(va)}")
            elif v is None:
                emit(f"vtable {marker}, but {sym} is not in the binary")
            elif hexa(marker) != va:
                emit(f"vtable {marker}, but {sym} = {fmt(va)}")
        return out


def function_ents(ctx):
    def build():
        m = {}
        for e in ents(ctx):
            m[(e.fn["key"], e.spec)] = e
        return m
    return cached(ctx, "ent_by_key", build)


def _declared_target(e, kind):
    if kind == "D0":
        return e.deleting
    if kind == "D1":
        return e.addr if e.addr is not None else e.base
    return e.addr


def _equivalent_names(e, name):
    """The symbol of a slot, plus the D1/D2 alias that binfacts may list instead."""
    out = {name}
    v = (e.variants or {}) if e else {}
    for pair in (("D1", "D2"), ("C1", "C2")):
        if name in (v.get(pair[0]), v.get(pair[1])):
            out |= {v.get(pair[0]), v.get(pair[1])}
    out.discard(None)
    return out


class V2(Rule):
    id = "V2"
    title = "vtable groups and slots: counts, vbase offsets, typeinfo, pure, targets and thunks match _ZTV"
    needs = ("typedb", "binfacts")
    scope = "entity"
    deep = True

    def run(self, ctx):
        b = bin_index(ctx)
        by = function_ents(ctx)
        recs = {x["name"]: x for x in ctx.typedb.get("records", [])}
        out = []
        for r in tree_records(ctx):
            sym, vt = r.get("vtable_symbol"), r.get("vtable")
            v = b.vtables.get(sym) if sym else None
            if not vt or not v:
                continue

            def emit(msg, tag):
                out.append(self.finding(r["file"], r["line"], msg, entity=r["name"], mangled=sym,
                                        key=f"V2:{sym}:{tag}"))

            groups, bgroups = vt["groups"], v["groups"]
            if len(groups) != len(bgroups):
                emit(f"declared {len(groups)} vtable groups, in binary {len(bgroups)}", "groups")
            pool = list(bgroups)
            for gi, g in enumerate(groups):
                bg = next((x for x in pool if x["offset_to_top"] == g["offset_to_top"]), None)
                if bg is None:
                    emit(f"group {gi}: no group with offset_to_top {g['offset_to_top']} in binary", f"g{gi}")
                    continue
                pool.remove(bg)
                if list(bg["vbase_offsets"]) != list(g["vbase_offsets"]):
                    emit(f"group {gi}: vbase_offsets {g['vbase_offsets']}, in binary {bg['vbase_offsets']}",
                         f"g{gi}:vbase")
                want_ti = b.typeinfo_addr(r.get("typeinfo_symbol")) if r.get("typeinfo_symbol") else None
                got = bg["typeinfo"]
                got_a = hexa(got)
                if want_ti is not None and (got_a if got_a is not None else got) not in (want_ti, r["typeinfo_symbol"]):
                    emit(f"group {gi}: typeinfo {got}, expected {r['typeinfo_symbol']} at {fmt(want_ti)}",
                         f"g{gi}:typeinfo")
                if len(g["slots"]) != len(bg["slots"]):
                    emit(f"group {gi}: declared {len(g['slots'])} slots, in binary {len(bg['slots'])}", f"g{gi}:slots")
                    continue
                foreign = g["offset_to_top"] != 0 and not self._base_has_vtable(b, r, g["offset_to_top"], recs)
                for si, (s, bs) in enumerate(zip(g["slots"], bg["slots"])):
                    if g["offset_to_top"] != 0 and bs["kind"] == "func" and not bs.get("symbols"):
                        # an unexported secondary-group target (inlined thunk body, or a method that takes the
                        # subobject's this): counted; judged only when the declared slot has no entity in the class
                        ent = by.get((s.get("function"), r["name"] if r.get("template") else None))                             or by.get((s.get("function"), None))
                        if foreign or ent is not None:
                            continue
                    msg = self._slot(b, by, r, s, bs, base_dtors(ctx))
                    if msg:
                        emit(f"group {gi} slot {si}: {msg}", f"g{gi}:s{si}")
        return out

    @staticmethod
    def _base_has_vtable(b, r, offset_to_top, recs):
        """True when the base subobject at -offset_to_top has a _ZTV in binfacts (or cannot be identified)."""
        for d in r.get("bases") or ():
            if d.get("offset") == -offset_to_top and not d.get("virtual"):
                base = recs.get(d["name"]) or {}
                return bool(base.get("vtable_symbol") and b.vtables.get(base["vtable_symbol"]))
        return True

    @staticmethod
    def _slot(b, by, r, s, bs, bd):
        target = hexa(bs["target"])
        names = bs.get("symbols") or []
        binary = names[0] if names else (fmt(target) if bs["kind"] != "pure" else "pure")
        e = by.get((s.get("function"), r["name"] if r.get("template") else None)) or by.get((s.get("function"), None))
        if s["kind"] == "pure" or bs["kind"] == "pure":
            if s["kind"] != bs["kind"]:
                return f"declared {'pure' if s['kind'] == 'pure' else s['symbol']}, in binary {binary}"
            return None
        if s["kind"] in ("D0", "D1") and bs["kind"] != "thunk":
            bnames, baddrs = bd.of(e.fn.get("scope") if e else r["name"])
            if (names and set(names) <= bnames) or (target in baddrs and (e is None or e.addr != target)):
                return None      # the class has no destructor of its own: the slot is the base's one
        is_thunk = s.get("thunk") is not None
        if is_thunk != (bs["kind"] == "thunk"):
            return f"declared {s['symbol']}, in binary {binary}"
        if names and not (set(names) & _equivalent_names(e, s["symbol"])):
            return f"declared {s['symbol']}, in binary {binary}"
        if e is None:
            return None
        if is_thunk:
            if target not in e.thunks:
                return f"thunk {fmt(target)} of {s['symbol']} is not declared (thunk marker)"
            return None
        want = _declared_target(e, s["kind"])
        if want is not None and want != target:
            return f"{s['symbol']} is declared at {fmt(want)}, the slot points to {fmt(target)}"
        return None


def _canon(name):
    """A class name with template arguments in one spelling: no blanks, numeric suffixes lower case (4U = 4u)."""
    return re.sub(r"(?<![\w])(\d+)([uUlL]+)(?!\w)", lambda m: m.group(1) + m.group(2).lower(), "".join(name.split()))


def _rtti_kind(bases):
    if not bases:
        return "class"
    b = bases[0]
    if len(bases) == 1 and not b["virtual"] and b["offset"] == 0 and b["access"] == "public":
        return "si"
    return "vmi"


class V3(Rule):
    id = "V3"
    title = "bases (kind, order, offsets, virtual, public) match the RTTI of the class"
    needs = ("typedb", "binfacts")
    scope = "entity"
    deep = True

    def run(self, ctx):
        b = bin_index(ctx)
        recs = {r["name"]: r for r in ctx.typedb.get("records", [])}
        out = []
        for r in tree_records(ctx):
            sym = r.get("typeinfo_symbol")
            t = b.typeinfo.get(sym) if sym else None
            if t is None:
                continue

            def emit(msg, tag):
                out.append(self.finding(r["file"], r["line"], msg, entity=r["name"], mangled=sym,
                                        key=f"V3:{sym}:{tag}"))

            bases, rbases = r.get("bases") or [], t.get("bases") or []
            if len(bases) != len(rbases):
                emit(f"bases {[x['name'] for x in bases]}, in RTTI {[x['class'] for x in rbases]}", "count")
                continue
            bad = False
            for i, (d, rb) in enumerate(zip(bases, rbases)):
                base_rec = recs.get(d["name"])
                bsym = base_rec.get("typeinfo_symbol") if base_rec else None
                same = (bsym == rb["typeinfo"]) if bsym and rb.get("typeinfo") else \
                    (_canon(d["name"]) == _canon(rb["class"]))
                bits = []
                if not same:
                    bits.append("class")
                if d["virtual"] != rb["virtual"]:
                    bits.append("virtual")
                elif not d["virtual"] and d["offset"] != rb["offset"]:
                    bits.append("offset")
                if (d["access"] == "public") != bool(rb["public"]):
                    bits.append("access")
                if bits:
                    bad = True
                    emit(f"base {i} {d['name']}: in RTTI {'public ' if rb['public'] else ''}"
                         f"{'virtual ' if rb['virtual'] else ''}{rb['class']} {'+' if rb['offset'] >= 0 else ''}"
                         f"{rb['offset']} ({', '.join(bits)} differ)", f"base{i}")
            if not bad and _rtti_kind(bases) != t["kind"]:
                emit(f"kind {_rtti_kind(bases)} from the bases, in RTTI {t['kind']}", "kind")
        return out
