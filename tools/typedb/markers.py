"""Comment lexer, strict marker grammar, marker-like lines and `comment` extraction (no clang)."""
from __future__ import annotations

import functools
import re
from dataclasses import dataclass

KEYWORDS = ("origin", "size", "vtable", "base", "deleting", "thunk", "inline", "pure", "absent", "lua")


@dataclass(frozen=True)
class Marker:
    line: int
    kind: str
    value: str | None
    fastcc: bool
    spec: str | None
    col: int
    trailing: bool


@dataclass(frozen=True)
class Block:
    first_line: int
    last_line: int
    markers: tuple[Marker, ...]


@dataclass(frozen=True)
class Diag:
    code: str
    line: int
    col: int
    message: str


@dataclass(frozen=True)
class FileScan:
    blocks: tuple[Block, ...]
    trailing: dict[int, Marker]
    comments: dict[int, tuple[int, str]]
    diags: tuple[Diag, ...]
    origin: Marker | None


@dataclass(frozen=True)
class LineComment:
    line: int
    col: int
    text: str
    full: bool


def lex_line_comments(text: str) -> list[LineComment]:
    """Return every `//` comment outside strings, char literals and block comments."""
    n = len(text)
    i = 0
    line = 1
    ls = 0
    out: list[LineComment] = []
    while i < n:
        c = text[i]
        if c == "\n":
            line += 1
            i += 1
            ls = i
            continue
        if c == "/" and i + 1 < n and text[i + 1] == "/":
            col = i - ls + 1
            full = text[ls:i].strip() == ""
            first_end = -1
            start_line = line
            j = i + 2
            while j < n:
                if text[j] == "\n":
                    if first_end < 0:
                        first_end = j
                    k = j - 1
                    if k >= 0 and text[k] == "\r":
                        k -= 1
                    if k >= i + 2 and text[k] == "\\":
                        j += 1
                        line += 1
                        ls = j
                        continue
                    break
                j += 1
            if first_end < 0:
                first_end = j
            out.append(LineComment(line if first_end == j else _line_of(text, i), col,
                                   text[i + 2:first_end].rstrip("\r"), full))
            i = j
            continue
        if c == "/" and i + 1 < n and text[i + 1] == "*":
            j = text.find("*/", i + 2)
            end = n if j < 0 else j + 2
            nl = text.count("\n", i, end)
            if nl:
                line += nl
                ls = text.rfind("\n", i, end) + 1
            i = end
            continue
        if c == '"' and _is_raw_prefix(text, i):
            k = text.find("(", i + 1, i + 18)
            delim = text[i + 1:k] if k >= 0 else None
            if delim is not None and not re.search(r'[\s()\\"]', delim):
                j = text.find(")" + delim + '"', k + 1)
                end = n if j < 0 else j + len(delim) + 2
                nl = text.count("\n", i, end)
                if nl:
                    line += nl
                    ls = text.rfind("\n", i, end) + 1
                i = end
                continue
        if c == '"' or c == "'":
            j = i + 1
            while j < n:
                ch = text[j]
                if ch == "\\" and j + 1 < n:
                    if text[j + 1] == "\n":
                        line += 1
                        j += 2
                        ls = j
                    elif text[j + 1] == "\r" and j + 2 < n and text[j + 2] == "\n":
                        line += 1
                        j += 3
                        ls = j
                    else:
                        j += 2
                elif ch == c:
                    j += 1
                    break
                elif ch == "\n":
                    break
                else:
                    j += 1
            i = j
            continue
        i += 1
    return out


def _line_of(text: str, pos: int) -> int:
    return text.count("\n", 0, pos) + 1


def _is_raw_prefix(text: str, quote: int) -> bool:
    s = quote
    while s > 0 and (text[s - 1].isalnum() or text[s - 1] == "_"):
        s -= 1
    return text[s:quote] in ("R", "LR", "uR", "UR", "u8R")


class _Bad(Exception):
    pass


_HEX_ANY = re.compile(r"0[xX][0-9a-fA-F]+\Z")
_HEX = re.compile(r"0x[0-9a-f]+\Z")
_ADDR = re.compile(r"0x[0-9a-f]{8}\Z")
_OFFSET = re.compile(r"\+0x[0-9a-f]+\Z")


def _hex_problem(tok: str, digits: int | None) -> str:
    if _HEX_ANY.match(tok):
        if "X" in tok or re.search(r"[A-F]", tok):
            return f"hex must be lowercase: {tok!r}"
        if digits is not None:
            return f"address must be exactly {digits} hex digits: {tok!r}"
    return f"bad hex value {tok!r}"


_OP = re.compile(r"\boperator\s*(<=>|<<=|>>=|->\*|->|<<|>>|<=|>=|<|>)")


@functools.lru_cache(maxsize=None)
def mask_ops(s: str) -> str:
    """Same-length copy of `s` with the angle brackets of operator names (`operator<<`, `operator->`) hidden, so that
    scanning for template argument lists does not take them for brackets."""
    return _OP.sub(lambda m: m.group(0)[:m.start(1) - m.start(0)] + "_" * len(m.group(1)), s)


def args_start(s: str) -> int:
    """Index of the `<` that opens the template argument list of the first component of `s`, or -1."""
    return mask_ops(s).find("<")


@functools.lru_cache(maxsize=None)
def strip_args(s: str) -> str:
    """`operator<< <int>` -> `operator<<`, `Foo<int>` -> `Foo`."""
    i = args_start(s)
    return (s if i < 0 else s[:i]).strip()


def split_top(s, sep=","):
    """Split at `sep` outside any <>, (), [] (a `::` separator is two characters)."""
    out, depth, cur, i = [], 0, [], 0
    masked = mask_ops(s)
    while i < len(s):
        c = masked[i]
        if c in "<([":
            depth += 1
        elif c in ">)]":
            depth -= 1
        if depth == 0 and masked.startswith(sep, i):
            out.append("".join(cur))
            cur = []
            i += len(sep)
            continue
        cur.append(s[i])
        i += 1
    out.append("".join(cur))
    return out


@functools.lru_cache(maxsize=None)
def normalize_spec(s: str) -> str:
    """Canonical SPEC: spaces kept only between two word characters (`unsigned int`); `operator<< <int>` keeps the
    space after the operator (a lone `<<<` would lex as `<<` `<`)."""
    s = re.sub(r"\s+", " ", s.strip())
    s = re.sub(r"(?<=\W) | (?=\W)", "", s)
    return re.sub(r"(\boperator(?:<=>|<<=|<<|<=|<))<", r"\1 <", s)


def _check_spec(s: str) -> str:
    s = s.strip()
    if not s:
        raise _Bad("empty SPEC")
    core = re.sub(r"(?<=>)(?:::[A-Za-z_]\w*)+\Z", "", s)  # `Outer<int>::Nested`: classes nested in the template
    if not core.endswith(">"):
        raise _Bad(f"SPEC must end with '>' or '>::Name' and nothing may follow it: {s!r}")
    if not (s[0].isalpha() or s[0] in "_:"):
        raise _Bad(f"bad SPEC start: {s!r}")
    if re.search(r"[;{}#\"]", s):
        raise _Bad(f"bad character in SPEC: {s!r}")
    depth = par = 0
    seen = False
    for ch in mask_ops(s):
        if ch == "(":
            par += 1
        elif ch == ")":
            par -= 1
            if par < 0:
                raise _Bad(f"unbalanced SPEC: {s!r}")
        elif par == 0:
            if ch == "<":
                depth += 1
                seen = True
            elif ch == ">":
                depth -= 1
                if depth < 0:
                    raise _Bad(f"unbalanced SPEC: {s!r}")
    if depth != 0 or par != 0 or not seen:
        raise _Bad(f"unbalanced SPEC: {s!r}")
    return normalize_spec(s)


def _opt_spec(tail: str | None) -> str | None:
    return None if tail is None else _check_spec(tail)


def _split(rest: str) -> tuple[str, str | None]:
    tok, sep, tail = rest.partition(" ")
    if sep and (tail == "" or tail.startswith(" ")):
        raise _Bad("exactly one space between tokens")
    return tok, (tail if sep else None)


def parse_body(body: str) -> tuple[str, str | None, bool, str | None]:
    """Parse a marker body into (kind, value, fastcc, spec); raise _Bad on error."""
    if body == "":
        raise _Bad("empty marker")
    word, sep, rest = body.partition(" ")
    rest_or_none = rest if sep else None
    if word.startswith("0x") or word.startswith("0X") or re.match(r"[0-9]", word):
        if not _ADDR.match(word):
            raise _Bad(_hex_problem(word, 8))
        tail = rest_or_none
        fast = False
        if tail == "fastcc":
            fast, tail = True, None
        elif tail is not None and tail.startswith("fastcc "):
            fast, tail = True, tail[7:]
            if tail == "" or tail.startswith(" "):
                raise _Bad("exactly one space between tokens")
        return "addr", word, fast, _opt_spec(tail)
    if word in ("inline", "pure", "absent"):
        if rest_or_none is not None:
            raise _Bad(f"'{word}' takes no arguments")
        return word, None, False, None
    if word == "origin":
        if rest_or_none not in ("binary", "guess"):
            raise _Bad("origin must be 'binary' or 'guess'")
        return "origin", rest_or_none, False, None
    if word == "lua":
        if rest_or_none is None or not rest_or_none.strip():
            raise _Bad("lua requires text")
        return "lua", rest_or_none.rstrip(), False, None
    if word in ("size", "vtable"):
        if rest_or_none is None:
            raise _Bad(f"'{word}' requires a value")
        tok, tail = _split(rest_or_none)
        if tok != "?":
            ok = _HEX.match(tok) if word == "size" else _ADDR.match(tok)
            if not ok:
                raise _Bad(_hex_problem(tok, None if word == "size" else 8))
        return word, tok, False, _opt_spec(tail)
    if word in ("base", "deleting", "thunk"):
        if rest_or_none is None:
            if word == "base":
                return "base", None, False, None
            raise _Bad(f"'{word}' requires an address")
        if word == "base" and not (rest_or_none.startswith("0x") or rest_or_none.startswith("0X")):
            return "base", None, False, _check_spec(rest_or_none)
        tok, tail = _split(rest_or_none)
        if not _ADDR.match(tok):
            raise _Bad(_hex_problem(tok, 8))
        return word, tok, False, _opt_spec(tail)
    raise _Bad(f"unknown marker word {word!r}")


def _parse_offset(rest: str) -> str:
    if not rest.startswith(" ") or rest.startswith("  "):
        raise _Bad("'///<' must be followed by exactly one space and '+0x<hex>'")
    tok = rest[1:].rstrip()
    if not _OFFSET.match(tok):
        raise _Bad(f"bad field offset {tok!r}, expected '+0x<lowercase hex>'")
    return tok[1:]


_NEAR_OFFSET = re.compile(r"<\s*\+0x[0-9a-f]+\Z")
_NEAR_START = re.compile(r"(?:0[xX]|(?:%s)(?=\s|\Z))" % "|".join(KEYWORDS))


def _group_runs(lines) -> list[tuple[int, int]]:
    runs: list[tuple[int, int]] = []
    for ln in sorted(lines):
        if runs and runs[-1][1] == ln - 1:
            runs[-1] = (runs[-1][0], ln)
        else:
            runs.append((ln, ln))
    return runs


def scan(text: str, *, strict: bool = True) -> FileScan:
    if text.startswith("﻿"):
        text = text[1:]
    lcs = lex_line_comments(text)
    diags: list[Diag] = []
    plain: dict[int, str] = {}
    marker_lines: dict[int, Marker | None] = {}
    trailing: dict[int, Marker] = {}
    origin: Marker | None = None
    near_done: set[int] = set()

    def diag(code: str, lc: LineComment, msg: str) -> None:
        diags.append(Diag(code, lc.line, lc.col, msg))

    for lc in lcs:
        t = lc.text
        if not strict:
            if lc.full:
                plain[lc.line] = t.lstrip("/")
            continue
        if not t.startswith("/"):
            if lc.full:
                plain[lc.line] = t
            s = t.strip()
            near = bool(_NEAR_OFFSET.match(s))
            if not near:
                try:
                    parse_body(s)
                    near = True
                except _Bad:
                    pass
            if near:
                near_done.add(lc.line)
                diag("near-marker", lc, f"'//' comment looks like a marker; use '///': {s!r}")
            continue
        s = t[1:]
        if s.startswith("/"):
            diag("marker-syntax", lc, "four or more slashes; a marker starts with exactly '///'")
            continue
        if not lc.full:
            if s.startswith("<"):
                try:
                    val = _parse_offset(s[1:])
                    trailing[lc.line] = Marker(lc.line, "offset", val, False, None, lc.col, True)
                except _Bad as e:
                    diag("marker-syntax", lc, str(e))
            else:
                diag("marker-misplaced", lc, "'///' after code; markers go on their own line (or '///<')")
            continue
        if s.startswith("<"):
            diag("marker-misplaced", lc, "'///<' on its own line; it ends a field's line")
            continue
        if s.strip() == "":
            diag("marker-syntax", lc, "empty '///'")
            marker_lines[lc.line] = None
            continue
        if not s.startswith(" ") or s.startswith("  "):
            diag("marker-syntax", lc, "'///' must be followed by exactly one space")
            marker_lines[lc.line] = None
            continue
        try:
            kind, val, fast, spec = parse_body(s[1:].rstrip())
        except _Bad as e:
            diag("marker-syntax", lc, str(e))
            marker_lines[lc.line] = None
            continue
        m = Marker(lc.line, kind, val, fast, spec, lc.col, False)
        if kind == "origin":
            if lc.line != 1:
                diag("marker-misplaced", lc, "'origin' is only allowed on line 1")
                marker_lines[lc.line] = None
            else:
                origin = m
            continue
        marker_lines[lc.line] = m

    blocks: list[Block] = []
    runs = _group_runs(marker_lines)
    for a, b in runs:
        ms = tuple(m for ln in range(a, b + 1) if (m := marker_lines.get(ln)) is not None)
        if ms:
            blocks.append(Block(a, b, ms))

    if strict:
        for a, b in runs:
            for ln in (a - 1, b + 1):
                t = plain.get(ln)
                if t is None or ln in near_done:
                    continue
                if _NEAR_START.match(t.strip()):
                    lc = next(c for c in lcs if c.line == ln)
                    near_done.add(ln)
                    diag("near-marker", lc, f"'//' comment next to a marker block looks like a marker: {t.strip()!r}")

    comments: dict[int, tuple[int, str]] = {}
    for a, b in _group_runs(plain):
        lines = []
        for ln in range(a, b + 1):
            t = plain[ln]
            if t.startswith(" "):
                t = t[1:]
            lines.append(t.rstrip())
        comments[b + 1] = (a, "\n".join(lines))

    diags.sort(key=lambda d: (d.line, d.col, d.code))
    return FileScan(tuple(blocks), trailing, comments, tuple(diags), origin)
