"""Remove mod-only conditional compilation.

Macros listed in mod_macros.txt are treated as undefined. `defined`, `!`, `&&`,
`||` and parentheses are simplified; anything else that touches such a macro is
an error. Conditions without mod macros are kept verbatim.
"""
import fnmatch
import re
from pathlib import Path

DATA = Path(__file__).resolve().parent / "data"

_DIRECTIVE = re.compile(r"^[ \t]*#[ \t]*(\w+)(.*)$", re.S)
_TOKEN = re.compile(r"\s*(defined|[A-Za-z_]\w*|0[xX][0-9a-fA-F]+[uUlL]*|\d+[uUlL]*|&&|\|\||==|!=|<=|>=|<<|>>|[()!<>+\-*/%&|^~?:,])")
_COMMENTS = re.compile(r"/\*.*?\*/|//[^\n]*", re.S)
_WORD = re.compile(r"[A-Za-z_]\w*")

T, F, U = "T", "F", "U"


class UnifdefError(Exception):
    pass


def load_patterns(path=None):
    path = Path(path) if path else DATA / "mod_macros.txt"
    pats = []
    for line in path.read_text(encoding="utf-8").splitlines():
        line = line.strip()
        if line and not line.startswith("#"):
            pats.append(line)
    return pats


class _Expr:
    def __init__(self, val, text, prec):
        self.val, self.text, self.prec = val, text, prec


def _tokenize(src):
    pos, out = 0, []
    src = src.strip()
    while pos < len(src):
        m = _TOKEN.match(src, pos)
        if not m:
            raise UnifdefError(f"cannot tokenize: {src!r}")
        out.append(m.group(1))
        pos = m.end()
    return out


class _Parser:
    def __init__(self, tokens, is_mod, raw):
        self.t, self.i, self.is_mod, self.raw = tokens, 0, is_mod, raw

    def peek(self):
        return self.t[self.i] if self.i < len(self.t) else None

    def take(self):
        tok = self.peek()
        self.i += 1
        return tok

    def bad(self):
        return UnifdefError(f"unresolvable expression: {self.raw!r}")

    def parse(self):
        e = self.or_()
        if self.peek() is not None:
            raise self.bad()
        return e

    def or_(self):
        items = [self.and_()]
        while self.peek() == "||":
            self.take()
            items.append(self.and_())
        return self._combine(items, "||", T, F)

    def and_(self):
        items = [self.not_()]
        while self.peek() == "&&":
            self.take()
            items.append(self.not_())
        return self._combine(items, "&&", F, T)

    def _combine(self, items, op, absorbing, neutral):
        if len(items) == 1:
            return items[0]
        if any(x.val == absorbing for x in items):
            return _Expr(absorbing, "", 9)
        rest = [x for x in items if x.val != neutral]
        if not rest:
            return _Expr(neutral, "", 9)
        if len(rest) == 1:
            return rest[0]
        prec = 1 if op == "||" else 2
        text = f" {op} ".join(x.text if x.prec > prec else f"({x.text})" for x in rest)
        return _Expr(U, text, prec)

    def not_(self):
        if self.peek() == "!":
            self.take()
            e = self.not_()
            if e.val == T:
                return _Expr(F, "", 9)
            if e.val == F:
                return _Expr(T, "", 9)
            return _Expr(U, "!" + (e.text if e.prec >= 9 else f"({e.text})"), 8)
        return self.atom()

    def atom(self):
        tok = self.take()
        if tok is None:
            raise self.bad()
        if tok == "(":
            e = self.or_()
            if self.take() != ")":
                raise self.bad()
            if e.val != U or e.prec >= 9:
                return e
            return _Expr(U, f"({e.text})", 9)
        if tok == "defined":
            paren = self.peek() == "("
            if paren:
                self.take()
            name = self.take()
            if paren and self.take() != ")":
                raise self.bad()
            if name is None or not _WORD.fullmatch(name):
                raise self.bad()
            if self.is_mod(name):
                return _Expr(F, "", 9)
            return _Expr(U, f"defined({name})" if paren else f"defined {name}", 9)
        if _WORD.fullmatch(tok):
            if self.is_mod(tok):
                return _Expr(F, "", 9)
            return _Expr(U, tok, 9)
        if re.match(r"\d", tok):
            return _Expr(U, tok, 9)
        raise self.bad()


def evaluate(cond, is_mod):
    """Returns (value, residual_text, touched)."""
    plain = _COMMENTS.sub(" ", cond)
    if not any(is_mod(w) for w in _WORD.findall(plain)):
        return U, cond.strip(), False
    tokens = _tokenize(plain)
    for k, tok in enumerate(tokens):
        if is_mod(tok):
            prev = tokens[k - 1] if k else None
            nxt = tokens[k + 1] if k + 1 < len(tokens) else None
            if prev not in (None, "(", "!", "&&", "||", "defined") or nxt not in (None, ")", "&&", "||"):
                raise UnifdefError(f"unresolvable expression: {cond.strip()!r}")
    e = _Parser(tokens, is_mod, cond.strip()).parse()
    return e.val, e.text, True


class _Group:
    def __init__(self, parent_active):
        self.taken = False
        self.opened = False
        self.active = False
        self.dummy = not parent_active


def process(text, patterns, name="<text>"):
    def is_mod(word):
        return any(fnmatch.fnmatchcase(word, pat) for pat in patterns)

    lines = text.split("\n")
    out = []
    stack = []
    i = 0
    n = len(lines)

    def emitting():
        return all(g.active for g in stack)

    def branch(g, cond, phys, kind):
        try:
            val, resid, touched = evaluate(cond, is_mod)
        except UnifdefError as e:
            raise UnifdefError(f"{name}: {e}") from None
        if g.taken or val == F:
            g.active = False
        elif val == T:
            g.taken = True
            g.active = True
            if g.opened:
                out.append("#else")
        else:
            g.active = True
            if not touched and (kind != "elif" or g.opened):
                out.extend(phys)
            else:
                out.append(("#elif " if g.opened else "#if ") + resid)
            g.opened = True

    while i < n:
        line = lines[i]
        i += 1
        m = _DIRECTIVE.match(line)
        phys = [line]
        if m:
            while phys[-1].endswith("\\") and i < n:
                phys.append(lines[i])
                i += 1
            if len(phys) > 1:
                m = _DIRECTIVE.match(" ".join(x.rstrip("\\") for x in phys))
        if not m:
            if emitting():
                out.append(line)
            continue
        d, rest = m.group(1), m.group(2)
        if d == "pragma" and re.match(r"\s*message\b", rest):
            continue
        if d in ("if", "ifdef", "ifndef"):
            g = _Group(emitting())
            stack.append(g)
            if g.dummy:
                continue
            if d == "if":
                cond = rest
            else:
                cond = ("defined(%s)" if d == "ifdef" else "!defined(%s)") % _COMMENTS.sub(" ", rest).strip()
            branch(g, cond, phys, d)
            continue
        if d in ("elif", "else", "endif"):
            if not stack:
                raise UnifdefError(f"{name}: #{d} without #if")
            g = stack[-1]
            if g.dummy:
                if d == "endif":
                    stack.pop()
                continue
            if d == "endif":
                stack.pop()
                if g.opened:
                    out.extend(phys)
            elif d == "else":
                if g.taken:
                    g.active = False
                else:
                    g.active = True
                    if g.opened:
                        out.extend(phys)
            else:
                branch(g, rest, phys, "elif")
            continue
        if emitting():
            out.extend(phys)
    if stack:
        raise UnifdefError(f"{name}: unterminated #if")
    result = "\n".join(out)
    for w in set(_WORD.findall(result)):
        if is_mod(w):
            raise UnifdefError(f"{name}: mod macro {w} left in output")
    return result


def process_default(text, name="<text>"):
    return process(text, load_patterns(), name)
