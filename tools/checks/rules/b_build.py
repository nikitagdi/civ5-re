"""B1/B2: every .h compiles alone, every .cpp passes -fsyntax-only (clang_args("alone"): prelude via -include, no PCH)."""
from checks import clangrun, includes
from checks.api import Rule

TREE = includes.TREE


def _fmt(d):
    return f"{d.path}:{d.line}: {d.message}" if d.path else d.message


def _jobs(ctx, paths, template):
    graph = ctx.includes
    return [clangrun.Job(p, template.format(rel=p[len(TREE):]), sorted(graph.includes([p]))) for p in paths]


class B1(Rule):
    id = "B1"
    title = ".h compiles on its own (two #include lines, -fsyntax-only, clang_args('alone'))"
    needs = ("files", "clang")
    scope = "entity"

    def run(self, ctx):
        paths = ctx.scoped(lambda p: p.startswith(TREE) and p.endswith(".h"))
        if not paths:
            return []
        results = clangrun.run(ctx, _jobs(ctx, paths, '#include "{rel}"\n#include "{rel}"\n'))
        out = []
        for p in paths:
            r = results[p]
            if r.ok:
                continue
            out.append(self.finding(p, 1, f"does not compile alone: {_fmt(r.errors[0])}",
                                    detail=[_fmt(d) for d in r.errors[1:]], key=f"B1:{p}"))
        return out


class B2(Rule):
    id = "B2"
    title = ".cpp with its own #includes passes -fsyntax-only (same flags as B1)"
    needs = ("files", "clang")
    scope = "entity"

    def run(self, ctx):
        paths = ctx.scoped(lambda p: p.startswith(TREE) and p.endswith(".cpp"))
        if not paths:
            return []
        results = clangrun.run(ctx, _jobs(ctx, paths, '#include "{rel}"\n'))
        out, seen = [], set()
        for p in paths:
            for d in results[p].errors:
                if d.path and d.path.lower() == p.lower():
                    line, msg = d.line, d.message
                else:
                    line, msg = 1, _fmt(d)
                if (p, line, msg) in seen:
                    continue
                seen.add((p, line, msg))
                out.append(self.finding(p, line, msg, key=f"B2:{p}:{line}:{msg}"))
        return out
