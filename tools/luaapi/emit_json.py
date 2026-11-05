"""build/lua-api.json: stable order, no timestamps."""
from __future__ import annotations

import json


def dumps(model: dict) -> str:
    return json.dumps(model, indent=1, ensure_ascii=False) + "\n"


def report(model: dict, mismatches=(), top: int = 50) -> str:
    cov = model["coverage"]
    lines = ["lua-api coverage" + (" (partial: no binfacts, declared functions only)" if cov["partial"] else ""), ""]
    lines.append(f"entry points {cov['entry_points']}: marked {cov['marked']}, none {cov['none']}, "
                 f"unknown {cov['unknown']}, unmarked {cov['unmarked']}, undeclared {cov['undeclared']}")
    ev, pl = cov["events"], cov["payloads"]
    lines.append(f"events: in binary {ev['in_binary']}, declared {ev['declared']}, with arguments {ev['with_args']}")
    lines.append(f"payloads: ToLua {pl['to_lua']}, push {pl['push']}, FromLua {pl['from_lua']}, pull {pl['pull']}")
    lines += ["", f"{'cxx scope':44} {'lua':20} {'total':>5} {'marked':>6} {'none':>5} {'unk':>5} {'missing':>7}"]
    for o in cov["by_owner"]:
        lines.append(f"{o['cxx_scope'][:44]:44} {(o['lua'] or '-')[:20]:20} {o['total']:>5} {o['marked']:>6} "
                     f"{o['none']:>5} {o['unknown']:>5} {o['missing']:>7}")

    def calls(m):
        u = m["game_uses"]
        return (u["dot"] + u["method"]) if u else 0

    unc = sorted(model["missing"], key=lambda m: (-calls(m), m["cxx"], m["mangled"]))[:top]
    lines += ["", f"uncovered entry points, most calls in the game's Lua first (top {top}):"]
    for m in unc:
        lines.append(f"{calls(m):>6}  {m['status']:10} {m['cxx']}" + (f"  -> {m['hint']}" if m["hint"] else ""))
    if mismatches:
        lines += ["", "usage mismatches:"] + [f"  {x}" for x in mismatches]
    errs = [d for d in model["diagnostics"] if d["severity"] == "error"]
    warns = len(model["diagnostics"]) - len(errs)
    lines += ["", f"diagnostics: {len(errs)} errors, {warns} warnings"]
    for d in model["diagnostics"][:200]:
        lines.append(f"  {d['file']}:{d['line']}: {d['rule']}" + (" [warn]" if d["severity"] == "warn" else "")
                     + f" {d['message']}")
    return "\n".join(lines) + "\n"
