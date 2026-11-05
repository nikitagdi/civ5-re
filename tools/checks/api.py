"""Public API for rule authors: Rule, Finding, Context, InputError, RuleSkip.

A rule file lives in tools/checks/rules/ (built-in) or tools/<module>/checkrules.py (plugin).
Every subclass of Rule with a non-empty `id` defined in the file is registered; nothing else is needed.
"""
import dataclasses
import re

ID_RE = re.compile(r"^[A-Z]+\d+$")
ERROR, WARN = "error", "warn"
NEEDS = ("files", "typedb", "binfacts", "clang", "ghidra")
SCOPES = ("file", "entity", "tree")
STAGED_KINDS = ("self", "closure", "tree", "always", "full")
OVER = ("tree", "any")


class InputError(Exception):
    """A required input is missing or broken: the run ends with exit code 2 naming `name`."""

    def __init__(self, name, reason):
        super().__init__(f"{name}: {reason}")
        self.name = name
        self.reason = reason


class RuleSkip(Exception):
    """Raised by a rule that cannot run for a stated reason; the rule is printed as skipped."""


JSON_KEYS = ("rule", "severity", "path", "line", "message", "code", "detail",
             "entity", "mangled", "expected", "actual", "key", "waived")


@dataclasses.dataclass
class Finding:
    rule: str = ""
    severity: str = ERROR
    path: str = None
    line: int = None
    message: str = ""
    code: str = None
    detail: list = dataclasses.field(default_factory=list)
    entity: str = None
    mangled: str = None
    expected: str = None
    actual: str = None
    key: str = None
    waived: bool = False
    fatal: bool = False

    def to_json(self):
        return {k: getattr(self, k) for k in JSON_KEYS}


class Rule:
    """id: ^[A-Z]+\\d+$ · title: what it checks · needs: subset of NEEDS
    scope: file | entity | tree
    staged: self | closure | tree | always | full (default from scope: file->self, entity->closure, tree->tree)
        self: only the changed files · closure: changed files and their includers
        tree: whole tree is analysed, only findings in the closure are shown · always: whole tree, all shown
        full: runs only when the whole tree is checked
    over: tree (Civ5Expansion2 sources) | any (all changed files); selects which changes trigger a staged run
    deep: needs layout/vtable data of typedb; skipped when typedb has a clang-error
    fixture_with: ids of other rules that must also run in this rule's fixtures"""

    id = ""
    title = ""
    needs = ()
    scope = "file"
    staged = None
    over = "tree"
    deep = False
    fixture_with = ()
    plugin = None

    def run(self, ctx):
        raise NotImplementedError

    @property
    def staged_kind(self):
        return self.staged or {"file": "self", "entity": "closure", "tree": "tree"}[self.scope]

    def finding(self, path, line, message, **kw):
        kw.setdefault("severity", ERROR)
        return Finding(rule=self.id, path=path, line=line, message=message, **kw)

    def warn(self, path, line, message, **kw):
        return self.finding(path, line, message, severity=WARN, **kw)


def __getattr__(name):
    if name == "Context":
        from .context import Context
        return Context
    raise AttributeError(name)
