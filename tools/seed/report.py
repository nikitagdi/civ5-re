"""build/seed-report.json: recalculated from binfacts at every run."""
import json
import re
from collections import Counter, defaultdict
from pathlib import Path

from seed import luaentry

MARKER = re.compile(r"^\s*/// (pure|absent|inline|base|fastcc|lua \?|size \?|vtable \?|0x[0-9a-f]{8} fastcc|origin binary|origin guess)")


def new_report():
    return {"problems": [], "decisions": defaultdict(list), "timings": {}}


def _markers(files):
    c = Counter()
    for text in files.values():
        for ln in text.split("\n"):
            s = ln.strip()
            if not s.startswith("/// "):
                continue
            body = s[4:]
            if body.startswith("0x"):
                c["addr"] += 1
                if " fastcc" in body:
                    c["fastcc"] += 1
                continue
            head = body.split(" ")[0]
            if head in ("base", "deleting", "thunk", "pure", "absent", "inline"):
                c[head] += 1
                if head == "base" and len(body.split(" ")) == 2:
                    c["base_single"] += 1
            elif body.startswith("lua ?"):
                c["lua ?"] += 1
            elif body.startswith("size ?"):
                c["size ?"] += 1
            elif body.startswith("vtable ?"):
                c["vtable ?"] += 1
            elif head == "vtable":
                c["vtable"] += 1
            elif body.startswith("origin "):
                c["origin " + body.split(" ")[1]] += 1
    return dict(sorted(c.items()))


def planned_coverage(m, facts, existing):
    """Denominator symbols the existing tree or the scaffolds declare, by kind (A10 of checks is the measure)."""
    addrs = set()
    for sc in m.scopes.values():
        for f in sc.fns:
            for i in f.insts:
                addrs.update(a for a in (i.addr, i.base, i.deleting) if a)
                addrs.update(i.thunks)
        for v in sc.vars:
            addrs.update(i.addr for i in v.insts if i.addr)
        addrs.update(a for a in [sc.vtable] + list(sc.vtables.values()) if a and a != "?")
    skipped = {s["mangled"] for s in m.skipped}
    per = {k: [0, 0] for k in facts.kinds}
    for s in facts.den:
        per[s["kind"]][1] += 1
        if s["mangled"] not in skipped and (existing.declares_symbol(s["mangled"]) or s["addr"] in addrs):
            per[s["kind"]][0] += 1
    return {k: {"declared": a, "total": b} for k, (a, b) in sorted(per.items())}


def fill(rep, m, facts, files, occupied, vstats, units, seconds, existing=None):
    decisions = dict(m.decisions)
    for k, v in rep["decisions"].items():
        decisions.setdefault(k, []).extend(v)
    entities = Counter()
    for sc in m.scopes.values():
        if sc.kind == "class" and sc.path:
            entities["class_template" if sc.tparams is not None else "class"] += 1
        elif sc.kind == "namespace" and sc.path and (sc.fns or sc.vars):
            entities["namespace"] += 1
        elif sc.kind == "enum":
            entities["enum"] += 1
        for f in sc.fns:
            entities["function"] += 1
        for v in sc.vars:
            entities["data"] += 1
    by_kind = facts.bf["stats"]["denominator"]["by_kind"]
    lua = luaentry.counts(facts.bf)
    rep.update({
        "schema": 1,
        "files": len(files),
        "entities": dict(sorted(entities.items())),
        "markers": _markers(files),
        "origin": {k: sum(1 for t in files.values() if t.startswith(f"/// origin {k}")) for k in ("binary", "guess")},
        "denominator": by_kind,
        "coverage_planned": planned_coverage(m, facts, existing) if existing is not None else None,
        "lua_entries": lua,
        "params_unknown": sum(1 for t in files.values() for ln in t.split("\n") if "(UNK_PARAMS)" in ln),
        "skipped": {"files": sorted(occupied), "entities": sorted(m.skipped, key=lambda s: s["mangled"])},
        "skipped_by_reason": dict(Counter(s["reason"].split(" in ")[0] for s in m.skipped).most_common()),
        "occupied": sorted(occupied),
        "vtables": dict(sorted(vstats.items())),
        "decisions": {k: {"count": len(v), "items": sorted(set(v))[:2000]} for k, v in sorted(decisions.items())},
        "units": len(units),
    })
    rep["timings"]["model_s"] = round(seconds, 1)


def write(root, rep):
    p = Path(root) / "build" / "seed-report.json"
    p.parent.mkdir(exist_ok=True)
    p.write_text(json.dumps(rep, indent=1, sort_keys=True, default=list) + "\n", encoding="utf-8", newline="\n")
