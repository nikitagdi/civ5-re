import struct

DW_EH_PE_omit = 0xFF


class EhFrameError(Exception):
    pass


def _uleb(data, pos):
    result = shift = 0
    while True:
        b = data[pos]
        pos += 1
        result |= (b & 0x7F) << shift
        shift += 7
        if not b & 0x80:
            return result, pos


def _sleb(data, pos):
    result = shift = 0
    while True:
        b = data[pos]
        pos += 1
        result |= (b & 0x7F) << shift
        shift += 7
        if not b & 0x80:
            if b & 0x40:
                result -= 1 << shift
            return result, pos


def read_encoded(data, pos, enc, section_addr, data_base=0):
    """Reads a DW_EH_PE pointer. Returns (value, new_pos); value is a 32-bit address for pcrel/datarel."""
    if enc == DW_EH_PE_omit:
        return None, pos
    fmt = enc & 0x0F
    app = enc & 0x70
    start = pos
    if fmt == 0x00:
        v, pos = struct.unpack_from("<I", data, pos)[0], pos + 4
    elif fmt == 0x01:
        v, pos = _uleb(data, pos)
    elif fmt == 0x02:
        v, pos = struct.unpack_from("<H", data, pos)[0], pos + 2
    elif fmt == 0x03:
        v, pos = struct.unpack_from("<I", data, pos)[0], pos + 4
    elif fmt == 0x04:
        v, pos = struct.unpack_from("<Q", data, pos)[0], pos + 8
    elif fmt == 0x09:
        v, pos = _sleb(data, pos)
    elif fmt == 0x0A:
        v, pos = struct.unpack_from("<h", data, pos)[0], pos + 2
    elif fmt == 0x0B:
        v, pos = struct.unpack_from("<i", data, pos)[0], pos + 4
    elif fmt == 0x0C:
        v, pos = struct.unpack_from("<q", data, pos)[0], pos + 8
    else:
        raise EhFrameError(f"unsupported pointer format 0x{enc:02x}")
    if app == 0x10:
        v += section_addr + start
    elif app == 0x30:
        v += data_base
    elif app not in (0x00,):
        raise EhFrameError(f"unsupported pointer application 0x{enc:02x}")
    return v & 0xFFFFFFFF, pos


def _parse_cie(data, body, end, section_addr):
    """Returns the FDE pointer encoding of a CIE whose body starts at `body` (after length and id)."""
    pos = body + 1
    aug_end = data.index(b"\0", pos)
    aug = data[pos:aug_end].decode("latin-1")
    pos = aug_end + 1
    _, pos = _uleb(data, pos)
    _, pos = _sleb(data, pos)
    ra, pos = (data[pos], pos + 1) if data[body] == 1 else _uleb(data, pos)
    fde_enc = 0
    if aug.startswith("z"):
        alen, pos = _uleb(data, pos)
        apos = pos
        for ch in aug[1:]:
            if ch == "R":
                fde_enc = data[apos]
                apos += 1
            elif ch == "L":
                apos += 1
            elif ch == "P":
                penc = data[apos]
                _, apos = read_encoded(data, apos + 1, penc & 0x7F, section_addr)
            elif ch in "SB":
                continue
            else:
                raise EhFrameError(f"unknown CIE augmentation {aug!r}")
    return fde_enc


def parse(data, section_addr):
    """Returns [(start, size)] for every FDE of an .eh_frame section, in section order."""
    out = []
    cies = {}
    pos = 0
    n = len(data)
    while pos + 4 <= n:
        length = struct.unpack_from("<I", data, pos)[0]
        if length == 0:
            pos += 4
            continue
        if length == 0xFFFFFFFF:
            raise EhFrameError("64-bit eh_frame entries are not supported")
        body = pos + 8
        end = pos + 4 + length
        if end > n:
            raise EhFrameError(f"eh_frame entry at 0x{section_addr + pos:08x} overruns the section")
        cie_id = struct.unpack_from("<I", data, pos + 4)[0]
        if cie_id == 0:
            cies[pos] = _parse_cie(data, body, end, section_addr)
        else:
            cie_pos = pos + 4 - cie_id
            if cie_pos not in cies:
                raise EhFrameError(f"FDE at 0x{section_addr + pos:08x} refers to unknown CIE")
            enc = cies[cie_pos]
            start, p = read_encoded(data, body, enc, section_addr)
            size, _ = read_encoded(data, p, enc & 0x0F, section_addr)
            out.append((start, size))
        pos = end
    return out
