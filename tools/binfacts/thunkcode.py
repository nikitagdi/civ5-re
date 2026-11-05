"""Recognizes this-adjusting thunks (adjust the `this` argument at 4(%esp), then jmp) in raw i386 code."""
import struct


def decode(code, va):
    """code: bytes at va. Returns (jump target, adjustment added to this) or None."""
    n = len(code)
    adj = None
    i = 0
    if n >= 8 and code[0] == 0x83 and code[1] in (0x44, 0x6C) and code[2:4] == b"\x24\x04":
        v = struct.unpack_from("<b", code, 4)[0]
        adj, i = (v if code[1] == 0x44 else -v), 5
    elif n >= 11 and code[0] == 0x81 and code[1] in (0x44, 0x6C) and code[2:4] == b"\x24\x04":
        v = struct.unpack_from("<i", code, 4)[0]
        adj, i = (v if code[1] == 0x44 else -v), 8
    elif n >= 16 and code[:4] == b"\x8b\x44\x24\x04":
        if code[4] == 0x83 and code[5] in (0xC0, 0xE8):
            v = struct.unpack_from("<b", code, 6)[0]
            adj, i = (v if code[5] == 0xC0 else -v), 7
        elif code[4] in (0x05, 0x2D):
            v = struct.unpack_from("<i", code, 5)[0]
            adj, i = (v if code[4] == 0x05 else -v), 9
        if adj is not None:
            if code[i:i + 4] != b"\x89\x44\x24\x04":
                return None
            i += 4
    if adj is None or len(code) < i + 5 or code[i] != 0xE9:
        return None
    rel = struct.unpack_from("<i", code, i + 1)[0]
    return (va + i + 5 + rel) & 0xFFFFFFFF, adj
