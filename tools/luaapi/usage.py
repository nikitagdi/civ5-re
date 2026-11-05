"""Scanner of the game's Lua: call counters only (report and `game_uses`); no game text is kept."""
from __future__ import annotations

import os
import re
from pathlib import Path

CALL = re.compile(r"(?<![\w.:])([A-Za-z_]\w*(?:\.[A-Za-z_]\w*)*)([.:])([A-Za-z_]\w*)[ \t]*\(([ \t]*)(\)?)")
LONG_COMMENT = re.compile(r"--\[(=*)\[.*?\]\1\]", re.S)
LINE_COMMENT = re.compile(r"--[^\n]*")
EVENT_LIBS = ("Events", "LuaEvents", "GameEvents")


def strip_comments(text: str) -> str:
    return LINE_COMMENT.sub("", LONG_COMMENT.sub("", text))


class Usage:
    """members: owner -> name -> {dot, colon, colon_args}; controls: method -> n (`Controls.X:M(`);
    events: name -> {Add, Remove, CallImmediate, call, ...}; luaevents/gameevents: name -> n."""

    def __init__(self):
        self.files = 0
        self.members = {}
        self.controls = {}
        self.events = {}
        self.luaevents = {}
        self.gameevents = {}

    def _member(self, owner, name):
        return self.members.setdefault(owner, {}).setdefault(name, {"dot": 0, "colon": 0, "colon_args": 0})

    def add_text(self, text: str):
        self.files += 1
        for m in CALL.finditer(strip_comments(text)):
            chain, sep, name, _, close = m.groups()
            has_args = close != ")"
            head, _, tail = chain.partition(".")
            if head in EVENT_LIBS and not tail:
                table = {"Events": self.events, "LuaEvents": self.luaevents, "GameEvents": self.gameevents}[head]
                if head == "Events":
                    ev = table.setdefault(name, {})
                    ev["call"] = ev.get("call", 0) + 1
                else:
                    table[name] = table.get(name, 0) + 1
                continue
            if head in EVENT_LIBS and tail and "." not in tail:
                table = {"Events": self.events, "LuaEvents": self.luaevents, "GameEvents": self.gameevents}[head]
                if head == "Events":
                    ev = table.setdefault(tail, {})
                    ev[name] = ev.get(name, 0) + 1
                else:
                    table[tail] = table.get(tail, 0) + 1
                continue
            if head == "Controls" and tail and "." not in tail and sep == ":":
                self.controls[name] = self.controls.get(name, 0) + 1
                continue
            d = self._member(chain, name)
            if sep == ".":
                d["dot"] += 1
            else:
                d["colon"] += 1
                d["colon_args"] += has_args

    def add_file(self, path):
        self.add_text(Path(path).read_bytes().decode("utf-8-sig", "replace"))

    def to_dict(self) -> dict:
        return {"files": self.files, "members": self.members, "controls": self.controls,
                "events": self.events, "luaevents": self.luaevents, "gameevents": self.gameevents}


def lua_files(game_dir) -> list:
    root = Path(game_dir) / "Assets"
    out = []
    if root.is_dir():
        for d, dirs, names in os.walk(root):
            dirs[:] = sorted(dirs)
            out += [Path(d, n) for n in sorted(names) if n.lower().endswith(".lua")]
    return out


def scan_dir(game_dir) -> Usage:
    u = Usage()
    for f in lua_files(game_dir):
        u.add_file(f)
    return u


def mismatches(model: dict, usage: dict) -> list:
    """Lines for the report: `.`/`:` disagreements and calls to undeclared members of exe tables."""
    out = []
    members = usage.get("members", {})
    declared = {}
    for f in model["functions"]:
        if f["owner"] is not None:
            declared.setdefault(f["owner"], {}).setdefault(f["name"], f)
    tables = {t["name"] for t in model["types"] if t["kind"] == "table"}
    for owner in sorted(members):
        for name in sorted(members[owner]):
            u = members[owner][name]
            f = declared.get(owner, {}).get(name)
            if f is None:
                if owner in tables:
                    out.append(f"undeclared  {owner}.{name}  dot {u['dot']} colon {u['colon']}")
                continue
            if f["call"] == "dot" and u["colon_args"]:
                out.append(f"colon-with-args  {owner}:{name}  {u['colon_args']} calls, declared with '.'")
            elif f["call"] == "method" and u["dot"] and owner in {g["name"] for g in model["globals"]}:
                out.append(f"dot  {owner}.{name}  {u['dot']} calls, declared with ':'")
    return out
