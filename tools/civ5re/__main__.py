import argparse
import importlib
import sys

from civ5re.config import ConfigError

MODULES = ["binfacts", "typedb", "ghidra", "checks", "seed", "luaapi"]


class DispatchError(Exception):
    pass


class _Subparsers:
    def __init__(self, inner):
        self._inner = inner
        self.names = set()

    def add_parser(self, name, **kwargs):
        if name in self.names:
            raise DispatchError(f"duplicate command: {name}")
        self.names.add(name)
        return self._inner.add_parser(name, **kwargs)


def _import_cli(module):
    try:
        return importlib.import_module(f"{module}.cli")
    except ModuleNotFoundError as e:
        if e.name in (module, f"{module}.cli"):
            return None
        raise


def build_parser(modules=None, import_cli=None):
    import_cli = import_cli or _import_cli
    parser = argparse.ArgumentParser(prog="civ5re")
    subparsers = _Subparsers(parser.add_subparsers(dest="command", metavar="<command>"))
    for module in MODULES if modules is None else modules:
        cli = import_cli(module)
        if cli is not None:
            cli.register(subparsers)
    return parser


def _domain_errors():
    errors = [ConfigError, DispatchError]
    try:
        from binfacts.api import BinfactsError
        errors.append(BinfactsError)
    except ImportError:
        pass
    return tuple(errors)


def main(argv=None):
    try:
        parser = build_parser()
    except DispatchError as e:
        print(f"civ5re: {e}", file=sys.stderr)
        return 2
    args = parser.parse_args(argv)
    if not getattr(args, "func", None):
        parser.print_help()
        return 2
    try:
        return args.func(args)
    except _domain_errors() as e:
        print(f"civ5re: {e}", file=sys.stderr)
        return 2
    except Exception:
        import traceback
        traceback.print_exc()
        return 2


if __name__ == "__main__":
    sys.exit(main())
