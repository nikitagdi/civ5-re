"""Mini Itanium demangler: mangled name -> structured names and types (T_, S_ resolved), and C++ spelling.

Covers what Civ5XP Firaxis symbols use: nested and template names, operators, ctor/dtor, builtins, cv,
pointers, references, function and member pointers, arrays, literals as template args, substitutions,
template parameters, thunks (`_ZTh`), special names (`_ZTV`, `_ZTI`, `_ZTS`, `_ZGV`). Anything else raises
`MangleError`.
"""
from dataclasses import dataclass, field


class MangleError(Exception):
    pass


BUILTINS = {
    "v": "void", "w": "wchar_t", "b": "bool", "c": "char", "a": "signed char", "h": "unsigned char",
    "s": "short", "t": "unsigned short", "i": "int", "j": "unsigned int", "l": "long", "m": "unsigned long",
    "x": "long long", "y": "unsigned long long", "n": "__int128", "o": "unsigned __int128", "f": "float",
    "d": "double", "e": "long double", "g": "__float128", "z": "...",
}
DBUILTINS = {"n": "decltype(nullptr)", "s": "char16_t", "i": "char32_t", "a": "auto", "c": "decltype(auto)"}

OPERATORS = {
    "nw": "new", "na": "new[]", "dl": "delete", "da": "delete[]", "ps": "+", "ng": "-", "ad": "&", "de": "*",
    "co": "~", "pl": "+", "mi": "-", "ml": "*", "dv": "/", "rm": "%", "an": "&", "or": "|", "eo": "^",
    "aS": "=", "pL": "+=", "mI": "-=", "mL": "*=", "dV": "/=", "rM": "%=", "aN": "&=", "oR": "|=", "eO": "^=",
    "ls": "<<", "rs": ">>", "lS": "<<=", "rS": ">>=", "eq": "==", "ne": "!=", "lt": "<", "gt": ">", "le": "<=",
    "ge": ">=", "nt": "!", "aa": "&&", "oo": "||", "pp": "++", "mm": "--", "cm": ",", "pm": "->*", "pt": "->",
    "cl": "()", "ix": "[]",
}
STD_ABBR = {"t": ("std",), "a": ("std", "allocator"), "b": ("std", "basic_string"),
            "s": ("std", "string"), "i": ("std", "istream"), "o": ("std", "ostream"), "d": ("std", "iostream")}


# -- AST ---------------------------------------------------------------------------------------------------
@dataclass(frozen=True)
class Part:
    """One component of a qualified name. kind: name | op | cv (conversion) | ctor | dtor."""
    name: str
    kind: str = "name"
    targs: tuple = None          # template args (Type | Lit), or None
    conv: object = None          # conversion operator type


@dataclass(frozen=True)
class Name:
    parts: tuple                 # tuple[Part]

    def scope(self):
        return Name(self.parts[:-1])

    @property
    def last(self):
        return self.parts[-1]


@dataclass(frozen=True)
class Builtin:
    name: str


@dataclass(frozen=True)
class Named:
    name: Name


@dataclass(frozen=True)
class TParam:
    index: int


@dataclass(frozen=True)
class Ptr:
    inner: object


@dataclass(frozen=True)
class LRef:
    inner: object


@dataclass(frozen=True)
class RRef:
    inner: object


@dataclass(frozen=True)
class CV:
    inner: object
    const: bool = False
    volatile: bool = False


@dataclass(frozen=True)
class Func:
    ret: object
    params: tuple
    variadic: bool = False
    const: bool = False


@dataclass(frozen=True)
class MemPtr:
    cls: object
    inner: object


@dataclass(frozen=True)
class Array:
    size: int
    inner: object


@dataclass(frozen=True)
class Lit:
    type: object
    value: int


@dataclass
class Symbol:
    """A demangled symbol. kind: function | data | vtable | typeinfo | typeinfo_name | guard | thunk | local."""
    kind: str
    name: Name = None
    params: tuple = None         # None for data
    variadic: bool = False
    const: bool = False          # const method
    ret: object = None           # functions with template args at the end of the name
    targs: tuple = None          # function template args (for T_)
    type: object = None          # vtable/typeinfo: the class type
    thunk_offset: int = None
    target: "Symbol" = None      # thunk: the target encoding
    extra: dict = field(default_factory=dict)


# -- parser ------------------------------------------------------------------------------------------------
class _P:
    def __init__(self, s):
        self.s, self.i = s, 0
        self.subs = []
        self.targs = None        # current template args for T_

    def peek(self, n=1):
        return self.s[self.i:self.i + n]

    def eat(self, t):
        if self.s.startswith(t, self.i):
            self.i += len(t)
            return True
        return False

    def need(self, t):
        if not self.eat(t):
            raise MangleError(f"expected {t!r} at {self.i} in {self.s}")

    def number(self):
        neg = self.eat("n")
        j = self.i
        while self.i < len(self.s) and self.s[self.i].isdigit():
            self.i += 1
        if j == self.i:
            raise MangleError(f"number expected at {j} in {self.s}")
        v = int(self.s[j:self.i])
        return -v if neg else v

    def seq_id(self):
        if self.eat("_"):
            return 0
        j = self.i
        while self.s[self.i] != "_":
            self.i += 1
        v = 0
        for ch in self.s[j:self.i]:
            v = v * 36 + (int(ch) if ch.isdigit() else ord(ch) - ord("A") + 10)
        self.need("_")
        return v + 1

    # names
    def source_name(self):
        n = self.number()
        t = self.s[self.i:self.i + n]
        self.i += n
        return t

    def unqualified(self):
        c = self.peek()
        if c.isdigit():
            return Part(self.source_name())
        if c == "L":
            self.i += 1
            return Part(self.source_name())
        if c == "C" and self.peek(2)[1:] in "123":
            self.i += 2
            return Part("", "ctor")
        if c == "D" and self.peek(2)[1:] in "012":
            self.i += 2
            return Part("", "dtor")
        two = self.peek(2)
        if two == "cv":
            self.i += 2
            saved = self.targs
            t = self.type()
            self.targs = saved
            return Part("", "cv", conv=t)
        if two in OPERATORS:
            self.i += 2
            return Part(OPERATORS[two], "op")
        raise MangleError(f"unqualified name at {self.i} in {self.s}")

    def template_args(self):
        self.need("I")
        out = []
        while not self.eat("E"):
            out.append(self.template_arg())
        return tuple(out)

    def template_arg(self):
        if self.peek() == "L":
            return self.expr_primary()
        if self.peek() == "X":
            raise MangleError(f"expression template arg in {self.s}")
        if self.peek() == "J":
            raise MangleError(f"pack template arg in {self.s}")
        return self.type()

    def expr_primary(self):
        self.need("L")
        if self.peek() == "_" and self.peek(2) == "_Z":
            raise MangleError(f"symbol template arg in {self.s}")
        t = self.type()
        v = self.number()
        self.need("E")
        return Lit(t, v)

    def name(self):
        """-> (Name, const_method). Adds substitutions as Itanium does."""
        c = self.peek()
        if c == "N":
            return self.nested()
        if c == "Z":
            raise MangleError(f"local name in {self.s}")
        if self.peek(2) == "St":
            self.i += 2
            part = self.unqualified()
            parts = (Part("std"), part)
        elif c == "S":
            sub = self.substitution()
            if not isinstance(sub, Name):
                raise MangleError(f"type substitution used as a name in {self.s}")
            if self.peek() != "I":
                raise MangleError(f"bare substitution as a name in {self.s}")
            last = sub.parts[-1]
            ta = self.template_args()
            n = Name(sub.parts[:-1] + (Part(last.name, last.kind, ta, last.conv),))
            self.subs.append(n)
            return n, False
        else:
            parts = (self.unqualified(),)
        if self.peek() == "I":
            self.subs.append(Named(Name(parts)))
            ta = self.template_args()
            last = parts[-1]
            parts = parts[:-1] + (Part(last.name, last.kind, ta, last.conv),)
        return Name(parts), False

    def nested(self):
        self.need("N")
        const = False
        while self.peek() in ("r", "V", "K"):
            if self.peek() == "K":
                const = True
            self.i += 1
        if self.peek() in ("R", "O"):
            raise MangleError(f"ref-qualified method in {self.s}")
        parts = ()
        while not self.eat("E"):
            c = self.peek()
            if c == "S" and self.peek(2) == "St":
                self.i += 2
                parts = (Part("std"),)
                continue
            if c == "S":
                sub = self.substitution()
                if not isinstance(sub, (Name, Named)):
                    raise MangleError(f"dependent prefix in {self.s}")
                parts = sub.parts if isinstance(sub, Name) else sub.name.parts
                continue
            if c == "I":
                ta = self.template_args()
                last = parts[-1]
                parts = parts[:-1] + (Part(last.name, last.kind, ta, last.conv),)
                if self.peek() != "E":
                    self.subs.append(Name(parts))
                continue
            if c == "T":
                raise MangleError(f"template param prefix in {self.s}")
            parts = parts + (self.unqualified(),)
            if self.peek() != "E":
                self.subs.append(Name(parts))
        return Name(parts), const

    def substitution(self):
        self.need("S")
        c = self.peek()
        if c in STD_ABBR and c != "t":
            self.i += 1
            return Name(tuple(Part(x) for x in STD_ABBR[c]))
        k = self.seq_id()
        if k >= len(self.subs):
            raise MangleError(f"substitution {k} out of range in {self.s}")
        return self.subs[k]

    # types
    def type(self):
        c = self.peek()
        if c in BUILTINS:
            self.i += 1
            return Builtin(BUILTINS[c])
        if c == "D":
            d = self.peek(2)[1:]
            if d in DBUILTINS:
                self.i += 2
                return Builtin(DBUILTINS[d])
            raise MangleError(f"D type at {self.i} in {self.s}")
        if c in "rVK":
            const = volatile = False
            while self.peek() in ("r", "V", "K"):
                ch = self.s[self.i]
                self.i += 1
                if ch == "K":
                    const = True
                elif ch == "V":
                    volatile = True
            inner = self.type()
            if isinstance(inner, Func):
                t = Func(inner.ret, inner.params, inner.variadic, const)
            else:
                t = CV(inner, const, volatile)
            self.subs.append(t)
            return t
        if c in "PROC":
            if c == "C":
                raise MangleError(f"complex type in {self.s}")
            self.i += 1
            inner = self.type()
            t = {"P": Ptr, "R": LRef, "O": RRef}[c](inner)
            self.subs.append(t)
            return t
        if c == "F":
            self.i += 1
            self.eat("Y")
            ret = self.type()
            params, variadic = self.bare_params(end="E")
            t = Func(ret, params, variadic)
            self.subs.append(t)
            return t
        if c == "A":
            self.i += 1
            n = self.number() if self.peek().isdigit() else None
            if n is None:
                raise MangleError(f"array without size in {self.s}")
            self.need("_")
            t = Array(n, self.type())
            self.subs.append(t)
            return t
        if c == "M":
            self.i += 1
            cls = self.type()
            inner = self.type()
            t = MemPtr(cls, inner)
            self.subs.append(t)
            return t
        if c == "T":
            self.i += 1
            k = 0 if self.eat("_") else self.number() + 1
            if not self.eat("_") and k:
                raise MangleError(f"template param in {self.s}")
            t = TParam(k)
            self.subs.append(t)
            if self.peek() == "I":
                raise MangleError(f"template template param in {self.s}")
            return t
        if c == "S":
            if self.peek(2) == "St":
                n, _ = self.name()
                t = Named(n)
                self.subs.append(t)
                return t
            sub = self.substitution()
            if self.peek() == "I":
                base = sub.name if isinstance(sub, Named) else sub
                if not isinstance(base, Name):
                    raise MangleError(f"template args after a type substitution in {self.s}")
                ta = self.template_args()
                last = base.parts[-1]
                t = Named(Name(base.parts[:-1] + (Part(last.name, last.kind, ta, last.conv),)))
                self.subs.append(t)
                return t
            return Named(sub) if isinstance(sub, Name) else sub
        if c == "U":
            raise MangleError(f"vendor qualifier in {self.s}")
        if c.isdigit() or c == "N" or c == "Z":
            n, _ = self.name()
            t = Named(n)
            self.subs.append(t)
            return t
        raise MangleError(f"type at {self.i} in {self.s}")

    def bare_params(self, end=None):
        params, variadic = [], False
        while self.i < len(self.s) and (end is None or self.peek() != end):
            if self.peek() == "." or self.peek() == "E" and end is None:
                break
            t = self.type()
            if t == Builtin("..."):
                variadic = True
            else:
                params.append(t)
        if end:
            self.need(end)
        if params == [Builtin("void")]:
            params = []
        return tuple(params), variadic

    def encoding(self):
        """-> Symbol(kind function|data)."""
        n, const = self.name()
        if self.i >= len(self.s) or self.peek() in ("E", "."):
            return Symbol("data", name=n)
        ret = None
        targs = n.last.targs if n.last.kind not in ("ctor", "dtor", "cv") else None
        if targs is not None:
            self.targs = targs
            ret = self.type()
        params, variadic = self.bare_params()
        return Symbol("function", name=n, params=params, variadic=variadic, const=const, ret=ret, targs=targs)


def parse(mangled):
    """-> Symbol. Raises MangleError for unsupported forms."""
    if not mangled.startswith("_Z"):
        raise MangleError(f"not an Itanium name: {mangled}")
    p = _P(mangled)
    p.i = 2
    if p.eat("TV"):
        return Symbol("vtable", type=p.type())
    if p.eat("TI"):
        return Symbol("typeinfo", type=p.type())
    if p.eat("TS"):
        return Symbol("typeinfo_name", type=p.type())
    if p.eat("GV"):
        n, _ = p.name()
        return Symbol("guard", name=n)
    if p.eat("Th"):
        off = p.number()
        p.need("_")
        tgt = p.encoding()
        return Symbol("thunk", name=tgt.name, params=tgt.params, variadic=tgt.variadic, const=tgt.const,
                      ret=tgt.ret, targs=tgt.targs, thunk_offset=off, target=tgt)
    if p.peek() == "T":
        raise MangleError(f"special name in {mangled}")
    sym = p.encoding()
    if p.i != len(mangled) and not mangled[p.i:].startswith("."):
        raise MangleError(f"trailing text at {p.i} in {mangled}")
    return sym


# -- spelling ----------------------------------------------------------------------------------------------
STD_STRING = Name((Part("std"), Part("__1"), Part("basic_string", targs=(
    Builtin("char"),
    Named(Name((Part("std"), Part("__1"), Part("char_traits", targs=(Builtin("char"),))))),
    Named(Name((Part("std"), Part("__1"), Part("allocator", targs=(Builtin("char"),))))),
))))


class Speller:
    """C++ spelling. `tparams` names TParam(k) (default T<k>); `rename(Name) -> str|None` overrides a name."""

    def __init__(self, tparams=None, rename=None, lit=None):
        self.tparams = tparams
        self.rename = rename
        self.lit_hook = lit

    def name(self, n):
        if self.rename:
            r = self.rename(n)
            if r is not None:
                return r
        if n == STD_STRING:
            return "std::string"
        out = []
        parts = n.parts
        if len(parts) >= 2 and parts[0].name == "std" and parts[1].name == "__1":
            parts = (parts[0],) + parts[2:]
        for i, p in enumerate(parts):
            out.append(self.part(p, parts[i - 1].name if i else None))
        return "::".join(out)

    def part(self, p, prev):
        if p.kind == "ctor":
            base = prev
        elif p.kind == "dtor":
            base = "~" + prev
        elif p.kind == "op":
            base = "operator" + (" " if p.name[0].isalpha() else "") + p.name
        elif p.kind == "cv":
            base = "operator " + self.type(p.conv)
        else:
            base = p.name
        if p.targs is not None:
            if base.endswith("<"):
                base += " "      # `operator<< <int>`: the operator's `<` is not a bracket
            base += self.targs(p.targs)
        return base

    def targs(self, ta):
        inner = ", ".join(self.targ(a) for a in ta)
        if inner.endswith(">"):
            inner += " "
        return f"<{inner}>"

    def targ(self, a):
        if isinstance(a, Lit):
            return self.lit(a)
        return self.type(a)

    def lit(self, a):
        if self.lit_hook:
            r = self.lit_hook(a)
            if r is not None:
                return r
        t = a.type
        if t == Builtin("bool"):
            return "true" if a.value else "false"
        if isinstance(t, Builtin):
            suffix = {"unsigned int": "u", "long": "l", "unsigned long": "ul", "long long": "ll",
                      "unsigned long long": "ull"}.get(t.name, "")
            if t.name in ("char", "signed char", "unsigned char", "short", "unsigned short"):
                return f"({t.name}){a.value}"
            return f"{a.value}{suffix}"
        return f"({self.type(t)}){a.value}"

    def type(self, t, d=""):
        """Declaration of `d` (a declarator, may be empty) with type `t`."""
        if isinstance(t, Builtin):
            return t.name + (" " + d.lstrip() if d.strip() and d[0] not in "*&" else d)
        if isinstance(t, Named):
            return self._base(self.name(t.name), d)
        if isinstance(t, TParam):
            name = self.tparams[t.index] if self.tparams and t.index < len(self.tparams) else f"T{t.index}"
            return self._base(name, d)
        if isinstance(t, CV):
            q = (" const" if t.const else "") + (" volatile" if t.volatile else "")
            if isinstance(t.inner, (Ptr, LRef, RRef, MemPtr)):
                return self.type(t.inner, q + (d if not d or d[0] in "*&" else " " + d.lstrip()))
            return self._base(self.type(t.inner) + q, d)
        if isinstance(t, (Ptr, LRef, RRef)):
            sym = {Ptr: "*", LRef: "&", RRef: "&&"}[type(t)]
            inner = t.inner
            if isinstance(inner, (Func, Array)):
                return self.type(inner, "(" + sym + d.strip() + ")")
            return self.type(inner, sym + d)
        if isinstance(t, Func):
            params = self.params(t.params, t.variadic)
            tail = "(" + params + ")" + (" const" if t.const else "")
            dd = d.strip()
            return self.type(t.ret, " " + dd + tail if dd else tail)
        if isinstance(t, MemPtr):
            cls = self.type(t.cls)
            if isinstance(t.inner, Func):
                return self.type(t.inner, "(" + cls + "::*" + d.strip() + ")")
            return self.type(t.inner, " " + cls + "::*" + d.strip())
        if isinstance(t, Array):
            return self.type(t.inner, (d if d.startswith("(") else (" " + d.strip() if d.strip() else "")) +
                             f"[{t.size}]")
        raise MangleError(f"cannot spell {t!r}")

    @staticmethod
    def _base(base, d):
        if not d:
            return base
        if d[0] in "*&":
            return base + d
        if d[0] == " ":
            return base + d
        return base + " " + d

    def params(self, params, variadic=False, names=None):
        out = []
        for k, p in enumerate(params):
            n = names[k] if names else ""
            out.append(self.type(p, (" " + n) if n else ""))
        if variadic:
            out.append("...")
        return ", ".join(out)


def spell(t, **kw):
    return Speller(**kw).type(t)


# -- walking -----------------------------------------------------------------------------------------------
def names_in(obj, out=None, nested=True):
    """Every Name referenced by a type / template arg / name (as Named types), outermost first."""
    if out is None:
        out = []
    if isinstance(obj, Named):
        out.append(obj.name)
        for p in obj.name.parts:
            if p.targs:
                for a in p.targs:
                    names_in(a, out)
    elif isinstance(obj, Name):
        for p in obj.parts:
            if p.targs:
                for a in p.targs:
                    names_in(a, out)
            if p.conv is not None:
                names_in(p.conv, out)
    elif isinstance(obj, (Ptr, LRef, RRef, CV, Array)):
        names_in(obj.inner, out)
    elif isinstance(obj, Func):
        names_in(obj.ret, out)
        for p in obj.params:
            names_in(p, out)
    elif isinstance(obj, MemPtr):
        names_in(obj.cls, out)
        names_in(obj.inner, out)
    elif isinstance(obj, Lit):
        names_in(obj.type, out)
    return out


def substitute(obj, mapping):
    """Replace sub-trees equal to a key of `mapping` (type -> replacement)."""
    if obj in mapping:
        return mapping[obj]
    if isinstance(obj, Named):
        return Named(substitute(obj.name, mapping))
    if isinstance(obj, Name):
        return Name(tuple(Part(p.name, p.kind, tuple(substitute(a, mapping) for a in p.targs)
                               if p.targs is not None else None,
                               substitute(p.conv, mapping) if p.conv is not None else None) for p in obj.parts))
    if isinstance(obj, Ptr):
        return Ptr(substitute(obj.inner, mapping))
    if isinstance(obj, LRef):
        return LRef(substitute(obj.inner, mapping))
    if isinstance(obj, RRef):
        return RRef(substitute(obj.inner, mapping))
    if isinstance(obj, CV):
        return CV(substitute(obj.inner, mapping), obj.const, obj.volatile)
    if isinstance(obj, Array):
        return Array(obj.size, substitute(obj.inner, mapping))
    if isinstance(obj, Func):
        return Func(substitute(obj.ret, mapping), tuple(substitute(p, mapping) for p in obj.params),
                    obj.variadic, obj.const)
    if isinstance(obj, MemPtr):
        return MemPtr(substitute(obj.cls, mapping), substitute(obj.inner, mapping))
    if isinstance(obj, Lit):
        return Lit(substitute(obj.type, mapping), obj.value)
    return obj


def strip_targs(n):
    return Name(tuple(Part(p.name, p.kind, None, p.conv) for p in n.parts))


def has_tparam(obj):
    if isinstance(obj, TParam):
        return True
    if isinstance(obj, Named):
        return has_tparam(obj.name)
    if isinstance(obj, Name):
        return any(p.targs and any(has_tparam(a) for a in p.targs) for p in obj.parts)
    if isinstance(obj, (Ptr, LRef, RRef, CV, Array)):
        return has_tparam(obj.inner)
    if isinstance(obj, Func):
        return has_tparam(obj.ret) or any(has_tparam(p) for p in obj.params)
    if isinstance(obj, MemPtr):
        return has_tparam(obj.cls) or has_tparam(obj.inner)
    if isinstance(obj, Lit):
        return has_tparam(obj.type)
    return False
