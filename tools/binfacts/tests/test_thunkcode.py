import struct
import unittest

from binfacts import thunkcode


def jmp(at, target):
    return b"\xe9" + struct.pack("<i", target - (at + 5))


class DecodeThunk(unittest.TestCase):
    VA = 0x1000

    def check(self, code, want):
        self.assertEqual(thunkcode.decode(code, self.VA), want)

    def test_add_sub_imm8(self):
        self.check(b"\x83\x44\x24\x04\xf8" + jmp(self.VA + 5, 0x2000), (0x2000, -8))
        self.check(b"\x83\x6c\x24\x04\x08" + jmp(self.VA + 5, 0x2000), (0x2000, -8))
        self.check(b"\x83\x44\x24\x04\x10" + jmp(self.VA + 5, 0x500), (0x500, 16))

    def test_add_sub_imm32(self):
        self.check(b"\x81\x44\x24\x04" + struct.pack("<i", 0x460) + jmp(self.VA + 8, 0x2000), (0x2000, 0x460))
        self.check(b"\x81\x6c\x24\x04" + struct.pack("<i", 0x460) + jmp(self.VA + 8, 0x2000), (0x2000, -0x460))

    def test_load_adjust_store(self):
        code = b"\x8b\x44\x24\x04\x83\xc0\x0c\x89\x44\x24\x04"
        self.check(code + jmp(self.VA + len(code), 0x3000), (0x3000, 12))
        code = b"\x8b\x44\x24\x04\x2d" + struct.pack("<i", 0x200) + b"\x89\x44\x24\x04"
        self.check(code + jmp(self.VA + len(code), 0x3000), (0x3000, -0x200))

    def test_real_bodies_and_truncation_are_not_thunks(self):
        self.check(b"\x8b\x44\x24\x04\x83\xc0\x09\xc3" + b"\x90" * 8, None)
        self.check(b"\x53\x83\xec\x08\xe8\x00\x00\x00\x00\x5b\x81\xc3" + b"\x00" * 8, None)
        self.check(b"\x83\x44\x24\x04\xf8\xe9", None)
        self.check(b"", None)


if __name__ == "__main__":
    unittest.main()
