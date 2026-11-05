"""A1-A11: declared addresses, names and globals against the exported symbols of the binary."""
import bisect
import re
from collections import namedtuple

from checks.api import InputError, Rule
from civ5re import placeholders as ph

DATA_SECTIONS = (".data", ".bss", ".rodata", ".data.rel.ro", ".tdata", ".tbss")
THUNK_RE = re.compile(r"^_ZT(?:hn?\d+_|vn?\d+_n?\d+_|c(?:hn?\d+_|vn?\d+_n?\d+_){2})(.*)$")
GHIDRA_HINT = "civ5re ghidra functions"
ROLES = ("addr", "base", "deleting")

Sym = namedtuple("Sym", "name addr size type kind group section demangled")
Section = namedtuple("Section", "name addr end")
Ent = namedtuple("Ent", "fn inst spec mangled variants addr base deleting thunks fastcc inline pure absent "
                        "path line label")
Decl = namedtuple("Decl", "ent role addr name")


def hexa(v):
    if isinstance(v, int):
        return v
    if isinstance(v, str) and v.startswith("0x"):
        try:
            return int(v, 16)
        except ValueError:
            return None
    return None


def fmt(a):
    return "0x%08x" % a


def cached(ctx, key, build):
    store = ctx.__dict__.setdefault("_avl_cache", {})
    if key not in store:
        store[key] = build()
    return store[key]


class Bin:
    """Lookups over binfacts.json."""

    def __init__(self, bf):
        self.bf = bf
        self.syms = []
        self.by_name = {}
        self.at = {}
        self.names_at = {}
        for s in bf.get("symbols", []):
            x = Sym(s["mangled"], hexa(s["addr"]), s.get("size") or 0, s.get("type"), s.get("kind"),
                    s.get("group"), s.get("section"), s.get("demangled") or s["mangled"])
            self.syms.append(x)
            self.by_name.setdefault(x.name, []).append(x)
            self.at.setdefault(x.addr, []).append(x)
            self.names_at.setdefault(x.addr, set()).add(x.name)
        self.sections = [Section(s["name"], hexa(s["addr"]), hexa(s["addr"]) + s["size"])
                         for s in bf.get("sections", [])]
        eh = sorted((hexa(e["addr"]), e["size"]) for e in bf.get("functions_eh", []))
        self.eh_starts = [a for a, _ in eh]
        self.eh_size = dict(eh)
        objs = sorted((s.addr, s.addr + max(s.size, 1), s.name) for s in self.syms if s.type == "object")
        self.obj_starts = [o[0] for o in objs]
        self.objs = objs
        self.obj_maxend = []
        m = 0
        for o in objs:
            m = max(m, o[1])
            self.obj_maxend.append(m)
        self.vtables = {v["symbol"]: v for v in bf.get("vtables", [])}
        self.typeinfo = {t["symbol"]: t for t in bf.get("typeinfo", [])}
        self.pure_addrs = {hexa(p["addr"]) for p in bf.get("plt", []) if p.get("name") == "__cxa_pure_virtual"}
        self.thunk_syms = {}
        for s in self.syms:
            m = THUNK_RE.match(s.name)
            if m and s.type == "func":
                self.thunk_syms.setdefault("_Z" + m.group(1), []).append(s)

    def funcs_at(self, addr):
        return [s for s in self.at.get(addr, []) if s.type == "func"]

    def section_of(self, addr):
        return [s for s in self.sections if s.addr <= addr < s.end]

    def fde_containing(self, addr):
        i = bisect.bisect_right(self.eh_starts, addr) - 1
        if i >= 0 and addr < self.eh_starts[i] + self.eh_size[self.eh_starts[i]]:
            return self.eh_starts[i]
        return None

    def overlapping_object(self, a, e, skip=None):
        i = bisect.bisect_left(self.obj_starts, e) - 1
        while i >= 0 and self.obj_maxend[i] > a:
            o = self.objs[i]
            if o[1] > a and o[0] < e and o[2] != skip:
                return o
            i -= 1
        return None

    def typeinfo_addr(self, symbol):
        t = self.typeinfo.get(symbol)
        return hexa(t["addr"]) if t else None


def bin_index(ctx):
    return cached(ctx, "bin", lambda: Bin(ctx.binfacts))


class BaseDtors:
    """Destructors of the (transitive) bases of a class: their symbol names and marker addresses. A derived class
    without a destructor of its own shares the base's one (alias), so its address and slot may be the base's."""

    def __init__(self, db):
        recs = {r["name"]: r for r in db.get("records", [])}
        dt = {}
        for f in db.get("functions", []):
            if f.get("kind") == "dtor":
                dt.setdefault(f.get("scope"), []).append(f)
        self.recs, self.dt, self._memo = recs, dt, {}

    def of(self, cls):
        """(names, addrs) of the destructors of every base of cls."""
        if cls in self._memo:
            return self._memo[cls]
        names, addrs, seen, stack = set(), set(), set(), [cls]
        while stack:
            r = self.recs.get(stack.pop())
            for b in (r or {}).get("bases") or ():
                n = b.get("name")
                if n in seen:
                    continue
                seen.add(n)
                stack.append(n)
                for f in self.dt.get(n, ()):
                    names |= {x for x in (f.get("variants") or {}).values() if x} | {f.get("mangled")}
                    mk = f.get("markers") or {}
                    addrs |= {hexa(mk.get(k)) for k in ("addr", "base", "deleting")} - {None}
        self._memo[cls] = (names, addrs)
        return self._memo[cls]


def base_dtors(ctx):
    return cached(ctx, "base_dtors", lambda: BaseDtors(ctx.typedb))


def role_names(e):
    v = e.variants or {}
    if "C1" in v or "C2" in v:
        return {"addr": v.get("C1") or e.mangled, "base": v.get("C2"), "deleting": None}
    if "D1" in v or "D2" in v or "D0" in v:
        return {"addr": v.get("D1") or e.mangled, "base": v.get("D2"), "deleting": v.get("D0")}
    return {"addr": e.mangled, "base": None, "deleting": None}


def entities(db):
    """Function entities with markers: plain functions, and one per SPEC instantiation of a template member."""
    out = []
    for f in db.get("functions", []):
        mk = f.get("markers") or {}
        path, line = f["file"], f["line"]
        qual = f.get("qualified") or f.get("name")
        if f.get("mangled") or not f.get("instances"):
            out.append(Ent(f, None, None, f.get("mangled"), f.get("variants"), hexa(mk.get("addr")),
                           hexa(mk.get("base")), hexa(mk.get("deleting")),
                           tuple(hexa(t) for t in mk.get("thunks") or []), bool(mk.get("fastcc")),
                           bool(mk.get("inline")), bool(mk.get("pure")), bool(mk.get("absent")),
                           path, line, qual))
        for i in f.get("instances") or []:
            label = f"{i['spec']}::{f['name']}" if f.get("scope_kind") == "class" else i["spec"]
            out.append(Ent(f, i, i["spec"], i.get("mangled"), i.get("variants"), hexa(i.get("addr")),
                           hexa(i.get("base")), hexa(i.get("deleting")),
                           tuple(hexa(t) for t in i.get("thunks") or []), bool(i.get("fastcc")),
                           False, False, False, path, line, label))
    return out


def ents(ctx):
    return cached(ctx, "ents", lambda: entities(ctx.typedb))


def decls(ctx):
    """Address declarations of functions: one per marker with an address (thunk role included)."""
    def build():
        out = []
        for e in ents(ctx):
            names = role_names(e)
            for role in ROLES:
                a = getattr(e, role)
                if a is not None:
                    out.append(Decl(e, role, a, names[role]))
            for t in e.thunks:
                if t is not None:
                    out.append(Decl(e, "thunk", t, None))
        return out
    return cached(ctx, "decls", build)


def code_decls(ctx):
    return [d for d in decls(ctx) if d.role != "thunk"]


def where(d):
    return f"{d.ent.path}:{d.ent.line} {d.ent.label}"


class A1(Rule):
    id = "A1"
    title = "the address of a function with an exported mangled name matches the symbol's address"
    needs = ("typedb", "binfacts")
    scope = "entity"

    def run(self, ctx):
        b = bin_index(ctx)
        out = []
        for d in code_decls(ctx):
            if d.ent.spec is not None or d.name is None:
                continue
            for s in b.by_name.get(d.name, []):
                if s.type == "func" and s.addr != d.addr:
                    out.append(self.finding(
                        d.ent.path, d.ent.line,
                        f"{d.name}: marker {fmt(d.addr)}, in binary {fmt(s.addr)}"
                        + ("" if d.role == "addr" else f" ({d.role})"),
                        entity=d.ent.label, mangled=d.name, expected=fmt(s.addr), actual=fmt(d.addr),
                        key=f"A1:{d.name}"))
                    break
        return out


class A2(Rule):
    id = "A2"
    title = "an address that starts an exported function carries the declared mangled name"
    needs = ("typedb", "binfacts")
    scope = "entity"

    def run(self, ctx):
        b = bin_index(ctx)
        out = []
        for d in code_decls(ctx):
            if d.name is None:
                continue
            here = b.funcs_at(d.addr)
            if not here or any(s.name == d.name for s in here):
                continue
            if d.ent.fn.get("kind") == "dtor" and any(s.name in base_dtors(ctx).of(d.ent.fn.get("scope"))[0]
                                                      for s in here):
                continue         # the class has no destructor of its own: it is the base's one
            s = here[0]
            out.append(self.finding(
                d.ent.path, d.ent.line,
                f"at {fmt(d.addr)} exported {s.demangled}, declared {d.name}",
                entity=d.ent.label, mangled=d.name, expected=s.name, actual=d.name,
                key=f"A2:{fmt(d.addr)}:{d.name}"))
        return out


def structor_kind(e):
    v = e.variants or {}
    if "C1" in v or "C2" in v:
        return "ctor"
    if "D1" in v or "D2" in v or "D0" in v:
        return "dtor"
    return None


class A3(Rule):
    id = "A3"
    title = "ctor/dtor markers: '///' = C1/D1, 'base' = C2/D2, 'deleting' = D0, each only where the variant is exported"
    needs = ("typedb", "binfacts")
    scope = "entity"

    def run(self, ctx):
        b = bin_index(ctx)
        out = []

        def exp(name):
            for s in b.by_name.get(name, []):
                if s.type == "func":
                    return s.addr
            return None

        for e in ents(ctx):
            kind = structor_kind(e)
            if kind is None or (e.addr is None and e.base is None and e.deleting is None):
                continue
            v = e.variants
            first, second = ("C1", "C2") if kind == "ctor" else ("D1", "D2")
            n1, n2, n0 = v.get(first), v.get(second), v.get("D0")
            a1, a2, a0 = (exp(n) if n else None for n in (n1, n2, n0))
            if a1 is None and a2 is None and a0 is None:
                continue

            def emit(msg, tag):
                out.append(self.finding(e.path, e.line, msg, entity=e.label, mangled=n1 or n2,
                                        key=f"A3:{n1 or n2}:{tag}"))

            if e.addr is not None and a1 is None:
                emit(f"address {fmt(e.addr)} on {first} {n1}, which is not exported"
                     + (f"; only {second} is: use 'base {fmt(a2)}'" if a2 is not None else ""), "addr")
            if e.addr is None and a1 is not None:
                emit(f"{first} is exported at {fmt(a1)}: address marker missing", "addr-missing")
            if e.base is not None:
                if a2 is None:
                    emit(f"base {fmt(e.base)} on {second} {n2}, which is not exported", "base")
                elif a1 is not None and a1 == a2:
                    emit(f"base {fmt(e.base)} extra: {second} = {first} = {fmt(a1)}", "base")
            elif a2 is not None and (a1 is None or a1 != a2):
                if a1 is None:
                    if e.addr is None:
                        emit(f"only {second} is exported at {fmt(a2)}: 'base {fmt(a2)}' missing", "base-missing")
                else:
                    emit(f"base missing: {second} is at {fmt(a2)}, {first} at {fmt(a1)}", "base-missing")
            if kind == "dtor":
                if e.deleting is not None and a0 is None:
                    emit(f"deleting {fmt(e.deleting)} on D0 {n0}, which is not exported", "deleting")
                elif e.deleting is None and a0 is not None:
                    emit(f"deleting missing: D0 is exported at {fmt(a0)}", "deleting-missing")
            elif e.deleting is not None:
                emit(f"deleting {fmt(e.deleting)} on a constructor", "deleting")
        return out


def thunk_target(name):
    m = THUNK_RE.match(name)
    return "_Z" + m.group(1) if m else None


class A4(Rule):
    id = "A4"
    title = "the thunk markers of a method are the exported _ZTh/_ZTv symbols that target it"
    needs = ("typedb", "binfacts")
    scope = "entity"

    def run(self, ctx):
        b = bin_index(ctx)
        out = []
        for e in ents(ctx):
            if e.addr is None and e.base is None:
                continue
            names = {n for n in (e.variants or {}).values() if n} | ({e.mangled} if e.mangled else set())
            want = {}
            for n in names:
                for s in b.thunk_syms.get(n, []):
                    want[s.addr] = s.name
            have = {t for t in e.thunks if t is not None}
            for a in sorted(set(want) - have):
                out.append(self.finding(e.path, e.line, f"no thunk {fmt(a)} ({want[a]})", entity=e.label,
                                        mangled=want[a], key=f"A4:{want[a]}"))
            for a in sorted(have - set(want)):
                other = [s.name for s in b.funcs_at(a)]
                why = f"{fmt(a)} is {other[0]}" if other else f"no exported thunk at {fmt(a)}"
                out.append(self.finding(e.path, e.line, f"thunk {fmt(a)} extra: {why} does not target "
                                        f"{e.mangled or e.label}", entity=e.label,
                                        key=f"A4:{e.mangled or e.label}:{fmt(a)}"))
        return out


class A5(Rule):
    id = "A5"
    title = "the instantiation named by '/// 0x... SPEC' is exported at exactly that address"
    needs = ("typedb", "binfacts")
    scope = "entity"
    deep = True

    def run(self, ctx):
        b = bin_index(ctx)
        out = []
        for d in code_decls(ctx):
            if d.ent.spec is None or d.name is None:
                continue
            for s in b.by_name.get(d.name, []):
                if s.type == "func" and s.addr != d.addr:
                    out.append(self.finding(
                        d.ent.path, d.ent.line,
                        f"{d.ent.label}: marker {fmt(d.addr)}, in binary {fmt(s.addr)}"
                        + ("" if d.role == "addr" else f" ({d.role})"),
                        entity=d.ent.label, mangled=d.name, expected=fmt(s.addr), actual=fmt(d.addr),
                        key=f"A5:{d.name}"))
                    break
        return out


class A6(Rule):
    id = "A6"
    title = "inline, pure or absent, but a symbol with this mangled name exists in the binary"
    needs = ("typedb", "binfacts")
    scope = "entity"

    def run(self, ctx):
        b = bin_index(ctx)
        out = []
        for e in ents(ctx):
            word = "inline" if e.inline else "pure" if e.pure else "absent" if e.absent else None
            if word is None:
                continue
            names = {n for n in (e.variants or {}).values() if n} | ({e.mangled} if e.mangled else set())
            found = sorted((s.addr, s.name) for n in names for s in b.by_name.get(n, []) if s.type == "func")
            if found:
                a, n = found[0]
                out.append(self.finding(e.path, e.line, f"{word}, but symbol exists: {fmt(a)} ({n})",
                                        entity=e.label, mangled=n, key=f"A6:{n}"))
        for g in ctx.typedb.get("globals", []):
            if not g.get("inline_marker"):
                continue
            found = [s for s in b.by_name.get(g.get("mangled"), []) if s.type != "func"]
            if found:
                out.append(self.finding(g["file"], g["line"],
                                        f"inline, but symbol exists: {fmt(found[0].addr)} ({found[0].name})",
                                        entity=g.get("qualified"), mangled=found[0].name,
                                        key=f"A6:{found[0].name}"))
        return out


class A7(Rule):
    id = "A7"
    title = "a non-exported function address starts a function (FDE or Ghidra list); fastcc <=> r= in the list"
    needs = ("typedb", "binfacts", "ghidra")
    scope = "entity"

    def run(self, ctx):
        b = bin_index(ctx)
        g = ctx.ghidra
        out = []
        for d in code_decls(ctx):
            e, a = d.ent, d.addr
            label = e.label

            def err(msg, tag):
                out.append(self.finding(e.path, e.line, f"{fmt(a)} {msg}", entity=label, key=f"A7:{fmt(a)}:{tag}"))

            if b.funcs_at(a):
                if e.fastcc and d.role == "addr":
                    err("is exported, fastcc is only for internal functions", "fastcc-exported")
                continue
            secs = b.section_of(a)
            if not any(s.name == ".text" for s in secs):
                err(f"is not in .text ({secs[0].name if secs else 'no section'})", "section")
                continue
            fde_start = a in b.eh_size
            fde_in = b.fde_containing(a)
            row = g.containing(a) if g else None
            if row is not None and row[0] != a:
                err(f"inside {row[2]}+0x{a - row[0]:x}", "inside")
                continue
            if fde_in is not None and fde_in != a:
                err(f"inside FDE {fmt(fde_in)}+0x{a - fde_in:x}", "inside")
                continue
            listed = row is not None
            if g is None:
                parts = []
                if not fde_start:
                    parts.append("has no FDE")
                if e.fastcc:
                    parts.append("fastcc cannot be checked")
                if parts:
                    out.append(self.warn(e.path, e.line, f"{fmt(a)} {', '.join(parts)}, no Ghidra list: {GHIDRA_HINT}",
                                         entity=label, key=f"A7:{fmt(a)}:nolist"))
                continue
            if not (fde_start or listed):
                err("is not a function start: no FDE, not in the Ghidra list", "start")
                continue
            has_r = g.fastcc(a)
            if e.fastcc and not has_r:
                err("fastcc, but it has no r= in the Ghidra list", "fastcc")
            elif has_r and not e.fastcc and d.role == "addr":
                err("has r= in the Ghidra list, but no fastcc", "r")
        return out


class A8(Rule):
    id = "A8"
    title = "one address, one entity; only names that binfacts itself puts on one address may share it"
    needs = ("typedb", "binfacts")
    scope = "tree"

    def run(self, ctx):
        b = bin_index(ctx)
        items = []
        for d in decls(ctx):
            e = d.ent
            ident = (e.fn["key"], e.spec)
            items.append((e.path, e.line, d.addr, ident, d.name, e.label, e.fn.get("kind"), e.fn.get("scope"),
                          True if e.fn.get("virtual") else None))
        for g in ctx.typedb.get("globals", []):
            a = hexa(g.get("addr_marker"))
            if a is not None:
                items.append((g["file"], g["line"], a, ("global", g.get("qualified")), g.get("mangled"),
                              g.get("qualified"), "global", None, None))
        by_addr = {}
        for it in sorted(items, key=lambda i: (i[0], i[1], i[3][0], str(i[3][1]))):
            by_addr.setdefault(it[2], []).append(it)
        out = []
        for a in sorted(by_addr):
            group = by_addr[a]
            idents = []
            for it in group:
                if it[3] not in [x[3] for x in idents]:
                    idents.append(it)
            if len(idents) < 2:
                continue
            if len({x[3][0] for x in idents}) == 1 and idents[0][6] != "global":
                continue         # instances of one template family may be folded into one address (ICF)
            if None not in {x[8] for x in idents}:
                continue         # virtual methods only: vtable slot targets are inherited or folded (ICF)
            names = b.names_at.get(a, set())
            named = [x for x in idents if any(i[4] is not None and i[4] in names for i in group if i[3] == x[3])]
            first = (named or idents)[0]
            for it in idents:
                if it is first or it in named:
                    continue
                out.append(self.finding(it[0], it[1], f"{fmt(a)} already taken: {first[0]}:{first[1]} {first[5]}",
                                        entity=it[5], key=f"A8:{fmt(a)}:{it[5]}"))
        return out


class A9(Rule):
    id = "A9"
    title = "a global: exported - address and sizeof match the symbol; otherwise inside a data section, no overlap"
    needs = ("typedb", "binfacts")
    scope = "entity"

    def run(self, ctx):
        b = bin_index(ctx)
        out = []
        placed = []
        for g in ctx.typedb.get("globals", []):
            a = hexa(g.get("addr_marker"))
            if a is None:
                continue
            name, size = g.get("qualified"), g.get("size")
            path, line = g["file"], g["line"]
            syms = [s for s in b.by_name.get(g.get("mangled"), []) if s.type != "func"]
            if syms:
                s = syms[0]
                if s.addr != a:
                    out.append(self.finding(path, line, f"{name}: marker {fmt(a)}, in binary {fmt(s.addr)}",
                                            entity=name, mangled=s.name, expected=fmt(s.addr), actual=fmt(a),
                                            key=f"A9:{s.name}"))
                elif size is not None and s.size and size != s.size:
                    out.append(self.finding(path, line, f"{name}: sizeof {size}, symbol has {s.size}", entity=name,
                                            mangled=s.name, expected=str(s.size), actual=str(size),
                                            key=f"A9:{s.name}"))
                continue
            span = max(size or 0, 1)
            cands = [s for s in b.section_of(a) if s.name in DATA_SECTIONS]
            if not cands:
                secs = b.section_of(a)
                out.append(self.finding(path, line,
                                        f"{name}: {fmt(a)} is not in a data section "
                                        f"({secs[0].name if secs else 'no section'})", entity=name,
                                        key=f"A9:{name}"))
                continue
            if not any(a + span <= s.end for s in cands):
                out.append(self.finding(path, line, f"{name}: [{fmt(a)}, {fmt(a + span)}) leaves {cands[0].name}",
                                        entity=name, key=f"A9:{name}"))
                continue
            hit = b.overlapping_object(a, a + span)
            if hit:
                out.append(self.finding(path, line, f"{name}: [{fmt(a)}, {fmt(a + span)}) overlaps {hit[2]} "
                                        f"at {fmt(hit[0])}", entity=name, key=f"A9:{name}"))
                continue
            placed.append((a, a + span, name, path, line))
        placed.sort()
        for i, (a, e, name, path, line) in enumerate(placed):
            for a2, e2, name2, _p, _l in placed[i + 1:]:
                if a2 >= e:
                    break
                out.append(self.finding(path, line, f"{name}: [{fmt(a)}, {fmt(e)}) overlaps {name2} at {fmt(a2)}",
                                        entity=name, key=f"A9:{name}:{name2}"))
        return out


def coverage(db, bf):
    """(per kind {kind: [covered, total]}, undeclared [(kind, name, addr)]) over the firaxis symbols that
    binfacts.stats.denominator.by_kind counts."""
    kinds = list(((bf.get("stats") or {}).get("denominator") or {}).get("by_kind") or {})
    pairs, thunks, vtables = set(), set(), set()
    for e in entities(db):
        names = role_names(e)
        v = [x for k, x in (e.variants or {}).items() if k != "D0" and x]
        for role, a in (("addr", e.addr), ("base", e.base)):
            if a is not None:
                for n in (v or [names[role]]):
                    pairs.add((n, a))
                pairs.add((names[role], a))
        if e.deleting is not None:
            pairs.add((names["deleting"], e.deleting))
        thunks.update(t for t in e.thunks if t is not None)
    for g in db.get("globals", []):
        a = hexa(g.get("addr_marker"))
        if a is not None:
            pairs.add((g.get("mangled"), a))
        for inst in g.get("instances") or ():
            a = hexa(inst.get("addr"))
            if a is not None:
                pairs.add((inst.get("mangled"), a))
    for r in db.get("records", []):
        a = hexa(r.get("vtable_marker"))
        if a is not None:
            vtables.add(a)
        for m in r.get("instance_markers") or []:
            if m.get("kind") == "vtable" and hexa(m.get("value")) is not None:
                vtables.add(hexa(m["value"]))
    per = {k: [0, 0] for k in kinds}
    missing = []
    for s in bf.get("symbols", []):
        k = s.get("kind")
        if s.get("group") != "firaxis" or k not in per:
            continue
        a = hexa(s["addr"])
        per[k][1] += 1
        if k == "thunk":
            ok = a in thunks
        elif k == "vtable":
            ok = a in vtables
        else:
            ok = (s["mangled"], a) in pairs
        if ok:
            per[k][0] += 1
        else:
            missing.append((k, s["mangled"], a))
    return per, missing


class A10(Rule):
    id = "A10"
    title = "every symbol of the binfacts coverage denominator is declared with the correct address"
    needs = ("typedb", "binfacts")
    scope = "tree"
    staged = "always"

    def run(self, ctx):
        bf = ctx.binfacts
        den = (bf.get("stats") or {}).get("denominator") or {}
        total = den.get("total")
        per, missing = coverage(ctx.typedb, bf)
        counted = sum(t for _c, t in per.values())
        if total != counted:
            raise InputError("binfacts", f"stats.denominator.total {total} differs from the {counted} firaxis symbols "
                                         "of its kinds")
        if not missing:
            return []
        detail = [f"{k}: {t - c} of {t} not declared" for k, (c, t) in per.items() if t - c]
        detail += [f"{k} {n} {fmt(a)}" for k, n, a in missing[:20]]
        if len(missing) > 20:
            detail.append(f"... and {len(missing) - 20} more")
        make = self.finding if ctx.export_coverage == "required" else self.warn
        return [make(None, None, f"not declared {len(missing)} of {total}", detail=detail, key="A10")]


class A11(Rule):
    id = "A11"
    title = "an external non-virtual function with a real name sits at an exported address"
    needs = ("typedb", "binfacts")
    scope = "entity"

    def run(self, ctx):
        b = bin_index(ctx)
        out = []
        for d in code_decls(ctx):
            f = d.ent.fn
            if f.get("linkage") != "external" or b.funcs_at(d.addr):
                continue
            if f.get("virtual"):
                continue         # a virtual method is reached through its slot, named after the introducing slot
            parts = (f.get("qualified") or f.get("name") or "").split("::")
            if any(ph.is_placeholder(p) for p in (parts[-1], parts[-2] if len(parts) > 1 else "")):
                continue
            out.append(self.finding(d.ent.path, d.ent.line,
                                    f"{d.ent.label} is external, but {fmt(d.addr)} is not exported",
                                    entity=d.ent.label, mangled=d.name, key=f"A11:{d.name or d.ent.label}"))
        return out
