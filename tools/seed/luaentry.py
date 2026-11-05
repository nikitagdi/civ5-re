"""Lua entry points from binfacts only (lua-api spec, "Entry points"; seed places `/// lua ?` above them).

Kinds: `l` (an `l*` function of a non-template scope), `template` (an `l*` member of a class template;
all instantiations of one member are one entry point), `thunk` (`LuaThunk_*` with no scope).
`LuaToString_*` get only an address marker, so they are not entry points.
"""
import re

L_NAME = re.compile(r"^l[A-Za-z_]")
THUNK_PREFIX = "LuaThunk_"
LUA_PARAM = "lua_State*"


def _strip_args(text):
    out, depth = [], 0
    for ch in text:
        if ch == "<":
            depth += 1
        elif ch == ">":
            depth -= 1
        elif depth == 0:
            out.append(ch)
    return "".join(out)


def is_candidate(s):
    return s.get("group") == "firaxis" and s.get("params") == LUA_PARAM and s.get("kind") in ("function", "method")


def classify(s):
    """'l' | 'template' | 'thunk' | None for one binfacts symbol."""
    if not is_candidate(s):
        return None
    scope, name = s.get("scope"), s.get("name") or ""
    if not scope:
        return "thunk" if name.startswith(THUNK_PREFIX) else None
    if not L_NAME.match(name):
        return None
    return "template" if "<" in scope else "l"


def entries(bf):
    """[{kind, key, name, scope, mangled, addr}] sorted by (kind, key); one entry per template member."""
    out, seen = [], set()
    for s in bf["symbols"]:
        kind = classify(s)
        if kind is None:
            continue
        if kind == "template":
            scope = _strip_args(s["scope"])
            key = f"{scope}::{s['name']}"
            if key in seen:
                continue
            seen.add(key)
            out.append({"kind": kind, "key": key, "name": s["name"], "scope": scope,
                        "mangled": s["mangled"], "addr": s["addr"]})
        else:
            out.append({"kind": kind, "key": s["mangled"], "name": s["name"], "scope": s.get("scope") or "",
                        "mangled": s["mangled"], "addr": s["addr"]})
    out.sort(key=lambda e: (e["kind"], e["key"]))
    return out


def counts(bf):
    c = {"l": 0, "template": 0, "thunk": 0}
    for e in entries(bf):
        c[e["kind"]] += 1
    c["total"] = sum(c.values())
    return c


class Index:
    """Lookups used by the marker pass."""

    def __init__(self, bf):
        ents = entries(bf)
        self.mangled = {e["mangled"] for e in ents if e["kind"] != "template"}
        self.template_keys = {e["key"] for e in ents if e["kind"] == "template"}

    def is_entry(self, mangled=None, scope=None, name=None):
        if mangled is not None and mangled in self.mangled:
            return True
        return scope is not None and f"{scope}::{name}" in self.template_keys
