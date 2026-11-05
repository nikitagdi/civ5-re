"""LLVM IR text: globals, `@_ZTV…` initializers, DI metadata."""
from __future__ import annotations

import re


def unquote_symbol(s: str) -> str:
    """`@name` or `@"quoted\\01"` → symbol text."""
    if s.startswith("@"):
        s = s[1:]
    if s.startswith('"') and s.endswith('"'):
        s = s[1:-1]
        s = _unescape(s)
    return s


def split_top(s: str, sep=","):
    """Split at `sep` outside (), [], {}, <> and string literals."""
    out, depth, cur, i, n = [], 0, [], 0, len(s)
    while i < n:
        c = s[i]
        if c == '"':
            j = i + 1
            while j < n and s[j] != '"':
                j += 2 if s[j] == "\\" else 1
            cur.append(s[i:j + 1])
            i = j + 1
            continue
        if c in "([{<":
            depth += 1
        elif c in ")]}>":
            depth -= 1
        if c == sep and depth == 0:
            out.append("".join(cur).strip())
            cur = []
        else:
            cur.append(c)
        i += 1
    tail = "".join(cur).strip()
    if tail or out:
        out.append(tail)
    return out


class DINode:
    __slots__ = ("id", "kind", "f", "distinct")

    def __init__(self, id_, kind, fields, distinct):
        self.id, self.kind, self.f, self.distinct = id_, kind, fields, distinct

    def get(self, k, default=None):
        return self.f.get(k, default)

    def __repr__(self):
        return f"!{self.id} = !{self.kind}({self.f})"


class Global:
    __slots__ = ("name", "linkage", "type", "init", "dbg")

    def __init__(self, name, type_, init, dbg):
        self.name, self.type, self.init, self.dbg = name, type_, init, dbg


_META = re.compile(r"^!(\d+) = (distinct )?!(DI\w+|\{)(.*)$")
_GLOBAL = re.compile(r'^(@(?:"(?:[^"\\]|\\.)*"|[-\w$.]+)) = (.*)$')
_DBG = re.compile(r", !dbg !(\d+)")


class Ref(int):
    """A metadata reference `!N` (distinct from a plain number)."""


def _unescape(s: str) -> str:
    return re.sub(r"\\(\\|[0-9A-Fa-f]{2})", lambda m: "\\" if m.group(1) == "\\" else chr(int(m.group(1), 16)), s)


def _parse_value(v: str):
    v = v.strip()
    if v.startswith("!") and v[1:].isdigit():
        return Ref(int(v[1:]))
    if v == "null":
        return None
    if v.startswith('"') and v.endswith('"'):
        return _unescape(v[1:-1])
    if v.startswith("i32 ") or v.startswith("i64 "):
        try:
            return int(v.split()[1])
        except ValueError:
            return v
    try:
        return int(v)
    except ValueError:
        return v


_FIELD = re.compile(r'(\w+): ("(?:[^"\\]|\\.)*"|![A-Za-z]+\([^()]*\)|[^,]*)(?:, |$)')
_ITEM = re.compile(r'("(?:[^"\\]|\\.)*"|!"(?:[^"\\]|\\.)*"|[^,]+)(?:, |$)')


def _parse_fields(body: str):
    out = {}
    for m in _FIELD.finditer(body):
        out[m.group(1)] = _parse_value(m.group(2))
    return out


def _parse_tuple(body: str):
    body = body.strip()
    if body.endswith("}"):
        body = body[:-1]
    return [_parse_value(m.group(1)) for m in _ITEM.finditer(body) if m.group(1).strip()]


class _Lazy:
    """Metadata parsed on first access (most DI nodes of a large module are never read)."""

    def __init__(self, parser):
        self.raw = {}
        self._done = {}
        self._parser = parser

    def get(self, idx, default=None):
        v = self._done.get(idx)
        if v is not None:
            return v
        r = self.raw.get(idx)
        if r is None:
            return default
        v = self._parser(idx, r)
        self._done[idx] = v
        return v

    def __getitem__(self, idx):
        v = self.get(idx)
        if v is None:
            raise KeyError(idx)
        return v

    def __contains__(self, idx):
        return idx in self.raw

    def values(self):
        return [self.get(i) for i in self.raw]

    def select(self, pred):
        """Parsed nodes whose raw (kind, distinct, body) satisfies `pred`."""
        return [self.get(i) for i, r in self.raw.items() if pred(r)]


def _mk_node(idx, r):
    kind, distinct, rest = r
    body = rest[1:rest.rfind(")")] if rest.startswith("(") else ""
    return DINode(idx, kind, _parse_fields(body), distinct)


class Module:
    def __init__(self):
        self.meta = _Lazy(_mk_node)
        self.tuples = _Lazy(lambda idx, r: _parse_tuple(r))
        self.globals = {}
        self.declared = set()
        self.defined = set()


_ARRAY = re.compile(r"\[(\d+) x ptr\] \[")


def parse(text: str) -> Module:
    m = Module()
    for line in text.split("\n"):
        if not line:
            continue
        c = line[0]
        if c == "!":
            mm = _META.match(line)
            if not mm:
                continue
            idx, distinct, kind, rest = int(mm.group(1)), bool(mm.group(2)), mm.group(3), mm.group(4)
            if kind == "{":
                m.tuples.raw[idx] = rest
            else:
                m.meta.raw[idx] = (kind, distinct, rest)
        elif c == "@":
            gm = _GLOBAL.match(line)
            if not gm:
                continue
            name = unquote_symbol(gm.group(1))
            rhs = gm.group(2)
            dbg = None
            d = _DBG.search(rhs)
            if d:
                dbg = int(d.group(1))
            m.globals[name] = Global(name, None, rhs, dbg)
        elif line.startswith("declare "):
            sm = re.search(r'@("(?:[^"\\]|\\.)*"|[-\w$.]+)\(', line)
            if sm:
                m.declared.add(unquote_symbol(sm.group(1)))
        elif line.startswith("define "):
            sm = re.search(r'@("(?:[^"\\]|\\.)*"|[-\w$.]+)\(', line)
            if sm:
                m.defined.add(unquote_symbol(sm.group(1)))
    return m


def _init_part(rhs: str):
    """The initializer text of `global/constant <type> <init>, align …`."""
    m = re.search(r"\b(?:global|constant) ", rhs)
    if not m:
        return None
    s = rhs[m.end():]
    parts = split_top(s)
    return parts[0] if parts else None


def int_array(g: Global):
    s = _init_part(g.init)
    if s is None:
        return None
    m = re.search(r"\] \[(.*)\]$", s)
    if not m:
        return None
    vals = []
    for part in split_top(m.group(1)):
        t = part.split()
        if len(t) == 2:
            vals.append(int(t[1]))
    return vals


def ptr_target(g: Global):
    s = _init_part(g.init)
    if s is None:
        return None
    s = s.strip()
    if s.startswith("ptr "):
        s = s[4:].strip()
    if s.startswith("@"):
        return unquote_symbol(s)
    m = re.match(r'\{ i32, i32 \} \{ i32 ptrtoint \(ptr (@(?:"(?:[^"\\]|\\.)*"|[-\w$.]+)) to i32\)', s)
    if m:  # pointer to a non-virtual member function: {address, this adjustment}
        return unquote_symbol(m.group(1))
    return None


def _entry(e: str):
    e = e.strip()
    if e.startswith("ptr "):
        e = e[4:].strip()
    if e == "null":
        return ("int", 0)
    if e.startswith("@"):
        return ("sym", unquote_symbol(e))
    m = re.match(r"inttoptr \(i(?:32|64) (-?\d+) to ptr\)", e)
    if m:
        return ("int", int(m.group(1)))
    m = re.search(r"@(\"(?:[^\"\\]|\\.)*\"|[-\w$.]+)", e)
    if m:
        return ("sym", unquote_symbol(m.group(1)))
    return ("other", e)


def vtable_arrays(g: Global):
    """`{ [N x ptr] [...], ... }` → list of entry lists (("sym", name) | ("int", v))."""
    s = _init_part(g.init)
    if s is None:
        return None
    s = s.strip()
    if s.startswith("{"):
        depth = 0
        for i, ch in enumerate(s):
            if ch == "{":
                depth += 1
            elif ch == "}":
                depth -= 1
                if depth == 0:
                    s = s[i + 1:].strip()
                    break
        if s.startswith("{"):
            s = s[1:s.rfind("}")]
    groups = []
    for part in re.split(r", (?=\[\d+ x ptr\] \[)", s.strip()):
        mm = re.match(r"\[\d+ x ptr\] \[(.*)\]$", part.strip(), re.S)
        if mm:
            groups.append([_entry(e) for e in re.split(r", (?=ptr )", mm.group(1))])
    return groups


def typeinfo_base(g: Global):
    """Third pointer of an `__si_class_type_info` (the base's _ZTI) or the first base of a vmi one."""
    s = _init_part(g.init)
    if s is None:
        return None
    syms = re.findall(r"@(\"(?:[^\"\\]|\\.)*\"|[-\w$.]+)", s)
    syms = [unquote_symbol(x) for x in syms]
    out = [x for x in syms if x.startswith("_ZTI")]
    return out[0] if out else None


def flags(node: DINode, key="flags"):
    v = node.get(key)
    if not v or not isinstance(v, str):
        return set()
    return {x.strip() for x in v.split("|")}
