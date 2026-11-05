"""The existing tree (SDK copies, pilot) via typedb, and third-party names: what `seed tree` must not redeclare."""
import re
from pathlib import Path

TREE = "Civ5Expansion2/"


def _plain(name):
    """`BaseVector<X,true>::Y` -> ('BaseVector', 'Y')."""
    out, depth, cur = [], 0, ""
    i = 0
    while i < len(name):
        ch = name[i]
        if ch == "<":
            depth += 1
        elif ch == ">":
            depth -= 1
        elif depth == 0 and name.startswith("::", i):
            out.append(cur)
            cur = ""
            i += 2
            continue
        elif depth == 0:
            cur += ch
        i += 1
    out.append(cur)
    return tuple(x.strip() for x in out if x.strip() != "")


class Existing:
    def __init__(self, db):
        self.db = db
        self.mangled = set()
        self.kinds = {}           # path -> record kind | enum | typedef | namespace
        self.files = {}           # path -> file relative to the repo
        self.templates = set()
        self.instances = set()    # declared SPEC names (normalized)
        self.records = {}
        for f in db.get("functions", []):
            self._names(f)
            for i in f.get("instances") or ():
                self._names(i)
        for g in db.get("globals", []):
            if g.get("mangled"):
                self.mangled.add(g["mangled"])
            for i in g.get("instances") or ():
                if i.get("mangled"):
                    self.mangled.add(i["mangled"])
        self.all_records = {}
        for r in db.get("records", []):
            self.all_records.setdefault(_plain(r["name"]), r)
            if r.get("origin") != "tree":
                continue
            p = _plain(r["name"])
            self.records[p] = r
            self._kind(p, r.get("kind") or "class", r["file"])
            if r.get("template"):
                self.templates.add(p)
            for m in r.get("instance_markers") or ():
                if m.get("kind") == "vtable" and m.get("value") not in (None, "?"):
                    self.vtables_marked.add(m["value"])
            if r.get("vtable_marker") not in (None, "?"):
                self.vtables_marked.add(r["vtable_marker"])
        for e in db.get("enums", []):
            if e["file"].startswith(TREE) and "(anon" not in e["qualified"]:
                self._kind(_plain(e["qualified"]), "enum", e["file"])
        for t in db.get("typedefs", []):
            if t["file"].startswith(TREE):
                self._kind(_plain(t["qualified"]), "typedef", t["file"])
        for f in db.get("functions", []):
            if f.get("scope_kind") == "namespace" and f.get("scope") and f["file"].startswith(TREE):
                p = _plain(f["scope"])
                for k in range(1, len(p) + 1):
                    self.kinds.setdefault(p[:k], "namespace")

    vtables_marked = set()

    def _names(self, f):
        if f.get("mangled"):
            self.mangled.add(f["mangled"])
        for v in (f.get("variants") or {}).values():
            if v:
                self.mangled.add(v)

    def _kind(self, path, kind, file):
        if path not in self.kinds or self.kinds[path] == "namespace":
            self.kinds[path] = kind
            self.files[path] = file
        for k in range(1, len(path)):
            self.kinds.setdefault(path[:k], "namespace")

    def template_params(self, path):
        """Parameters of an existing class template as written: ['class T', 'int N', 'template <class> class A']."""
        r = self.records.get(path)
        if r is None or self.root is None:
            return None
        try:
            lines = (self.root / r["file"]).read_text(encoding="utf-8").splitlines()
        except OSError:
            return None
        heads = [ln for ln in lines[max(0, r["line"] - 4):r["line"]] if ln.strip().startswith("template")]
        if not heads:
            return None
        text = heads[-1]
        i = text.index("template")
        j = text.index("<", i)
        depth, k = 0, j
        while k < len(text):
            if text[k] == "<":
                depth += 1
            elif text[k] == ">":
                depth -= 1
                if depth == 0:
                    break
            k += 1
        inner = text[j + 1:k]
        out, depth, cur = [], 0, ""
        for ch in inner:
            if ch == "<":
                depth += 1
            elif ch == ">":
                depth -= 1
            if ch == "," and depth == 0:
                out.append(cur.strip())
                cur = ""
            else:
                cur += ch
        if cur.strip():
            out.append(cur.strip())
        return out

    root = None
    fwd_names = set()

    def scan_forward(self, root):
        """Class names the tree only forward-declares (pilot, SDK): class evidence for the model."""
        self.root = Path(root)
        f = Foreign.__new__(Foreign)
        f.names = {}
        for p in sorted((self.root / "Civ5Expansion2").rglob("*.h")):
            f._scan(p.relative_to(self.root).as_posix(), p.read_text(encoding="utf-8", errors="replace"))
        self.fwd_names = {k for k, (kind, _) in f.names.items() if kind in ("class", "struct", "union")
                          and k not in self.kinds}

    _sdk = None

    def handwritten(self, path):
        """A record of the tree outside the SDK copies (the pilot): seed may complete it (section 6)."""
        if Existing._sdk is None:
            from seed import inputs
            Existing._sdk = {TREE + e.dest for e in inputs.sdk_entries()}
        r = self.records.get(path)
        return bool(r) and r["file"] not in Existing._sdk and self.kinds.get(path) not in ("enum", "typedef")

    def declares_symbol(self, mangled):
        return mangled in self.mangled

    def kind(self, path):
        return self.kinds.get(path)

    def file(self, path):
        return self.files.get(path)


_DECL = re.compile(r"(class|struct|union|enum)\s+(\w+)")
_TYPEDEF = re.compile(r"\btypedef\b[^;{}]*?\b(\w+)\s*(?:\[[^\]]*\])?\s*;")
_NS = re.compile(r"\bnamespace\s+(\w+)\s*\{")


class Foreign:
    """Names declared by third_party/compat, fwd and Lua headers (include spelling per header)."""

    HEADERS = ("compat/Windows.h", "compat/tbb.h", "compat/civ5re_prelude.h", "compat/hash_map", "compat/hash_set",
               "compat/sqlite3.h", "lua-5.1.4/include/lua.h",
               "lua-5.1.4/include/lauxlib.h", "lua-5.1.4/include/lualib.h")
    IN_PCH = ("compat/civ5re_prelude.h",)

    def __init__(self, third_party):
        tp = Path(third_party)
        self.names = {}
        self.virtuals = {}    # class path -> [(name, ret_text, param type names, const)] in declaration order
        files = list(self.HEADERS) + sorted(f"fwd/{p.name}" for p in (tp / "fwd").glob("*.h"))
        for rel in files:
            p = tp / rel
            if p.exists():
                text = p.read_text(encoding="utf-8", errors="replace")
                self._scan(rel, text)
                self._scan_virtuals(text)

    def _scan(self, rel, text):
        text = re.sub(r"//[^\n]*|/\*.*?\*/", "", text, flags=re.S)
        # namespace stack by braces
        stack, depth, ns_at = [], 0, []
        pos = 0
        tokens = re.finditer(r"\bnamespace\s+(\w+)\s*\{|\{|\}|\b(?:class|struct|union|enum)\s+\w+\s*(?=[;:{])|"
                             r"\btypedef\b[^;{}]*?;", text)
        for t in tokens:
            s = t.group(0)
            if s.startswith("namespace"):
                stack.append(t.group(1))
                ns_at.append(depth)
                depth += 1
            elif s == "{":
                depth += 1
            elif s == "}":
                depth -= 1
                if ns_at and ns_at[-1] == depth:
                    ns_at.pop()
                    stack.pop()
            elif s.startswith("typedef"):
                m = _TYPEDEF.search(s)
                if m and (not ns_at or depth == ns_at[-1] + 1):
                    self.names.setdefault(tuple(stack) + (m.group(1),), ("typedef", rel))
            else:
                m = _DECL.match(s)
                if m and (not ns_at or depth == ns_at[-1] + 1) and (ns_at or depth == 0):
                    self.names.setdefault(tuple(stack) + (m.group(2),), (m.group(1), rel))
            pos = t.end()
        _ = pos

    _CLASS_OPEN = re.compile(r"\bnamespace\s+(\w+)\s*\{|\b(?:class|struct)\s+(\w+)\s*(?::[^{;]*)?\{|\{|\}")
    _VIRTUAL = re.compile(r"^\s*virtual\s+(.*?)(~?\w+)\s*\(([^)]*)\)\s*(const)?\s*(?:=\s*0)?\s*$", re.S)
    _BUILTIN_WORDS = {"bool", "char", "short", "int", "long", "unsigned", "signed", "float", "double"}

    def _scan_virtuals(self, text):
        """The virtual functions of each class in declaration order (a destructor is ('~',)). A class with a
        virtual whose parameters are not plain builtin types is recorded as None (its slots cannot be named)."""
        text = re.sub(r"//[^\n]*|/\*.*?\*/", "", text, flags=re.S)
        stack = []      # [kind, name, the scope's own text without nested braces]
        tokens = list(self._CLASS_OPEN.finditer(text))
        for k, t in enumerate(tokens):
            s = t.group(0)
            if s == "}":
                if not stack:
                    continue
                kind, name, body = stack.pop()
                if kind == "class":
                    path = tuple(n for kd, n, _ in stack if kd != "block") + (name,)
                    self._class_virtuals(path, body)
                if stack:
                    stack[-1][2] += ";"      # a nested block ends a statement (inline body)
            elif s == "{":
                stack.append(["block", "", ""])
            else:
                stack.append(["namespace", t.group(1), ""] if t.group(1) else ["class", t.group(2), ""])
            if stack:
                stack[-1][2] += text[t.end():tokens[k + 1].start() if k + 1 < len(tokens) else len(text)]

    def _class_virtuals(self, path, body):
        out = []
        for stmt in re.sub(r"\b(?:public|private|protected)\s*:", ";", body).split(";"):
            m = self._VIRTUAL.match(stmt)
            if not m:
                continue
            ret, name, params, const = m.group(1).strip(), m.group(2), m.group(3).strip(), bool(m.group(4))
            types = []
            for p in [x.strip() for x in params.split(",")] if params and params != "void" else []:
                words = p.split()
                if len(words) > 1 and words[-1] not in self._BUILTIN_WORDS:
                    words = words[:-1]
                if not words or not all(w in self._BUILTIN_WORDS for w in words):
                    self.virtuals[path] = None
                    return
                types.append(" ".join(words))
            ns = path[:-1]
            if ret.rstrip("*& ") and (ns + (ret.rstrip("*& "),)) in self.names:
                ret = "::".join(ns) + "::" + ret if ns else ret
            out.append((name, ret, tuple(types), const))
        self.virtuals[path] = out

    def kind(self, path):
        v = self.names.get(path)
        if v is None and path and all(self.names.get(path[:k], ("",))[0] in ("class", "struct", "union")
                                      for k in range(1, len(path))) and len(path) > 1:
            return None
        return v[0] if v else None

    def header(self, path):
        v = self.names.get(path)
        return v[1] if v else None

    def include(self, path):
        h = self.header(path)
        if h is None or h in self.IN_PCH:
            return None
        for d in ("lua-5.1.4/include/", "compat/", "fwd/"):
            if h.startswith(d):
                return "<" + h[len(d):] + ">"
        return "<" + h + ">"
