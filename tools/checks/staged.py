"""--staged / --files: the git index, the file-class table and the dependent closure."""
import fnmatch
import subprocess
from collections import namedtuple

from . import includes

PARTIAL = "partial indexing is not supported: `git add` or `git stash -k`"

Change = namedtuple("Change", "status path")
Scope = namedtuple("Scope", "run show")  # run: None = whole tree; show: None = everything

FULL_GLOBS = (
    "tools/checks/*", "tools/typedb/*", "tools/binfacts/*", "tools/civ5re/placeholders.py",
    "tools/*/checkrules.py", "tools/*/fixtures/*", "third_party/*", "bins.lock", "civ5re.cfg",
)
EVIDENCE = ("tools/seed/data/source_paths_unique.txt", "tools/seed/data/sdk_lib_paths.txt",
            "tools/seed/data/sdk_map.txt")
EVIDENCE_RULES = ("O2", "O3", "O4")


class StagedError(Exception):
    pass


def classify(path):
    if includes.is_tree_source(path):
        return "tree" + path[path.rindex("."):]
    if any(fnmatch.fnmatchcase(path, g) for g in FULL_GLOBS):
        return "full"
    if path in EVIDENCE:
        return "evidence"
    return "other"


def _git(repo, *args):
    try:
        r = subprocess.run(["git", *args], cwd=repo, capture_output=True)
    except OSError as e:
        raise StagedError(f"git is not available: {e}") from None
    if r.returncode != 0:
        raise StagedError(f"git {' '.join(args[:2])} failed: {r.stderr.decode('utf-8', 'replace').strip()}")
    return r.stdout.decode("utf-8", "surrogateescape")


def parse_name_status(text):
    parts = [p for p in text.split("\0")]
    if parts and parts[-1] == "":
        parts.pop()
    out, i = [], 0
    while i < len(parts):
        st = parts[i]
        n = 2 if st[0] in "RC" else 1
        paths = parts[i + 1:i + 1 + n]
        if len(paths) != n:
            raise StagedError(f"cannot parse git name-status output near {st!r}")
        if st[0] == "R":
            out.append(Change("D", paths[0]))
            out.append(Change("A", paths[1]))
        else:
            out.append(Change(st[0], paths[-1]))
        i += 1 + n
    return out


def index_changes(repo):
    """Changes in the index; raises StagedError on partial indexing."""
    changes = parse_name_status(_git(repo, "diff", "--cached", "--name-status", "-z", "--no-renames"))
    staged = {c.path for c in changes}
    dirty = set(p for p in _git(repo, "diff", "--name-only", "-z").split("\0") if p)
    both = sorted(staged & dirty)
    if both:
        raise StagedError(f"{PARTIAL}: {', '.join(both)}")
    return changes


def tracked(repo):
    return set(p for p in _git(repo, "ls-files", "-z").split("\0") if p)


class Plan:
    def __init__(self, full, reason, changed, closure, tree_changed, evidence):
        self.full = full
        self.reason = reason
        self.changed = changed
        self.closure = closure
        self.tree_changed = tree_changed
        self.evidence = evidence

    def scope(self, rule):
        """Scope for a rule, or None when the change does not trigger it."""
        if self.full:
            return Scope(None, None)
        if rule.id in EVIDENCE_RULES and self.evidence:
            return Scope(None, None)
        kind = rule.staged_kind
        if kind == "full":
            return None
        if kind == "self":
            run = self.changed if rule.over == "any" else self.tree_changed
            return Scope(run, run) if run else None
        if kind == "closure":
            return Scope(self.closure, self.closure) if self.tree_changed else None
        if kind == "tree":
            return Scope(None, self.closure) if self.tree_changed else None
        return Scope(None, None) if self.tree_changed else None  # always


def plan(changes, graph, tracked_paths=None):
    """changes: [Change]; graph: includes.Graph over the working tree; tracked_paths: for the untracked check."""
    changed = frozenset(c.path for c in changes if c.status != "D")
    why = None
    for c in changes:
        if c.status == "D":
            why = why or f"{c.path} deleted or renamed"
        elif classify(c.path) == "full":
            why = why or f"{c.path} changed"
    tree_changed = frozenset(p for p in changed if includes.is_tree_source(p))
    closure = set()
    for p in tree_changed:
        closure.add(p)
        if not p.endswith(".cpp"):
            closure |= graph.includers([p])
    if tracked_paths is not None:
        reach = closure | graph.includes(tree_changed)
        bad = sorted(p for p in reach if p not in tracked_paths)
        if bad:
            raise StagedError(f"untracked file in the include closure: {', '.join(bad)} "
                              "(git add it or remove the include)")
    return Plan(bool(why), why, changed, frozenset(closure), tree_changed,
                any(classify(p) == "evidence" for p in changed))


def verify():
    errs = []
    cases = {
        "Civ5Expansion2/a.h": "tree.h", "Civ5Expansion2/a.cpp": "tree.cpp", "Civ5Expansion2/a.inl": "tree.inl",
        "Civ5Expansion2/a.txt": "other", "tools/checks/runner.py": "full", "tools/typedb/x/y.py": "full",
        "tools/lua/checkrules.py": "full", "tools/luaapi/fixtures/LUA1/ok/x": "full", "third_party/pch.h": "full",
        "bins.lock": "full", "civ5re.cfg": "full", "tools/checks/waivers.json": "full",
        "tools/seed/data/sdk_map.txt": "evidence", "tools/seed/data/win_rel_all.txt": "other",
        "NOTICE.md": "other", "README.md": "other",
    }
    for p, want in cases.items():
        if classify(p) != want:
            errs.append(f"classify({p}) = {classify(p)}, want {want}")
    st = parse_name_status("M\0a.h\0D\0b.h\0R100\0c.h\0d.h\0")
    if st != [Change("M", "a.h"), Change("D", "b.h"), Change("D", "c.h"), Change("A", "d.h")]:
        errs.append(f"name-status parsing: {st}")
    return errs
