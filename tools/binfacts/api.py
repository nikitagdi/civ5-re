SCHEMA = 1


class BinfactsError(Exception):
    pass


def load(path=None):
    """Returns the binfacts dict.

    Without a path: build/binfacts.json, rebuilt when stale if the binary and llvm-cxxfilt are available.
    With a path: that file as is; only the schema is checked.
    """
    from binfacts import build
    if path is not None:
        return build.read(path)
    from civ5re import config
    facts, _action = build.ensure(config.load())
    return facts
