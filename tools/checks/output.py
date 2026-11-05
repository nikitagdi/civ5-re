"""Text and JSON output of a check run."""
import json
import os
from pathlib import Path

from .api import ERROR, WARN

BUDGETS = {"staged": 30, "files": 30, "full": 180, "root": 180}
HINT_PY = r".venv\Scripts\python"


def finding_lines(f):
    sev = " [warn]" if f.severity == WARN else ""
    head = f"{f.path}:{f.line}: {f.rule}{sev} {f.message}" if f.path else f"civ5re: {f.rule}{sev} {f.message}"
    lines = [head]
    detail = list(f.detail)
    if not detail and f.expected:
        detail.append(f"expected: {f.expected}")
        if f.actual:
            detail.append(f"actual: {f.actual}")
    lines += [f"    {d}" for d in detail]
    return lines


def _selftest_lines(st):
    s = st["status"]
    if s == "cached":
        out = ["selftest: from cache (tools unchanged)"]
    elif s == "ok":
        out = [f"selftest: ok ({st['fixtures']} fixtures)"]
    elif s == "failed":
        out = [f"selftest: FAILED ({len(st['failed'])} of {st['fixtures']} fixtures)"]
        out += [f"civ5re: selftest {x['id']}/{x['case']}: {x['message']}" for x in st["failed"]]
    else:
        return []
    return out


def recheck_hint(res):
    errs = [f for f in res.findings if f.severity == ERROR and not f.waived and not f.fatal and f.path]
    if not errs:
        return None
    ids = sorted({f.rule for f in errs})
    files = sorted({f.path for f in errs})[:5]
    return f"re-check: {HINT_PY} -m civ5re check --only {','.join(ids)} --files {' '.join(files)}"


WARN_LINES = 10  # printed lines per rule for warnings; errors are always complete


def render_findings(findings, limit=WARN_LINES):
    """Text lines of the findings; warnings of one rule share `limit` lines, the rest is counted."""
    out, used, cut = [], {}, {}
    for f in findings:
        if f.waived:
            continue
        lines = finding_lines(f)
        if f.severity == WARN:
            room = max(0, limit - used.get(f.rule, 0))
            used[f.rule] = used.get(f.rule, 0) + min(room, len(lines))
            if len(lines) > room:
                cut[f.rule] = cut.get(f.rule, 0) + len(lines) - room
                lines = lines[:room]
        out += lines
    for rule, n in cut.items():
        out.append(f"civ5re: {rule} [warn] {n} more output lines not shown (full list: build/checks/last.json or --json)")
    return out


def render_text(res):
    out = render_findings(res.findings)
    quiet = [r["id"] for r in res.rules if r["status"] == "skipped" and r["reason"] == "not affected by the change"]
    for r in res.rules:
        if r["status"] == "skipped" and r["id"] not in quiet:
            out.append(f"civ5re: {r['id']} skipped: {r['reason']}")
    if quiet:
        out.append(f"civ5re: {len(quiet)} rules not affected by the change: {','.join(quiet)}")
    out += [f"civ5re: {n}" for n in res.notes]
    out += _selftest_lines(res.selftest)
    ran = sum(1 for r in res.rules if r["status"] == "ran")
    slow = [(r["seconds"], r["id"]) for r in res.rules if r["seconds"] >= 1]
    slow += [(s, k) for k, s in res.timings.items() if s >= 1 and k in ("typedb", "binfacts", "clang")]
    slow_txt = f" ({', '.join(f'{k} {s:.1f}' for s, k in sorted(slow, reverse=True))})" if slow else ""
    c = res.counts
    out.append(f"rules {res.n_builtin} + plugins {res.n_plugin}: ran {ran} · error {c['error']} · warn {c['warn']}"
               f" · waivers {c['waivers']} (matched {res.matched}) · {res.seconds:.1f} s{slow_txt}")
    budget = BUDGETS.get(res.mode)
    if budget and res.seconds > 2 * budget:
        out.append(f"civ5re: [warn] run took {res.seconds:.0f} s, more than twice the {budget} s budget for {res.mode}")
    hint = recheck_hint(res)
    if hint:
        out.append(hint)
    return "\n".join(out) + "\n"


def render_json(payload):
    return json.dumps(payload, indent=1, ensure_ascii=False) + "\n"


def write_json(path, payload):
    path = Path(path)
    path.parent.mkdir(parents=True, exist_ok=True)
    tmp = path.with_suffix(path.suffix + ".tmp")
    tmp.write_text(render_json(payload), encoding="utf-8", newline="\n")
    os.replace(tmp, path)
