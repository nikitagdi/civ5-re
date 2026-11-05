import json
import sys
from pathlib import Path

from civ5re import config as _config


def _run(args):
    from typedb.build import build_db
    from typedb.compile import EnvError

    root = _config.ROOT
    tree = Path(args.tree) if args.tree else root / "Civ5Expansion2"
    if args.tree and (tree / "Civ5Expansion2").is_dir():
        tree = tree / "Civ5Expansion2"
    build = Path(args.build) if args.build else root / "build"
    third_party = Path(args.third_party) if args.third_party else None
    if third_party is None and args.tree and (tree.parent / "third_party").is_dir():
        third_party = tree.parent / "third_party"
    try:
        out = build_db(tree, build, markers="off" if args.no_markers else "strict", third_party=third_party,
                       force=args.force, keep=args.keep)
    except EnvError as e:
        print(f"civ5re typedb: {e}", file=sys.stderr)
        return 2
    db = json.loads(out.read_text(encoding="utf-8"))
    errs = [d for d in db["diagnostics"] if d["severity"] == "error"]
    for d in db["diagnostics"][:200]:
        print(f"{d['file']}:{d['line']}: {d['code']}: {d['message']}")
    if len(db["diagnostics"]) > 200:
        print(f"... {len(db['diagnostics']) - 200} more")
    s = db["stats"]["entities"]
    print(f"typedb: {out} - {s['records']} records, {s['functions']} functions, {s['fields']} fields, "
          f"{len(errs)} errors")
    return 1 if errs else 0


def register(subparsers):
    p = subparsers.add_parser("typedb", help=".h tree -> build/typedb.json")
    p.add_argument("--tree", help="tree root (default Civ5Expansion2/)")
    p.add_argument("--build", help="build directory (default build/)")
    p.add_argument("--third-party", dest="third_party",
                   help="third_party/ to use (default: next to --tree if present, else the repo's)")
    p.add_argument("--no-markers", action="store_true", help='markers="off"')
    p.add_argument("--force", action="store_true", help="ignore the cache")
    p.add_argument("--keep", action="store_true", help="keep the output of passes A/B")
    p.set_defaults(func=_run)
