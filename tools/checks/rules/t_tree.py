import re

from checks.api import Rule

CODE_EXT = {".h", ".hh", ".hpp", ".hxx", ".inl", ".ipp", ".c", ".cc", ".cpp", ".cxx"}
MIRROR_EXT = (".h", ".inl", ".cpp")
CODE_HOMES = re.compile(r"^(?:Civ5Expansion2/|third_party/|tools/(?:.+/)?(?:fixtures|tests)/)")
MD_ROOT = {"README.md", "NOTICE.md"}
MD_DIRS = ()
MD_HOMES = re.compile(r"^tools/(?:.+/)?(?:fixtures|tests)/")


def _ext(path):
    name = path.rsplit("/", 1)[-1]
    return name[name.rfind("."):].lower() if "." in name else ""


class T1(Rule):
    id = "T1"
    title = "C/C++ only in the mirror, third_party and tool fixtures/tests; the mirror holds only .h .inl .cpp"
    needs = ("files",)
    over = "any"

    def run(self, ctx):
        out = []
        for p in ctx.scoped():
            if p.startswith("Civ5Expansion2/"):
                if not p.endswith(MIRROR_EXT):
                    out.append(self.finding(p, 1, "non-source file in the mirror: only .h .inl .cpp allowed",
                                            key=f"T1:{p}"))
            elif _ext(p) in CODE_EXT and not CODE_HOMES.match(p):
                out.append(self.finding(p, 1, "source outside the mirror", key=f"T1:{p}"))
        return out


class T2(Rule):
    id = "T2"
    title = "md files only as README.md and NOTICE.md at the root (tool fixtures/tests exempt)"
    needs = ("files",)
    over = "any"

    def run(self, ctx):
        out = []
        for p in ctx.scoped(lambda p: _ext(p) == ".md"):
            if p in MD_ROOT or p.startswith(MD_DIRS) or MD_HOMES.match(p):
                continue
            out.append(self.finding(p, 1, "md outside allowed locations", key=f"T2:{p}"))
        return out


class T3(Rule):
    id = "T3"
    title = "#include \"...\" resolves from the file's directory or Civ5Expansion2/; no '..', no .cpp, no cycles"
    needs = ("files",)

    def run(self, ctx):
        graph = ctx.includes
        out = []
        for p in sorted(graph.edges):
            if not ctx.in_scope(p):
                continue
            for e in graph.edges[p]:
                if ".." in e.spec.split("/"):
                    out.append(self.finding(p, e.line, f'include with "..": "{e.spec}"', key=f"T3:{p}:dotdot:{e.spec}"))
                elif e.target is None:
                    out.append(self.finding(p, e.line, f'include not found: "{e.spec}"', key=f"T3:{p}:missing:{e.spec}"))
                elif e.target.endswith(".cpp"):
                    out.append(self.finding(p, e.line, f'include of a .cpp: "{e.spec}"', key=f"T3:{p}:cpp:{e.spec}"))
        cyc = graph.cycle_edges()
        if cyc:                              # sdk_map is read only when there is a cycle to judge
            sdk = {"civ5expansion2/" + f[1].strip().lower() for f in
                   (ln.split("	") for ln in ctx.evidence("sdk_map") if ln.strip() and not ln.startswith("#")) if len(f) > 1}
            comp = graph.components()
            members = {}
            for path, c in comp.items():
                members.setdefault(c, []).append(path)
        for src, e in cyc:
            if all(m.lower() in sdk for m in members[comp[src]]):
                continue                     # a cycle of SDK copies is the original source
            if ctx.in_scope(src):
                out.append(self.finding(src, e.line, f'include cycle through "{e.spec}"', key=f"T3:{src}:cycle:{e.target}"))
        return out
