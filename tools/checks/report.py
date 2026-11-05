"""--report: progress metrics from typedb.json and binfacts.json."""
import time

from civ5re import placeholders as ph

from .api import InputError
from .rules.a_symbols import coverage as declared_coverage
from .context import Context
from .output import render_json, write_json
from .runner import Result

SCHEMA = "civ5re.report/1"


def folder(path):
    parts = (path or "").split("/")[1:-1]
    n = 4 if parts[:3] == ["Civ5", "Src", "App"] else 2
    return "/".join(parts[:n]) or "."


def _addr(v):
    if isinstance(v, str) and v.startswith("0x"):
        try:
            return int(v, 16)
        except ValueError:
            return None
    return v if isinstance(v, int) else None


def _last(name):
    return (name or "").rsplit("::", 1)[-1]


def placeholders(db):
    by_kind, by_folder = {}, {}

    def hit(kind, path):
        by_kind[kind] = by_kind.get(kind, 0) + 1
        by_folder[folder(path)] = by_folder.get(folder(path), 0) + 1

    for r in db.get("records", []):
        if r.get("origin") != "tree":
            continue
        p = r.get("file")
        if ph.classify(_last(r.get("name"))) and ph.classify(_last(r.get("name"))).kind == "class":
            hit("class", p)
        if r.get("size_marker") == "?":
            hit("size ?", p)
        if r.get("vtable_marker") == "?":
            hit("vtable ?", p)
        for m in r.get("instance_markers") or []:
            if m.get("value") == "?":
                hit(f"{m.get('kind')} ?", p)
        for m in (r.get("markers") or {}).get("lua") or []:
            if (m.get("text") or "").strip() == "?":
                hit("lua ?", p)
        for f in r.get("fields") or []:
            c = ph.classify(f.get("name") or "")
            if c and c.kind in ("field", "unk"):
                hit(c.kind, p)
    for f in db.get("functions", []):
        p = f.get("file")
        c = ph.classify(f.get("name") or "")
        if c and c.kind in ("fun", "vf"):
            hit(c.kind, p)
        if f.get("return") == "UNK_RET":
            hit("unk_ret", p)
        if f.get("params_unknown"):
            hit("unk_params", p)
        zero = ph.zero_based_params(prm.get("name") or "" for prm in f.get("params") or [])
        for prm in f.get("params") or []:
            c = ph.classify(prm.get("name") or "")
            if c and c.kind == "param" and not zero:
                hit("param", p)
        for m in (f.get("markers") or {}).get("lua") or []:
            if (m.get("text") or "").strip() == "?":
                hit("lua ?", p)
    for g in db.get("globals", []):
        p = g.get("file")
        c = ph.classify(_last(g.get("qualified")))
        if c and c.kind == "global":
            hit("global", p)
        if "UNK_DATA" in str(g.get("type")):
            hit("unk_data", p)
    return {"total": sum(by_kind.values()), "by_kind": dict(sorted(by_kind.items())),
            "by_folder": dict(sorted(by_folder.items()))}


def coverage(db, bf):
    per, _missing = declared_coverage(db, bf)
    den = (bf.get("stats") or {}).get("denominator") or {}
    kinds = list((den.get("by_kind") or {}).keys())
    total, covered = {k: per[k][1] for k in kinds}, {k: per[k][0] for k in kinds}
    classes = {r.get("name") for r in db.get("records", [])}
    ti = {r.get("typeinfo_symbol") for r in db.get("records", []) if r.get("typeinfo_symbol")}
    gl = {g.get("mangled") for g in db.get("globals", [])}
    gl |= {i.get("mangled") for g in db.get("globals", []) for i in g.get("instances") or ()}
    by_class = {k: [0, 0] for k in ("typeinfo", "typeinfo_name", "guard")}
    for s in bf.get("symbols", []):
        if s.get("group") != "firaxis":
            continue
        k = s.get("kind")
        if k in by_class:
            m = s.get("mangled") or ""
            by_class[k][0] += 1
            if k == "typeinfo":
                ok = m in ti or s.get("scope") in classes
            elif k == "typeinfo_name":
                ok = "_ZTI" + m[4:] in ti or s.get("scope") in classes
            else:
                ok = "_Z" + m[4:] in gl
            by_class[k][1] += ok
    return {
        "by_kind": {k: {"declared": covered[k], "total": total[k], "denominator": (den.get("by_kind") or {}).get(k)}
                    for k in kinds},
        "declared": sum(covered.values()), "total": sum(total.values()), "denominator_total": den.get("total"),
        "covered_by_class": {k: {"covered": v[1], "total": v[0]} for k, v in by_class.items()},
    }


def finished_functions(db, bf):
    exported = {(s.get("mangled"), _addr(s.get("addr"))) for s in bf.get("symbols", [])}
    done = total = 0
    for f in db.get("functions", []):
        total += 1
        a = _addr((f.get("markers") or {}).get("addr"))
        if (f.get("mangled"), a) not in exported:
            continue
        if f.get("return") == "UNK_RET" or f.get("params_unknown"):
            continue
        pnames = [p.get("name") or "" for p in f.get("params") or []]
        if ph.zero_based_params(pnames):
            pnames = []
        if any(ph.is_placeholder(n) for n in [f.get("name") or "", *pnames]):
            continue
        done += 1
    return {"done": done, "total": total}


def layouts(db):
    recs = [r for r in db.get("records", []) if r.get("origin") == "tree"]
    sized = sum(1 for r in recs if r.get("size_marker") not in (None, "?"))
    fields = [f for r in recs for f in r.get("fields") or []]
    named = sum(1 for f in fields if not ph.is_placeholder(f.get("name") or ""))
    return {"classes_with_size": {"done": sized, "total": len(recs)},
            "fields_with_name": {"done": named, "total": len(fields)}}


def compute(db, bf):
    return {"schema": SCHEMA, "placeholders": placeholders(db), "export_coverage": coverage(db, bf),
            "finished_functions": finished_functions(db, bf), "layouts": layouts(db)}


def render_text(rep):
    p, c, f, l = rep["placeholders"], rep["export_coverage"], rep["finished_functions"], rep["layouts"]
    out = [f"placeholders: {p['total']} " + " ".join(f"{k}={v}" for k, v in p["by_kind"].items())]
    out += [f"    {k}: {v}" for k, v in p["by_folder"].items()]
    out.append(f"export coverage: {c['declared']} of {c['total']} (denominator {c['denominator_total']})")
    out += [f"    {k}: {v['declared']} of {v['total']}" for k, v in c["by_kind"].items()]
    out += [f"    covered by class, {k}: {v['covered']} of {v['total']}" for k, v in c["covered_by_class"].items()]
    out.append(f"finished functions: {f['done']} of {f['total']}")
    out.append(f"layouts: classes with size {l['classes_with_size']['done']} of {l['classes_with_size']['total']},"
               f" fields with a name {l['fields_with_name']['done']} of {l['fields_with_name']['total']}")
    return "\n".join(out) + "\n"


def run_report(settings):
    t0 = time.perf_counter()
    res = Result("report")
    ctx = Context(settings)
    rep = None
    try:
        rep = compute(ctx.typedb, ctx.binfacts)
    except InputError as e:
        res.fatal("input", f"{e.name}: {e.reason}")
    if rep is not None:
        write_json(settings.out / "report.json", rep)
    res.recount()
    res.inputs.update({k: v for k, v in ctx.inputs_info([]).items() if k != "plugins"})
    res.exit = 2 if res.has_fatal else 0
    res.seconds = time.perf_counter() - t0
    return res, rep
