"""M0-M6: typedb diagnostics routed one-to-one by code, placeholders (M5), decompiler traces (M6)."""
import re
from collections import defaultdict

from checks import includes
from checks.api import ERROR, WARN, Rule
from civ5re import placeholders as ph

TREE = includes.TREE

CODE_RULE = {
    "marker-syntax": "M1", "marker-orphan": "M1", "marker-ambiguous": "M1", "marker-duplicate": "M1",
    "marker-macro": "M1",
    "near-marker": "M2",
    "marker-missing": "M3",
    "marker-misplaced": "M4", "marker-conflict": "M4", "marker-unsupported": "M4",
    "cpp-undeclared": "P2",
    "file-unreached": "M0", "probe-failed": "M0", "clang-error": "M0", "internal": "M0",
}
ORIGIN_MISSING = "no /// origin"


def route(diag):
    """Rule id that reports this typedb diagnostic; a code without a row goes to M0."""
    code = diag.get("code")
    if code == "marker-missing" and diag.get("line") == 1 and str(diag.get("message", "")).startswith(ORIGIN_MISSING):
        return "O1"
    return CODE_RULE.get(code, "M0")


class DiagRule(Rule):
    """Base for rules that carry typedb diagnostics of their own id over 1:1 (no id: not registered)."""

    needs = ("typedb",)

    def text(self, diag):
        return f"{diag['code']}: {diag.get('message', '')}"

    def run(self, ctx):
        out = []
        for d in ctx.typedb.get("diagnostics") or []:
            if route(d) != self.id:
                continue
            path, line = d.get("file"), d.get("line")
            if path and not ctx.in_scope(path):
                continue
            out.append(self.finding(
                path, line, self.text(d), code=d["code"],
                severity=WARN if d.get("severity") == "warning" else ERROR,
                key=f"{self.id}:{path}:{d['code']}:{d.get('message', '')}"))
        return out


class M0(DiagRule):
    id = "M0"
    title = "typedb diagnostics without a marker rule: file-unreached, probe-failed, clang-error, internal, unknown codes"


class M1(DiagRule):
    id = "M1"
    title = "marker parsing and binding: marker-syntax, marker-orphan, marker-ambiguous, marker-duplicate, marker-macro"


class M2(DiagRule):
    id = "M2"
    title = "near-marker: an ordinary comment that looks like a marker"


class M3(DiagRule):
    id = "M3"
    title = "marker-missing: a required marker is absent (the file origin is O1)"


class M4(DiagRule):
    id = "M4"
    title = "marker-misplaced, marker-conflict, marker-unsupported: the marker does not fit the entity"


# ---------------------------------------------------------------------------------------------
# M5: one placeholder dictionary (civ5re.placeholders)

WHERE = {"fun": "function names", "vf": "virtual method names", "global": "global names", "class": "class names",
         "field": "field names", "unk": "field names", "param": "parameter names",
         "unk_ret": "return types", "unk_params": "whole parameter lists", "unk_data": "global array element types"}
FN_NAME, GLOBAL_NAME, CLASS_NAME, FIELD_NAME = {"fun", "vf"}, {"global"}, {"class"}, {"field", "unk"}
STRICT_KINDS = ("fun", "vf", "global", "class", "field", "unk", "unk_ret", "unk_params", "unk_data")
TYPE_WORDS = {"UNK_RET": "unk_ret", "UNK_DATA": "unk_data"}


def _declared_by_base(recs, by_scope, cls, name):
    """A vf_NN numbered by the class that introduced the slot: an override keeps that name (seed section 3). A
    base outside the tree (third_party) introduced the slot there, so it is not judged."""
    seen, stack = set(), [cls]
    while stack:
        for b in (recs.get(stack.pop()) or {}).get("bases") or ():
            n = b.get("name")
            if n not in seen:
                seen.add(n)
                stack.append(n)
                if (recs.get(n) or {}).get("origin") != "tree" or name in by_scope.get(n, ()):
                    return True
    return False


def _numbered_below(recs, cls, name, idx):
    """A vf_NN lower than the slot index in a class with bases: the number comes from the class that introduced
    the slot in its own (secondary) vtable group."""
    m = ph.classify(name)
    return bool(m and m.kind == "vf" and int(m.value) < idx and (recs.get(cls) or {}).get("bases"))


def _addr(a):
    return int(a, 16) if isinstance(a, str) and a.startswith("0x") else None


def _plain(name):
    """Last component of a qualified name without template arguments."""
    depth, out = 0, []
    for ch in name:
        if ch == "<":
            depth += 1
        elif ch == ">":
            depth = max(0, depth - 1)
        elif depth == 0:
            out.append(ch)
    return "".join(out).split("::")[-1]


class M5(Rule):
    id = "M5"
    title = "placeholders agree with their evidence (Fun_/g_/Class_/field_/unk_/vf_/aN/UNK_*); no Ghidra-style names"
    needs = ("typedb", "binfacts")

    def run(self, ctx):
        db, bf = ctx.typedb, ctx.binfacts
        by_addr = defaultdict(list)
        for s in bf.get("symbols", ()):
            by_addr[s["addr"]].append(s["mangled"])
        vtables = {v["addr"]: v for v in bf.get("vtables", ())}
        recs = {r["name"]: r for r in db.get("records", ())}
        by_scope = defaultdict(set)
        for fn in db.get("functions", ()):
            if fn.get("virtual"):
                by_scope[fn.get("scope")].add(fn.get("name"))
        seen, out = set(), []

        def report(path, line, msg, entity):
            if path and not ctx.in_scope(path):
                return
            if (path, line, msg) in seen:
                return
            seen.add((path, line, msg))
            out.append(self.finding(path, line, msg, entity=entity, key=f"M5:{path}:{entity}:{msg}"))

        def name_ok(name, allowed, path, line, entity, what):
            """True when the name is a placeholder of an allowed kind (the caller verifies its evidence)."""
            if ph.is_ghidra_style(name):
                report(path, line, f"{what} {name} is Ghidra style; use the civ5re placeholder dictionary", entity)
                return None
            m = ph.classify(name)
            if m is None:
                return None
            if m.kind in allowed:
                return m
            if m.kind in STRICT_KINDS:
                report(path, line, f"{what} {name} is a {m.kind} placeholder; it belongs to {WHERE[m.kind]}", entity)
            return None

        def types_ok(texts, path, line, entity, what):
            for t in texts:
                for word, kind in TYPE_WORDS.items():
                    if t and re.search(rf"\b{word}\b", t):
                        if kind == "unk_data" and what == "global" and "[" in t:
                            continue
                        report(path, line, f"{word} in the type of a {what} ({t}); it belongs to {WHERE[kind]}", entity)

        for f in db.get("functions", ()):
            path, line, ent = f["file"], f["line"], f.get("qualified") or f["name"]
            if not path.startswith(TREE):
                continue
            mk = f.get("markers") or {}
            m = name_ok(f["name"], FN_NAME, path, line, ent, "function")
            if m and m.kind == "fun":
                own = "0x" + m.value
                if not mk.get("addr"):
                    report(path, line, f"{f['name']} has no address marker; Fun_<addr> is its own address", ent)
                elif mk["addr"] != own:
                    report(path, line, f"{f['name']} placed over address {mk['addr']}", ent)
            elif m and m.kind == "vf":
                idx = f.get("vtable_index")
                if idx is None:
                    if not (f.get("template") or (recs.get(f.get("scope")) or {}).get("template")):
                        report(path, line, f"{f['name']} is not a virtual method (no vtable_index)", ent)
                elif f["name"] != ph.vf_name(idx) and not (
                        f.get("virtual") and (_declared_by_base(recs, by_scope, f.get("scope"), f["name"])
                                              or _numbered_below(recs, f.get("scope"), f["name"], idx))):
                    report(path, line, f"{f['name']} at vtable_index {idx}, expected {ph.vf_name(idx)}", ent)
            zero = ph.zero_based_params(p.get("name") or "" for p in f.get("params") or ())
            for i, p in enumerate(f.get("params") or (), 1):
                pn = p.get("name")
                if pn and not (zero and ph.classify(pn) and ph.classify(pn).kind == "param"):
                    pm = name_ok(pn, {"param"}, path, line, ent, "parameter")
                    if pm and pn != ph.param_name(i):
                        report(path, line, f"parameter {pn} at position {i}, expected {ph.param_name(i)}", ent)
            types_ok([p.get("type") for p in f.get("params") or ()], path, line, ent, "parameter")
            ret = f.get("return")
            if ret and ret != "UNK_RET":
                types_ok([ret], path, line, ent, "return")
            if f.get("params_unknown"):
                addrs = [mk.get("addr"), mk.get("base"), mk.get("deleting")]
                for inst in f.get("instances") or ():
                    addrs += [inst.get("addr"), inst.get("base"), inst.get("deleting")]
                for a in dict.fromkeys(x for x in addrs if x):
                    cpp = [s for s in by_addr.get(a, ()) if s.startswith("_Z")]
                    if cpp:
                        report(path, line, f"UNK_PARAMS, but C++ symbol {cpp[0]} is exported at {a}: "
                                           "its mangled name gives the parameters", ent)

        for g in db.get("globals", ()):
            path, line, ent = g["file"], g["line"], g["qualified"]
            if not path.startswith(TREE):
                continue
            name = ent.split("::")[-1]
            m = name_ok(name, GLOBAL_NAME, path, line, ent, "global")
            if m:
                own = "0x" + m.value
                if not g.get("addr_marker"):
                    report(path, line, f"{name} has no address marker; g_<addr> is its own address", ent)
                elif g["addr_marker"] != own:
                    report(path, line, f"{name} placed over address {g['addr_marker']}", ent)
            types_ok([g.get("type")], path, line, ent, "global")

        for r in db.get("records", ()):
            path, line, ent = r["file"], r["line"], r["name"]
            if r.get("origin") != "tree" or not path.startswith(TREE):
                continue
            m = name_ok(_plain(ent), CLASS_NAME, path, line, ent, "class")
            if m:
                own = "0x" + m.value
                vt = r.get("vtable_marker")
                if vt != own:
                    report(path, line, f"{_plain(ent)} has vtable marker {vt}, expected {own}", ent)
                elif own in vtables:
                    report(path, line, f"{_plain(ent)}: the vtable at {own} is named {vtables[own]['class']} in RTTI", ent)
            for fl in r.get("fields") or ():
                fname = fl.get("name")
                if not fname:
                    continue
                fent = f"{ent}::{fname}"
                fm = name_ok(fname, FIELD_NAME, path, fl["line"], fent, "field")
                if fm:
                    om = _addr(fl.get("offset_marker"))
                    if om is None:
                        report(path, fl["line"], f"{fname} has no ///< offset", fent)
                    elif om != int(fm.value, 16):
                        report(path, fl["line"], f"{fname} placed at +0x{om:x}", fent)
                types_ok([fl.get("type")], path, fl["line"], fent, "field")
        return out


# ---------------------------------------------------------------------------------------------
# M6: decompiler traces

TRACE_RE = re.compile(
    r"\b(?:undefined\d+|param_\d+|[A-Za-z]{1,3}Var\d+|local_[0-9A-Za-z]+|LAB_[0-9A-Fa-f]+|CONCAT\d+|SUB\d+"
    r"|in_stack_\w+|extraout_\w+|unaff_\w+)\b")


def strip_comments_and_strings(text):
    """Same length and line structure; comment and string/char literal contents become spaces."""
    out, i, n = [], 0, len(text)

    def blank(s):
        return "".join(c if c == "\n" else " " for c in s)

    while i < n:
        c = text[i]
        two = text[i:i + 2]
        if two == "//":
            j = text.find("\n", i)
            j = n if j < 0 else j
            while j < n and text[j - 1] == "\\":
                k = text.find("\n", j + 1)
                j = n if k < 0 else k
            out.append(blank(text[i:j]))
            i = j
        elif two == "/*":
            j = text.find("*/", i + 2)
            j = n if j < 0 else j + 2
            out.append(blank(text[i:j]))
            i = j
        elif c == "R" and text[i + 1:i + 2] == '"' and (i == 0 or not (text[i - 1].isalnum() or text[i - 1] == "_")):
            m = re.compile(r'R"([^()\\ \t\n]{0,16})\(').match(text, i)
            if m:
                end = text.find(")" + m.group(1) + '"', m.end())
                j = n if end < 0 else end + len(m.group(1)) + 2
                out.append(blank(text[i:j]))
                i = j
            else:
                out.append(c)
                i += 1
        elif c in "\"'":
            j = i + 1
            while j < n and text[j] != c and text[j] != "\n":
                j += 2 if text[j] == "\\" and text[j + 1:j + 2] not in ("", "\n") else 1
            j = min(j, n)
            if j < n and text[j] == c:
                out.append(c + blank(text[i + 1:j]) + c)
                i = j + 1
            else:
                out.append(c + blank(text[i + 1:j]))
                i = j
        else:
            out.append(c)
            i += 1
    return "".join(out)


class M6(Rule):
    id = "M6"
    title = "no decompiler traces (undefinedN, param_N, iVar3, local_xx, LAB_, CONCATnn, SUBnn, in_stack_, extraout_, unaff_)"
    needs = ("files",)

    def run(self, ctx):
        out = []
        for p in ctx.scoped(includes.is_tree_source):
            code = strip_comments_and_strings(ctx.read(p))
            for n, line in enumerate(code.split("\n"), 1):
                seen = set()
                for m in TRACE_RE.finditer(line):
                    if m.group(0) in seen:
                        continue
                    seen.add(m.group(0))
                    out.append(self.finding(p, n, f"decompiler trace '{m.group(0)}'", key=f"M6:{p}:{m.group(0)}"))
        return out
