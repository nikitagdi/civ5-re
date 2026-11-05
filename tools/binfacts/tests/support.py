import json
from pathlib import Path

FIXTURES = Path(__file__).resolve().parent / "fixtures"
FIXTURE_ELF = FIXTURES / "fixture.elf"


def llvm_bin():
    """Directory with clang++, ld.lld and llvm-cxxfilt, or None."""
    from civ5re import config
    try:
        cfg = config.load()
        cxxfilt = cfg.tool("llvm-cxxfilt")
    except config.ConfigError:
        return None
    d = cxxfilt.parent
    names = ("llvm-cxxfilt", "clang++", "ld.lld")
    ok = all(cfg.tool(n).is_file() for n in names)
    return d if ok else None


def load_expected():
    with open(FIXTURES / "expected.json", "rb") as f:
        return json.load(f)


class TableDemangler:
    """Demangler answering from the committed expected.json, so structure tests need no LLVM."""

    def __init__(self, facts=None):
        facts = facts or load_expected()
        self.table = {s["mangled"]: s["demangled"] for s in facts["symbols"]}

    def many(self, names):
        return [self.table[n] for n in names]

    def one(self, name):
        return self.table[name]

    def get_version(self):
        return "table"
