import re

from binfacts.api import BinfactsError

TI_VTABLES = {
    "_ZTVN10__cxxabiv117__class_type_infoE": "class",
    "_ZTVN10__cxxabiv120__si_class_type_infoE": "si",
    "_ZTVN10__cxxabiv121__vmi_class_type_infoE": "vmi",
}
OTHER_TI = re.compile(r"^_ZTVN10__cxxabiv\d+__\w*type_infoE$|^_ZTVSt9type_info$")


class Typeinfo:
    __slots__ = ("addr", "symbol", "kind", "name", "class_", "flags", "bases", "base_refs")


def _vptr_kind(elf, ptr, vtable_addrs):
    sym, value = ptr
    if sym is not None and sym.name in TI_VTABLES:
        return TI_VTABLES[sym.name]
    for name, kind in TI_VTABLES.items():
        va = vtable_addrs.get(name)
        if va is not None and value in (va, va + 8):
            return kind
    if sym is not None and OTHER_TI.match(sym.name):
        return "other"
    for name, va in vtable_addrs.items():
        if OTHER_TI.match(name) and value in (va, va + 8):
            return "other"
    return None


def parse(elf, defined, extra_roots, demangler):
    """Reads every typeinfo object reachable from the exported _ZTI symbols and `extra_roots`.

    Returns (list of Typeinfo sorted by address, edges). Base references are resolved to
    (addr|None, symbol name|None, mangled class name) and stored in `base_refs`.
    """
    by_name = {s.name: s for s in defined}
    vtable_addrs = {n: by_name[n].value for n in by_name if n.startswith("_ZTVN10__cxxabiv") or n == "_ZTVSt9type_info"}
    sym_at = {}
    external = {}
    for s in defined:
        if s.name.startswith("_ZTI"):
            if elf.is_nobits(s.value):
                external[s.value] = s.name
            else:
                sym_at[s.value] = s.name
    todo = sorted(set(sym_at) | set(extra_roots))
    found = {}
    errors = []
    while todo:
        addr = todo.pop()
        if addr in found:
            continue
        t = Typeinfo()
        t.addr = addr
        t.symbol = sym_at.get(addr)
        vp = elf.pointer(addr)
        np_ = elf.pointer(addr + 4)
        if vp is None or np_ is None:
            errors.append(f"0x{addr:08x}: typeinfo is outside the image")
            continue
        t.kind = _vptr_kind(elf, vp, vtable_addrs)
        if t.kind is None:
            errors.append(f"0x{addr:08x}: unknown typeinfo vptr 0x{vp[1]:08x}")
            continue
        name = elf.cstring(np_[1])
        if name is None:
            errors.append(f"0x{addr:08x}: typeinfo name pointer 0x{np_[1]:08x} is not a string")
            continue
        t.name = name
        t.flags = None
        t.base_refs = []
        if t.kind == "si":
            t.base_refs.append((elf.pointer(addr + 8), 0, False, True))
        elif t.kind == "vmi":
            t.flags = elf.u32(addr + 8)
            count = elf.u32(addr + 12)
            for j in range(count):
                bp = elf.pointer(addr + 16 + j * 8)
                of = elf.u32(addr + 20 + j * 8)
                off = of >> 8
                if off & 0x800000:
                    off -= 1 << 24
                t.base_refs.append((bp, off, bool(of & 1), bool(of & 2)))
        found[addr] = t
        for bp, *_ in t.base_refs:
            if bp is not None and bp[0] is None and bp[1] and bp[1] not in found and bp[1] not in external:
                todo.append(bp[1])
    if errors:
        raise BinfactsError("typeinfo parse errors:\n" + "\n".join(errors[:50]))
    out = [found[a] for a in sorted(found)]
    for t in out:
        resolved = []
        for bp, off, virt, pub in t.base_refs:
            if bp is None:
                raise BinfactsError(f"0x{t.addr:08x}: base typeinfo pointer is outside the image")
            sym, value = bp
            if value in external:
                resolved.append((None, external[value], None, off, virt, pub))
            elif sym is None or sym.defined:
                target = found.get(value)
                if target is None:
                    raise BinfactsError(f"0x{t.addr:08x}: base typeinfo 0x{value:08x} was not found")
                resolved.append((target.addr, target.symbol, target.name, off, virt, pub))
            else:
                if not sym.name.startswith("_ZTI"):
                    raise BinfactsError(f"0x{t.addr:08x}: base refers to {sym.name}")
                resolved.append((None, sym.name, None, off, virt, pub))
        t.base_refs = resolved
    names = {"_ZTI" + t.name.lstrip("*") for t in out}
    for t in out:
        for _a, sym, nm, *_ in t.base_refs:
            names.add(sym if nm is None else "_ZTI" + nm.lstrip("*"))
    demangler.many(sorted(names))
    for t in out:
        t.class_ = _class_name(demangler, "_ZTI" + t.name.lstrip("*"))
        t.bases = []
        for addr, sym, nm, off, virt, pub in t.base_refs:
            full = sym if nm is None else "_ZTI" + nm.lstrip("*")
            t.bases.append({"class": _class_name(demangler, full), "typeinfo": sym, "offset": off,
                            "virtual": virt, "public": pub})
    return out


def _class_name(demangler, mangled):
    text = demangler.one(mangled)
    if not text.startswith("typeinfo for "):
        raise BinfactsError(f"cannot demangle typeinfo name {mangled}")
    return text[len("typeinfo for "):]
