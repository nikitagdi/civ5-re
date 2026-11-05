from checks.api import Rule

WAIVERS = "tools/checks/waivers.json"


class W0(Rule):
    id = "W0"
    title = "a waiver needs a reason; a waiver that matches no finding is stale"
    needs = ("files",)
    scope = "tree"
    staged = "full"
    over = "any"
    fixture_with = ("T1", "T2", "T3")

    def run(self, ctx):
        out = []
        for w in ctx.waivers:
            if not w.valid:
                out.append(self.finding(WAIVERS, w.line, f"waiver without reason: {w.rule} {w.key}",
                                        key=f"W0:noreason:{w.rule}:{w.key}"))
            elif w.stale:
                out.append(self.warn(WAIVERS, w.line, f"waiver matches no finding: {w.rule} {w.key}",
                                     key=f"W0:stale:{w.rule}:{w.key}"))
        return out
