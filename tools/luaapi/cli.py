"""`civ5re lua-api`: build/lua-api.json, build/lua-stubs/, build/lua-api-coverage.txt (+ copy to --out)."""
from __future__ import annotations

import hashlib
import json
import sys
from pathlib import Path

from civ5re import config as _config

STUBS = "lua-stubs"
JSON_NAME = "lua-api.json"
REPORT_NAME = "lua-api-coverage.txt"


def _err(msg):
    print(f"civ5re lua-api: {msg}", file=sys.stderr)


def _load_typedb(root):
    from typedb import api
    path = root / "build" / "typedb.json"
    if not path.is_file():
        _err(f"{path} not found: run `civ5re typedb` (or `civ5re seed tree`) first")
        return None, None
    try:
        db = api.load(path)
    except api.SchemaError as e:
        _err(f"{e}: run `civ5re typedb`")
        return None, None
    return db, hashlib.sha256(path.read_bytes()).hexdigest()


def _load_binfacts(root):
    path = root / "build" / "binfacts.json"
    if not path.is_file():
        print("lua-api: build/binfacts.json not found: coverage is partial (run `civ5re binfacts`)")
        return None
    from binfacts import api
    try:
        return api.load(str(path))
    except Exception as e:  # noqa: BLE001 - a broken binfacts only makes the export partial
        print(f"lua-api: cannot read build/binfacts.json ({e}): coverage is partial")
        return None


def _write(base: Path, files: dict):
    for rel, text in files.items():
        p = base / rel
        p.parent.mkdir(parents=True, exist_ok=True)
        p.write_bytes(text.encode("utf-8"))


def _write_all(base: Path, json_text, report_text, stubs):
    stub_dir = base / STUBS
    if stub_dir.is_dir():
        for f in sorted(stub_dir.rglob("*"), reverse=True):
            rel = f.relative_to(stub_dir).as_posix()
            if f.is_file() and rel not in stubs:
                f.unlink()
    _write(base, {JSON_NAME: json_text, REPORT_NAME: report_text,
                  **{f"{STUBS}/{k}": v for k, v in stubs.items()}})


def _show(model, path):
    hits = [f for f in model["functions"] if f["path"] == path]
    miss = [m for m in model["missing"] if path in (m["cxx"], m["hint"])]
    if not hits and not miss:
        _err(f"nothing named {path}")
        return 1
    for f in hits:
        c = f["cxx"]
        print(f"{f['path']}  [{f['bind']}]  /// lua {f['raw']}")
        print(f"  C++    {c['qualified']}  {c['file']}:{c['line']}  mangled {c['mangled']}  addr {c['addr']}")
        if f["thunk"]:
            print(f"  thunk  {f['thunk']['name']}  {f['thunk']['addr']}")
        if f["game_uses"] is not None:
            print(f"  calls  dot {f['game_uses']['dot']}, method {f['game_uses']['method']}")
        if f["doc"]:
            print(f"  doc    {f['doc']}")
    for m in miss:
        print(f"{m['cxx']}  [{m['status']}]  {m['mangled']}  addr {m['addr']}" + (f"  hint {m['hint']}" if m["hint"] else ""))
    return 0


def _run(args):
    return execute(_config.load(), args)


def execute(cfg, args):
    from luaapi import emit_json, emit_stubs, model, usage as usage_mod
    root = Path(cfg.root)
    db, tsha = _load_typedb(root)
    if db is None:
        return 2
    facts = _load_binfacts(root)
    usage = None
    if args.no_usage:
        print("lua-api: usage scan skipped (--no-usage)")
    else:
        game = cfg.path("game_dir")
        if game is None or not game.is_dir():
            print("lua-api: game_dir is empty or missing: usage scan skipped")
        else:
            usage = usage_mod.scan_dir(game)
            print(f"lua-api: scanned {usage.files} game Lua files")
    udict = usage.to_dict() if usage is not None else None
    inputs = {"typedb_sha256": tsha, "binfacts_sha256": (facts.get("binary") or {}).get("sha256") if facts else None}
    doc = model.build(db, facts, usage=udict, inputs=inputs)
    if args.show:
        return _show(doc, args.show)
    json_text = emit_json.dumps(doc)
    report_text = emit_json.report(doc, usage_mod.mismatches(doc, udict) if udict is not None else ())
    stubs = emit_stubs.generate(doc)
    build = root / "build"
    _write_all(build, json_text, report_text, stubs)
    out = Path(args.out) if args.out else cfg.path("lua-api", "out")
    if out is not None:
        _write_all(out, json_text, report_text, stubs)
    cov = doc["coverage"]
    errs = [d for d in doc["diagnostics"] if d["severity"] == "error"]
    print(f"lua-api: {len(doc['functions'])} functions, {len(doc['events'])} events, {len(doc['types'])} types; "
          f"entry points {cov['entry_points']} (marked {cov['marked']}, none {cov['none']}, unknown {cov['unknown']}, "
          f"unmarked {cov['unmarked']}, undeclared {cov['undeclared']})"
          + (", partial" if cov["partial"] else ""))
    print(f"lua-api: {build / JSON_NAME}, {build / STUBS}/ ({len(stubs)} files), {build / REPORT_NAME}"
          + (f"; copy in {out}" if out is not None else ""))
    for d in doc["diagnostics"][:50]:
        print(f"{d['file']}:{d['line']}: {d['rule']}" + (" [warn]" if d["severity"] == "warn" else "")
              + f" {d['message']}")
    print(f"lua-api: {len(errs)} errors, {len(doc['diagnostics']) - len(errs)} warnings")
    return 1 if errs else 0


def register(subparsers):
    p = subparsers.add_parser("lua-api", help="Lua API contract -> build/lua-api.json, LuaLS stubs, coverage")
    p.add_argument("--no-usage", action="store_true", help="do not read the game's Lua")
    p.add_argument("--out", help="also copy the output to DIR (default: [lua-api] out)")
    p.add_argument("--show", metavar="PATH", help="print one entry (e.g. UI.GetHeadSelectedUnit)")
    p.set_defaults(func=_run)
