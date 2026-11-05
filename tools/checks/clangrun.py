"""Parallel `clang -fsyntax-only` over small translation units, with a content-addressed result cache."""
import hashlib
import json
import os
import re
import subprocess
from concurrent.futures import ThreadPoolExecutor
from collections import namedtuple

Job = namedtuple("Job", "name text closure")        # closure: repo paths whose contents are part of the TU
Diag = namedtuple("Diag", "path line col message")
Result = namedtuple("Result", "ok errors cached")

DIAG_RE = re.compile(r"^(.*?):(\d+):(\d+): (?:fatal error|error): (.*)$")
CACHE_REL = ("cache", "clang.json")


def parse_diagnostics(stderr, base):
    base = str(base).replace("\\", "/").rstrip("/") + "/"
    out = []
    for line in stderr.splitlines():
        m = DIAG_RE.match(line)
        if not m:
            continue
        p = m.group(1).replace("\\", "/")
        if p.lower().startswith(base.lower()):
            p = p[len(base):]
        out.append(Diag(p, int(m.group(2)), int(m.group(3)), m.group(4)))
    return out


def third_party_stamp(ctx):
    def stamp():
        h = hashlib.sha256()
        for d, dirs, names in os.walk(ctx.third_party):
            dirs.sort()
            for n in sorted(names):
                p = os.path.join(d, n)
                try:
                    st = os.stat(p)
                except OSError:
                    continue
                h.update(f"{os.path.relpath(p, ctx.third_party)}|{st.st_size}|{st.st_mtime_ns}\n".encode())
        return h.hexdigest()
    return ctx._lazy("third_party_stamp", stamp)


def cache_key(version, args, tp_stamp, job, read):
    h = hashlib.sha256()
    for part in (version, "\0".join(args), tp_stamp, job.text):
        h.update(part.encode("utf-8", "replace") + b"\x01")
    for p in sorted(set(job.closure)):
        h.update(p.encode() + b"\x00" + hashlib.sha256(read(p).encode("utf-8", "replace")).digest())
    return h.hexdigest()


def _load_cache(path):
    try:
        return json.loads(path.read_text(encoding="utf-8"))
    except (OSError, ValueError):
        return {}


def _save_cache(path, data):
    path.parent.mkdir(parents=True, exist_ok=True)
    tmp = path.with_suffix(".tmp")
    tmp.write_text(json.dumps(data, sort_keys=True), encoding="utf-8")
    os.replace(tmp, path)


def run(ctx, jobs):
    """{job.name: Result}. Raises InputError when clang is missing."""
    exe, version = ctx.clang
    args = ctx.clang_args("alone") + ["-fsyntax-only", "-x", "c++", "-"]
    stamp = third_party_stamp(ctx)
    cache_path = ctx.settings.out.joinpath(*CACHE_REL)
    cache = _load_cache(cache_path)
    keys = {j.name: cache_key(version, args, stamp, j, ctx.read) for j in jobs}
    results, todo = {}, []
    for j in jobs:
        hit = cache.get(keys[j.name])
        if hit is not None:
            errs = [Diag(*d) for d in hit]
            results[j.name] = Result(not errs, errs, True)
        else:
            todo.append(j)

    def compile_one(j):
        r = subprocess.run([*exe, *args], input=j.text.encode("utf-8"), cwd=ctx.base, capture_output=True)
        err = r.stderr.decode("utf-8", "replace")
        diags = parse_diagnostics(err, ctx.base)
        if r.returncode != 0 and not diags:
            diags = [Diag(None, None, None, f"clang failed with exit code {r.returncode}: {err.strip()[:500]}")]
        return j, diags

    if todo:
        with ThreadPoolExecutor(max_workers=max(1, ctx.jobs)) as pool:
            for j, diags in pool.map(compile_one, todo):
                results[j.name] = Result(not diags, diags, False)
                if all(d.path is not None for d in diags):
                    cache[keys[j.name]] = [list(d) for d in diags]
        _save_cache(cache_path, cache)
    return results
