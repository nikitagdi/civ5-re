"""Step 6 of the seed pipeline: `///` markers for the SDK copies, from typedb (markers="off") and binfacts.

mark() takes the unmarked texts {dest: text}, the typedb database built on them and binfacts, and returns the
marked texts. Only facts of the two sources are written; anything unknown becomes `?`.
"""
import re
from collections import defaultdict

from . import luaentry

ROOT = "Civ5Expansion2/"
UNVERIFIED_SHIMS = {"CRITICAL_SECTION", "_RTL_CRITICAL_SECTION"}
BUILTIN_WORDS = {
    "unsigned", "signed", "int", "char", "short", "long", "bool", "float", "double", "void", "wchar_t",
    "const", "volatile", "__int64", "u", "l", "ul", "ll", "ull",
}
ORDER = {"size": 0, "vtable": 1, "addr": 2, "base": 3, "deleting": 4, "thunk": 5, "inline": 2, "pure": 2,
         "absent": 2, "lua": 6}


class MarkError(Exception):
    pass


def strip_args(text):
    out, depth = [], 0
    for ch in text:
        if ch == "<":
            depth += 1
        elif ch == ">":
            depth -= 1
        elif depth == 0:
            out.append(ch)
    return "".join(out)


def split_top(text):
    parts, depth, cur = [], 0, []
    for ch in text:
        if ch in "<([":
            depth += 1
        elif ch in ">)]":
            depth -= 1
        if ch == "," and depth == 0:
            parts.append("".join(cur).strip())
            cur = []
        else:
            cur.append(ch)
    tail = "".join(cur).strip()
    if tail or parts:
        parts.append(tail)
    return parts


def nparams(params):
    return len(split_top(params)) if params else 0


def dest_of(path):
    if not path.startswith(ROOT):
        raise MarkError(f"path outside the tree: {path}")
    return path[len(ROOT):]


# facts

class Facts:
    def __init__(self, bf):
        self.sym = {}
        self.scoped = defaultdict(list)    # strip_args(scope) -> firaxis symbols whose scope is a template-id
        self.vtables = defaultdict(list)   # strip_args(class) -> [(class, addr)]
        for s in bf["symbols"]:
            if s.get("section") == "*ABS*":
                continue
            self.sym.setdefault(s["mangled"], s)
            scope = s.get("scope") or ""
            if s.get("group") == "firaxis" and "<" in scope:
                self.scoped[strip_args(scope)].append(s)
        for v in bf.get("vtables", ()):
            if not v.get("construction"):
                self.vtables[strip_args(v["class"])].append((v["class"], v["addr"]))
        self.lua = luaentry.Index(bf)

    def addr(self, mangled):
        s = self.sym.get(mangled) if mangled else None
        return s["addr"] if s else None


# record helpers

def _identifiers(type_text):
    return re.findall(r"[A-Za-z_][\w]*(?:::[A-Za-z_]\w*)*", type_text or "")


class Model:
    """Indexes over the typedb database that the marker rules need."""

    def __init__(self, db, texts, layouts=None):
        self.db = db
        self.texts = texts
        self.layouts = layouts or {}
        self.lines_cache = {}
        self.records = {r["name"]: r for r in db["records"]}
        self.tree_records = [r for r in db["records"] if r["origin"] == "tree" and r["complete"]]
        self.known_types = {r["name"] for r in db["records"]}
        self.known_types |= {e["qualified"] for e in db["enums"]} | {t["qualified"] for t in db["typedefs"]}
        self.instances = defaultdict(list)
        for r in db["records"]:
            if r["origin"] == "tree" and r["complete"] and r["size"] is not None and "<" in r["name"]:
                self.instances[strip_args(r["name"])].append(r)
        self.thunks = defaultdict(list)
        for r in self.tree_records:
            for g in (r.get("vtable") or {}).get("groups", ()):
                for slot in g["slots"]:
                    if slot.get("thunk") and slot.get("function"):
                        self.thunks[slot["function"]].append(slot["symbol"])
        self.shim_dependent = self._shim_dependent()

    def lines(self, path):
        lines = self.lines_cache.get(path)
        if lines is None:
            lines = self.lines_cache[path] = self.texts[dest_of(path)].split("\n")
        return lines

    def source_line(self, path, line):
        return self.lines(path)[line - 1]

    def is_implicit_instance(self, r):
        """A template-id record that is not written as `template <> ...` is a compiler instantiation."""
        t = r["template"]
        if not (t and t["args"] is not None):
            return False
        return re.match(r"\s*template\s*<\s*>", self.source_line(r["file"], r["line"])) is None

    def is_pattern(self, r):
        t = r["template"]
        return bool(t and t["args"] is None)

    def _shim_dependent(self):
        deps = set()
        changed = True
        while changed:
            changed = False
            for r in self.tree_records:
                if r["name"] in deps:
                    continue
                names = [b["name"] for b in r["bases"]]
                for f in r["fields"]:
                    names += _identifiers(f["type"])
                if any(n in UNVERIFIED_SHIMS or n in deps for n in names):
                    deps.add(r["name"])
                    changed = True
        return deps

    def spec_resolvable(self, spec, std_ok=False):
        """Every name in the SPEC is a type the tree declares (`std_ok`: or a standard library name, which the
        pch makes visible but typedb lists only when a field reaches it)."""
        for tok in _identifiers(spec):
            if tok in BUILTIN_WORDS or tok.isdigit():
                continue
            if tok in self.known_types or (std_ok and tok.startswith("std::")):
                continue
            return False
        return True

    def layout_offset(self, r, f):
        """Offset from a dummy instantiation (clang record-layout dump); anonymous members count from the parent."""
        name = r["name"]
        if "::(anon " in name:
            parent = self.layouts.get(name.rpartition("::")[0])
            if parent and f["name"] in parent:
                return parent[f["name"]]
        block = self.layouts.get(name)
        return block.get(f["name"]) if block else None

    def field_offset_known(self, r):
        return all(f["offset"] is not None or not f["name"] or self.field_offset(r, f) is not None
                   for f in r["fields"])

    def field_offset(self, r, f):
        off = f["offset"]
        if off is None:
            for inst in sorted(self.instances.get(r["name"], ()), key=lambda i: i["name"]):
                for g in inst["fields"]:
                    if g["name"] == f["name"] and g["line"] == f["line"] and g["offset"] is not None:
                        return self.field_offset(inst, g)
            return self.layout_offset(r, f)
        acc, seen = r.get("accessed_from"), 0
        while acc and seen < 8:
            off += acc["offset"]
            r = self.records.get(acc["record"])
            acc = r.get("accessed_from") if r else None
            seen += 1
        return off


def decl_end_line(lines, start):
    """1-based line holding the `;` that ends the declaration starting on line `start`."""
    depth = 0
    state = None
    for n in range(start - 1, len(lines)):
        line = lines[n]
        i = 0
        while i < len(line):
            c, two = line[i], line[i:i + 2]
            if state is None:
                if two == "//":
                    break
                if two == "/*":
                    state = "block"
                    i += 2
                    continue
                if c in "\"'":
                    state = c
                elif c in "{([":
                    depth += 1
                elif c in "})]":
                    depth -= 1
                elif c == ";" and depth <= 0:
                    return n + 1
            elif state == "block":
                if two == "*/":
                    state = None
                    i += 2
                    continue
            else:
                if c == "\\":
                    i += 2
                    continue
                if c == state:
                    state = None
            i += 1
        if state in ("\"", "'"):
            state = None
    return start


# edits

class Edits:
    def __init__(self):
        self.above = defaultdict(dict)    # dest -> line -> {text: order}
        self.trailing = defaultdict(dict)  # dest -> line -> text
        self.counts = defaultdict(int)

    def add(self, dest, line, kind, text):
        slot = self.above[dest].setdefault(line, {})
        if text not in slot:
            slot[text] = ORDER[kind]
            self.counts[kind] += 1

    def field(self, dest, line, text):
        self.trailing[dest][line] = text
        self.counts["offset"] += 1


def addr_line(kind, addr, spec=None):
    head = {"addr": "", "base": "base ", "deleting": "deleting ", "thunk": "thunk "}[kind]
    return f"/// {head}{addr}" + (f" {spec}" if spec else "")


def structor_lines(c1, c2, d0, spec=None):
    """Main, base and deleting lines of a constructor or destructor from the symbol addresses."""
    out = []
    if c1:
        out.append(("addr", addr_line("addr", c1, spec)))
        if c2 and c2 != c1:
            out.append(("base", addr_line("base", c2, spec)))
    elif c2:
        out.append(("base", addr_line("base", c2, spec)))
    if d0:
        out.append(("deleting", addr_line("deleting", d0, spec)))
    return out


def no_code_kind(f):
    if f["pure"] and not f["has_body"]:
        return "pure"
    return "inline" if f["has_body"] else "absent"


class Marker:
    def __init__(self, db, bf, origins, texts, layouts=None):
        self.db = db
        self.texts = texts
        self.m = Model(db, texts, layouts)
        self.facts = Facts(bf)
        self.origins = origins
        self.edits = Edits()
        self.report = defaultdict(list)
        self.lua_marked = set()
        self.pattern_funcs = defaultdict(list)
        for f in db["functions"]:
            if f["scope_kind"] == "class" and f["mangled"] is None:
                self.pattern_funcs[f["scope"]].append(f)

    # -- records and fields -------------------------------------------------------------------
    def records(self):
        for r in self.m.tree_records:
            if self.m.is_implicit_instance(r):
                continue
            dest = dest_of(r["file"])
            line = r["line"]
            self.edits.add(dest, line, "size", f"/// size {self.size_text(r)}")
            self.vtable_lines(r, dest, line)
            for f in r["fields"]:
                if not f["name"]:
                    continue
                off = self.m.field_offset(r, f)
                if off is None:
                    self.report["field_offset_unknown"].append(f"{dest}:{f['line']} {r['name']}::{f['name']}")
                    continue
                end = decl_end_line(self.m.lines(r["file"]), f["line"])
                self.edits.field(dest, end, f"///< +0x{off:x}")

    def size_text(self, r):
        if self.m.is_pattern(r) or r["size"] is None:
            return "?"
        if r["name"] in self.m.shim_dependent:
            self.report["size_unverified"].append(r["name"])
            return "?"
        return f"0x{r['size']:x}"

    def vtable_lines(self, r, dest, line):
        if self.m.is_pattern(r):
            if r["dynamic"] is not True:
                return
            insts = [(c, a) for c, a in self.facts.vtables.get(r["name"], ()) if self.m.spec_resolvable(c)]
            if not insts:
                self.edits.add(dest, line, "vtable", "/// vtable ?")
                self.report["vtable_unknown"].append(r["name"])
            for c, a in sorted(insts):
                self.edits.add(dest, line, "vtable", f"/// vtable {a} {c}")
            return
        if not r["dynamic"]:
            return
        a = self.facts.addr(r.get("vtable_symbol"))
        if a:
            self.edits.add(dest, line, "vtable", f"/// vtable {a}")
        else:
            self.edits.add(dest, line, "vtable", "/// vtable ?")
            self.report["vtable_unknown"].append(r["name"])

    # -- functions ----------------------------------------------------------------------------
    def is_unsupported(self, f):
        if f["scope_kind"] != "class" or f["mangled"] is not None:
            return False
        scope = self.m.records.get(f["scope"])
        if scope is None or not (scope["template"] or "<" in f["scope"]):
            return False
        return self.m.source_line(f["file"], f["line"]).lstrip().startswith("template")

    def function_main(self, f, symbols):
        """[(kind, line)] for one function; `symbols` = (c1, c2, d0) addresses or a single address in c1."""
        c1, c2, d0 = symbols
        if f["kind"] in ("ctor", "dtor"):
            lines = structor_lines(c1, c2, d0)
            if lines:
                return lines
        elif c1:
            return [("addr", addr_line("addr", c1))]
        kind = no_code_kind(f)
        return [(kind, f"/// {kind}")]

    def functions(self):
        for f in self.db["functions"]:
            if not f["file"].startswith(ROOT):
                continue
            dest = dest_of(f["file"])
            if self.is_unsupported(f):
                self.report["unsupported"].append(f"{dest}:{f['line']} {f['qualified']}")
                continue
            if f["mangled"] is None and (f["scope_kind"] == "class" or "<" in (f["scope"] or "")):
                self.template_function(f, dest)
                continue
            self.plain_function(f, dest)

    def plain_function(self, f, dest):
        v = f["variants"] or {}
        c1 = self.facts.addr(f["mangled"])
        c2 = self.facts.addr(v.get("C2") or v.get("D2"))
        d0 = self.facts.addr(v.get("D0"))
        lines = self.function_main(f, (c1, c2, d0))
        for sym in self.m.thunks.get(f["key"], ()):
            a = self.facts.addr(sym)
            if a:
                lines.append(("thunk", addr_line("thunk", a)))
        self.emit(f, dest, lines)

    def template_function(self, f, dest):
        """Member of a class template or function template: SPEC lines per binary instantiation, else no-code kind."""
        scope = f["scope"]
        lines = []
        syms = self.facts.scoped.get(scope, ())
        groups = defaultdict(lambda: {})
        for s in syms:
            spec = s["scope"]
            if not spec.endswith(">") or not self.m.spec_resolvable(spec):
                continue
            if not self.matches(f, s):
                continue
            groups[spec][s.get("variant") or "-"] = s
        for spec in sorted(groups):
            g = groups[spec]
            if f["kind"] in ("ctor", "dtor"):
                keys = ("C1", "C2", "D0") if f["kind"] == "ctor" else ("D1", "D2", "D0")
                c1 = g.get(keys[0], {}).get("addr")
                c2 = g.get(keys[1], {}).get("addr")
                d0 = g.get("D0", {}).get("addr") if f["kind"] == "dtor" else None
                lines += structor_lines(c1, c2, d0, spec)
            else:
                a = g.get("-", {}).get("addr")
                if a:
                    lines.append(("addr", addr_line("addr", a, spec)))
        if not lines:
            kind = no_code_kind(f)
            lines = [(kind, f"/// {kind}")]
        self.emit(f, dest, lines)

    def matches(self, f, s):
        if f["kind"] == "ctor":
            if s["kind"] != "ctor":
                return False
        elif f["kind"] == "dtor":
            if s["kind"] != "dtor":
                return False
        elif s["kind"] not in ("function", "method") or s["name"] != f["name"]:
            return False
        if nparams(s.get("params")) != len(f["params"]):
            return False
        cands = [g for g in self.pattern_funcs[f["scope"]] if g["kind"] == f["kind"]
                 and (f["kind"] in ("ctor", "dtor") or g["name"] == f["name"])
                 and len(g["params"]) == len(f["params"]) and bool(g["const"]) == bool(f["const"])]
        if len(cands) != 1 or bool(s.get("cv") == "const") != bool(f["const"]):
            return False
        return True

    def emit(self, f, dest, lines):
        scope = strip_args(f["scope"]) if f["scope"] else None
        if self.facts.lua.is_entry(f["mangled"], scope, f["name"]):
            lines.append(("lua", "/// lua ?"))
            self.lua_marked.add(f["mangled"] or f"{f['scope']}::{f['name']}")
        for kind, text in lines:
            self.edits.add(dest, f["line"], kind, text)
        main = [(k, t) for k, t in lines if k in ("addr", "base", "inline")]
        for r in f["redecls"]:
            for kind, text in main:
                self.edits.add(dest_of(r["file"]), r["line"], kind, text)

    # -- globals ------------------------------------------------------------------------------
    def globals(self):
        for g in self.db["globals"]:
            if not g["file"].startswith(ROOT):
                continue
            dest = dest_of(g["file"])
            if g["mangled"] is None:
                self.template_global(g, dest)
                continue
            a = self.facts.addr(g["mangled"])
            if a:
                self.edits.add(dest, g["line"], "addr", f"/// {a}")
                continue
            text = self.m.source_line(g["file"], g["line"])
            if "const" in g["type"] and "=" in text and "extern" not in text:
                self.edits.add(dest, g["line"], "inline", "/// inline")
            else:
                self.report["global_without_symbol"].append(f"{dest}:{g['line']} {g['qualified']}")

    def template_global(self, g, dest):
        """SPEC address lines of a static member of a class template: above its out-of-class definition (above the
        `template` line), or above the in-class declaration when there is none."""
        scope = g["qualified"].rpartition("::")[0]
        name = g["qualified"].rpartition("::")[2]
        places = [(dest_of(r["file"]), r["line"]) for r in g.get("redecls", ())] or [(dest, g["line"])]
        place = places[0]
        for s in sorted(self.facts.scoped.get(scope, ()), key=lambda s: s["addr"]):
            if s["kind"] != "data" or s["name"] != name:
                continue
            spec = s["scope"]
            if not spec.endswith(">"):
                continue
            if self.m.spec_resolvable(spec, std_ok=True):
                self.edits.add(place[0], place[1], "addr", f"/// {s['addr']} {spec}")
            else:
                self.report["template_global_unresolved"].append(f"{spec}::{name}")


# layout probes: dummy instantiations of class templates that have fields

def template_params(lines, start):
    """Parameters of the `template <...>` header that begins on line `start` (1-based), or None."""
    text = "\n".join(lines[start - 1:start + 40])
    m = re.match(r"\s*template\s*<", text)
    if not m:
        return None
    depth, i = 1, m.end()
    while i < len(text) and depth:
        if text[i] == "<":
            depth += 1
        elif text[i] == ">":
            depth -= 1
        i += 1
    if depth:
        return None
    return split_top(text[m.end():i - 1])


def dummy_args(params):
    """Arguments for the parameters without a default, or None when one cannot be faked."""
    out = []
    for p in params:
        if "=" in p:
            break
        p = " ".join(p.split())
        if re.match(r"(typename|class)\b", p):
            if "..." in p:
                break
            out.append("int")
        elif p.startswith("template"):
            return None
        else:
            m = re.match(r"(.*?)\s*\b\w+\s*$", p)
            typ = (m.group(1) if m else p).strip()
            if not typ:
                return None
            out.append("0" if "*" in typ else f"(({typ})1)")
    return out


def probe_source(model, texts):
    """[(expression, record)] that complete the patterns whose field offsets are unknown."""
    need = {}
    for r in model.tree_records:
        if not model.is_pattern(r):
            continue
        if not any(f["name"] and f["offset"] is None for f in r["fields"]):
            continue
        if model.field_offset_known(r):
            continue
        need[r["name"]] = r
    exprs, seen = [], set()
    for name, r in sorted(need.items()):
        parts = name.split("::")
        root = None
        for k in range(1, len(parts) + 1):
            cand = model.records.get("::".join(parts[:k]))
            if cand and model.is_pattern(cand) and cand["file"].startswith(ROOT):
                root, rest = cand, parts[k:]
                break
        if root is None:
            continue
        for k, p in enumerate(rest):
            if p.startswith("("):
                rest = rest[:k]
                break
        params = template_params(model.lines(root["file"]), root["line"])
        args = dummy_args(params) if params is not None else None
        if args is None:
            continue
        expr = f"{root['name']}<{', '.join(args)}>" + "".join(f"::{p}" for p in rest)
        if expr not in seen:
            seen.add(expr)
            exprs.append((expr, name))
    return exprs


def render_probe(exprs):
    lines = [f"static_assert(sizeof({e}) > 0, \"\");" for e in exprs]
    return "\n".join(lines) + "\n"


_DUMP_LINE = re.compile(r"^\s*(\d+)(?::\d+-\d+)?\s*\|(\s*)(.*?)\s*$")
_ANON = re.compile(r"\((?:anonymous|unnamed)(?: (?:struct|union|class))? at [^()]*?:(\d+):\d+\)")


def parse_layout_blocks(text):
    """[{raw: record name, fields: {field: byte offset}}] of a clang `-fdump-record-layouts-complete` dump.
    Members of nameless anonymous unions/structs are flattened into the enclosing block (offsets from the
    enclosing record, as in `///<`)."""
    blocks = []
    cur = None
    for line in text.split("\n"):
        if "*** Dumping AST Record Layout" in line:
            cur, anon_chain = None, []
            continue
        m = _DUMP_LINE.match(line)
        if not m:
            continue
        off, depth, body = int(m[1]), (len(m[2]) - 1) // 2, m[3]
        if cur is None:
            kind, _, rest = body.partition(" ")
            rest = _ANON.sub(lambda a: f"(anon {kind} L{a[1]})", rest)
            cur = {"raw": rest, "fields": {}}
            anon_chain = []
            blocks.append(cur)
            continue
        if body.endswith(("(base)", "vtable pointer)", "vbtable pointer)")):
            continue
        del anon_chain[depth - 1:]
        if len(anon_chain) < depth - 1:
            anon_chain += [False] * (depth - 1 - len(anon_chain))
        nameless = body.endswith(")") and "(anonymous" in body or body.endswith(")") and "(unnamed" in body
        if all(anon_chain[:depth - 1]):
            name = re.search(r"([A-Za-z_]\w*)$", body)
            if name and not nameless:
                cur["fields"].setdefault(name.group(1), off)
        anon_chain.append(nameless)
    return blocks


def parse_layout_dump(text):
    """{strip_args(record name): {field: byte offset}} for the template instantiations in a dump; the
    instantiation sorted first by name wins."""
    found = defaultdict(list)
    for b in parse_layout_blocks(text):
        if "<" in b["raw"]:
            found[strip_args(b["raw"])].append(b)
    return {key: sorted(blocks, key=lambda b: b["raw"])[0]["fields"] for key, blocks in found.items()}


# classes that typedb does not list (a primary template and its partial specializations share one record)

def scopes_at(text, line):
    """Heads (text before the `{`) of the scopes that enclose the start of 1-based `line`, outermost first."""
    stack, head = [], []
    state, cur, i, n = None, 1, 0, len(text)
    bol = True
    while i < n:
        if cur >= line:
            return [h.strip() for h in stack]
        c, two = text[i], text[i:i + 2]
        if c == "\n":
            cur += 1
            bol = True
            if state in ("line", "pp"):
                state = None
            i += 1
            continue
        if state == "line":
            i += 1
            continue
        if state == "block":
            if two == "*/":
                state = None
                i += 2
                continue
            i += 1
            continue
        if state in ("\"", "'"):
            if c == "\\":
                i += 2
                continue
            if c == state:
                state = None
            i += 1
            continue
        if state == "pp":
            if c == "\\" and text[i + 1:i + 2] == "\n":
                cur += 1
                i += 2
                continue
            i += 1
            continue
        if bol and c == "#":
            bol = False
            state = "pp"
            i += 1
            continue
        if not c.isspace():
            bol = False
        if two == "//":
            state = "line"
            i += 2
            continue
        if two == "/*":
            state = "block"
            i += 2
            continue
        if c in "\"'":
            state = c
            head.append(c)
        elif c == "{":
            stack.append("".join(head))
            head = []
        elif c == "}":
            if stack:
                stack.pop()
            head = []
        elif c == ";":
            head = []
        else:
            head.append(c)
        i += 1
    return [h.strip() for h in stack]


def _balanced(text, start):
    """Index just past the `>` that closes the `<` at text[start]."""
    depth = 0
    for i in range(start, len(text)):
        if text[i] == "<":
            depth += 1
        elif text[i] == ">":
            depth -= 1
            if depth == 0:
                return i + 1
    return -1


def parse_class_head(head):
    """{params, name, spec} of `[template <...>] class|struct Name[<spec>] ...`, or None."""
    head = " ".join(head.split())
    params = None
    m = re.match(r"template\s*<", head)
    if m:
        end = _balanced(head, m.end() - 1)
        if end < 0:
            return None
        params = split_top(head[m.end():end - 1])
        head = head[end:].lstrip()
    m = re.match(r"(?:class|struct|union)\s+([\w:]+)\s*", head)
    if not m:
        return None
    spec = None
    rest = head[m.end():]
    if rest.startswith("<"):
        end = _balanced(rest, 0)
        spec = rest[1:end - 1] if end > 0 else None
    return {"params": params, "name": m.group(1), "spec": spec}


def dummy_for(p):
    p = " ".join(p.split())
    p = p.split("=")[0].strip()
    if re.match(r"(typename|class)\b", p):
        return "int" if "..." not in p else None
    if p.startswith("template"):
        return None
    m = re.match(r"(.*?)\s*\b\w+\s*$", p)
    typ = (m.group(1) if m else p).strip()
    if not typ:
        return None
    return "0" if "*" in typ else f"(({typ})1)"


def class_expression(text, line):
    """Instantiation with dummy arguments of the template class that encloses `line`, or None."""
    namespaces, chain = [], []
    for head in scopes_at(text, line):
        m = re.match(r"(?:inline\s+)?namespace\s+([\w:]+)", head)
        if m:
            namespaces.append(m.group(1))
            continue
        c = parse_class_head(head)
        if c:
            chain.append(c)
    roots = [k for k, c in enumerate(chain) if c["params"] is not None]
    if not roots:
        return None
    root = chain[roots[0]]
    if any(c["params"] is not None for c in chain[roots[0] + 1:]):
        return None
    dummies = [dummy_for(p) for p in root["params"]]
    names = [re.search(r"(\w+)\s*(?:=.*)?$", p.split("=")[0].strip()) for p in root["params"]]
    if root["spec"] is not None:
        args = root["spec"]
        for nm, d in zip(names, dummies):
            if nm is None or d is None:
                return None
            args = re.sub(rf"\b{re.escape(nm.group(1))}\b", d, args)
    else:
        args = ", ".join(d for p, d in zip(root["params"], dummies) if "=" not in p and d is not None)
    scope = "::".join(namespaces)
    expr = (f"{scope}::" if scope else "") + f"{root['name']}<{args}>"
    return expr + "".join(f"::{c['name']}" for c in chain[roots[0] + 1:])


# applying

def line_comment_start(code):
    """Index of the `//` that starts a trailing comment on one line, or -1."""
    state = None
    i, n = 0, len(code)
    while i < n:
        c, two = code[i], code[i:i + 2]
        if state is None:
            if two == "//":
                return i
            if two == "/*":
                state = "block"
                i += 2
                continue
            if c == '"' or c == "'":
                state = c
        elif state == "block":
            if two == "*/":
                state = None
                i += 2
                continue
        else:
            if c == "\\":
                i += 2
                continue
            if c == state:
                state = None
        i += 1
    return -1


def put_trailing(line, marker):
    """([comment lines to put above], new line): `///<` goes at the end, an old trailing comment moves up."""
    indent = line[:len(line) - len(line.lstrip())]
    at = line_comment_start(line)
    if at < 0:
        return [], f"{line.rstrip()} {marker}"
    return [f"{indent}{line[at:].rstrip()}"], f"{line[:at].rstrip()} {marker}"


def append_trailing(text, by_line):
    lines = text.split("\n")
    out = []
    for n, line in enumerate(lines, 1):
        if n in by_line:
            before, line = put_trailing(line, by_line[n])
            out += before
        out.append(line)
    return "\n".join(out)


def apply_edits(text, dest, edits, origin):
    lines = text.split("\n")
    above = edits.above.get(dest, {})
    trailing = edits.trailing.get(dest, {})
    out = [f"/// origin {origin}"]
    for n, line in enumerate(lines, 1):
        if n == len(lines) and line == "":
            out.append(line)
            continue
        indent = line[:len(line) - len(line.lstrip())]
        if n in trailing:
            before, line = put_trailing(line, trailing[n])
            out += before
        if n in above:
            for t, _o in sorted(above[n].items(), key=lambda kv: (kv[1], kv[0])):
                out.append(f"{indent}{t}")
        out.append(line)
    return "\n".join(out)


# step 7: members that exist in Civ5XP but not in the SDK class

SEED_NOTE = "// seed: in Civ5XP, not in SDK"
STD_NAMES = (
    (re.compile(r"std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char>\s*>"), "std::string"),
    (re.compile(r"std::__1::basic_string<wchar_t, std::__1::char_traits<wchar_t>, std::__1::allocator<wchar_t>\s*>"),
     "std::wstring"),
    (re.compile(r"std::__1::"), "std::"),
)
_FN_PTR = re.compile(r"\((?:[\w:<>, ]+::)?[*&]+\)")


def declared_symbols(db):
    out = set()
    for f in db["functions"]:
        out.add(f["mangled"])
        out.update((f["variants"] or {}).values())
        for i in f["instances"]:
            out.add(i["mangled"])
            out.update((i["variants"] or {}).values())
    for g in db["globals"]:
        out.add(g["mangled"])
    out.discard(None)
    return out


def std_spelling(text):
    for rx, repl in STD_NAMES:
        text = rx.sub(repl, text)
    return text


def param_decl(type_text, n):
    t = std_spelling(type_text)
    m = _FN_PTR.search(t)
    if m:
        return t[:m.end() - 1] + f"a{n}" + t[m.end() - 1:]
    return f"{t} a{n}"


def exe_only(db, bf, tree_records=None):
    """{record name: [entity]}: Civ5XP members of non-template SDK classes that the SDK does not declare.
    entity: {kind, name, params, cv, variants: {variant: addr}, size, static}."""
    declared = declared_symbols(db)
    recs = {r["name"]: r for r in (tree_records or db["records"])
            if r["origin"] == "tree" and r["complete"] and not r["template"]}
    groups = {}
    for s in bf["symbols"]:
        if s.get("group") != "firaxis" or s.get("kind") not in ("function", "method", "ctor", "dtor", "data"):
            continue
        scope = s.get("scope")
        if scope not in recs or s["mangled"] in declared or s.get("section") == "*ABS*":
            continue
        if "<" in (s.get("name") or ""):
            continue    # an instantiation of a function template, not a member
        key = (scope, s["kind"], s["demangled"]) if s["kind"] in ("ctor", "dtor") else (scope, s["kind"], s["mangled"])
        e = groups.setdefault(key, {"scope": scope, "kind": s["kind"], "name": s["name"], "params": s.get("params"),
                                    "cv": s.get("cv"), "variants": {}, "size": s["size"], "mangled": [],
                                    "lua": luaentry.classify(s) == "l"})
        e["variants"][s.get("variant") or "-"] = s["addr"]
        e["mangled"].append(s["mangled"])
    out = defaultdict(list)
    for e in sorted(groups.values(), key=lambda e: (min(e["variants"].values()), e["name"])):
        out[e["scope"]].append(e)
    return out


def entity_lines(e, class_name):
    """Marker lines and the declaration text for one exe-only member."""
    leaf = class_name.rpartition("::")[2]
    v = e["variants"]
    if e["kind"] == "data":
        lines = [f"/// {v['-']}", f"static UNK_DATA {e['name']}[0x{e['size']:x}];"]
        return lines
    if e["kind"] == "ctor":
        marks = [t for _k, t in structor_lines(v.get("C1"), v.get("C2"), None)]
        head = f"{leaf}"
        ret = ""
    elif e["kind"] == "dtor":
        marks = [t for _k, t in structor_lines(v.get("D1"), v.get("D2"), v.get("D0"))]
        head = f"~{leaf}"
        ret = "virtual " if v.get("D0") else ""
    else:
        marks = [f"/// {v['-']}"]
        head, ret = e["name"], "UNK_RET "
        if e["lua"]:
            marks.append("/// lua ?")
    params = split_top(e["params"]) if e["params"] else []
    if e["kind"] == "method" or e["kind"] == "function":
        if params == ["lua_State*"]:
            ret = "static int "
    decl_params = ", ".join(param_decl(p, i + 1) for i, p in enumerate(params))
    cv = " const" if e["cv"] == "const" else ""
    return marks + [f"{ret}{head}({decl_params}){cv};"]


def class_end(lines, record):
    """0-based index of the line that closes the class body, or None."""
    end = decl_end_line(lines, record["line"])
    i = end - 1
    return i if re.match(r"^\s*\};?\s*(//.*)?$", lines[i]) else None


def unknown_types(ents, known):
    """Qualified names in the parameter lists that nothing declares (forward declarations are added)."""
    out = set()
    for e in ents:
        for p in split_top(e["params"]) if e["params"] else []:
            for tok in _identifiers(std_spelling(p)):
                if tok in BUILTIN_WORDS or tok in known or tok.startswith("std::") or tok in ("lua_State", "size_t"):
                    continue
                out.add(tok)
    return out


def forward_decl(name):
    parts = name.split("::")
    return "".join(f"namespace {p} {{ " for p in parts[:-1]) + f"class {parts[-1]}; " + "}" * (len(parts) - 1)


def include_end(lines, first_decl):
    """0-based index after the last `#include` that precedes line `first_decl` (1-based)."""
    at = 0
    for i, line in enumerate(lines[:first_decl - 1]):
        if re.match(r"\s*#\s*include\b", line):
            at = i + 1
    return at


def append_exe_members(texts, db, bf, report):
    """Adds the exe-only members to the end of their SDK classes; returns the new texts."""
    members = exe_only(db, bf)
    recs = {r["name"]: r for r in db["records"] if r["origin"] == "tree" and r["complete"] and not r["template"]}
    known = {r["name"] for r in db["records"]} | {e["qualified"] for e in db["enums"]} \
        | {t["qualified"] for t in db["typedefs"]}
    edits = defaultdict(list)   # dest -> [(index, [lines])]
    for scope, ents in sorted(members.items()):
        r = recs[scope]
        dest = dest_of(r["file"])
        lines = texts[dest].split("\n")
        at = class_end(lines, r)
        if at is None:
            raise MarkError(f"{dest}:{r['line']}: cannot find the end of class {scope}")
        base = re.match(r"\s*", lines[r["line"] - 1]).group(0)
        indent = base + "\t"
        block = [f"{indent}{SEED_NOTE}", f"{base}public:"]
        for e in ents:
            block += [indent + x for x in entity_lines(e, scope)]
            report.setdefault("exe_only", []).append(f"{dest} {scope}::{e['name']}({e['params'] or ''})")
        edits[dest].append((at, block))
        for name in sorted(unknown_types(ents, known)):
            if any(name.startswith(k + "::") for k in recs):
                raise MarkError(f"{dest}: cannot forward-declare nested type {name}")
            first = min(x["line"] for x in db["records"] if x["origin"] == "tree" and x["file"] == r["file"])
            edits[dest].append((include_end(lines, first), [forward_decl(name)]))
            report.setdefault("forward_declared", []).append(f"{dest} {name}")
    out = dict(texts)
    for dest, items in edits.items():
        lines = texts[dest].split("\n")
        for at, block in sorted(items, key=lambda x: x[0], reverse=True):
            lines[at:at] = block
        out[dest] = "\n".join(lines)
    return out


def vtable_mismatches(db, bf):
    """Classes whose SDK virtual order differs from the `_ZTV` slots in Civ5XP: [(class, slot, expected, got)].
    A slot matches by symbol name, or by address when the SDK symbol is exported (aliased destructors)."""
    by_class = {v["class"]: v for v in bf.get("vtables", ()) if not v.get("construction")}
    addr = {s["mangled"]: s["addr"] for s in bf["symbols"] if s.get("section") != "*ABS*"}
    out = []
    for r in db["records"]:
        if r["origin"] != "tree" or not r["complete"] or r["template"] or not r.get("vtable"):
            continue
        v = by_class.get(r["name"])
        if v is None:
            continue
        mine = [s for g in r["vtable"]["groups"] for s in g["slots"]]
        theirs = [s for g in v["groups"] for s in g["slots"]]
        if len(mine) != len(theirs):
            out.append((r["name"], "count", len(theirs), len(mine)))
            continue
        for i, (a, b) in enumerate(zip(mine, theirs)):
            if a["kind"] in ("D1", "D0"):
                continue    # destructor slots may alias a base destructor or be pure
            if a["kind"] == "pure" or b["kind"] == "pure":
                ok = a["kind"] == "pure" and b["kind"] == "pure"
            else:
                ok = not b["symbols"] or a["symbol"] in b["symbols"] or addr.get(a["symbol"]) == b["target"]
            if not ok:
                out.append((r["name"], i, b["symbols"][:1], a["symbol"]))
                break
    return out


def probe_expressions(texts, db):
    """[(expression, record)] for the dummy instantiations; empty when every field offset is known."""
    return probe_source(Model(db, texts), texts)


def insert_above(text, by_line):
    """Marker lines {1-based line: [markers]} above the given lines of an already marked text (indent of the line)."""
    out = []
    for n, line in enumerate(text.split("\n"), 1):
        for marker in by_line.get(n, ()):
            out.append(line[:len(line) - len(line.lstrip())] + marker)
        out.append(line)
    return "\n".join(out)


def mark(texts, db, bf, origins, layouts=None):
    """Returns ({dest: marked text}, report dict). layouts: see parse_layout_dump."""
    mk = Marker(db, bf, origins, texts, layouts)
    mk.records()
    mk.functions()
    mk.globals()
    marked = {}
    for dest, text in texts.items():
        marked[dest] = apply_edits(text, dest, mk.edits, origins[dest])
    report = {k: sorted(v) for k, v in mk.report.items()}
    report["counts"] = dict(mk.edits.counts)
    report["lua_marked"] = sorted(mk.lua_marked)
    return marked, report
