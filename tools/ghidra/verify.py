"""state.json (what is in the program) against plan.json (what should be there). Contract: plan.py docstring."""

import json
from collections import Counter
from pathlib import Path

KINDS = (
    "type.missing", "type.extra", "type.kind", "type.length", "type.desc",
    "type.field-missing", "type.field-extra", "type.field-name", "type.field-offset", "type.field-type",
    "type.field-bit", "type.value",
    "function.missing", "function.extra", "function.name", "function.scope", "function.scope-kind",
    "function.cc", "function.storage", "function.return", "function.params", "function.varargs",
    "function.source", "function.body",
    "data.missing", "data.extra", "data.name", "data.type", "data.length",
    "label.missing", "label.extra",
)


class VerifyError(Exception):
    pass


def _div(kind, entity, ident, expected, actual, message=None):
    return {"kind": kind, "entity": entity, "id": ident, "expected": expected, "actual": actual,
            "message": message or f"{ident}: {kind}: expected {_short(expected)}, got {_short(actual)}"}


def _short(v):
    s = json.dumps(v, sort_keys=True, ensure_ascii=False)
    return s if len(s) <= 120 else s[:117] + "..."


def _field_key(f):
    return (f["off"], (f.get("bit") or {}).get("shift", 0))


def _compare_type(pt, st, out):
    tid = pt["id"]
    if pt["kind"] != st.get("kind"):
        out.append(_div("type.kind", "type", tid, pt["kind"], st.get("kind")))
        return
    k = pt["kind"]
    if k in ("struct", "union"):
        if pt["size"] != st.get("size"):
            out.append(_div("type.length", "type", tid, pt["size"], st.get("size")))
        if (pt.get("desc") or None) != (st.get("desc") or None):
            out.append(_div("type.desc", "type", tid, pt.get("desc"), st.get("desc")))
        want = list(pt["fields"])
        have = list(st.get("fields", []))
        for pf in list(want):
            match = None
            for sf in have:
                if _field_key(pf) == _field_key(sf) and pf["name"] == sf["name"]:
                    match = sf
                    break
            if match is None:
                for sf in have:
                    if pf["name"] == sf["name"]:
                        out.append(_div("type.field-offset", "type", f"{tid}.{pf['name']}", pf["off"], sf["off"]))
                        have.remove(sf)
                        want.remove(pf)
                        break
                continue
            want.remove(pf)
            have.remove(match)
            if pf["type"] != match["type"]:
                out.append(_div("type.field-type", "type", f"{tid}.{pf['name']}", pf["type"], match["type"]))
            if (pf.get("bit") or None) != (match.get("bit") or None):
                out.append(_div("type.field-bit", "type", f"{tid}.{pf['name']}", pf.get("bit"), match.get("bit")))
        for pf in list(want):
            for sf in have:
                if _field_key(pf) == _field_key(sf):
                    out.append(_div("type.field-name", "type", f"{tid}@{pf['off']}", pf["name"], sf["name"]))
                    have.remove(sf)
                    want.remove(pf)
                    break
        for pf in want:
            out.append(_div("type.field-missing", "type", f"{tid}.{pf['name']}", pf["name"], None))
        for sf in have:
            out.append(_div("type.field-extra", "type", f"{tid}.{sf['name']}", None, sf["name"]))
    elif k == "enum":
        if pt["size"] != st.get("size") or [list(v) for v in pt["values"]] != [list(v) for v in st.get("values", [])]:
            out.append(_div("type.value", "type", tid, {"size": pt["size"], "values": pt["values"]},
                            {"size": st.get("size"), "values": st.get("values")}))
    elif k == "typedef":
        if pt["to"] != st.get("to"):
            out.append(_div("type.value", "type", tid, pt["to"], st.get("to")))
    elif k == "funcdef":
        want = {x: pt[x] for x in ("cc", "ret", "params", "varargs")}
        have = {x: st.get(x) for x in want}
        if want != have:
            out.append(_div("type.value", "type", tid, want, have))


def _scope_kinds(plan):
    return {tuple(s["path"]): s["kind"] for s in plan["scopes"]}


def _compare_function(pf, sf, kinds, out):
    addr = pf["addr"]
    ident = f"{addr} {'::'.join(pf['scope'] + [pf['name']])}"
    if sf is None or not sf.get("exists", True):
        out.append(_div("function.missing", "function", addr, ident, None))
        return
    if sf.get("name") != pf["name"]:
        out.append(_div("function.name", "function", addr, pf["name"], sf.get("name")))
    if list(sf.get("scope", [])) != pf["scope"]:
        out.append(_div("function.scope", "function", addr, pf["scope"], sf.get("scope")))
    else:
        want = [kinds.get(tuple(pf["scope"][:i + 1]), "namespace") for i in range(len(pf["scope"]))]
        if list(sf.get("scope_kinds", [])) != want:
            out.append(_div("function.scope-kind", "function", addr, want, sf.get("scope_kinds")))
    # A partial function without a cc (UNK_PARAMS, not a method) leaves cc and storage to Parameter ID.
    free = pf["partial"] and not pf["cc"]
    if pf["cc"] and sf.get("cc") != pf["cc"] or (not free and bool(sf.get("custom")) != pf["custom"]):
        out.append(_div("function.cc", "function", addr, {"cc": pf["cc"], "custom": pf["custom"]},
                        {"cc": sf.get("cc"), "custom": sf.get("custom")}))
    if pf["ret"] != sf.get("ret"):
        out.append(_div("function.return", "function", addr, pf["ret"], sf.get("ret")))
    if pf["cc"] == "__thiscall" and not pf["custom"] and pf["this_type"] != sf.get("auto_this"):
        out.append(_div("function.params", "function", addr, {"this": pf["this_type"]},
                        {"this": sf.get("auto_this")}))
    if not pf["partial"]:
        have = sf.get("params") or []
        want = pf["params"]
        if pf["custom"]:
            if ([p.get("storage") for p in want] != [p.get("storage") for p in have] or
                    pf["ret_storage"] != sf.get("ret_storage")):
                out.append(_div("function.storage", "function", addr,
                                {"params": [p.get("storage") for p in want], "ret": pf["ret_storage"]},
                                {"params": [p.get("storage") for p in have], "ret": sf.get("ret_storage")}))
        if ([(p["name"], p["type"]) for p in want] != [(p["name"], p["type"]) for p in have] or
                bool(pf["auto_retptr"]) != bool(sf.get("auto_retptr"))):
            out.append(_div("function.params", "function", addr,
                            {"params": [(p["name"], p["type"]) for p in want], "retptr": pf["auto_retptr"]},
                            {"params": [(p["name"], p["type"]) for p in have], "retptr": sf.get("auto_retptr")}))
        if pf["varargs"] != bool(sf.get("varargs")):
            out.append(_div("function.varargs", "function", addr, pf["varargs"], sf.get("varargs")))
        if sf.get("sig_source") != "IMPORTED":
            out.append(_div("function.source", "function", addr, "IMPORTED", sf.get("sig_source"),
                            f"{addr}: signature source is {sf.get('sig_source')}"))
    if sf.get("name_source") != "IMPORTED":
        out.append(_div("function.source", "function", addr, "IMPORTED", sf.get("name_source"),
                        f"{addr}: name source is {sf.get('name_source')}"))


def compare(plan, state):
    out = []
    ptypes = {t["id"]: t for t in plan["types"]}
    stypes = {t["id"]: t for t in state.get("types", [])}
    for tid in sorted(ptypes):
        if tid not in stypes:
            out.append(_div("type.missing", "type", tid, ptypes[tid]["kind"], None))
        else:
            _compare_type(ptypes[tid], stypes[tid], out)
    for tid in sorted(stypes):
        if tid not in ptypes:
            out.append(_div("type.extra", "type", tid, None, stypes[tid].get("kind")))

    kinds = _scope_kinds(plan)
    sfuncs = {f["addr"]: f for f in state.get("funcs", [])}
    pfuncs = {f["addr"]: f for f in plan["functions"]}
    for addr in sorted(pfuncs):
        _compare_function(pfuncs[addr], sfuncs.get(addr), kinds, out)
    for addr in sorted(sfuncs):
        f = sfuncs[addr]
        if addr not in pfuncs and f.get("owned") and f.get("exists", True):
            out.append(_div("function.extra", "function", addr, None, f.get("name")))

    sdata = {d["addr"]: d for d in state.get("data", [])}
    pdata = {d["addr"]: d for d in plan["data"]}
    for addr in sorted(pdata):
        pd, sd = pdata[addr], sdata.get(addr)
        if sd is None or not sd.get("exists", True):
            out.append(_div("data.missing", "data", addr, pd["name"], None))
            continue
        if sd.get("name") != pd["name"] or list(sd.get("scope", [])) != pd["scope"]:
            out.append(_div("data.name", "data", addr, pd["scope"] + [pd["name"]],
                            list(sd.get("scope", [])) + [sd.get("name")]))
        if sd.get("type") != pd["type"]:
            out.append(_div("data.type", "data", addr, pd["type"], sd.get("type")))
        if sd.get("size") != pd["size"]:
            out.append(_div("data.length", "data", addr, pd["size"], sd.get("size")))
    for addr in sorted(sdata):
        d = sdata[addr]
        if addr not in pdata and d.get("owned") and d.get("exists", True):
            out.append(_div("data.extra", "data", addr, None, d.get("name")))

    sbodies = {b["addr"]: b for b in state.get("bodies", [])}
    for pb in plan.get("bodies", []):
        sb = sbodies.get(pb["addr"])
        want = int(pb["addr"], 16) + pb["size"]
        if sb is None:
            out.append(_div("function.body", "function", pb["addr"], hex(want), None))
        elif sb.get("status") == "ok" and (sb.get("end") is None or int(sb["end"], 16) < want):
            out.append(_div("function.body", "function", pb["addr"], f"end >= 0x{want:08x}", sb.get("end")))
        elif sb.get("status") not in ("ok",) and not str(sb.get("status")).startswith("skipped: "):
            out.append(_div("function.body", "function", pb["addr"], "ok", sb.get("status")))

    slabels = {(lb["addr"], tuple(lb.get("scope", [])), lb["name"]): lb for lb in state.get("labels", [])
               if lb.get("exists", True)}
    plabels = {(lb["addr"], tuple(lb["scope"]), lb["name"]) for lb in plan["labels"]}
    for key in sorted(plabels):
        if key not in slabels:
            out.append(_div("label.missing", "label", key[0], "::".join(key[1] + (key[2],)), None))
    for key in sorted(slabels):
        if key not in plabels and slabels[key].get("owned"):
            out.append(_div("label.extra", "label", key[0], None, "::".join(key[1] + (key[2],))))
    return out


def verify(plan, state):
    if state.get("plan_sha256") != plan["header"]["plan_sha256"]:
        raise VerifyError("state.json belongs to another plan: run civ5re ghidra apply")
    return compare(plan, state)


def summary(divs):
    return dict(sorted(Counter(d["kind"] for d in divs).items()))


def format_divergences(divs, limit=50):
    lines = [d["message"] for d in divs[:limit]]
    if len(divs) > limit:
        lines.append(f"... and {len(divs) - limit} more")
    return lines


def load_json(path):
    try:
        return json.loads(Path(path).read_text(encoding="utf-8"))
    except FileNotFoundError:
        raise VerifyError(f"{path} not found") from None
    except json.JSONDecodeError as e:
        raise VerifyError(f"{path}: {e}") from None


def check_report(report, plan_sha256):
    """Returns (changes, conflicts, aborted) of an apply-report.json for the given plan."""
    if report.get("plan_sha256") != plan_sha256:
        raise VerifyError("apply-report.json belongs to another plan")
    c = report.get("counters", {})
    return int(c.get("changes", 0)), int(c.get("conflicts", 0)), report.get("aborted")
