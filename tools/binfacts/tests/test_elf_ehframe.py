import struct
import unittest

from binfacts import ehframe
from binfacts.elf import Elf, ElfError, R_386_32, R_386_GLOB_DAT


class PointerEncodings(unittest.TestCase):
    def test_absolute_and_unsigned(self):
        self.assertEqual(ehframe.read_encoded(bytes.fromhex("78563412"), 0, 0x03, 0), (0x12345678, 4))
        self.assertEqual(ehframe.read_encoded(bytes.fromhex("3412"), 0, 0x02, 0), (0x1234, 2))
        self.assertEqual(ehframe.read_encoded(bytes.fromhex("e58e26"), 0, 0x01, 0), (624485, 3))

    def test_signed(self):
        self.assertEqual(ehframe.read_encoded(bytes.fromhex("7f"), 0, 0x09, 0), (0xFFFFFFFF, 1))
        self.assertEqual(ehframe.read_encoded(bytes.fromhex("feff"), 0, 0x0A, 0), (0xFFFFFFFE, 2))
        self.assertEqual(ehframe.read_encoded(bytes.fromhex("f0ffffff"), 0, 0x0B, 0x1000), (0xFFFFFFF0, 4))

    def test_pc_relative_uses_the_position_of_the_field(self):
        data = b"\0" * 8 + struct.pack("<i", 0x10)
        self.assertEqual(ehframe.read_encoded(data, 8, 0x1B, 0x1000), (0x1000 + 8 + 0x10, 12))
        data = b"\0" * 4 + struct.pack("<i", -0x20)
        self.assertEqual(ehframe.read_encoded(data, 4, 0x1B, 0x2000)[0], 0x2000 + 4 - 0x20)

    def test_data_relative_and_omit(self):
        data = struct.pack("<i", 0x40)
        self.assertEqual(ehframe.read_encoded(data, 0, 0x3B, 0x1000, data_base=0x2000)[0], 0x2040)
        self.assertEqual(ehframe.read_encoded(data, 0, 0xFF, 0), (None, 0))

    def test_unsupported_encodings(self):
        with self.assertRaises(ehframe.EhFrameError):
            ehframe.read_encoded(b"\0" * 8, 0, 0x0D, 0)
        with self.assertRaises(ehframe.EhFrameError):
            ehframe.read_encoded(b"\0" * 8, 0, 0x23, 0)


def _cie(enc=0x1B):
    body = struct.pack("<I", 0) + bytes([1]) + b"zR\0" + bytes([1, 0x7C, 8, 1, enc])
    body += bytes([0x0C, 4, 4, 0x88, 1])
    body += b"\0" * (-(len(body) + 4) % 4)
    return struct.pack("<I", len(body)) + body


def _fde(section_pos, cie_pos, section_addr, start, size, enc=0x1B):
    field_pos = section_pos + 8
    body = struct.pack("<I", section_pos + 4 - cie_pos)
    body += struct.pack("<i", start - (section_addr + field_pos)) + struct.pack("<i", size) + bytes([0])
    body += b"\0" * (-(len(body) + 4) % 4)
    return struct.pack("<I", len(body)) + body


class EhFrame(unittest.TestCase):
    def test_fdes_across_terminators(self):
        addr = 0x1000
        data = _cie()
        f1 = _fde(len(data), 0, addr, 0x8048000, 0x20)
        data += f1
        f2 = _fde(len(data), 0, addr, 0x8048100, 0x30)
        data += f2 + b"\0\0\0\0"
        second = len(data)
        data += _cie()
        data += _fde(len(data), second, addr, 0x8049000, 0x40)
        data += b"\0\0\0\0"
        self.assertEqual(ehframe.parse(data, addr), [(0x8048000, 0x20), (0x8048100, 0x30), (0x8049000, 0x40)])

    def test_absolute_pointer_encoding(self):
        data = _cie(enc=0x03)
        pos = len(data)
        body = struct.pack("<I", pos + 4) + struct.pack("<II", 0x8048000, 0x10) + bytes([0, 0, 0, 0])
        data += struct.pack("<I", len(body)) + body
        self.assertEqual(ehframe.parse(data, 0x1000), [(0x8048000, 0x10)])

    def test_fde_without_cie_is_an_error(self):
        data = _fde(0, 0, 0x1000, 0x8048000, 0x10)
        with self.assertRaises(ehframe.EhFrameError):
            ehframe.parse(data, 0x1000)


def synthetic_elf():
    """A tiny EXEC ELF32: .data with pointers, .bss, .dynsym/.dynstr/.rel.dyn."""
    base = 0x08000000
    strtab = b"\0obj\0ext\0plt_ext\0"
    shstr = b"\0.data\0.bss\0.dynsym\0.dynstr\0.rel.dyn\0.shstrtab\0"

    def shname(n):
        return shstr.index(b"\0" + n.encode() + b"\0") + 1

    syms = [(0, 0, 0, 0, 0), (1, base + 0x110, 4, 0x11, 1), (5, 0, 0, 0x12, 0), (9, base + 0x50, 0, 0x12, 0)]
    dynsym = b"".join(struct.pack("<IIIBBH", n, v, sz, info, 0, shndx) for n, v, sz, info, shndx in syms)
    rels = [(base + 0x100, (1 << 8) | R_386_32), (base + 0x104, (3 << 8) | R_386_GLOB_DAT),
            (base + 0x108, (2 << 8) | R_386_32)]
    reldyn = b"".join(struct.pack("<II", o, i) for o, i in rels)
    data = bytearray(0x20)
    struct.pack_into("<IIII", data, 0, 4, 0xDEAD, 0x20, base + 0x300)
    parts = [(".data", data), (".dynsym", dynsym), (".dynstr", strtab), (".rel.dyn", reldyn), (".shstrtab", shstr)]
    off = 0x100
    blob = b"\0" * 0x100
    placed = {}
    for name, content in parts:
        if name == ".data":
            off = 0x100
        placed[name] = len(blob)
        blob += bytes(content)
        blob += b"\0" * (-len(blob) % 4)
    shoff = len(blob)
    sections = [(0, 0, 0, 0, 0, 0, 0, 0, 0, 0)]
    sections.append((shname(".data"), 1, 3, base + 0x100, placed[".data"], len(data), 0, 0, 4, 0))
    sections.append((shname(".bss"), 8, 3, base + 0x2000, len(blob), 0x20, 0, 0, 4, 0))
    sections.append((shname(".dynsym"), 11, 2, 0, placed[".dynsym"], len(dynsym), 4, 1, 4, 16))
    sections.append((shname(".dynstr"), 3, 2, 0, placed[".dynstr"], len(strtab), 0, 0, 1, 0))
    sections.append((shname(".rel.dyn"), 9, 2, 0, placed[".rel.dyn"], len(reldyn), 3, 0, 4, 8))
    sections.append((shname(".shstrtab"), 3, 0, 0, placed[".shstrtab"], len(shstr), 0, 0, 1, 0))
    sh = b"".join(struct.pack("<IIIIIIIIII", *s) for s in sections)
    ehdr = b"\x7fELF" + bytes([1, 1, 1, 0]) + b"\0" * 8
    ehdr += struct.pack("<HHIIIIIHHHHHH", 2, 3, 1, base + 0x100, 52, shoff, 0, 52, 32, 1, 40, len(sections), 6)
    total = shoff + len(sh)
    phdr = struct.pack("<IIIIIIII", 1, 0, base, base, total, 0x2100, 7, 0x1000)
    image = bytearray(blob + sh)
    image[0:len(ehdr)] = ehdr
    image[52:52 + 32] = phdr
    return bytes(image), base


class SyntheticElf(unittest.TestCase):
    def setUp(self):
        data, self.base = synthetic_elf()
        self.elf = Elf(data)

    def test_sections_and_symbols(self):
        self.assertEqual([s.name for s in self.elf.sections][1:3], [".data", ".bss"])
        names = [s.name for s in self.elf.defined_symbols()]
        self.assertEqual(names, ["obj"])
        self.assertEqual([s.name for s in self.elf.plt_imports()], ["plt_ext"])

    def test_pointer_through_r_386_32(self):
        sym, value = self.elf.pointer(self.base + 0x100)
        self.assertEqual((sym.name, value), ("obj", self.base + 0x110 + 4))

    def test_pointer_through_glob_dat(self):
        sym, value = self.elf.pointer(self.base + 0x104)
        self.assertEqual((sym.name, value), ("plt_ext", self.base + 0x50))

    def test_r_386_32_to_an_undefined_symbol_keeps_the_addend(self):
        sym, value = self.elf.pointer(self.base + 0x108)
        self.assertEqual((sym.name, value), ("ext", 0x20))

    def test_absolute_pointer_without_relocation(self):
        self.assertEqual(self.elf.pointer(self.base + 0x10C), (None, self.base + 0x300))

    def test_bss_reads_as_zero_and_unmapped_is_none(self):
        self.assertEqual(self.elf.u32(self.base + 0x2010), 0)
        self.assertIsNone(self.elf.u32(self.base + 0x100000))
        self.assertTrue(self.elf.is_nobits(self.base + 0x2010))
        self.assertFalse(self.elf.is_nobits(self.base + 0x100))

    def test_not_an_elf(self):
        with self.assertRaises(ElfError):
            Elf(b"MZ" + b"\0" * 100)


if __name__ == "__main__":
    unittest.main()
