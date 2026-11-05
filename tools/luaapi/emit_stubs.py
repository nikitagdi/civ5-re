"""LuaLS stubs (`---@meta`) from the model: path -> text, deterministic."""
from __future__ import annotations

import json

from .model import STD_TABLES

HEADER = "---@meta\n-- generated, do not edit\n\n"
RESERVED = frozenset("and break do else elseif end false for function if in local nil not or repeat return "
                     "then true until while goto".split())
ANY = {"k": "prim", "name": "any"}
LIBCLS = {"Events": "Event", "LuaEvents": "LuaEvent", "GameEvents": "GameEvent"}


def lt(t: dict) -> str:
    """LuaLS spelling of a parsed type."""
    k = t["k"]
    if k in ("prim", "name"):
        return t["name"]
    if k == "eventargs":
        return "any"
    if k == "array":
        e = lt(t["elem"])
        return f"({e})[]" if t["elem"]["k"] in ("union", "fun", "nullable") else e + "[]"
    if k == "nullable":
        i = lt(t["inner"])
        return f"({i})?" if t["inner"]["k"] in ("union", "fun") else i + "?"
    if k == "map":
        return f"table<{lt(t['key'])}, {lt(t['value'])}>"
    if k == "record":
        return "{ " + ", ".join(f"{f['name']}{'?' if f['optional'] else ''}: {lt(f['t'])}"
                                for f in t["fields"]) + " }"
    if k == "union":
        return "|".join(f"({lt(x)})" if x["k"] == "fun" else lt(x) for x in t["items"])
    if k == "fun":
        s = f"fun({fun_params(t['params'])})"
        if t["returns"]:
            s += ": " + ", ".join(lt(r["t"]) for r in t["returns"])
        return s
    raise ValueError(k)


def ident(name: str) -> str:
    return name + "_" if name in RESERVED else name


def fun_params(params, self_type=None) -> str:
    out = [f"self: {self_type}"] if self_type else []
    for p in params:
        if p["t"]["k"] == "eventargs":
            out.append("...: any")
        elif p["vararg"]:
            out.append(f"...: {lt(p['t'])}")
        else:
            out.append(f"{ident(p['name'])}{'?' if p['optional'] else ''}: {lt(p['t'])}")
    return ", ".join(out)


def expand(params, handler_args, call_args):
    """Replace `EventArgs`: a handler's arguments for `fun(EventArgs)`, the call arguments at top level."""
    out = []
    for p in params:
        t = p["t"]
        if t["k"] == "eventargs":
            out += _fields(call_args)
        elif t["k"] == "fun" and any(x["t"]["k"] == "eventargs" for x in t["params"]):
            nt = dict(t, params=[q for x in t["params"] for q in
                                 (_fields(handler_args) if x["t"]["k"] == "eventargs" else [x])])
            out.append(dict(p, t=nt, type=p["type"]))
        else:
            out.append(p)
    return out


def _fields(args):
    if args is None:
        return [{"name": "...", "optional": False, "vararg": True, "type": "any", "t": ANY}]
    return [{"name": f["name"], "optional": f["optional"], "vararg": False, "type": f["type"], "t": f["t"]}
            for f in args]


def _doc(doc, out):
    for line in (doc or "").splitlines():
        out.append(("--- " + line).rstrip())


def function_stub(fns, self_type=None) -> str:
    """Lines for one Lua path: the first entry is the declaration, the rest are overloads."""
    f = fns[0]
    lines = []
    _doc(f["doc"], lines)
    for p in f["params"]:
        if p["t"]["k"] == "eventargs":
            lines.append("---@param ... any")
        elif p["vararg"]:
            lines.append(f"---@param ... {lt(p['t'])}")
        else:
            lines.append(f"---@param {ident(p['name'])}{'?' if p['optional'] else ''} {lt(p['t'])}")
    for r in f["returns"]:
        lines.append(f"---@return {lt(r['t'])}" + (f" {r['name']}" if r["name"] else ""))
    for o in fns[1:]:
        s = "---@overload fun(" + fun_params(o["params"], self_type if o["call"] == "method" else None) + ")"
        if o["returns"]:
            s += ": " + ", ".join(lt(r["t"]) for r in o["returns"])
        lines.append(s)
    names = [ident(p["name"]) for p in f["params"] if p["t"]["k"] != "eventargs" and not p["vararg"]]
    if any(p["vararg"] or p["t"]["k"] == "eventargs" for p in f["params"]):
        names.append("...")
    sep = ":" if f["call"] == "method" else "."
    head = f["name"] if f["owner"] is None else f"{f['owner']}{sep}{f['name']}"
    if f["name"] in RESERVED or not f["name"].isidentifier():
        head = f"{f['owner']}[\"{f['name']}\"]" if f["owner"] else f"_G[\"{f['name']}\"]"
        names = (["self"] if f["call"] == "method" else []) + names
        lines.append(f"{head} = function({', '.join(names)}) end")
    else:
        lines.append(f"function {head}({', '.join(names)}) end")
    return "\n".join(lines)


def _group(functions):
    groups = {}
    for f in functions:
        groups.setdefault(f["path"], []).append(f)
    return [groups[k] for k in sorted(groups)]


def _prefixes(owners):
    seen = []
    for o in sorted(owners):
        parts = o.split(".")
        for i in range(1, len(parts) + 1):
            p = ".".join(parts[:i])
            if p not in seen:
                seen.append(p)
    return seen


def _decl(prefixes):
    return "".join(f"{p} = {p} or {{}}\n" for p in prefixes)


def _body(groups, self_type=None):
    return "\n\n".join(function_stub(g, self_type) for g in groups)


def generate(model: dict) -> dict:
    files = {}
    files[".luarc.json"] = json.dumps({"runtime": {"version": "Lua 5.1"}, "workspace": {"library": ["."]}},
                                      indent=2) + "\n"
    types = model["types"]
    funcs = model["functions"]
    tables = [t for t in types if t["kind"] == "table"]
    classes = [t for t in types if t["kind"] == "class"]
    table_names = [t["name"] for t in tables]
    class_names = {t["name"] for t in classes}

    out = [HEADER]
    for t in types:
        if t["kind"] == "shape":
            _doc(t["doc"], out)
            out.append(f"---@class {t['name']}")
            out += [f"---@field {f['name']}{'?' if f['optional'] else ''} {lt(f['t'])}" for f in t["fields"]]
            out.append("")
        elif t["kind"] == "external":
            out.append(f"---@class {t['name']}")
            out.append(f"-- Lekmod: {t['file']}")
            out.append("")
    files["_types.lua"] = "\n".join(out).rstrip("\n") + "\n"

    out = [HEADER]
    for t in types:
        if t["kind"] == "enum":
            _doc(t["doc"], out)
            out.append(f"---@enum {t['name']}")
            out.append(f"{t['name']} = {{")
            out += [f"    {v['name']} = {v['value']}," for v in t["values"]]
            out += ["}", ""]
    files["_enums.lua"] = "\n".join(out).rstrip("\n") + "\n"

    def owner_kind(f):
        o = f["owner"]
        if o is None or o.split(".")[0] in STD_TABLES:
            return "global"
        for n in table_names:
            if o == n or o.startswith(n + "."):
                return ("table", n)
        if o in class_names:
            return ("class", o)
        return "global"

    placed = {}
    for f in funcs:
        placed.setdefault(owner_kind(f), []).append(f)
    out = [HEADER]
    for g in model["globals"]:
        _doc(g["doc"], out)
        out.append(f"---@type {lt(g['t'])}")
        out.append(f"{g['name']} = nil\n")
    glob = placed.get("global", [])
    owners = [f["owner"] for f in glob if f["owner"] and f["owner"].split(".")[0] not in STD_TABLES]
    if owners:
        out.append(_decl(_prefixes(owners)))
    if glob:
        out.append(_body(_group(glob)))
    files["_globals.lua"] = "\n".join(out).rstrip("\n") + "\n"

    for t in tables:
        mine = placed.get(("table", t["name"]), [])
        out = [HEADER]
        _doc(t["doc"], out)
        out.append(_decl(_prefixes([t["name"]] + [f["owner"] for f in mine])))
        if mine:
            out.append(_body(_group(mine)))
        files[f"tables/{t['name']}.lua"] = "\n".join(out).rstrip("\n") + "\n"

    for c in classes:
        mine = placed.get(("class", c["name"]), [])
        out = [HEADER]
        _doc(c["doc"], out)
        out.append(f"---@class {c['name']}" + (f" : {c['base']}" if c["base"] else ""))
        out.append(f"local {c['name']} = {{}}\n")
        if mine:
            out.append(_body(_group(mine), c["name"]))
        files[f"classes/{c['name']}.lua"] = "\n".join(out).rstrip("\n") + "\n"

    out = [HEADER]
    libs = {}
    for e in model["events"]:
        libs.setdefault(e["lib"] or "Events", []).append(e)
    for lib in sorted(libs):
        out.append(f"---@class {lib}")
        out += [f"---@field {e['name']} {LIBCLS.get(lib, lib)}.{e['name']}" for e in libs[lib]]
        out.append(f"\n---@type {lib}\n{lib} = nil\n")
    for lib in sorted(libs):
        base = LIBCLS.get(lib, lib)
        members = [f for f in funcs if f["owner"] == base and f["call"] != "dot"]
        for e in libs[lib]:
            cls = f"{base}.{e['name']}"
            out.append(f"---@class {cls} : {base}" if base in class_names else f"---@class {cls}")
            for f in members:
                if any(p["t"]["k"] == "eventargs" or (p["t"]["k"] == "fun" and any(
                        x["t"]["k"] == "eventargs" for x in p["t"]["params"])) for p in f["params"]):
                    ps = expand(f["params"], e["handler_args"], e["call_args"])
                    s = f"---@field {f['name']} fun({fun_params(ps, cls)})"
                    if f["returns"]:
                        s += ": " + ", ".join(lt(r["t"]) for r in f["returns"])
                    out.append(s)
            out.append("")
    files["events.lua"] = "\n".join(out).rstrip("\n") + "\n"
    return files
