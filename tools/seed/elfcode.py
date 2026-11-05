"""Read-only access to the code bytes of Civ5XP (ELF32): which primary slot an unexported thunk calls."""
import struct


class Code:
    def __init__(self, path):
        self.data = open(path, "rb").read()
        d = self.data
        shoff = struct.unpack_from("<I", d, 0x20)[0]
        shentsize, shnum = struct.unpack_from("<HH", d, 0x2E)
        self.secs = []
        for i in range(shnum):
            s = struct.unpack_from("<IIIIIIIIII", d, shoff + i * shentsize)
            if s[1] != 8 and s[3]:          # not NOBITS, has an address
                self.secs.append((s[3], s[5], s[4]))

    def read(self, va, n):
        for addr, size, off in self.secs:
            if addr <= va < addr + size:
                o = va - addr + off
                return self.data[o:o + min(n, addr + size - va)]
        return b""

    def calls(self, va, n=64):
        """(direct call targets, indirect `call [reg+disp8]` displacements) in the first n bytes."""
        b = self.read(va, n)
        direct, indirect = [], []
        i = 0
        while i < len(b):
            op = b[i]
            if op == 0x81 and i + 6 <= len(b) and b[i + 1] == 0xC3:
                i += 6
                continue
            if op == 0xE8 and i + 5 <= len(b):
                rel = struct.unpack_from("<i", b, i + 1)[0]
                if rel:
                    direct.append(va + i + 5 + rel)
                i += 5
                continue
            if op == 0xE9 and i + 5 <= len(b):
                direct.append(va + i + 5 + struct.unpack_from("<i", b, i + 1)[0])
                break
            if op == 0xFF and i + 3 <= len(b) and (b[i + 1] & 0xF8) == 0x50 and b[i + 1] != 0x54:
                indirect.append(b[i + 2])
                i += 3
                continue
            if op == 0xFF and i + 2 <= len(b) and (b[i + 1] & 0xF8) == 0x10:
                indirect.append(0)
                i += 2
                continue
            i += 1
        return direct, indirect
