from binfacts import thunkcode
from binfacts.api import BinfactsError

PURE_VIRTUAL = "__cxa_pure_virtual"
THUNK_PREFIXES = ("_ZTh", "_ZTv", "_ZTc")


def _signed(v):
    return v - (1 << 32) if v & 0x80000000 else v


def _word_kind(elf, ptr):
    if ptr is None:
        return "offset", 0
    value = ptr[1]
    if value == 0:
        return "offset", 0
    sec = elf.section_at(value)
    if sec is None:
        return "offset", value
    return ("slot" if sec.executable else "typeinfo"), value


def parse_one(elf, sym):
    """Raw groups of one _ZTV/_ZTC object: [{offset_to_top, vbase_offsets, typeinfo_addr, address_point, slots}]."""
    groups = []
    prefix = []
    cur = None
    for k in range(sym.size // 4):
        va = sym.value + 4 * k
        kind, value = _word_kind(elf, elf.pointer(va))
        if kind == "offset":
            prefix.append(_signed(value))
        elif kind == "typeinfo":
            if not prefix:
                raise BinfactsError(f"{sym.name}: typeinfo pointer without offset_to_top at 0x{va:08x}")
            cur = {"offset_to_top": prefix[-1], "vbase_offsets": prefix[:-1], "typeinfo_addr": value,
                   "address_point": va + 4, "slots": []}
            groups.append(cur)
            prefix = []
        else:
            if prefix or cur is None:
                raise BinfactsError(f"{sym.name}: zero or unrecognized word at 0x{va:08x}")
            cur["slots"].append((va, value))
    if prefix or not groups:
        raise BinfactsError(f"{sym.name}: trailing zero or unrecognized words")
    return groups


def build(elf, defined, typeinfos, demangler):
    """Returns the vtables[] list of the output."""
    ti_by_addr = {t.addr: t for t in typeinfos}
    names_at = {}
    for s in defined:
        names_at.setdefault(s.value, []).append(s.name)
    imports = {s.value: s.name for s in elf.plt_imports()}
    imports.update(dict(elf.plt_stubs()))
    pure = next((a for a, n in imports.items() if n == PURE_VIRTUAL), None)
    thunks = {s.value for s in defined if s.name.startswith(THUNK_PREFIXES)}
    vts = sorted((s for s in defined if s.name.startswith(("_ZTV", "_ZTC")) and not elf.is_nobits(s.value)), key=lambda s: (s.value, s.name))
    dem = demangler.many([s.name for s in vts])
    parsed = []
    for s, text in zip(vts, dem):
        construction = s.name.startswith("_ZTC")
        prefix = "construction vtable for " if construction else "vtable for "
        if not text.startswith(prefix):
            raise BinfactsError(f"cannot demangle {s.name}")
        cls = text[len(prefix):]
        if construction:
            cls = cls.split("-in-", 1)[0]
        raw = parse_one(elf, s)
        parsed.append((s, construction, cls, raw))
    out = []
    for s, construction, cls, raw in parsed:
        groups = []
        for g in raw:
            ti = ti_by_addr.get(g["typeinfo_addr"])
            if ti is None:
                raise BinfactsError(f"{s.name}: group typeinfo 0x{g['typeinfo_addr']:08x} is not a typeinfo")
            if ti.class_ != cls:
                raise BinfactsError(f"{s.name}: group typeinfo is {ti.class_}, expected {cls}")
            slots = []
            for _va, target in g["slots"]:
                extra = {}
                if target == pure:
                    kind, syms = "pure", [PURE_VIRTUAL]
                elif target in imports:
                    kind, syms = "import", [imports[target]]
                elif target in thunks:
                    kind, syms = "thunk", sorted(names_at.get(target, []))
                else:
                    syms = sorted(names_at.get(target, []))
                    kind = "func"
                    if not syms and g["offset_to_top"] != 0:
                        dec = thunkcode.decode(elf.read(target, 16) or b"", target)
                        if dec is not None:
                            kind = "thunk"
                            extra = {"thunk_target": dec[0], "adjust": dec[1]}
                slots.append({"target": target, "symbols": syms, "kind": kind, **extra})
            groups.append({"offset_to_top": g["offset_to_top"], "vbase_offsets": g["vbase_offsets"],
                           "typeinfo": g["typeinfo_addr"], "address_point": g["address_point"], "slots": slots})
        out.append({"symbol": s.name, "class": cls, "addr": s.value, "size": s.size,
                    "construction": construction, "groups": groups})
    return out


def typeinfo_roots(elf, defined):
    roots = set()
    for s in defined:
        if s.name.startswith(("_ZTV", "_ZTC")) and not elf.is_nobits(s.value):
            for g in parse_one(elf, s):
                roots.add(g["typeinfo_addr"])
    return roots
