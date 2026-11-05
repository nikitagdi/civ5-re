"""Rule discovery: built-in tools/checks/rules/*.py and plugins tools/<module>/checkrules.py."""
import importlib
import importlib.util
import inspect
import pkgutil
import sys
import tempfile
import traceback
from pathlib import Path

from . import api

PLUGIN_FILE = "checkrules.py"


class Loaded:
    """rules: [Rule]; errors: [str] (each names the file); modules: plugin module names that were found."""

    def __init__(self):
        self.rules = []
        self.errors = []
        self.modules = []


def _collect(mod, plugin):
    out = []
    for _, cls in inspect.getmembers(mod, inspect.isclass):
        if issubclass(cls, api.Rule) and cls is not api.Rule and cls.id and cls.__module__ == mod.__name__:
            r = cls()
            r.plugin = plugin
            out.append(r)
    return sorted(out, key=lambda r: r.id)


def _invalid(rule):
    if not api.ID_RE.match(rule.id):
        return f"rule id {rule.id!r} must match ^[A-Z]+\\d+$"
    if not rule.title:
        return f"rule {rule.id} has no title"
    bad = [n for n in rule.needs if n not in api.NEEDS]
    if bad:
        return f"rule {rule.id}: unknown needs {bad}"
    if rule.scope not in api.SCOPES:
        return f"rule {rule.id}: unknown scope {rule.scope!r}"
    if rule.staged is not None and rule.staged not in api.STAGED_KINDS:
        return f"rule {rule.id}: unknown staged {rule.staged!r}"
    if rule.over not in api.OVER:
        return f"rule {rule.id}: unknown over {rule.over!r}"
    return None


def _import_file(path, name):
    spec = importlib.util.spec_from_file_location(name, path)
    mod = importlib.util.module_from_spec(spec)
    sys.modules[name] = mod
    try:
        spec.loader.exec_module(mod)
    except BaseException:
        sys.modules.pop(name, None)
        raise
    return mod


def _brief(path, repo):
    try:
        return Path(path).resolve().relative_to(Path(repo).resolve()).as_posix()
    except ValueError:
        return Path(path).as_posix()


def _last_line(exc):
    tb = traceback.format_exception_only(type(exc), exc)
    return tb[-1].strip()


def load_builtin(repo):
    loaded = Loaded()
    from checks import rules as pkg
    for info in sorted(pkgutil.iter_modules(pkg.__path__), key=lambda i: i.name):
        file = Path(pkg.__path__[0]) / (info.name + ".py")
        name = f"checks.rules.{info.name}"
        try:
            sys.modules.pop(name, None)
            mod = importlib.import_module(name)
        except Exception as e:
            loaded.errors.append(f"{_brief(file, repo)}: cannot import: {_last_line(e)}")
            continue
        loaded.rules += _collect(mod, None)
    return loaded


def load_plugins(repo):
    loaded = Loaded()
    for file in sorted((Path(repo) / "tools").glob("*/" + PLUGIN_FILE)):
        module = file.parent.name
        if module == "checks":
            continue
        loaded.modules.append(module)
        try:
            mod = _import_file(file, f"_civ5re_plugin_{module}")
            loaded.rules += _collect(mod, module)
        except Exception as e:
            loaded.errors.append(f"{_brief(file, repo)}: cannot import: {_last_line(e)}")
    return loaded


def load_all(repo):
    """Built-in and plugin rules, validated; a duplicate id keeps the first and reports both files."""
    b, p = load_builtin(repo), load_plugins(repo)
    out = Loaded()
    out.errors = b.errors + p.errors
    out.modules = p.modules
    seen = {}
    for r in b.rules + p.rules:
        bad = _invalid(r)
        where = f"{'plugin ' + r.plugin if r.plugin else 'built-in'}"
        if bad:
            out.errors.append(f"{where}: {bad}")
            continue
        if r.id in seen:
            out.errors.append(f"duplicate rule id {r.id}: {seen[r.id]} and {where}")
            continue
        seen[r.id] = where
        out.rules.append(r)
    out.rules.sort(key=sort_key)
    return out


def sort_key(rule):
    letters = rule.id.rstrip("0123456789")
    return (rule.id == "W0", rule.plugin is not None, letters, int(rule.id[len(letters):]))


def fixture_dir(rule, repo, checks_dir):
    if rule.plugin:
        return Path(repo) / "tools" / rule.plugin / "fixtures" / rule.id
    return Path(checks_dir) / "fixtures" / rule.id


_GOOD = '''
from checks.api import Rule
class Z1(Rule):
    id = "Z1"
    title = "t"
    def run(self, ctx):
        return []
'''


def verify():
    errs = []
    with tempfile.TemporaryDirectory() as d:
        tools = Path(d) / "tools"
        for name, text in (("good", _GOOD), ("broken", "import no_such_module_xyz\n"),
                           ("dup", _GOOD.replace("Z1", "T1")), ("twin", _GOOD)):
            (tools / name).mkdir(parents=True)
            (tools / name / PLUGIN_FILE).write_text(text, encoding="utf-8")
        loaded = load_plugins(d)
        ids = sorted(r.id for r in loaded.rules)
        if ids != ["T1", "Z1", "Z1"]:
            errs.append(f"plugin rules: {ids}")
        if len(loaded.errors) != 1 or "broken" not in loaded.errors[0] or "checkrules.py" not in loaded.errors[0]:
            errs.append(f"broken plugin must name its file: {loaded.errors}")
        if sorted(loaded.modules) != ["broken", "dup", "good", "twin"]:
            errs.append(f"modules: {loaded.modules}")
    return errs
