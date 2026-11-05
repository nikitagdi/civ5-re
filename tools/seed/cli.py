import importlib
import sys

from civ5re import config as _config


def _env(args):
    from seed import env
    cfg = _config.load()
    return env.run(_config.ROOT, cfg.path("seed", "lekmod_dir"), cfg.path("seed", "libcxx_src"),
                   cfg.path("seed", "libc_src"), relock=args.relock)


def _tree(args):
    try:
        module = importlib.import_module("seed.tree")
    except ModuleNotFoundError as e:
        if e.name != "seed.tree":
            raise
        print("civ5re seed: 'tree' is not implemented yet", file=sys.stderr)
        return 2
    return module.run(args)


def _usage(parser):
    def run(_args):
        parser.print_help()
        return 2
    return run


def register(subparsers):
    p = subparsers.add_parser("seed", help="third_party/ and the SDK copies (env), scaffolds of the exe classes (tree)")
    p.set_defaults(func=_usage(p))
    sub = p.add_subparsers(dest="seed_command", metavar="<step>")
    e = sub.add_parser("env", help="third_party/ (prelude, compat, libc++/glibc/Lua, include_dirs.txt, pch.h) "
                                   "and the SDK copies with markers")
    e.add_argument("--relock", action="store_true", help="recalculate inputs.lock (only on operator request)")
    e.set_defaults(func=_env)
    t = sub.add_parser("tree", help="scaffolds of the exe classes")
    t.add_argument("--out", help="write the result to another folder (preview)")
    t.add_argument("--dry-run", action="store_true", help="model and report only, no writes")
    t.add_argument("--relock", action="store_true", help="recalculate inputs.lock (only on operator request)")
    t.add_argument("--coverage", choices=("required", "report"), default="required",
                   help="report: write even if exports stay undeclared; export_coverage is not switched on")
    t.set_defaults(func=_tree)
