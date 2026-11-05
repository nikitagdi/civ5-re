"""`/// lua` marker body -> AST.

Columns are 1-based within the marker body (plus `col_offset`).
Type forms `t` are plain dicts: {k: prim|name|array|map|record|fun|union|nullable|eventargs, ...}.
Name resolution is not done here; referenced names are exposed in `Marker.refs`.
"""
from __future__ import annotations

import re
from dataclasses import dataclass, field
from typing import ClassVar, NamedTuple

PRIMS = frozenset("nil boolean integer number string table function any userdata thread".split())
METAMETHODS = ("__index", "__call", "__newindex", "__len")


class GrammarError(ValueError):
    def __init__(self, message: str, col: int):
        super().__init__(f"{message} (column {col})")
        self.message = message
        self.col = col


class Ref(NamedTuple):
    name: str
    col: int


@dataclass(frozen=True, kw_only=True)
class Marker:
    kind: ClassVar[str]
    text: str
    refs: tuple = field(default=(), compare=False)
    eventargs: tuple = field(default=(), compare=False)


@dataclass(frozen=True, kw_only=True)
class Unknown(Marker):
    kind: ClassVar[str] = "?"


@dataclass(frozen=True, kw_only=True)
class NoneMarker(Marker):
    kind: ClassVar[str] = "none"


@dataclass(frozen=True, kw_only=True)
class Table(Marker):
    kind: ClassVar[str] = "table"
    path: str


@dataclass(frozen=True, kw_only=True)
class Class(Marker):
    kind: ClassVar[str] = "class"
    name: str
    base: str | None
    xml: tuple


@dataclass(frozen=True, kw_only=True)
class Global(Marker):
    kind: ClassVar[str] = "global"
    name: str
    type: str
    t: dict


@dataclass(frozen=True, kw_only=True)
class Enum(Marker):
    kind: ClassVar[str] = "enum"
    name: str


@dataclass(frozen=True, kw_only=True)
class Event(Marker):
    kind: ClassVar[str] = "event"
    path: str


@dataclass(frozen=True, kw_only=True)
class Push(Marker):
    kind: ClassVar[str] = "push"
    fields: list


@dataclass(frozen=True, kw_only=True)
class Pull(Marker):
    kind: ClassVar[str] = "pull"
    fields: list


@dataclass(frozen=True, kw_only=True)
class Shape(Marker):
    kind: ClassVar[str] = "shape"
    name: str
    fields: list


@dataclass(frozen=True, kw_only=True)
class Function(Marker):
    kind: ClassVar[str] = "function"
    owner: str | None
    name: str
    call: str  # dot | method | global
    params: list
    returns: list

    @property
    def path(self) -> str:
        if self.owner is None:
            return self.name
        return self.owner + (":" if self.call == "method" else ".") + self.name

    @property
    def is_eventargs(self) -> bool:
        return len(self.params) == 1 and self.params[0]["t"]["k"] == "eventargs"


# ---------------------------------------------------------------- formatting

def _needs_parens(t: dict) -> bool:
    k = t["k"]
    return k in ("union", "nullable") or (k == "fun" and bool(t["returns"]))


def format_type(t: dict) -> str:
    k = t["k"]
    if k in ("prim", "name"):
        return t["name"]
    if k == "eventargs":
        return "EventArgs"
    if k == "array":
        inner = format_type(t["elem"])
        return (f"({inner})" if _needs_parens(t["elem"]) else inner) + "[]"
    if k == "nullable":
        inner = format_type(t["inner"])
        return (f"({inner})" if _needs_parens(t["inner"]) else inner) + "?"
    if k == "map":
        return f"table<{format_type(t['key'])}, {format_type(t['value'])}>"
    if k == "record":
        return "{" + format_fields(t["fields"]) + "}"
    if k == "union":
        items = t["items"]
        parts = []
        for i, it in enumerate(items):
            s = format_type(it)
            if it["k"] == "union" or (it["k"] == "fun" and it["returns"] and i < len(items) - 1):
                s = f"({s})"
            parts.append(s)
        return "|".join(parts)
    if k == "fun":
        s = f"fun({format_params(t['params'])})"
        if t["returns"]:
            s += " -> " + format_returns(t["returns"])
        return s
    raise ValueError(f"bad type form {k!r}")


def format_fields(fields: list) -> str:
    return ", ".join(f"{f['name']}{'?' if f['optional'] else ''}: {f['type']}" for f in fields)


def format_params(params: list) -> str:
    out = []
    for p in params:
        if p["t"]["k"] == "eventargs":
            out.append("EventArgs")
        elif p["vararg"]:
            out.append(f"...: {p['type']}")
        else:
            out.append(f"{p['name']}{'?' if p['optional'] else ''}: {p['type']}")
    return ", ".join(out)


def format_returns(rets: list) -> str:
    return ", ".join(f"{r['name']}: {r['type']}" if r["name"] else r["type"] for r in rets)


def format_marker(m: Marker) -> str:
    k = m.kind
    if k == "?":
        return "?"
    if k == "none":
        return "none"
    if k == "table":
        return f"table {m.path}"
    if k == "class":
        s = f"class {m.name}"
        if m.base:
            s += f" : {m.base}"
        if m.xml:
            s += " xml " + ", ".join(m.xml)
        return s
    if k == "global":
        return f"global {m.name}: {m.type}"
    if k == "enum":
        return f"enum {m.name}"
    if k == "event":
        return f"event {m.path}"
    if k in ("push", "pull"):
        return f"{k} ({format_fields(m.fields)})"
    if k == "shape":
        return f"shape {m.name} {{{format_fields(m.fields)}}}"
    s = f"{m.path}({format_params(m.params)})"
    if m.returns:
        s += " -> " + format_returns(m.returns)
    return s


# ---------------------------------------------------------------- tokenizer

class _Tok(NamedTuple):
    kind: str  # id | p | eof
    text: str
    start: int
    ws: str


_TOK = re.compile(r"([ \t]*)(->|\.\.\.|\[\]|[A-Za-z_][A-Za-z0-9_]*|[(){}<>,:?|.])")
_WS = re.compile(r"[ \t]*")


def _tokenize(text: str, off: int) -> list:
    toks = []
    pos = 0
    n = len(text)
    while True:
        m = _TOK.match(text, pos)
        if m:
            tok = m.group(2)
            kind = "id" if (tok[0].isalpha() or tok[0] == "_") else "p"
            toks.append(_Tok(kind, tok, m.end(1), m.group(1)))
            pos = m.end()
            continue
        ws = _WS.match(text, pos).group()
        if pos + len(ws) == n:
            toks.append(_Tok("eof", "", n, ws))
            return toks
        raise GrammarError(f"unexpected character {text[pos + len(ws)]!r}", off + pos + len(ws) + 1)


# ---------------------------------------------------------------- parser

class _Parser:
    def __init__(self, text: str, strict: bool, off: int):
        self.text = text
        self.strict = strict
        self.off = off
        self.toks = _tokenize(text, off)
        self.i = 0
        self.refs: list = []
        self.ea: list = []

    def col(self, tok: _Tok) -> int:
        return self.off + tok.start + 1

    def peek(self, n: int = 0) -> _Tok:
        return self.toks[min(self.i + n, len(self.toks) - 1)]

    def isp(self, text: str, n: int = 0) -> bool:
        t = self.peek(n)
        return t.kind == "p" and t.text == text

    def isid(self, n: int = 0, text: str | None = None) -> bool:
        t = self.peek(n)
        return t.kind == "id" and (text is None or t.text == text)

    @staticmethod
    def desc(tok: _Tok) -> str:
        return "end of marker" if tok.kind == "eof" else f"'{tok.text}'"

    def fail(self, msg: str, tok: _Tok | None = None):
        raise GrammarError(msg, self.col(tok or self.peek()))

    def _space(self, tok: _Tok, sp):
        if not self.strict or sp is None:
            return
        ws = tok.ws
        wscol = self.off + tok.start - len(ws) + 1
        if sp is False:
            if ws:
                if tok.kind == "eof":
                    raise GrammarError("unexpected trailing space", wscol)
                if self.i == 0:
                    raise GrammarError("unexpected leading space", wscol)
                raise GrammarError(f"unexpected space before {self.desc(tok)}", wscol)
        else:
            if not ws:
                prev = self.toks[self.i - 1].text if self.i else ""
                raise GrammarError(f"needs a space after '{prev}'", self.col(tok))
            if ws != " ":
                raise GrammarError(f"expected a single space before {self.desc(tok)}", wscol)

    def take(self, sp) -> _Tok:
        tok = self.toks[self.i]
        self._space(tok, sp)
        self.i += 1
        return tok

    def expect_p(self, text: str, sp) -> _Tok:
        if not self.isp(text):
            self.fail(f"expected '{text}', found {self.desc(self.peek())}")
        return self.take(sp)

    def expect_id(self, sp, what: str = "a name") -> str:
        if not self.isid():
            self.fail(f"expected {what}, found {self.desc(self.peek())}")
        return self.take(sp).text

    def end(self):
        tok = self.peek()
        if tok.kind != "eof":
            self.fail(f"unexpected {self.desc(tok)}, expected end of marker")
        self._space(tok, False)

    def path(self, sp) -> str:
        parts = [self.expect_id(sp)]
        while self.isp("."):
            self.take(False)
            parts.append(self.expect_id(False))
        return ".".join(parts)

    # ---- types

    def type(self, sp, comma_ok: bool = False) -> dict:
        terms = [self.term(sp, comma_ok)]
        while self.isp("|"):
            self.take(False)
            terms.append(self.term(False, comma_ok))
        if len(terms) == 1:
            return terms[0]
        items = []
        for t in terms:
            items.extend(t["items"] if t["k"] == "union" else [t])
        return {"k": "union", "items": items}

    def term(self, sp, comma_ok: bool) -> dict:
        t = self.atom(sp, comma_ok)
        while self.isp("[]"):
            self.take(False)
            t = {"k": "array", "elem": t}
        if self.isp("?"):
            self.take(False)
            t = {"k": "nullable", "inner": t}
        return t

    def atom(self, sp, comma_ok: bool) -> dict:
        tok = self.peek()
        if tok.kind == "id":
            if tok.text == "fun" and self.isp("(", 1):
                return self.fun(sp, comma_ok)
            if tok.text == "table" and self.isp("<", 1):
                self.take(sp)
                self.take(False)
                key = self.type(False)
                self.expect_p(",", False)
                value = self.type(True)
                self.expect_p(">", False)
                return {"k": "map", "key": key, "value": value}
            self.take(sp)
            if tok.text in PRIMS:
                return {"k": "prim", "name": tok.text}
            self.refs.append(Ref(tok.text, self.col(tok)))
            return {"k": "name", "name": tok.text}
        if self.isp("("):
            self.take(sp)
            inner = self.type(False)
            self.expect_p(")", False)
            return inner
        if self.isp("{"):
            return self.record(sp)
        self.fail(f"expected a type, found {self.desc(tok)}")

    def record(self, sp) -> dict:
        self.expect_p("{", sp)
        fields = self.fields("}", allow_empty=False)
        self.expect_p("}", False)
        return {"k": "record", "fields": fields}

    def fun(self, sp, comma_ok: bool) -> dict:
        self.take(sp)
        self.take(False)
        params = self.params()
        self.expect_p(")", False)
        returns = []
        if self.isp("->"):
            self.take(True)
            returns = self.returns(comma_ok)
        return {"k": "fun", "params": params, "returns": returns}

    # ---- lists

    def field(self, sp) -> dict:
        name = self.expect_id(sp, "a field name")
        opt = False
        if self.isp("?"):
            self.take(False)
            opt = True
        self.expect_p(":", False)
        t = self.type(True)
        return {"name": name, "optional": opt, "type": format_type(t), "t": t}

    def fields(self, close: str, allow_empty: bool) -> list:
        out = []
        if self.isp(close) and allow_empty:
            return out
        sp = False
        while True:
            out.append(self.field(sp))
            if not self.isp(","):
                return out
            self.take(False)
            sp = True

    def param(self, sp) -> dict:
        if self.isp("..."):
            dots = self.take(sp)
            self.expect_p(":", False)
            t = self.type(True)
            if self.isp(","):
                self.fail("'...' must be the last parameter", dots)
            return {"name": "...", "optional": False, "vararg": True, "type": format_type(t), "t": t}
        f = self.field(sp)
        return {"name": f["name"], "optional": f["optional"], "vararg": False, "type": f["type"], "t": f["t"]}

    def params(self) -> list:
        if self.isp(")"):
            return []
        if self.isid(0, "EventArgs") and self.isp(")", 1):
            tok = self.take(False)
            self.ea.append(self.col(tok))
            return [{"name": None, "optional": False, "vararg": False, "type": "EventArgs",
                     "t": {"k": "eventargs"}}]
        out = []
        sp = False
        while True:
            out.append(self.param(sp))
            if not self.isp(","):
                return out
            self.take(False)
            sp = True

    def ret(self) -> dict:
        name = None
        if self.isid() and self.isp(":", 1):
            name = self.take(True).text
            self.take(False)
            t = self.type(True)
        else:
            t = self.type(True)
        return {"name": name, "type": format_type(t), "t": t}

    def returns(self, comma_ok: bool) -> list:
        out = [self.ret()]
        while comma_ok and self.isp(","):
            self.take(False)
            out.append(self.ret())
        return out

    # ---- markers

    def marker(self) -> Marker:
        first = self.peek()
        if first.kind == "eof":
            self.fail("empty marker")
        if self.isp("?"):
            self.take(False)
            self.end()
            return Unknown(text="?")
        if first.kind == "id":
            kw = first.text
            nxt = self.peek(1)
            if kw == "none" and nxt.kind == "eof":
                self.take(False)
                self.end()
                return NoneMarker(text="none")
            if kw in ("table", "class", "global", "enum", "event", "shape") and nxt.kind == "id":
                self.take(False)
                return getattr(self, "m_" + kw)()
            if kw in ("push", "pull") and self.isp("(", 1):
                self.take(False)
                return self.m_tuple(kw)
        return self.m_function()

    def m_table(self):
        path = self.path(True)
        self.end()
        return Table(path=path, text=f"table {path}")

    def m_class(self):
        name = self.expect_id(True)
        base = None
        xml = []
        if self.isp(":"):
            self.take(True)
            btok = self.peek()
            base = self.expect_id(True)
            self.refs.append(Ref(base, self.col(btok)))
        if self.isid(0, "xml"):
            self.take(True)
            xml.append(self.expect_id(True, "an xml tag"))
            while self.isp(","):
                self.take(False)
                xml.append(self.expect_id(True, "an xml tag"))
        self.end()
        m = Class(name=name, base=base, xml=tuple(xml), text="")
        return Class(name=name, base=base, xml=tuple(xml), text=format_marker(m))

    def m_global(self):
        name = self.expect_id(True)
        self.expect_p(":", False)
        t = self.type(True, comma_ok=True)
        self.end()
        return Global(name=name, type=format_type(t), t=t, text=f"global {name}: {format_type(t)}")

    def m_enum(self):
        name = self.expect_id(True)
        self.end()
        return Enum(name=name, text=f"enum {name}")

    def m_event(self):
        path = self.path(True)
        self.end()
        return Event(path=path, text=f"event {path}")

    def m_tuple(self, kind: str):
        self.expect_p("(", True)
        fields = self.fields(")", allow_empty=True)
        self.expect_p(")", False)
        self.end()
        cls = Push if kind == "push" else Pull
        return cls(fields=fields, text=f"{kind} ({format_fields(fields)})")

    def m_shape(self):
        name = self.expect_id(True)
        rec = self.record(True)
        self.end()
        return Shape(name=name, fields=rec["fields"], text=f"shape {name} {{{format_fields(rec['fields'])}}}")

    def m_function(self):
        names = [self.expect_id(False, "a marker or function name")]
        sep = "."
        while self.isp("."):
            self.take(False)
            names.append(self.expect_id(False))
        if self.isp(":"):
            self.take(False)
            names.append(self.expect_id(False))
            sep = ":"
        self.expect_p("(", False)
        params = self.params()
        self.expect_p(")", False)
        returns = []
        if self.isp("->"):
            self.take(True)
            returns = self.returns(True)
        self.end()
        owner = ".".join(names[:-1]) if len(names) > 1 else None
        call = "global" if owner is None else ("method" if sep == ":" else "dot")
        m = Function(owner=owner, name=names[-1], call=call, params=params, returns=returns, text="")
        return Function(owner=owner, name=names[-1], call=call, params=params, returns=returns,
                        text=format_marker(m))


def _parse(text: str, strict: bool, off: int):
    p = _Parser(text, strict, off)
    m = p.marker()
    return m, p


def parse(text: str, *, strict: bool = True, col_offset: int = 0) -> Marker:
    """Parse a marker body (the text after `/// lua `).

    strict=True raises GrammarError for any deviation from the canonical form;
    strict=False accepts arbitrary spacing. `text` of the result is canonical either way.
    """
    m, p = _parse(text, strict, col_offset)
    object.__setattr__(m, "refs", tuple(p.refs))
    object.__setattr__(m, "eventargs", tuple(p.ea))
    return m


def normalize(text: str) -> str:
    return parse(text, strict=False).text


def check(text: str, col_offset: int = 0) -> GrammarError | None:
    try:
        parse(text, col_offset=col_offset)
    except GrammarError as e:
        return e
    return None


def is_canonical(text: str) -> bool:
    return check(text) is None


def parse_type(text: str, *, strict: bool = True) -> dict:
    """Parse a bare type (`Unit?`, `table<string, integer>`) into its `t` form."""
    p = _Parser(text, strict, 0)
    t = p.type(False, comma_ok=True)
    p.end()
    return t
