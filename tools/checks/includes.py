"""#include "..." graph of the tree (regex based, no preprocessor)."""
import posixpath
import re
from collections import namedtuple

TREE = "Civ5Expansion2/"
SOURCE_EXT = (".h", ".inl", ".cpp")
THIRD = "third_party/"
INCLUDE_RE = re.compile(r'^[ \t]*#[ \t]*include[ \t]*"([^"\r\n]*)"', re.M)

Edge = namedtuple("Edge", "line spec target")


def is_tree_source(path):
    return path.startswith(TREE) and path.endswith(SOURCE_EXT)


def resolve(path, spec, exists, third_dirs=()):
    """First the file's directory, then Civ5Expansion2/, then third_party/. Returns a repo path or None."""
    cands = []
    local = posixpath.normpath(posixpath.join(posixpath.dirname(path), spec))
    if not local.startswith(".."):
        cands.append(local)
    if not spec.startswith(".."):
        cands.append(posixpath.normpath(TREE + spec))
        cands.append(THIRD + spec)
        cands += [f"{THIRD}{d}/{spec}" for d in third_dirs]
    for c in cands:
        if exists(c):
            return c
    return None


class Graph:
    def __init__(self, edges):
        self.edges = edges
        self.reverse = {}
        for src, es in edges.items():
            for e in es:
                if e.target in edges:
                    self.reverse.setdefault(e.target, set()).add(src)

    def includers(self, seeds):
        """Seeds and everything that includes them transitively."""
        out, todo = set(seeds), list(seeds)
        while todo:
            for p in self.reverse.get(todo.pop(), ()):
                if p not in out:
                    out.add(p)
                    todo.append(p)
        return out

    def includes(self, seeds):
        """Seeds and everything they include transitively (third_party targets are leaves)."""
        out, todo = set(seeds), list(seeds)
        while todo:
            for e in self.edges.get(todo.pop(), ()):
                if e.target and e.target not in out:
                    out.add(e.target)
                    todo.append(e.target)
        return out

    def cycle_edges(self):
        """[(src, Edge)] for every include edge that lies on a cycle."""
        comp = self._components()
        size = {}
        for c in comp.values():
            size[c] = size.get(c, 0) + 1
        out = []
        for src in sorted(self.edges):
            for e in self.edges[src]:
                if e.target in comp and comp[e.target] == comp[src] and (size[comp[src]] > 1 or e.target == src):
                    out.append((src, e))
        return out

    def components(self):
        """{path: component id} (strongly connected components)."""
        return self._components()

    def _components(self):
        index, low, onstack, stack, comp = {}, {}, set(), [], {}
        counter = [0, 0]
        for root in sorted(self.edges):
            if root in index:
                continue
            work = [(root, iter(self._succ(root)))]
            index[root] = low[root] = counter[0]
            counter[0] += 1
            stack.append(root)
            onstack.add(root)
            while work:
                v, it = work[-1]
                for w in it:
                    if w not in index:
                        index[w] = low[w] = counter[0]
                        counter[0] += 1
                        stack.append(w)
                        onstack.add(w)
                        work.append((w, iter(self._succ(w))))
                        break
                    if w in onstack:
                        low[v] = min(low[v], index[w])
                else:
                    work.pop()
                    if work:
                        p = work[-1][0]
                        low[p] = min(low[p], low[v])
                    if low[v] == index[v]:
                        while True:
                            w = stack.pop()
                            onstack.discard(w)
                            comp[w] = counter[1]
                            if w == v:
                                break
                        counter[1] += 1
        return comp

    def _succ(self, v):
        return [e.target for e in self.edges.get(v, ()) if e.target in self.edges]


def build(paths, read, exists, third_dirs=()):
    edges = {}
    for p in paths:
        if not is_tree_source(p):
            continue
        text = read(p)
        es = []
        for m in INCLUDE_RE.finditer(text):
            line = text.count("\n", 0, m.start()) + 1
            es.append(Edge(line, m.group(1), resolve(p, m.group(1), exists, third_dirs)))
        edges[p] = es
    return Graph(edges)


def verify():
    files = {
        "Civ5Expansion2/A/a.h": '#include "b.h"\n#include "A/c.h"\n#include "missing.h"\n',
        "Civ5Expansion2/A/b.h": '#include "a.h"\n',
        "Civ5Expansion2/A/c.h": '  #  include "../x.h"\n',
        "Civ5Expansion2/A/d.cpp": '// c\n#include "a.h"\n#include "lib.h"\n',
        "Civ5Expansion2/self.h": '#include "self.h"\n',
        "third_party/lib.h": "",
    }
    g = build(sorted(files), files.get, lambda p: p in files)
    errs = []
    a = g.edges["Civ5Expansion2/A/a.h"]
    if [(e.line, e.target) for e in a] != [(1, "Civ5Expansion2/A/b.h"), (2, "Civ5Expansion2/A/c.h"), (3, None)]:
        errs.append(f"resolution: {a}")
    if g.edges["Civ5Expansion2/A/c.h"][0].target is not None:
        errs.append("'..' include must not resolve")
    if g.edges["Civ5Expansion2/A/d.cpp"][1].target != "third_party/lib.h":
        errs.append("third_party include must resolve")
    if g.includers(["Civ5Expansion2/A/c.h"]) != {"Civ5Expansion2/A/c.h", "Civ5Expansion2/A/a.h",
                                                "Civ5Expansion2/A/b.h", "Civ5Expansion2/A/d.cpp"}:
        errs.append(f"includers: {sorted(g.includers(['Civ5Expansion2/A/c.h']))}")
    cyc = sorted((s, e.line) for s, e in g.cycle_edges())
    if cyc != [("Civ5Expansion2/A/a.h", 1), ("Civ5Expansion2/A/b.h", 1), ("Civ5Expansion2/self.h", 1)]:
        errs.append(f"cycles: {cyc}")
    return errs
