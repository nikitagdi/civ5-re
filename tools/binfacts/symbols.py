import re
import subprocess

from binfacts.api import BinfactsError

SPECIAL_PREFIXES = (
    ("_ZTV", "vtable"), ("_ZTI", "typeinfo"), ("_ZTS", "typeinfo_name"), ("_ZTC", "ctor_vtable"),
    ("_ZGV", "guard"), ("_ZTh", "thunk"), ("_ZTv", "thunk"), ("_ZTc", "thunk"),
)
SPECIAL_TEXT = re.compile(
    r"^(?:vtable for |typeinfo for |typeinfo name for |construction vtable for |guard variable for |"
    r"non-virtual thunk to |virtual thunk to |covariant return thunk to )")
IDENT = re.compile(r"[A-Za-z0-9_$]")
OPERATOR_TOKENS = (
    "<=>", "->*", "<<=", ">>=", "()", "[]", "->", "<<", ">>", "<=", ">=", "==", "!=", "&&", "||", "++", "--",
    "+=", "-=", "*=", "/=", "%=", "&=", "|=", "^=", "+", "-", "*", "/", "%", "^", "&", "|", "~", "!", "=",
    "<", ">", ",")
OPERATOR_WORDS = ("new[]", "delete[]", "new", "delete", "co_await")
CLOSERS = {"<": ">", "(": ")", "[": "]", "{": "}"}
VARIANTS_C = ("C1", "C2", "C3")
VARIANTS_D = ("D0", "D1", "D2")


class Demangler:
    def __init__(self, exe):
        self.exe = str(exe)
        self.cache = {}
        self.version = None

    def get_version(self):
        if self.version is None:
            p = subprocess.run([self.exe, "--version"], capture_output=True, text=True, check=True)
            self.version = " ".join(p.stdout.split())
        return self.version

    def many(self, names):
        todo = sorted({n for n in names if n not in self.cache})
        if todo:
            p = subprocess.run([self.exe], input=("\n".join(todo) + "\n").encode("utf-8"),
                               capture_output=True, check=True)
            out = p.stdout.decode("utf-8", "replace").split("\n")
            if len(out) < len(todo):
                raise BinfactsError("llvm-cxxfilt returned fewer lines than it was given")
            for n, d in zip(todo, out):
                self.cache[n] = d.rstrip("\r")
        return [self.cache[n] for n in names]

    def one(self, name):
        return self.many([name])[0]


def _is_ident(c):
    return c != "" and IDENT.match(c) is not None


def skip_operator(s, i):
    """s[i:] starts with `operator`; returns the index after the operator token."""
    j = i + 8
    k = j
    while k < len(s) and s[k] == " ":
        k += 1
    for tok in OPERATOR_TOKENS:
        if s.startswith(tok, k):
            return k + len(tok)
    for w in OPERATOR_WORDS:
        if s.startswith(w, k) and not _is_ident(s[k + len(w):k + len(w) + 1]):
            return k + len(w)
    depth = 0
    while k < len(s):
        c = s[k]
        if c in "<[":
            depth += 1
        elif c in ">]":
            depth -= 1
        elif c == "(" and depth == 0:
            return k
        k += 1
    return k


def _operator_at(s, i):
    return s.startswith("operator", i) and (i == 0 or not _is_ident(s[i - 1])) \
        and not _is_ident(s[i + 8:i + 9])


def match(s, i):
    """s[i] opens a bracket; returns the index after its closer."""
    stack = [CLOSERS[s[i]]]
    i += 1
    n = len(s)
    while i < n and stack:
        c = s[i]
        if _operator_at(s, i):
            i = skip_operator(s, i)
            continue
        if c == "-" and s.startswith("->", i):
            i += 2
            continue
        if c == "'":
            j = s.find("'", i + 1)
            i = n if j < 0 else j + 1
            continue
        top = stack[-1]
        if c == top:
            stack.pop()
        elif c in "([{" or (c == "<" and top == ">"):
            stack.append(CLOSERS[c])
        i += 1
    if stack:
        raise ValueError("unbalanced brackets")
    return i


def strip_templates(comp):
    out = []
    i = 0
    n = len(comp)
    while i < n:
        if _operator_at(comp, i):
            j = skip_operator(comp, i)
            out.append(comp[i:j])
            i = j
        elif comp[i] == "<":
            i = match(comp, i)
        elif comp[i] == "'":
            j = comp.find("'", i + 1)
            j = n if j < 0 else j + 1
            out.append(comp[i:j])
            i = j
        else:
            out.append(comp[i])
            i += 1
    return "".join(out)


_QUALS = re.compile(r"(?: (?:const|volatile|&&|&))*")


def split_entity(s):
    """Splits a demangled entity into (components, params, cv). Params is None for non-functions."""
    n = len(s)
    i = 0
    start = 0
    comps = []
    while i < n:
        c = s[i]
        if _operator_at(s, i):
            i = skip_operator(s, i)
        elif s.startswith("(anonymous namespace)", i):
            i += 21
        elif c in "<[{":
            i = match(s, i)
        elif c == "'":
            j = s.find("'", i + 1)
            i = n if j < 0 else j + 1
        elif c == " ":
            comps = []
            start = i + 1
            i += 1
        elif s.startswith("::", i):
            comps.append(s[start:i])
            start = i + 2
            i += 2
        elif c == "(":
            j = match(s, i)
            q = _QUALS.match(s, j)
            k = q.end()
            if k == n:
                quals = q.group().split()
                cv = " ".join(x for x in quals if x in ("const", "volatile")) or None
                comps.append(s[start:i])
                return comps, s[i + 1:j - 1], cv
            i = k
        else:
            i += 1
    comps.append(s[start:])
    return comps, None, None


def _strip_suffix(text, special):
    if special == "ctor_vtable":
        text = text.split("-in-", 1)[0]
    return text


def _tok(s, i):
    """One mangled-name unit starting at s[i]; returns the next index."""
    n = len(s)
    c = s[i]
    if c.isdigit():
        j = i
        while j < n and s[j].isdigit():
            j += 1
        return j + int(s[i:j])
    if c == "S":
        if i + 1 < n and s[i + 1] in "tabsiod":
            return i + 2
        return s.index("_", i) + 1
    if c == "T":
        return s.index("_", i) + 1
    if c == "A":
        j = i + 1
        while j < n and s[j].isdigit():
            j += 1
        return j + 1 if j < n and s[j] == "_" else j
    if c == "L":
        if s.startswith("_Z", i + 1):
            return _group(s, i + 3)
        j = _tok(s, i + 1)
        while j < n and s[j] != "E":
            j += 1
        return j + 1
    if c in "NIFJXZ":
        return _group(s, i + 1)
    if c == "C":
        if i + 1 < n and s[i + 1].isdigit():
            return i + 2
        if s.startswith("CI", i) and i + 2 < n and s[i + 2] in "12":
            return i + 3
        return i + 1
    if c == "D":
        d = s[i + 1:i + 2]
        if d in ("T", "t"):
            return _group(s, i + 2)
        if d == "v":
            j = i + 2
            while j < n and s[j].isdigit():
                j += 1
            return j + 1
        return i + 2
    if c == "U":
        d = s[i + 1:i + 2]
        if d == "t":
            j = i + 2
            while j < n and s[j].isdigit():
                j += 1
            return j + 1
        if d == "l":
            j = _group(s, i + 2)
            while j < n and s[j].isdigit():
                j += 1
            return j + 1
        return i + 1
    return i + 1


def _group(s, i):
    """Scans units from i until the closing E; returns the index after it."""
    n = len(s)
    while i < n and s[i] != "E":
        i = _tok(s, i)
    return i + 1


def find_variant(m):
    """Returns (variant, index of its first character) for a ctor/dtor mangled name, else None."""
    if not m.startswith("_Z") or len(m) < 4:
        return None
    n = len(m)
    try:
        i = 2
        if m[i] == "Z":
            i = _tok(m, i)
        if i >= n or m[i] != "N":
            return None
        i += 1
        while i < n and m[i] in "rVKRO":
            i += 1
        while i < n and m[i] != "E":
            if m[i] in "CD" and i + 2 < n and m[i + 1].isdigit() and m[i + 2] in "EI":
                return m[i:i + 2], i
            if m[i] == "L" and i + 1 < n and m[i + 1].isdigit():
                i += 1
                continue
            i = _tok(m, i)
    except (ValueError, IndexError):
        return None
    return None


class Parsed:
    __slots__ = ("mangled", "demangled", "kind", "variant", "variant_pos", "scope", "name", "params", "cv",
                 "key", "entity")


def classify(mangled, demangled):
    p = Parsed()
    p.mangled, p.demangled = mangled, demangled
    p.variant = p.variant_pos = p.params = p.cv = None
    if not mangled.startswith("_Z"):
        p.kind, p.scope, p.name, p.key, p.entity = "c", "", mangled, mangled, mangled
        return p
    special = None
    for prefix, kind in SPECIAL_PREFIXES:
        if mangled.startswith(prefix):
            special = kind
            break
    if mangled.startswith(("_ZTT", "_ZGR", "_ZGT", "_ZTW", "_ZTH")):
        raise ValueError("unsupported special name")
    text = demangled
    if special:
        m = SPECIAL_TEXT.match(text)
        if not m:
            raise ValueError("special name without its demangled prefix")
        text = _strip_suffix(text[m.end():], special)
    comps, params, cv = split_entity(text)
    if not comps or not comps[-1]:
        raise ValueError("empty name")
    p.entity = text
    p.name = comps[-1]
    p.scope = "::".join(comps[:-1])
    p.key = "::".join(strip_templates(c) for c in comps)
    p.params, p.cv = params, cv
    if special and special != "thunk":
        p.kind = special
        p.params = p.cv = None
        return p
    if special == "thunk":
        p.kind = "thunk"
        return p
    v = find_variant(mangled)
    if v and params is not None:
        p.variant, p.variant_pos = v
        if p.variant not in VARIANTS_C + VARIANTS_D:
            raise ValueError(f"unsupported variant {p.variant}")
        base = strip_templates(comps[-2]) if len(comps) > 1 else ""
        want = base if p.variant[0] == "C" else "~" + base
        if strip_templates(p.name) != want:
            raise ValueError(f"{p.variant} name {p.name!r} does not match scope")
        p.kind = "ctor" if p.variant[0] == "C" else "dtor"
        return p
    if params is None:
        p.kind = "data"
        return p
    p.kind = "function"
    return p


def parse_all(mangled_names, demangled_names):
    parsed = []
    errors = []
    for m, d in zip(mangled_names, demangled_names):
        try:
            parsed.append(classify(m, d))
        except ValueError as e:
            errors.append(f"{m}: {e}: {d}")
            parsed.append(None)
    if errors:
        raise BinfactsError(f"{len(errors)} symbols could not be parsed:\n" + "\n".join(errors[:50]))
    return parsed


def entity_key(p):
    """Mangled name with the ctor/dtor variant replaced by a wildcard."""
    i = p.variant_pos
    return p.mangled[:i] + p.variant[0] + "*" + p.mangled[i + 2:]
