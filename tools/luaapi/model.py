"""typedb (+binfacts) -> the Lua API model, and the LUA1..LUA9 checks as plain functions.

checkrules.py wraps `lint` into checks rules; nothing here imports checks.
"""
from __future__ import annotations

import re
from collections import namedtuple
from pathlib import Path

from . import entries, flua, grammar

SCHEMA = "civ5re-lua-api/1"
TOOL_VERSION = "1.0.0"
STD_TABLES = frozenset(("math", "table", "string", "os", "io", "debug", "coroutine"))
EVENT_CLASSES = frozenset(("Event", "LuaEvent", "GameEvent"))
CONTAINERS = {"SerialContainer": "serial", "NonSerialContainer": "nonserial",
              "LocalMachineContainer": "local", "SequencerContainer": "sequencer"}
MARSHAL = {"push": "ToLua", "pull": "FromLua"}
GAMECORE_FILE = Path(__file__).resolve().parent / "data" / "gamecore.txt"

Diag = namedtuple("Diag", "rule severity message file line")


def load_gamecore(path=None) -> dict:
    out = {}
    for raw in Path(path or GAMECORE_FILE).read_text(encoding="utf-8").splitlines():
        if raw.strip() and not raw.startswith("#"):
            name, _, file = raw.partition("\t")
            out[name.strip()] = file.strip()
    return out


# ---------------------------------------------------------------- helpers

_QUAL = re.compile(r"\b(const|volatile|struct|class|enum|union)\b")


def bare_type(spelling: str) -> str:
    return "".join(_QUAL.sub(" ", spelling or "").replace("&", " ").replace("*", " ").split())


def split_args(text: str) -> list:
    out, depth, cur = [], 0, []
    for ch in text:
        if ch in "<(":
            depth += 1
        elif ch in ">)":
            depth -= 1
        if ch == "," and depth == 0:
            out.append("".join(cur))
            cur = []
        else:
            cur.append(ch)
    if cur:
        out.append("".join(cur))
    return out


def parse_event_type(text: str):
    """`EventTemplate<Event_Int2Type<N>, P, Dispatcher, Container, ...>` -> {template, id, payload, container}."""
    t = "".join((text or "").split())
    m = re.match(r"^(?:[\w:]*::)?(EventTemplate|GameCoreEventTemplate)<(.*)>$", t)
    if not m:
        return None
    args = split_args(m.group(2))
    mi = re.match(r"^(?:[\w:]*::)?Event_Int2Type<(\d+)>$", args[0]) if args else None
    if not mi or len(args) < 2:
        return None
    container = None
    for a in args[2:]:
        base = a.rsplit("::", 1)[-1]
        if base.endswith("Container"):
            container = CONTAINERS.get(base, base[:-len("Container")].lower())
            break
    payload = args[1]
    return {"template": m.group(1), "id": int(mi.group(1)), "container": container,
            "payload": None if payload.rsplit("::", 1)[-1] == "NullEventClass" else payload}


def instantiation_events(facts) -> dict:
    """(container, id) -> instantiation scope for every `CvLuaEvent<...>` in binfacts."""
    out = {}
    for s in facts.get("symbols", ()):
        sc = s.get("scope") or ""
        if sc.startswith("CvLuaEvent<") and entries.strip_template_args(sc) == "CvLuaEvent":
            ev = parse_event_type(sc[len("CvLuaEvent<"):-1])
            if ev:
                out.setdefault((ev["container"], ev["id"]), sc)
    return out


def count_instantiations(facts) -> int:
    return len({s["scope"] for s in facts.get("symbols", ())
                if (s.get("scope") or "").startswith("CvLuaEvent<")
                and entries.strip_template_args(s["scope"]) == "CvLuaEvent"})


def doc_of(ent):
    c = ent.get("comment")
    return c["text"] if c else None


# ---------------------------------------------------------------- scan

class Item:
    __slots__ = ("ekind", "ent", "ekey", "file", "line", "text", "marker", "error")

    def __init__(self, ekind, ent, ekey, file, line, text, marker, error):
        self.ekind, self.ent, self.ekey = ekind, ent, ekey
        self.file, self.line, self.text = file, line, text
        self.marker, self.error = marker, error

    @property
    def kind(self):
        return self.marker.kind if self.marker is not None else None

    @property
    def loc(self):
        return f"{self.file}:{self.line}"


def parse_text(text):
    """(marker or None, strict error or None)."""
    try:
        return grammar.parse(text), None
    except grammar.GrammarError as e:
        try:
            return grammar.parse(text, strict=False), e
        except grammar.GrammarError:
            return None, e


class Scan:
    def __init__(self, db, gamecore=None):
        self.db = db
        self.gamecore = load_gamecore() if gamecore is None else gamecore
        self.items = []
        self.entities = {}
        self.records = {r["name"]: r for r in db.get("records", ())}
        self.fn_by_key = {}
        self.fn_by_mangled = {}
        self.fn_by_qualified = {}
        self._collect()
        self.items.sort(key=lambda i: (i.file, i.line, i.text))
        self._names()

    def _add(self, ekind, ent, ekey, markers):
        self.entities[ekey] = (ekind, ent)
        for m in (markers or {}).get("lua", ()):
            marker, err = parse_text(m["text"])
            self.items.append(Item(ekind, ent, ekey, m["file"], m["line"], m["text"], marker, err))

    def _collect(self):
        for f in self.db.get("functions", ()):
            self.fn_by_key[f["key"]] = f
            names = [f.get("mangled")] + list((f.get("variants") or {}).values())
            for inst in f.get("instances", ()):
                names += [inst.get("mangled")] + list((inst.get("variants") or {}).values())
            for n in names:
                if n:
                    self.fn_by_mangled.setdefault(n, f)
            self.fn_by_qualified.setdefault(f["qualified"], []).append(f)
            self._add("function", f, f["key"], f.get("markers"))
        for r in self.db.get("records", ()):
            self._add("record", r, "record:" + r["name"], r.get("markers"))
        for e in self.db.get("enums", ()):
            self._add("enum", e, "enum:" + e["qualified"], e.get("markers"))
        for t in self.db.get("typedefs", ()):
            self._add("typedef", t, "typedef:" + t["qualified"], t.get("markers"))

    def _names(self):
        self.classes, self.tables, self.shapes, self.enums = {}, {}, {}, {}
        self.by_ekey = {}
        for it in self.items:
            self.by_ekey.setdefault(it.ekey, []).append(it)
        self.type_items = []     # every item that defines a type name, in file order
        self.cxx_classes = {}    # C++ record -> Lua class name
        self.cxx_names = {}      # C++ record -> every Lua class/table name
        self.children = {}
        for r in self.records.values():
            for b in r.get("bases", ()):
                self.children.setdefault(b["name"], []).append(r["name"])
        for it in self.items:
            m = it.marker
            if m is None:
                continue
            k = m.kind
            if k == "class":
                self.classes.setdefault(m.name, it)
                self.type_items.append((m.name, it))
                if it.ekind == "record":
                    self.cxx_classes.setdefault(it.ent["name"], m.name)
                    self.cxx_names.setdefault(it.ent["name"], []).append(("class", m.name))
            elif k == "table":
                self.tables.setdefault(m.path, it)
                if it.ekind == "record":
                    self.cxx_names.setdefault(it.ent["name"], []).append(("table", m.path))
            elif k == "shape":
                self.shapes.setdefault(m.name, it)
                self.type_items.append((m.name, it))
            elif k == "enum":
                self.enums.setdefault(m.name, it)
                self.type_items.append((m.name, it))
        self.known_types = set(self.classes) | set(self.shapes) | set(self.enums) | set(self.gamecore)
        self.lines = [i for i in self.items if i.kind == "function"]
        self.paths = {}
        for it in self.lines:
            self.paths.setdefault(it.marker.path, []).append(it)
        self.marshallers = []
        for it in self.items:
            if it.kind in MARSHAL and it.ekind == "function" and it.ent["name"] == MARSHAL[it.kind] \
                    and len(it.ent["params"]) >= 2:
                self.marshallers.append((it.kind, bare_type(it.ent["params"][1]["type"]), it))

    def kinds_of(self, ent) -> set:
        return {i.kind for i in self.by_ekey.get(ent.get("key"), ()) if i.ekind == "function"} - {None}

    def descendants(self, name) -> set:
        seen, todo = set(), [name]
        while todo:
            n = todo.pop()
            if n not in seen:
                seen.add(n)
                todo.extend(self.children.get(n, ()))
        return seen

    def find_decl(self, ep):
        if ep.kind == "template":
            fns = self.fn_by_qualified.get(ep.key, [])
            fns = [f for f in fns if self.is_template_member(f)] or fns
            fns.sort(key=lambda f: (not (f.get("markers") or {}).get("lua"), f["file"], f["line"]))
            return fns[0] if fns else None
        return self.fn_by_mangled.get(ep.key)

    def is_template_member(self, f) -> bool:
        r = self.records.get(f.get("scope") or "")
        return bool(f.get("instances")) or bool(r and r.get("template"))

    def marshal(self, kind, *names):
        for k, t, it in self.marshallers:
            if k == kind and t in names:
                return it
        return None


def partial_entries(scan) -> list:
    """Entry points seen from declarations only (no binfacts): `l*` and `LuaThunk_*` taking lua_State*."""
    out = []
    for f in scan.db.get("functions", ()):
        ps = f.get("params") or []
        if len(ps) != 1 or bare_type(ps[0]["type"]) != "lua_State":
            continue
        name, scope = f["name"], f.get("scope") or ""
        if not scope and name.startswith(entries.THUNK_PREFIX):
            kind = "thunk"
        elif scope and entries.LNAME.match(name):
            kind = "template" if scan.is_template_member(f) else "l"
        else:
            continue
        key = f"{scope}::{name}" if kind == "template" else (f.get("mangled") or f["key"])
        out.append(entries.EntryPoint(kind, key, key, "" if kind == "thunk" else scope, name, (key,),
                                      (f.get("markers") or {}).get("addr") or ""))
    out.sort(key=lambda e: (entries.KINDS.index(e.kind), e.scope, e.name, e.key))
    return out


# ---------------------------------------------------------------- LUA1..LUA9

def _diag(rule, msg, it, severity="error"):
    return Diag(rule, severity, msg, it.file, it.line)


def lint_1(scan):
    return [_diag("LUA1", it.error.__str__(), it) for it in scan.items if it.error is not None]


def lint_2(scan):
    out = []
    for it in scan.items:
        m = it.marker
        if m is None:
            continue
        name = it.ent.get("name", "") if it.ekind == "function" else ""
        free = it.ekind == "function" and not it.ent.get("scope")
        if free and name.startswith("LuaToString_"):
            out.append(_diag("LUA2", "no '/// lua' is allowed on LuaToString_* (only the address)", it))
            continue
        if free and name.startswith(entries.THUNK_PREFIX) and m.kind != "?":
            out.append(_diag("LUA2", "on LuaThunk_* only '/// lua ?' is allowed", it))
            continue
        k = m.kind
        if k in ("function", "?", "none"):
            ok, want = it.ekind == "function", "a function"
        elif k in ("table", "class", "global"):
            ok, want = it.ekind == "record", "a class, struct or union"
        elif k == "enum":
            ok, want = it.ekind == "enum", "an enum"
        elif k == "event":
            ok, want = it.ekind == "typedef", "a typedef"
        elif k in MARSHAL:
            ok, want = it.ekind == "function" and name == MARSHAL[k], f"{MARSHAL[k]}"
        else:
            ok, want = it.ekind in ("record", "function"), "a struct or a function"
        if not ok:
            label = "function" if k == "function" else k
            out.append(_diag("LUA2", f"'{label}' marker must be above {want}, found {it.ekind}", it))
    return out


def lint_3(scan):
    out = []
    for it in scan.items:
        m = it.marker
        if m is None:
            continue
        seen = set()
        for r in m.refs:
            if r.name not in scan.known_types and r.name not in seen:
                seen.add(r.name)
                out.append(_diag("LUA3", f"unknown type '{r.name}'", it))
        if m.eventargs and not (m.kind == "function" and m.owner == "Event"):
            out.append(_diag("LUA3", "EventArgs is only allowed on members of class Event", it))
    return out


def lint_4(scan):
    out = []
    for it in scan.items:
        m = it.marker
        if it.kind != "function" or it.ekind != "function" or m.owner is None:
            continue
        if m.owner.split(".")[0] in STD_TABLES:
            continue
        ent = it.ent
        scope = ent.get("scope") or ""
        if ent.get("scope_kind") != "class" or not scope:
            continue
        have = []
        for rec in sorted(scan.descendants(scope)):
            have += scan.cxx_names.get(rec, [])
        if any(m.owner == n or m.owner.startswith(n + ".") for _, n in have):
            continue
        if have:
            what = ", ".join(f"{k} {n}" for k, n in have)
            out.append(_diag("LUA4", f"{m.path} on a class with {what}", it))
        else:
            out.append(_diag("LUA4", f"{m.path} on {scope}, which has no '/// lua table' or '/// lua class'", it))
    return out


def lint_5(scan):
    out = []
    first = {}
    for it in scan.lines:
        prev = first.setdefault(it.marker.path, it)
        if prev.ekey != it.ekey:
            out.append(_diag("LUA5", f"path {it.marker.path} already at {prev.loc}", it))
    seen = {}
    for name, it in scan.type_items:
        prev = seen.setdefault(name, it)
        if prev is not it:
            out.append(_diag("LUA5", f"type name {name} already at {prev.loc}", it))
    tables = {}
    for it in scan.items:
        if it.kind == "table":
            prev = tables.setdefault(it.marker.path, it)
            if prev is not it:
                out.append(_diag("LUA5", f"table {it.marker.path} already at {prev.loc}", it))
    names, pairs = {}, {}
    for it in scan.items:
        if it.kind != "event":
            continue
        prev = names.setdefault(it.marker.path, it)
        if prev is not it:
            out.append(_diag("LUA5", f"event {it.marker.path} already at {prev.loc}", it))
        ev = parse_event_type(it.ent.get("type", ""))
        if ev:
            p = pairs.setdefault((ev["container"], ev["id"]), it)
            if p is not it and p.marker.path != it.marker.path:
                out.append(_diag("LUA5", f"event id {ev['id']} in container {ev['container']} is already "
                                         f"{p.marker.path} at {p.loc}", it))
    return out


def _shape_maps(scan):
    shapes = {}
    for it in scan.shapes.values():
        if it.ekind == "record":
            shapes[it.ent["name"]] = it.marker.name
    return shapes


def _cpp_params(ent):
    return ent.get("params") or []


def lint_6(scan, facts):
    if facts is None:
        return []
    out = []
    eps = entries.entry_points(facts)
    mapper = flua.Mapper(scan.db.get("types"), _shape_maps(scan), dict(scan.cxx_classes))
    for ep in eps:
        if ep.kind != "thunk":
            continue
        path = entries.thunk_path(ep.name, scan.cxx_classes)
        lines = scan.paths.get(path, [])
        for extra in lines[1:]:
            out.append(_diag("LUA6", f"{path}: {len(lines)} lines for one thunk, first at {lines[0].loc}", extra))
        decl = scan.fn_by_mangled.get(ep.key)
        if decl is not None and lines:
            for it in scan.by_ekey.get(decl["key"], ()):
                if it.kind == "?":
                    out.append(_diag("LUA6", f"{path}: '?' on {ep.name} although the line exists at "
                                             f"{lines[0].loc}", it))
        for it in lines[:1]:
            out += _check_target(it, mapper)
    return out


def _check_target(it, mapper):
    out = []
    m, ent = it.marker, it.ent
    ps = _cpp_params(ent)
    if len(ps) == 1 and bare_type(ps[0]["type"]) == "lua_State":
        return out
    lp = [p for p in m.params if p["t"]["k"] != "eventargs"]
    fixed = [p for p in lp if not p["vararg"]]
    has_var = len(fixed) != len(lp)
    required = [p for p in fixed if not p["optional"]]
    n = len(ps)
    if n < len(required) or (n > len(fixed) and not has_var):
        out.append(_diag("LUA6", f"{m.path}: {len(fixed)} arguments, target has {n}", it))
        return out
    for p, cp in zip(fixed, ps):
        mp = mapper.map(cp["type"])
        if mp.type is None:
            out.append(_diag("LUA6", f"{m.path}: C++ type '{cp['type']}' of '{cp['name']}' is not in the "
                                     "mapping table", it, "warn"))
        elif not flua.compatible(mp, p["t"]):
            out.append(_diag("LUA6", f"{m.path}: argument '{p['name']}' is {p['type']}, target takes "
                                     f"{cp['type']} ({mp.type})", it))
    ret = ent.get("return")
    if ret is not None:
        mr = mapper.map(ret)
        if mr.void and m.returns:
            out.append(_diag("LUA6", f"{m.path}: returns {len(m.returns)} values, target returns void", it))
        elif m.returns and mr.type is None and not mr.void:
            out.append(_diag("LUA6", f"{m.path}: C++ return type '{ret}' is not in the mapping table",
                             it, "warn"))
        elif m.returns and not mr.void and not flua.compatible(mr, m.returns[0]["t"], is_return=True):
            out.append(_diag("LUA6", f"{m.path}: returns {m.returns[0]['type']}, target returns "
                                     f"{ret} ({mr.type})", it))
    return out


def lint_7(scan, facts):
    if facts is None:
        return []
    eps = entries.entry_points(facts)
    res = entries.classify(eps, find=scan.find_decl, kinds=scan.kinds_of, paths=set(scan.paths),
                           classes=scan.cxx_classes)
    out = []
    for ep in eps:
        st, decl = res[ep.key]
        if st != "unmarked":
            continue
        if ep.kind == "thunk":
            msg = f"{ep.name} without '/// lua ?' or a line with its path"
        else:
            msg = f"{ep.name} without /// lua (at least '?' is needed)"
        out.append(Diag("LUA7", "error", msg, decl["file"], decl["line"]))
    return out


def lint_8(scan, facts):
    out = []
    inst = instantiation_events(facts) if facts is not None else None
    for it in scan.items:
        if it.kind != "event":
            continue
        ev = parse_event_type(it.ent.get("type", ""))
        if ev is None:
            out.append(_diag("LUA8", f"typedef {it.ent['qualified']} under event is not an event template", it))
            continue
        if inst is not None and (ev["container"], ev["id"]) not in inst:
            out.append(_diag("LUA8", f"{it.marker.path} has no CvLuaEvent<...> instantiation in the binary",
                             it, "warn"))
        p = ev["payload"]
        if p is not None:
            names = (bare_type(it.ent["qualified"]), bare_type(it.ent.get("type", "")), bare_type(p))
            if scan.marshal("push", *names) is None:
                out.append(_diag("LUA8", f"{p} has no push", it, "warn"))
    pushes = {t: i for k, t, i in scan.marshallers if k == "push"}
    for k, t, it in scan.marshallers:
        if k == "pull" and t in pushes and it.marker is not None and pushes[t].marker is not None:
            a = [(f["name"], f["optional"], f["type"]) for f in pushes[t].marker.fields]
            b = [(f["name"], f["optional"], f["type"]) for f in it.marker.fields]
            if a != b:
                out.append(_diag("LUA8", f"{t}: push and pull differ (push at {pushes[t].loc})", it, "warn"))
    return out


def lint_9(scan):
    return [_diag("LUA9", 'lua none without a "why" comment', it)
            for it in scan.items if it.kind == "none" and not it.ent.get("comment")]


RULES = {
    "LUA1": (lint_1, False), "LUA2": (lint_2, False), "LUA3": (lint_3, False), "LUA4": (lint_4, False),
    "LUA5": (lint_5, False), "LUA6": (lint_6, True), "LUA7": (lint_7, True), "LUA8": (lint_8, True),
    "LUA9": (lint_9, False),
}


def lint(scan, facts=None, only=None) -> list:
    out = []
    for rid in sorted(RULES):
        if only and rid not in only:
            continue
        fn, wants_facts = RULES[rid]
        out += fn(scan, facts) if wants_facts else fn(scan)
    out.sort(key=lambda d: (d.file, d.line, d.rule, d.message))
    return out


# ---------------------------------------------------------------- model

def _loc(it):
    return it.file, it.line


def _type_entry(name, kind, it, **kw):
    ent = it.ent if it is not None else None
    d = {"name": name, "kind": kind, "base": None, "xml": [], "cxx": None, "origin": "exe",
         "fields": [], "values": [], "file": it.file if it else None, "line": it.line if it else None,
         "doc": doc_of(ent) if ent else None}
    d.update(kw)
    return d


def _types(scan):
    out = []
    for name, it in sorted(scan.classes.items()):
        m = it.marker
        out.append(_type_entry(name, "class", it, base=m.base, xml=list(m.xml),
                               cxx=it.ent["name"] if it.ekind == "record" else None))
    for path, it in sorted(scan.tables.items()):
        out.append(_type_entry(path, "table", it, cxx=it.ent["name"] if it.ekind == "record" else None))
    for name, it in sorted(scan.shapes.items()):
        out.append(_type_entry(name, "shape", it, fields=it.marker.fields,
                               cxx=it.ent["name"] if it.ekind == "record" else None))
    for name, it in sorted(scan.enums.items()):
        ent = it.ent
        vals = [{"name": v["name"], "value": v["value"]} for v in ent.get("values", ())] \
            if it.ekind == "enum" else []
        out.append(_type_entry(name, "enum", it, values=vals, cxx=ent.get("qualified")))
    for name, file in sorted(scan.gamecore.items()):
        if name not in scan.classes:
            out.append(_type_entry(name, "external", None, origin="gamecore", file=file))
    out.sort(key=lambda t: (t["name"], t["kind"]))
    return out


def _globals(scan):
    out = []
    for it in scan.items:
        if it.kind == "global":
            m = it.marker
            out.append({"name": m.name, "type": m.type, "t": m.t,
                        "cxx": it.ent["name"] if it.ekind == "record" else None,
                        "file": it.file, "line": it.line, "doc": doc_of(it.ent)})
    out.sort(key=lambda g: g["name"])
    return out


def _bind(scan, m, thunk):
    if m.owner is None:
        return "flua" if thunk else "lib"
    root = m.owner.split(".")[0]
    if m.owner in scan.tables or root in STD_TABLES or any(m.owner.startswith(t + ".") for t in scan.tables):
        return "lib"
    if m.owner in EVENT_CLASSES:
        return "event"
    return "flua"


def _uses(usage, scan, m, globals_):
    if usage is None:
        return None
    members = usage.get("members", {})
    if m.call in ("dot", "global"):
        d = members.get(m.owner or "", {}).get(m.name, {}) if m.owner else {}
        return {"dot": d.get("dot", 0), "method": d.get("colon", 0)}
    n = 0
    for g in globals_:
        if g["t"]["k"] == "name" and g["t"]["name"] == m.owner:
            n += members.get(g["name"], {}).get(m.name, {}).get("colon", 0)
    if any(g["name"] == "Controls" and g["t"]["k"] == "map" and g["t"]["value"].get("name") == m.owner
           for g in globals_):
        n += usage.get("controls", {}).get(m.name, 0)
    return {"dot": 0, "method": n}


def _functions(scan, thunks, usage, globals_):
    out = []
    counters = {}
    for it in scan.lines:
        m = it.marker
        ent = it.ent
        i = counters.get(m.path, 0)
        counters[m.path] = i + 1
        rec = scan.records.get(ent.get("scope") or "")
        th = thunks.get(m.path)
        out.append({
            "path": m.path, "owner": m.owner, "name": m.name, "call": m.call, "overload": i,
            "params": m.params, "returns": m.returns, "raw": m.text,
            "cxx": {"qualified": ent["qualified"], "mangled": ent.get("mangled"),
                    "addr": (ent.get("markers") or {}).get("addr"), "file": ent["file"], "line": ent["line"],
                    "template": bool(ent.get("instances")) or bool(rec and rec.get("template"))},
            "bind": _bind(scan, m, th), "thunk": th, "doc": doc_of(ent),
            "game_uses": _uses(usage, scan, m, globals_),
        })
    out.sort(key=lambda f: (f["path"], f["overload"]))
    return out


def _events(scan, facts, usage):
    inst = instantiation_events(facts) if facts is not None else {}
    out = []
    for it in scan.items:
        if it.kind != "event":
            continue
        td = it.ent
        ev = parse_event_type(td.get("type", "")) or {}
        parts = it.marker.path.split(".", 1)
        lib, name = (parts[0], parts[1]) if len(parts) == 2 else (None, parts[0])
        payload = ev.get("payload")
        names = (bare_type(td["qualified"]), bare_type(td.get("type", "")), bare_type(payload or ""))
        push = scan.marshal("push", *names)
        pull = scan.marshal("pull", *names)
        uses = None
        if usage is not None:
            u = usage.get("events", {}).get(name, {})
            uses = {"add": u.get("Add", 0), "call": u.get("call", 0), "call_immediate": u.get("CallImmediate", 0)}
        out.append({
            "lib": lib, "name": name, "container": ev.get("container"), "id": ev.get("id"),
            "cxx_type": td.get("type"), "payload": payload,
            "handler_args": push.marker.fields if push is not None else ([] if ev and payload is None else None),
            "call_args": pull.marker.fields if pull is not None else ([] if ev and payload is None else None),
            "binding": (ev.get("container"), ev.get("id")) in inst,
            "file": it.file, "line": it.line, "doc": doc_of(td), "game_uses": uses,
        })
    out.sort(key=lambda e: ((e["lib"] or ""), e["name"], e["file"], e["line"]))
    return out


def _hint(scan, ep):
    if ep.kind == "thunk":
        return entries.thunk_path(ep.name, scan.cxx_classes)
    for kind, name in scan.cxx_names.get(ep.scope, ()):
        if kind == "table":
            return f"{name}.{ep.name[1:]}"
    return None


def _coverage(scan, facts, eps, results, events, partial):
    counts = entries.count_statuses(results)
    decl_keys = {d["key"] for _, d in results.values() if d is not None}
    extra = {}
    for it in scan.items:
        if it.ekind == "function" and it.kind in ("function", "?", "none") and it.ekey not in decl_keys:
            extra.setdefault(it.ekey, set()).add(it.kind)
    for kinds in extra.values():
        counts["marked" if "function" in kinds else "none" if "none" in kinds else "unknown"] += 1
    owners = {}
    for ep in eps:
        st, _ = results[ep.key]
        scope = ep.scope or "LuaThunk_*"
        o = owners.setdefault(scope, {"cxx_scope": scope, "lua": None, "total": 0, "marked": 0, "none": 0,
                                      "unknown": 0, "missing": 0})
        o["total"] += 1
        if st in ("marked", "none", "unknown"):
            o[st] += 1
        else:
            o["missing"] += 1
        if o["lua"] is None and ep.scope:
            names = scan.cxx_names.get(ep.scope)
            if names:
                o["lua"] = names[0][1]
    n_to = n_from = 0
    if facts is not None:
        for s in facts.get("symbols", ()):
            if s.get("group") == "firaxis" and s.get("scope") == "LuaSupport":
                n_to += s.get("name") == "ToLua"
                n_from += s.get("name") == "FromLua"
    return {
        "partial": partial,
        "entry_points": len(eps) + len(extra),
        "marked": counts["marked"], "none": counts["none"], "unknown": counts["unknown"],
        "unmarked": counts["unmarked"], "undeclared": counts["undeclared"],
        "by_owner": sorted(owners.values(), key=lambda o: o["cxx_scope"]),
        "events": {"in_binary": count_instantiations(facts) if facts is not None else None,
                   "declared": len(events),
                   "with_args": sum(1 for e in events if e["handler_args"] is not None
                                    or e["call_args"] is not None)},
        "payloads": {"to_lua": n_to if facts is not None else None,
                     "push": sum(1 for k, _, _ in scan.marshallers if k == "push"),
                     "from_lua": n_from if facts is not None else None,
                     "pull": sum(1 for k, _, _ in scan.marshallers if k == "pull")},
    }


def build(db, facts=None, *, usage=None, gamecore=None, inputs=None) -> dict:
    """The `civ5re-lua-api/1` document. `usage` is the dict of usage.Usage.to_dict() or None."""
    scan = Scan(db, gamecore)
    partial = facts is None
    eps = partial_entries(scan) if partial else entries.entry_points(facts)
    results = entries.classify(eps, find=scan.find_decl, kinds=scan.kinds_of, paths=set(scan.paths),
                               classes=scan.cxx_classes)
    thunks = {}
    for ep in eps:
        if ep.kind == "thunk":
            thunks.setdefault(entries.thunk_path(ep.name, scan.cxx_classes), {"name": ep.name, "addr": ep.addr or None})
    globals_ = _globals(scan)
    events = _events(scan, facts, usage)
    functions = _functions(scan, thunks, usage, globals_)
    missing = []
    for ep in eps:
        st, _ = results[ep.key]
        if st in ("unknown", "unmarked", "undeclared"):
            hint = _hint(scan, ep)
            missing.append({"cxx": ep.label, "mangled": ep.symbol, "addr": ep.addr or None, "status": st,
                            "hint": hint, "game_uses": _hint_uses(usage, hint)})
    missing.sort(key=lambda x: (x["cxx"], x["mangled"]))
    diags = [{"rule": d.rule, "severity": d.severity, "message": d.message, "file": d.file, "line": d.line}
             for d in lint(scan, facts)]
    doc = {
        "schema": SCHEMA, "tool": {"luaapi": TOOL_VERSION},
        "inputs": dict({"typedb_sha256": None, "binfacts_sha256": None, "game_scanned": usage is not None},
                       **(inputs or {})),
        "types": _types(scan), "globals": globals_, "functions": functions, "events": events,
        "coverage": _coverage(scan, facts, eps, results, events, partial),
        "missing": missing, "diagnostics": diags,
    }
    doc["inputs"]["game_scanned"] = usage is not None
    return doc


def _hint_uses(usage, hint):
    if usage is None or not hint or ":" in hint:
        return None
    owner, _, name = hint.rpartition(".")
    d = usage.get("members", {}).get(owner, {}).get(name)
    return None if d is None else {"dot": d.get("dot", 0), "method": d.get("colon", 0)}
