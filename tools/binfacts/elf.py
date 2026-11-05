import struct

SHT_NOBITS = 8
SHF_WRITE, SHF_ALLOC, SHF_EXEC = 1, 2, 4
PT_LOAD = 1
R_386_32, R_386_COPY, R_386_GLOB_DAT, R_386_JUMP_SLOT, R_386_RELATIVE = 1, 5, 6, 7, 8
STT_NAMES = {0: "notype", 1: "object", 2: "func"}
STB_NAMES = {1: "global", 2: "weak"}


class ElfError(Exception):
    pass


class Section:
    __slots__ = ("name", "type", "flags", "addr", "offset", "size", "link", "info", "entsize")

    def __init__(self, name, type_, flags, addr, offset, size, link, info, entsize):
        self.name, self.type, self.flags, self.addr = name, type_, flags, addr
        self.offset, self.size, self.link, self.info, self.entsize = offset, size, link, info, entsize

    @property
    def executable(self):
        return bool(self.flags & SHF_EXEC)


class Symbol:
    __slots__ = ("index", "name", "value", "size", "type", "bind", "shndx")

    def __init__(self, index, name, value, size, info, shndx):
        self.index, self.name, self.value, self.size, self.shndx = index, name, value, size, shndx
        self.type = info & 0xF
        self.bind = info >> 4

    @property
    def defined(self):
        return self.shndx != 0


class Elf:
    def __init__(self, data):
        self.data = data
        self._parse_header()
        self._parse_sections()
        self._parse_segments()
        self._parse_dynsym()
        self._parse_relocs()

    @classmethod
    def open(cls, path):
        with open(path, "rb") as f:
            return cls(f.read())

    def _parse_header(self):
        d = self.data
        if d[:4] != b"\x7fELF" or d[4] != 1 or d[5] != 1:
            raise ElfError("not a little-endian ELF32 file")
        (self.e_type, self.machine, _v, self.entry, self.phoff, self.shoff, _f, _eh,
         self.phentsize, self.phnum, self.shentsize, self.shnum, self.shstrndx) = struct.unpack_from(
            "<HHIIIIIHHHHHH", d, 16)
        if self.machine != 3:
            raise ElfError("not an i386 ELF")

    def _cstr(self, off):
        end = self.data.index(b"\0", off)
        return self.data[off:end].decode("latin-1")

    def _parse_sections(self):
        raw = []
        for i in range(self.shnum):
            raw.append(struct.unpack_from("<IIIIIIIIII", self.data, self.shoff + i * self.shentsize))
        stroff = raw[self.shstrndx][4]
        self.sections = [
            Section(self._cstr(stroff + r[0]), r[1], r[2], r[3], r[4], r[5], r[6], r[7], r[9]) for r in raw]
        self.section_by_name = {s.name: s for s in self.sections}

    def _parse_segments(self):
        self.loads = []
        for i in range(self.phnum):
            t, off, va, _pa, filesz, memsz, _fl, _al = struct.unpack_from(
                "<IIIIIIII", self.data, self.phoff + i * self.phentsize)
            if t == PT_LOAD:
                self.loads.append((va, off, filesz, memsz))

    def section(self, name):
        return self.section_by_name.get(name)

    def section_data(self, name):
        s = self.section_by_name[name]
        return self.data[s.offset:s.offset + s.size]

    def section_at(self, va):
        for s in self.sections:
            if s.flags & SHF_ALLOC and s.addr <= va < s.addr + s.size:
                return s
        return None

    def is_nobits(self, va):
        s = self.section_at(va)
        return s is not None and s.type == SHT_NOBITS

    def is_exec(self, va):
        s = self.section_at(va)
        return s is not None and s.executable

    def _parse_dynsym(self):
        sec = self.section_by_name[".dynsym"]
        strtab = self.sections[sec.link].offset
        d = self.data
        self.symbols = []
        for i in range(sec.size // 16):
            name, value, size, info, _other, shndx = struct.unpack_from("<IIIBBH", d, sec.offset + i * 16)
            self.symbols.append(Symbol(i, self._cstr(strtab + name) if name else "", value, size, info, shndx))

    def _parse_relocs(self):
        self.relocs = {}
        for name in (".rel.dyn", ".rel.plt"):
            sec = self.section_by_name.get(name)
            if sec is None:
                continue
            for i in range(sec.size // 8):
                off, info = struct.unpack_from("<II", self.data, sec.offset + i * 8)
                self.relocs[off] = (info & 0xFF, info >> 8)

    def read(self, va, n):
        for base, off, filesz, memsz in self.loads:
            if base <= va and va + n <= base + memsz:
                lo = va - base
                avail = max(0, min(n, filesz - lo))
                return self.data[off + lo:off + lo + avail] + b"\0" * (n - avail)
        return None

    def u32(self, va):
        b = self.read(va, 4)
        return None if b is None else struct.unpack("<I", b)[0]

    def cstring(self, va):
        for base, off, filesz, _memsz in self.loads:
            if base <= va < base + filesz:
                start = off + va - base
                end = self.data.index(b"\0", start)
                return self.data[start:end].decode("latin-1")
        return None

    def pointer(self, va):
        """Resolves a pointer stored at va. Returns None (unmapped) or (symbol|None, value)."""
        v = self.u32(va)
        if v is None:
            return None
        r = self.relocs.get(va)
        if r and r[0] in (R_386_32, R_386_GLOB_DAT) and r[1]:
            sym = self.symbols[r[1]]
            return sym, (sym.value + v if r[0] == R_386_32 else sym.value)
        return None, v

    def defined_symbols(self):
        return [s for s in self.symbols if s.defined and s.name and s.type in STT_NAMES]

    def plt_imports(self):
        return [s for s in self.symbols if not s.defined and s.name and s.value]

    def plt_stubs(self):
        """[(stub address, symbol name)] for the lazy-binding PLT entries, decoded from `jmp *GOT` operands."""
        sec = self.section_by_name.get(".plt")
        if sec is None:
            return []
        data = self.section_data(".plt")
        got = self.section_by_name.get(".got.plt")
        by_slot = {off: self.symbols[r[1]].name for off, r in self.relocs.items() if r[0] == R_386_JUMP_SLOT}
        out = []
        for k in range(1, len(data) // 16):
            b = data[16 * k:16 * k + 16]
            if b[0] == 0xFF and b[1] == 0x25:
                slot = struct.unpack("<I", b[2:6])[0]
            elif b[0] == 0xFF and b[1] == 0xA3 and got is not None:
                slot = (got.addr + struct.unpack("<i", b[2:6])[0]) & 0xFFFFFFFF
            else:
                continue
            if slot in by_slot:
                out.append((sec.addr + 16 * k, by_slot[slot]))
        return out

    def flags_string(self, s):
        return "".join(c for bit, c in ((SHF_ALLOC, "A"), (SHF_WRITE, "W"), (SHF_EXEC, "X")) if s.flags & bit)
