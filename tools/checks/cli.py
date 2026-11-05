"""civ5re check / civ5re selftest."""
import sys
from pathlib import Path

from . import output, plugins, runner, selftest
from .context import Settings


def _csv(value):
    return [x.strip() for x in value.split(",") if x.strip()] if value else None


def _settings(args, mode):
    from civ5re import config
    cfg = config.load()
    return Settings(repo=cfg.root, cfg=cfg, mode=mode, root=getattr(args, "root", None),
                    files=getattr(args, "files", None) or [], only=_csv(getattr(args, "only", None)),
                    skip=_csv(getattr(args, "skip", None)) or [], json=getattr(args, "json", False),
                    jobs=getattr(args, "jobs", 0) or 0)


def _emit(settings, res, text=None):
    payload = res.to_json()
    if settings.write_last:
        output.write_json(settings.out / "last.json", payload)
    sys.stdout.write(output.render_json(payload) if settings.json else (text if text is not None
                                                                         else output.render_text(res)))
    return res.exit


def _list(settings):
    loaded = plugins.load_all(settings.repo)
    lines = []
    for r in loaded.rules:
        fixdir = plugins.fixture_dir(r, settings.repo, selftest.CHECKS_DIR)
        cases = selftest.discover_cases(fixdir)
        ok = sum(1 for c in cases if c.name.startswith("ok-"))
        bad = sum(1 for c in cases if c.name.startswith("bad-"))
        tag = f"plugin {r.plugin}" if r.plugin else "built-in"
        lines.append(f"{r.id:<5} {tag:<16} needs {','.join(r.needs) or '-':<22} fixtures ok {ok} bad {bad}  {r.title}")
    lines.append(f"rules {sum(1 for r in loaded.rules if not r.plugin)} + plugins "
                 f"{sum(1 for r in loaded.rules if r.plugin)} (modules: {', '.join(loaded.modules) or '-'})")
    lines += [f"civ5re: plugin error: {e}" for e in loaded.errors]
    sys.stdout.write("\n".join(lines) + "\n")
    return 2 if loaded.errors else 0


def _check(args):
    if args.root and args.staged:
        print("civ5re: --root cannot be combined with --staged", file=sys.stderr)
        return 2
    if args.files and (args.staged or args.root):
        print("civ5re: --files cannot be combined with --staged or --root", file=sys.stderr)
        return 2
    mode = "report" if args.report else "staged" if args.staged else "files" if args.files \
        else "root" if args.root else "full"
    settings = _settings(args, mode)
    if args.list:
        return _list(settings)
    if args.report:
        from . import report
        res, rep = report.run_report(settings)
        if settings.write_last:
            output.write_json(settings.out / "last.json", res.to_json())
        if settings.json:
            sys.stdout.write(output.render_json(rep if rep is not None else res.to_json()))
        else:
            sys.stdout.write(output.render_text(res) if rep is None else report.render_text(rep))
        return res.exit
    return _emit(settings, runner.run_check(settings))


def _selftest(args):
    settings = _settings(args, "full")
    loaded = plugins.load_all(settings.repo)
    only = _csv(args.only)
    st, tool_errors = selftest.run(settings, loaded, only)
    for line in output._selftest_lines(st):
        print(line)
    for e in tool_errors:
        print(f"civ5re: input {e}")
    return 2 if tool_errors else 3 if st["status"] == "failed" else 0


def register(subparsers):
    p = subparsers.add_parser("check", help="run the rule catalog over the tree")
    p.add_argument("--staged", action="store_true", help="git index and dependents (pre-commit)")
    p.add_argument("--files", nargs="+", metavar="FILE", help="these files and dependents")
    p.add_argument("--root", metavar="DIR", help="check the tree DIR/Civ5Expansion2")
    p.add_argument("--only", metavar="IDS", help="comma-separated rule ids")
    p.add_argument("--skip", metavar="IDS", help="comma-separated rule ids")
    p.add_argument("--json", action="store_true", help="machine-readable output")
    p.add_argument("--report", action="store_true", help="progress metrics")
    p.add_argument("--list", action="store_true", help="rules and plugins")
    p.add_argument("--jobs", type=int, default=0, metavar="N", help="worker count (default: all cores)")
    p.set_defaults(func=_check)
    s = subparsers.add_parser("selftest", help="run rule and plugin fixtures")
    s.add_argument("--only", metavar="IDS", help="comma-separated rule ids")
    s.add_argument("--jobs", type=int, default=0, metavar="N")
    s.set_defaults(func=_selftest)
