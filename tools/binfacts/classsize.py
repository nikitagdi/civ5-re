"""Lower bounds of class sizes from the code of constructors and destructors (i386, stack convention).

`this` is the first stack argument, [esp+4] on entry. The decoder is a small linear i386 decoder; it knows the
forms clang 3.4 emits for structors and stops (keeping what it proved so far) at the first form it does not know.
Only facts that hold on every path are kept: a register is `this+d` only if every incoming edge agrees.
"""
import struct
from collections import namedtuple

EAX, ECX, EDX, EBX, ESP, EBP, ESI, EDI = range(8)
CALLER_SAVED = (EAX, ECX, EDX)
ALU = ("add", "or", "adc", "sbb", "and", "sub", "xor", "cmp")
MAX_OFFSET = 1 << 20

Insn = namedtuple("Insn", "addr n kind op w mem dst src tgt")
Scan = namedtuple("Scan", "extent news complete")


def _insn(addr, n, kind, op=None, w=4, mem=None, dst=None, src=None, tgt=None):
    return Insn(addr, n, kind, op, w, mem, dst, src, tgt)


class _Stop(Exception):
    """Analysis cannot go on; what was proved so far stays valid."""


class _Discard(Exception):
    """Something already proved turned out to be unsound: drop the whole function."""


class _Cur:
    def __init__(self, code, i):
        self.code, self.i = code, i

    def u8(self):
        v = self.code[self.i]
        self.i += 1
        return v

    def s8(self):
        v = struct.unpack_from("<b", self.code, self.i)[0]
        self.i += 1
        return v

    def s32(self):
        v = struct.unpack_from("<i", self.code, self.i)[0]
        self.i += 4
        return v

    def u16(self):
        v = struct.unpack_from("<H", self.code, self.i)[0]
        self.i += 2
        return v

    def imm(self, osz):
        if osz == 4:
            return self.s32()
        v = struct.unpack_from("<h", self.code, self.i)[0]
        self.i += 2
        return v

    def modrm(self):
        """(reg field, rm register or None, memory operand or None); memory is (base, index, scale, disp)."""
        b = self.u8()
        mod, reg, rm = b >> 6, (b >> 3) & 7, b & 7
        if mod == 3:
            return reg, rm, None
        base, index, scale = rm, None, 1
        if rm == 4:
            s = self.u8()
            scale, idx, base = 1 << (s >> 6), (s >> 3) & 7, s & 7
            index = None if idx == 4 else idx
        disp = 0
        if mod == 1:
            disp = self.s8()
        elif mod == 2:
            disp = self.s32()
        elif base == 5 and mod == 0:
            base, disp = None, self.s32()
        if rm == 5 and mod == 0:
            base = None
        return reg, None, (base, index, scale, disp)


def _x87_width(op, reg):
    if op == 0xD8:
        return 4
    if op == 0xDC:
        return 8
    if op == 0xDA:
        return 4
    if op == 0xDE:
        return 2
    table = {
        0xD9: {0: 4, 2: 4, 3: 4, 4: 28, 5: 2, 6: 28, 7: 2},
        0xDB: {0: 4, 1: 4, 2: 4, 3: 4, 5: 10, 7: 10},
        0xDD: {0: 8, 1: 8, 2: 8, 3: 8, 4: 108, 6: 108, 7: 2},
        0xDF: {0: 2, 1: 2, 2: 2, 3: 2, 4: 10, 5: 8, 6: 10, 7: 8},
    }
    return table[op].get(reg)


def _sse_width(op2, pfx):
    """Memory operand width of the SSE/MMX forms that matter; None = unknown (the access is then not counted)."""
    if op2 in (0x10, 0x11):
        return {None: 16, 0x66: 16, 0xF3: 4, 0xF2: 8}[pfx]
    if op2 in (0x12, 0x13, 0x16, 0x17):
        return 8
    if op2 in (0x14, 0x15, 0x28, 0x29, 0x2B):
        return 16
    if op2 == 0x2A:
        return 4
    if op2 in (0x2C, 0x2D):
        return {0xF3: 4, 0xF2: 8}.get(pfx, 8)
    if op2 in (0x2E, 0x2F):
        return 8 if pfx == 0x66 else 4
    if op2 == 0x5A:
        return {0xF3: 4, 0xF2: 8, None: 8, 0x66: 16}[pfx]
    if op2 == 0x5B:
        return 16
    if 0x51 <= op2 <= 0x5F or op2 == 0xC2:
        return {0xF3: 4, 0xF2: 8}.get(pfx, 16)
    if op2 == 0x6E:
        return 4
    if op2 == 0x7E:
        return 8 if pfx == 0xF3 else 4
    if op2 in (0x6F, 0x7F):
        return 16 if pfx in (0x66, 0xF3) else 8
    if op2 in (0xD6,):
        return 8
    if op2 in (0xE7,):
        return 16 if pfx == 0x66 else 8
    if op2 == 0xC6:
        return 16
    if 0x60 <= op2 <= 0x6D or 0x74 <= op2 <= 0x76 or 0xD1 <= op2 <= 0xFE:
        return 16 if pfx == 0x66 else 8
    return None


SSE_OPS = set(range(0x10, 0x18)) | {0x28, 0x29, 0x2A, 0x2B, 0x2E, 0x2F, 0x6E, 0x6F, 0x70, 0x71, 0x72, 0x73, 0x77,
                                    0x7E, 0x7F, 0xC2, 0xC4, 0xC6, 0xD6, 0xE6, 0xE7, 0xF0} \
    | set(range(0x51, 0x70)) | set(range(0x74, 0x77)) | set(range(0xD1, 0xD6)) | set(range(0xD8, 0xDF)) \
    | set(range(0xE0, 0xE6)) | set(range(0xE8, 0xF0)) | set(range(0xF1, 0xF7)) | set(range(0xF8, 0xFF))


def decode(code, i, va):
    """Decodes one instruction at code[i]; None when the form is not known (or truncated)."""
    try:
        return _decode(code, i, va)
    except (IndexError, KeyError, struct.error):
        return None


def _decode(code, i, va):
    start = i
    c = _Cur(code, i)
    pfx = None
    seg = False
    while True:
        b = code[c.i]
        if b in (0x66, 0xF2, 0xF3):
            pfx = b
        elif b in (0x26, 0x2E, 0x36, 0x3E, 0x64, 0x65):
            seg = seg or b in (0x64, 0x65)
        elif b == 0xF0:
            pass
        elif b == 0x67:
            return None
        else:
            break
        c.i += 1
    osz = 2 if pfx == 0x66 else 4
    op = c.u8()
    addr = va + start

    def done(kind, **kw):
        mem = kw.get("mem")
        if seg and mem is not None:
            kw["mem"] = None
        return _insn(addr, c.i - start, kind, **kw)

    def ea(w):
        reg, rm, mem = c.modrm()
        loc = ("r", rm) if mem is None else ("m",)
        return reg, loc, mem

    if op < 0x40 and (op & 7) < 6:
        name = ALU[op >> 3]
        form = op & 7
        if form < 4:
            w = 1 if form % 2 == 0 else osz
            reg, loc, mem = ea(w)
            if form < 2:
                return done("alu", op=name, w=w, mem=mem, dst=None if name == "cmp" else loc, src=("r", reg))
            return done("alu", op=name, w=w, mem=mem, dst=None if name == "cmp" else ("r", reg), src=loc)
        w = 1 if form == 4 else osz
        v = c.s8() if form == 4 else c.imm(osz)
        return done("alu", op=name, w=w, dst=None if name == "cmp" else ("r", EAX), src=("i", v))
    if 0x40 <= op <= 0x4F:
        return done("alu", op="inc" if op < 0x48 else "dec", dst=("r", op & 7), w=osz)
    if 0x50 <= op <= 0x57:
        return done("push", src=("r", op & 7))
    if 0x58 <= op <= 0x5F:
        return done("pop", dst=("r", op & 7))
    if op == 0x68:
        return done("push", src=("i", c.s32()))
    if op == 0x6A:
        return done("push", src=("i", c.s8()))
    if op in (0x69, 0x6B):
        reg, loc, mem = ea(osz)
        v = c.imm(osz) if op == 0x69 else c.s8()
        return done("alu", op="imul", w=osz, mem=mem, dst=("r", reg), src=loc)
    if 0x70 <= op <= 0x7F:
        rel = c.s8()
        return done("jcc", tgt=(va + c.i + rel) & 0xFFFFFFFF)
    if op in (0x80, 0x81, 0x83):
        reg, loc, mem = ea(0)
        w = 1 if op == 0x80 else osz
        v = c.s8() if op != 0x81 else c.imm(osz)
        name = ALU[reg]
        return done("alu", op=name, w=w, mem=mem, dst=None if name == "cmp" else loc, src=("i", v))
    if op in (0x84, 0x85):
        w = 1 if op == 0x84 else osz
        reg, loc, mem = ea(w)
        return done("alu", op="test", w=w, mem=mem, src=loc)
    if op in (0x86, 0x87):
        reg, loc, mem = ea(1 if op == 0x86 else osz)
        return done("xchg", w=1 if op == 0x86 else osz, mem=mem, dst=loc, src=("r", reg))
    if op in (0x88, 0x89, 0x8A, 0x8B):
        w = 1 if op in (0x88, 0x8A) else osz
        reg, loc, mem = ea(w)
        if op < 0x8A:
            return done("mov", w=w, mem=mem, dst=loc, src=("r", reg))
        return done("mov", w=w, mem=mem, dst=("r", reg), src=loc)
    if op == 0x8D:
        reg, loc, mem = ea(4)
        if mem is None:
            return None
        return done("lea", mem=mem, dst=("r", reg))
    if op == 0x8F:
        reg, loc, mem = ea(4)
        return done("pop", w=4, mem=mem, dst=loc)
    if op == 0x90:
        return done("nop")
    if 0x91 <= op <= 0x97:
        return done("xchg", dst=("r", op & 7), src=("r", EAX))
    if op == 0x98:
        return done("alu", op="cwde", dst=("r", EAX))
    if op == 0x99:
        return done("alu", op="cdq", dst=("r", EDX))
    if op in (0x9E, 0x9F):
        return done("alu", op="ahf", dst=("r", EAX))
    if op in (0xA0, 0xA1, 0xA2, 0xA3):
        c.s32()
        return done("alu", op="moffs", dst=("r", EAX) if op < 0xA2 else None)
    if op in (0xA4, 0xA5, 0xA6, 0xA7, 0xAA, 0xAB, 0xAC, 0xAD, 0xAE, 0xAF):
        return done("str")
    if op == 0xA8:
        c.s8()
        return done("alu", op="test", w=1)
    if op == 0xA9:
        c.imm(osz)
        return done("alu", op="test")
    if 0xB0 <= op <= 0xB7:
        return done("mov", w=1, dst=("r", op & 3), src=("i", c.s8()))
    if 0xB8 <= op <= 0xBF:
        return done("mov", dst=("r", op & 7), src=("i", c.imm(osz)), w=osz)
    if op in (0xC0, 0xC1, 0xD0, 0xD1, 0xD2, 0xD3):
        w = 1 if op in (0xC0, 0xD0, 0xD2) else osz
        reg, loc, mem = ea(w)
        if op in (0xC0, 0xC1):
            c.s8()
        return done("alu", op="shift", w=w, mem=mem, dst=loc)
    if op == 0xC2:
        c.u16()
        return done("ret")
    if op == 0xC3:
        return done("ret")
    if op in (0xC6, 0xC7):
        w = 1 if op == 0xC6 else osz
        reg, loc, mem = ea(w)
        if reg != 0:
            return None
        v = c.s8() if op == 0xC6 else c.imm(osz)
        return done("mov", w=w, mem=mem, dst=loc, src=("i", v))
    if op == 0xC9:
        return done("leave")
    if op in (0xCC, 0xF4):
        return done("ret")
    if 0xD8 <= op <= 0xDF:
        b = code[c.i]
        reg, rm, mem = c.modrm()
        if mem is None:
            return done("alu", op="fp", dst=("r", EAX) if (op, b) == (0xDF, 0xE0) else None)
        w = _x87_width(op, reg)
        if w is None:
            return None
        return done("alu", op="fp", w=w, mem=mem)
    if op == 0xE8:
        rel = c.s32()
        return done("call", tgt=(va + c.i + rel) & 0xFFFFFFFF)
    if op == 0xE9:
        rel = c.s32()
        return done("jmp", tgt=(va + c.i + rel) & 0xFFFFFFFF)
    if op == 0xEB:
        rel = c.s8()
        return done("jmp", tgt=(va + c.i + rel) & 0xFFFFFFFF)
    if 0xE0 <= op <= 0xE3:
        rel = c.s8()
        return done("jcc", tgt=(va + c.i + rel) & 0xFFFFFFFF)
    if op in (0xF6, 0xF7):
        w = 1 if op == 0xF6 else osz
        reg, loc, mem = ea(w)
        if reg in (0, 1):
            v = c.s8() if op == 0xF6 else c.imm(osz)
            return done("alu", op="test", w=w, mem=mem, src=loc)
        if reg in (2, 3):
            return done("alu", op="neg", w=w, mem=mem, dst=loc)
        return done("muldiv", w=w, mem=mem)
    if op in (0xFE, 0xFF):
        w = 1 if op == 0xFE else osz
        reg, loc, mem = ea(w)
        if reg in (0, 1):
            return done("alu", op="inc", w=w, mem=mem, dst=loc)
        if op == 0xFE:
            return None
        if reg == 2:
            return done("call", w=4, mem=mem)
        if reg == 4:
            return done("jmp", w=4, mem=mem)
        if reg == 6:
            return done("push", w=4, mem=mem, src=loc)
        return None
    if op == 0x9C:
        return done("push", src=None)
    if op == 0x9D:
        return done("pop", dst=None)
    if op == 0x0F:
        return _decode_0f(c, start, addr, pfx, osz, seg, va)
    return None


def _decode_0f(c, start, addr, pfx, osz, seg, va):
    op2 = c.u8()

    def done(kind, **kw):
        if seg and kw.get("mem") is not None:
            kw["mem"] = None
        return _insn(addr, c.i - start, kind, **kw)

    if op2 == 0x0B:
        return done("ret")
    if op2 == 0x31:
        return done("multi")
    if op2 == 0xA2:
        return done("multi")
    if 0x80 <= op2 <= 0x8F:
        rel = c.s32()
        return done("jcc", tgt=(va + c.i + rel) & 0xFFFFFFFF)
    if 0x90 <= op2 <= 0x9F:
        reg, rm, mem = c.modrm()
        return done("alu", op="setcc", w=1, mem=mem, dst=("m",) if mem else ("r", rm & 3))
    if 0x40 <= op2 <= 0x4F:
        reg, rm, mem = c.modrm()
        return done("alu", op="cmov", w=osz, mem=mem, dst=("r", reg))
    if op2 in (0xB6, 0xB7, 0xBE, 0xBF):
        reg, rm, mem = c.modrm()
        return done("alu", op="movx", w=1 if op2 in (0xB6, 0xBE) else 2, mem=mem, dst=("r", reg))
    if op2 == 0xAF:
        reg, rm, mem = c.modrm()
        return done("alu", op="imul", w=osz, mem=mem, dst=("r", reg))
    if op2 in (0xBC, 0xBD):
        reg, rm, mem = c.modrm()
        return done("alu", op="bsf", w=osz, mem=mem, dst=("r", reg))
    if op2 in (0xA3, 0xAB, 0xB3, 0xBB):
        reg, rm, mem = c.modrm()
        return done("alu", op="bt", dst=None if op2 == 0xA3 else (("m",) if mem else ("r", rm)))
    if op2 == 0xBA:
        reg, rm, mem = c.modrm()
        c.s8()
        return done("alu", op="bt", w=1, mem=mem, dst=None if reg == 4 else (("m",) if mem else ("r", rm)))
    if op2 in (0xA4, 0xA5, 0xAC, 0xAD):
        reg, rm, mem = c.modrm()
        if op2 in (0xA4, 0xAC):
            c.s8()
        return done("alu", op="shld", w=osz, mem=mem, dst=("m",) if mem else ("r", rm))
    if op2 in (0xB0, 0xB1, 0xC0, 0xC1):
        reg, rm, mem = c.modrm()
        w = 1 if op2 in (0xB0, 0xC0) else osz
        return done("multi", w=w, mem=mem, dst=("m",) if mem else ("r", rm), src=("r", reg))
    if 0xC8 <= op2 <= 0xCF:
        return done("alu", op="bswap", dst=("r", op2 & 7))
    if 0x18 <= op2 <= 0x1F:
        reg, rm, mem = c.modrm()
        return done("nop", w=1, mem=mem)
    if op2 in (0x38, 0x3A, 0x05, 0x34, 0x35, 0x01, 0x00, 0x0D):
        return None
    reg, rm, mem = c.modrm()
    w = _sse_width(op2, pfx)
    if op2 in (0x70, 0x71, 0x72, 0x73, 0xC2, 0xC4, 0xC5, 0xC6):
        c.s8()
    dst = None
    if op2 in (0x2C, 0x2D, 0x50, 0xD7, 0xC5):
        dst = ("r", reg)
    elif op2 == 0x7E and pfx != 0xF3 and mem is None:
        dst = ("r", rm)
    if op2 in SSE_OPS or dst is not None or op2 in (0x2C, 0x2D, 0x50, 0xD7, 0xC5):
        if mem is not None and w is None:
            w = 1
        return done("alu", op="sse", w=w or 1, mem=mem, dst=dst)
    return done("multi", w=1, mem=mem, dst=("m",) if mem else ("r", rm), src=("r", reg))


# ---------------------------------------------------------------- abstract interpretation

def _meet(a, b):
    """Intersection of two states (regs, stack, sp); _Stop when the stack pointers disagree."""
    regs_a, stack_a, sp_a = a
    regs_b, stack_b, sp_b = b
    if sp_a != sp_b:
        raise _Stop()
    regs = [x if x == y else None for x, y in zip(regs_a, regs_b)]
    stack = {k: v for k, v in stack_a.items() if stack_b.get(k) == v}
    return regs, stack, sp_a


def _copy(state):
    regs, stack, sp = state
    return list(regs), dict(stack), sp


def scan(code, va, news=(), ctors=None):
    """Proves a lower bound of the size of `this`'s class from one function body.

    code: the bytes of the function at va. news: addresses of operator new(unsigned). ctors: {address: tag} of
    constructors. Returns Scan(extent, news, complete): extent is the largest this-offset + width touched
    (0 when nothing was proved), news is [(call site, ctor tag, size)] for `new T` immediately handed to a
    constructor, complete tells whether the whole body was decoded and interpreted.
    """
    insns = []
    i = 0
    while i < len(code):
        ins = decode(code, i, va)
        if ins is None:
            break
        insns.append(ins)
        i += ins.n
    complete = i >= len(code) and bool(insns)
    end = va + i
    index = {ins.addr: k for k, ins in enumerate(insns)}
    targets = {ins.tgt for ins in insns if ins.kind in ("jmp", "jcc") and ins.tgt is not None
               and va <= ins.tgt < end}
    if any(ins.kind == "jmp" and ins.mem is not None for ins in insns):
        complete = False
    st = _Run(va, end, index, targets, set(news), ctors or {})
    try:
        st.run(insns)
    except _Discard:
        return Scan(0, [], False)
    except _Stop:
        complete = False
    return Scan(st.extent, st.hits, complete and not st.stopped)


class _Run:
    def __init__(self, va, end, index, targets, news, ctors):
        self.va, self.end, self.index, self.targets = va, end, index, targets
        self.news, self.ctors = news, ctors
        self.extent = 0
        self.hits = []
        self.stopped = False
        self.regs = [None] * 8
        self.stack = {4: ("this", 0)}
        self.sp = 0
        self.snaps = {}
        self.heads = {}

    def state(self):
        return self.regs, self.stack, self.sp

    def run(self, insns):
        live = True
        for ins in insns:
            a = ins.addr
            if a in self.targets:
                inc = self.snaps.get(a)
                if live and inc is not None:
                    self.regs, self.stack, self.sp = _meet(self.state(), inc)
                elif not live:
                    if inc is None:
                        raise _Stop()
                    self.regs, self.stack, self.sp = _copy(inc)
                self.heads[a] = _copy(self.state())
                live = True
            elif not live:
                raise _Stop()
            live = self.step(ins)
            if self.stopped:
                raise _Stop()

    # values: ("this", d) | ("new", n) | ("imm", n) | ("sp", coordinate)
    def val(self, r):
        return ("sp", self.sp) if r == ESP else self.regs[r]

    def setreg(self, r, v, w=4):
        if r == ESP:
            raise _Stop()
        self.regs[r] = v if w == 4 else None

    def mem_value(self, mem):
        """Classifies a memory operand: ('this', offset) | ('stack', coordinate) | None."""
        base, index, _scale, disp = mem
        if base is None or index is not None:
            return None
        v = self.val(base)
        if v is None:
            return None
        if v[0] == "this":
            return ("this", v[1] + disp)
        if v[0] == "sp":
            return ("stack", v[1] + disp)
        return None

    def touch(self, ins):
        """Counts the memory access of ins and returns its classification."""
        if ins.mem is None:
            return None
        loc = self.mem_value(ins.mem)
        if loc is not None and loc[0] == "this" and 0 <= loc[1] < MAX_OFFSET:
            self.extent = max(self.extent, loc[1] + ins.w)
        return loc

    def kill_slots(self, c, w):
        for k in [k for k in self.stack if c - 3 <= k < c + w]:
            del self.stack[k]

    def read(self, src, ins, loc):
        if src is None:
            return None
        if src[0] == "r":
            return self.val(src[1]) if ins.w == 4 else None
        if src[0] == "i":
            return ("imm", src[1]) if ins.w == 4 else None
        if loc is not None and loc[0] == "stack" and ins.w == 4:
            return self.stack.get(loc[1])
        return None

    def write(self, dst, value, ins, loc):
        if dst is None:
            return
        if dst[0] == "r":
            r = dst[1] if ins.w != 1 else dst[1] & 3
            self.setreg(r, value, ins.w)
        elif loc is not None and loc[0] == "stack":
            self.kill_slots(loc[1], ins.w)
            if value is not None and ins.w == 4:
                self.stack[loc[1]] = value

    def step(self, ins):
        k = ins.kind
        if k == "nop":
            return True
        if k == "ret":
            return False
        if k == "jmp":
            self.touch(ins)
            if ins.mem is not None or ins.tgt is None:
                return False
            self.edge(ins)
            return False
        if k == "jcc":
            self.edge(ins)
            return True
        if k == "call":
            return self.call(ins)
        if k == "push":
            loc = self.touch(ins)
            src = ins.src
            if src is None:
                v = None
            elif src[0] == "i":
                v = ("imm", src[1])
            elif src[0] == "r":
                v = self.val(src[1])
            else:
                v = self.stack.get(loc[1]) if loc is not None and loc[0] == "stack" else None
            self.sp -= 4
            self.kill_slots(self.sp, 4)
            if v is not None:
                self.stack[self.sp] = v
            return True
        if k == "pop":
            loc = self.touch(ins)
            v = self.stack.get(self.sp)
            self.sp += 4
            if ins.dst is not None:
                self.write(ins.dst, v, ins._replace(w=4), loc)
            return True
        if k == "lea":
            v = None
            m = ins.mem
            if m[1] is None and m[0] is not None:
                b = self.val(m[0])
                if b is not None and b[0] in ("this", "sp"):
                    v = (b[0], b[1] + m[3])
            self.setreg(ins.dst[1], v)
            return True
        if k == "leave":
            bp = self.regs[EBP]
            if bp is None or bp[0] != "sp":
                raise _Stop()
            self.sp = bp[1]
            self.regs[EBP] = self.stack.get(self.sp)
            self.sp += 4
            return True
        if k == "str":
            for r in (ESI, EDI):
                v = self.regs[r]
                if v is not None and v[0] == "this":
                    self.stopped = True
            self.regs[ESI] = self.regs[EDI] = self.regs[ECX] = self.regs[EAX] = None
            return True
        if k in ("muldiv", "multi"):
            self.touch(ins)
            self.regs[EAX] = self.regs[EDX] = None
            if k == "multi":
                self.regs[ECX] = self.regs[EBX] = None
                if ins.dst is not None and ins.dst[0] == "r":
                    self.regs[ins.dst[1] & 7] = None
                if ins.src is not None and ins.src[0] == "r":
                    self.regs[ins.src[1] & 7] = None
                if ins.mem is not None:
                    self.stack = {a: b for a, b in self.stack.items() if a >= 4}
            return True
        if k == "xchg":
            self.touch(ins)
            for s in (ins.dst, ins.src):
                if s is not None and s[0] == "r":
                    self.setreg(s[1] & 7, None)
            if ins.mem is not None:
                self.stack = {a: b for a, b in self.stack.items() if a >= 4}
            return True
        loc = self.touch(ins)
        if k == "mov":
            self.write(ins.dst, self.read(ins.src, ins, loc), ins, loc)
            return True
        if k == "alu":
            return self.alu(ins, loc)
        raise _Stop()

    def alu(self, ins, loc):
        dst = ins.dst
        if dst is None:
            return True
        if dst[0] == "m":
            self.write(dst, None, ins, loc)
            return True
        r = dst[1]
        if ins.op in ("add", "sub") and ins.src and ins.src[0] == "i" and ins.w == 4:
            n = ins.src[1] if ins.op == "add" else -ins.src[1]
            if r == ESP:
                self.sp += n
                return True
            v = self.regs[r]
            if v is not None and v[0] in ("this", "sp"):
                self.regs[r] = (v[0], v[1] + n)
                return True
            self.regs[r] = None
            return True
        if r == ESP:
            raise _Stop()
        self.setreg(r if ins.w != 1 else r & 3, None)
        return True

    def edge(self, ins):
        t = ins.tgt
        if t is None or not (self.va <= t < self.end):
            return
        if t > ins.addr:
            cur = self.state()
            old = self.snaps.get(t)
            self.snaps[t] = _copy(cur) if old is None else _copy(_meet(old, cur))
            return
        head = self.heads.get(t)
        if head is None:
            raise _Stop()
        regs, stack, sp = head
        if sp != self.sp or any(regs[r] is not None and regs[r] != self.regs[r] for r in range(8)) or \
                any(self.stack.get(a) != v for a, v in stack.items()):
            raise _Discard()

    def call(self, ins):
        if ins.tgt == ins.addr + ins.n:      # `call next; pop reg` reads the program counter: the return address stays
            self.sp -= 4
            self.kill_slots(self.sp, 4)
            return True
        arg0 = self.stack.get(self.sp)
        result = None
        if ins.tgt is not None and ins.mem is None:
            if ins.tgt in self.news and arg0 is not None and arg0[0] == "imm" and arg0[1] > 0:
                result = ("new", arg0[1])
            elif ins.tgt in self.ctors and arg0 is not None and arg0[0] == "new":
                self.hits.append((ins.addr, self.ctors[ins.tgt], arg0[1]))
        else:
            self.touch(ins)
        for r in CALLER_SAVED:
            self.regs[r] = None
        self.regs[EAX] = result
        self.stack = {a: v for a, v in self.stack.items() if a >= 4}
        return True
