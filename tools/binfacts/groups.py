import re

from binfacts.api import BinfactsError

GROUPS = ("firaxis", "aspyr", "granny", "icu", "std", "lua", "thirdparty", "crt")
TARGETS = ("cpp", "c", "key", "mangled", "demangled")


class Rule:
    __slots__ = ("group", "target", "pattern", "regex", "line")

    def __init__(self, group, target, pattern, line):
        self.group, self.target, self.pattern, self.line = group, target, pattern, line
        try:
            self.regex = re.compile(pattern)
        except re.error as e:
            raise BinfactsError(f"groups.cfg:{line}: bad regex {pattern!r}: {e}") from None


def parse_rules(text):
    rules = []
    for n, raw in enumerate(text.splitlines(), 1):
        line = raw.strip()
        if not line or line.startswith("#"):
            continue
        parts = line.split(None, 2)
        if len(parts) != 3:
            raise BinfactsError(f"groups.cfg:{n}: expected `<group> <target> <regex>`")
        group, target, pattern = parts
        if group not in GROUPS:
            raise BinfactsError(f"groups.cfg:{n}: unknown group {group!r}")
        if target not in TARGETS:
            raise BinfactsError(f"groups.cfg:{n}: unknown target {target!r}")
        rules.append(Rule(group, target, pattern, n))
    return rules


def load_rules(path):
    with open(path, encoding="utf-8") as f:
        return parse_rules(f.read())


def _subject(rule_target, p):
    is_c = p.kind == "c"
    if rule_target == "cpp":
        return None if is_c else p.key
    if rule_target == "c":
        return p.mangled if is_c else None
    if rule_target == "key":
        return p.key
    if rule_target == "mangled":
        return p.mangled
    return p.demangled


def assign(rules, parsed):
    """Returns the group of each parsed symbol; raises with a list if any symbol has none."""
    out = []
    missing = []
    for p in parsed:
        group = None
        for r in rules:
            subject = _subject(r.target, p)
            if subject is not None and r.regex.search(subject):
                group = r.group
                break
        if group is None:
            missing.append(p.mangled)
        out.append(group)
    if missing:
        raise BinfactsError(f"{len(missing)} symbols have no group (add rules to groups.cfg):\n"
                            + "\n".join(missing[:50]))
    return out
