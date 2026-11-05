"""Lua entry points from binfacts symbols only.

No typedb and no checks here: seed and the LUA rules both call `entry_points(binfacts)`.
"""
from __future__ import annotations

import re
from dataclasses import dataclass

LNAME = re.compile(r"^l[A-Za-z_]")
THUNK_PREFIX = "LuaThunk_"
LUA_STATE = "lua_State*"
KINDS = ("l", "template", "thunk")
STATUSES = ("marked", "none", "unknown", "unmarked", "undeclared")


@dataclass(frozen=True)
class EntryPoint:
    kind: str      # l | template | thunk
    symbol: str    # mangled name of the lowest-address symbol
    key: str       # l/thunk: mangled name; template: "Scope::name" without template arguments
    scope: str     # "" for thunks; templates: scope without template arguments
    name: str
    symbols: tuple  # every mangled name of this entry point
    addr: str      # lowest address, "0x%08x"

    @property
    def label(self) -> str:
        return f"{self.scope}::{self.name}" if self.scope else self.name


def strip_template_args(text: str) -> str:
    out, depth = [], 0
    for ch in text:
        if ch == "<":
            depth += 1
        elif ch == ">":
            depth = max(0, depth - 1)
        elif depth == 0:
            out.append(ch)
    return "".join(out)


def entry_points(binfacts) -> list:
    """Entry points of the Lua API: 847 `l*`, 10 template members, 313 thunks on the real binary."""
    single: dict = {}
    grouped: dict = {}
    for s in binfacts.get("symbols", ()):
        if s.get("group") != "firaxis" or s.get("params") != LUA_STATE:
            continue
        name = s.get("name") or ""
        scope = s.get("scope") or ""
        if not scope and name.startswith(THUNK_PREFIX):
            single.setdefault(s["mangled"], ("thunk", "", name, s))
        elif scope and LNAME.match(name):
            if "<" in scope:
                bare = strip_template_args(scope)
                grouped.setdefault((bare, name), []).append(s)
            else:
                single.setdefault(s["mangled"], ("l", scope, name, s))
    out = [EntryPoint(kind, s["mangled"], s["mangled"], scope, name, (s["mangled"],), s.get("addr") or "")
           for kind, scope, name, s in single.values()]
    for (bare, name), syms in grouped.items():
        syms = sorted(syms, key=lambda x: (x.get("addr") or "", x["mangled"]))
        out.append(EntryPoint("template", syms[0]["mangled"], f"{bare}::{name}", bare, name,
                              tuple(x["mangled"] for x in syms), syms[0].get("addr") or ""))
    out.sort(key=lambda e: (KINDS.index(e.kind), e.scope, e.name, e.key))
    return out


def thunk_rest(name: str) -> str:
    return name[len(THUNK_PREFIX):] if name.startswith(THUNK_PREFIX) else name


def thunk_path(name: str, classes: dict) -> str:
    """`LuaThunk_<C>_<N>` -> `<Owner>:<N>` for the longest `C` in `classes` (C++ class -> Lua class);
    otherwise `LuaThunk_<N>` -> the global `N`."""
    rest = thunk_rest(name)
    for i in range(len(rest) - 1, 0, -1):
        if rest[i] == "_" and rest[:i] in classes and i + 1 < len(rest):
            return f"{classes[rest[:i]]}:{rest[i + 1:]}"
    return rest


def thunk_l_candidates(eps) -> dict:
    """thunk key -> `l*` entry point key whose name matches: `C::lN` for `LuaThunk_C_N`, or a lone `lN`."""
    ls = [e for e in eps if e.kind == "l"]
    by_pair = {(e.scope, e.name): e for e in ls}
    by_name: dict = {}
    for e in ls:
        by_name.setdefault(e.name, []).append(e)
    out = {}
    for t in eps:
        if t.kind != "thunk":
            continue
        rest = thunk_rest(t.name)
        hit = None
        for i, ch in enumerate(rest):
            if ch == "_" and (rest[:i], "l" + rest[i + 1:]) in by_pair:
                hit = by_pair[(rest[:i], "l" + rest[i + 1:])]
        if hit is None and len(by_name.get("l" + rest, ())) == 1:
            hit = by_name["l" + rest][0]
        if hit is not None:
            out[t.key] = hit.key
    return out


def classify(eps, *, find, kinds, paths, classes) -> dict:
    """entry key -> (status, declaration).

    find(ep) -> typedb function or None; kinds(decl) -> set of marker kinds on it ('function', '?', 'none');
    paths: Lua paths that have a full line; classes: C++ class -> Lua class (thunk link by Lua path).
    """
    out = {}
    for ep in eps:
        decl = find(ep)
        if decl is None:
            out[ep.key] = ("undeclared", None)
            continue
        k = kinds(decl)
        if ep.kind == "thunk" and thunk_path(ep.name, classes) in paths:
            st = "marked"
        elif "function" in k:
            st = "marked"
        elif "none" in k:
            st = "none"
        elif "?" in k:
            st = "unknown"
        else:
            st = "unmarked"
        out[ep.key] = (st, decl)
    return out


def count_statuses(results) -> dict:
    c = {s: 0 for s in STATUSES}
    for st, _ in results.values():
        c[st] += 1
    return c
