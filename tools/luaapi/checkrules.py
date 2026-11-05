"""checks plugin: rules LUA1..LUA9 over the `/// lua` markers."""
from checks.api import Rule

from luaapi import model


def _scan(ctx):
    cache = ctx.__dict__.setdefault("_luaapi", {})
    if "scan" not in cache:
        cache["scan"] = model.Scan(ctx.typedb)
    return cache["scan"]


def _diags(ctx, rule_id, facts):
    cache = ctx.__dict__.setdefault("_luaapi", {})
    key = (rule_id, facts)
    if key not in cache:
        cache[key] = model.lint(_scan(ctx), ctx.binfacts if facts else None, only={rule_id})
    return cache[key]


class _Lua(Rule):
    facts = False
    scope = "file"

    def run(self, ctx):
        out = []
        for d in _diags(ctx, self.id, self.facts):
            if self.scope == "file" and not ctx.in_scope(d.file):
                continue
            out.append(self.finding(d.file, d.line, d.message, severity=d.severity))
        return out


class LUA1(_Lua):
    id = "LUA1"
    title = "a '/// lua' line parses and is in canonical form"
    needs = ("typedb",)


class LUA2(_Lua):
    id = "LUA2"
    title = "the lua marker kind fits the entity; LuaThunk_* takes only '?', LuaToString_* nothing"
    needs = ("typedb",)


class LUA3(_Lua):
    id = "LUA3"
    title = "lua types name a class, enum, shape or gamecore class; EventArgs only in Event"
    needs = ("typedb",)


class LUA4(_Lua):
    id = "LUA4"
    title = "a function's Lua owner is the table or class of its C++ class or a derived class"
    needs = ("typedb",)


class LUA5(_Lua):
    id = "LUA5"
    title = "one Lua path per C++ entity; type names, event names and (container, id) are unique"
    needs = ("typedb",)
    scope = "tree"


class LUA6(_Lua):
    id = "LUA6"
    title = "one line per LuaThunk_* path, no '?' beside it, target arguments fit the lines"
    needs = ("typedb", "binfacts")
    scope = "tree"
    facts = True


class LUA7(_Lua):
    id = "LUA7"
    title = "every Lua entry point has a '/// lua' line (at least '?')"
    needs = ("typedb", "binfacts")
    scope = "tree"
    facts = True


class LUA8(_Lua):
    id = "LUA8"
    title = "an event typedef is an event template with a CvLuaEvent instantiation and push/pull"
    needs = ("typedb", "binfacts")
    scope = "tree"
    facts = True


class LUA9(_Lua):
    id = "LUA9"
    title = "'/// lua none' has an ordinary comment saying why"
    needs = ("typedb",)
