"""Structor variants C2/D2/D0 from C1/D1, verified with llvm-cxxfilt (one call for everything)."""
from __future__ import annotations

import subprocess

from typedb.compile import EnvError


def candidates(mangled: str, src: str, dst: str):
    out = []
    i = mangled.find(src)
    while i >= 0:
        out.append(mangled[:i] + dst + mangled[i + len(src):])
        i = mangled.find(src, i + 1)
    return out


def demangle_all(cxxfilt, names):
    names = sorted(set(names))
    if not names:
        return {}
    try:
        p = subprocess.run([str(cxxfilt)], input="\n".join(names) + "\n", capture_output=True,
                           text=True, encoding="utf-8")
    except OSError as e:
        raise EnvError(f"cannot run llvm-cxxfilt: {e}") from None
    lines = p.stdout.split("\n")
    if p.returncode != 0 or len(lines) < len(names):
        raise EnvError(f"llvm-cxxfilt failed: {p.stderr[-500:]}")
    return dict(zip(names, lines))


def plan(requests):
    """requests: [(mangled C1|D1, kind ctor|dtor, virtual)] → all names cxxfilt must see."""
    names = []
    for mangled, kind, _virtual in requests:
        names.append(mangled)
        if kind == "ctor":
            names += candidates(mangled, "C1", "C2")
        else:
            names += candidates(mangled, "D1", "D2") + candidates(mangled, "D1", "D0")
    return names


def resolve(mangled, kind, virtual, demangled):
    """→ (variants dict, error message|None)."""
    want = demangled.get(mangled)

    def pick(src, dst):
        hits = [c for c in candidates(mangled, src, dst) if demangled.get(c) == want]
        return hits[0] if len(hits) == 1 else None, len(hits)

    if kind == "ctor":
        c2, n = pick("C1", "C2")
        if c2 is None:
            return {"C1": mangled, "C2": None}, f"{n} C2 candidates match {mangled}"
        return {"C1": mangled, "C2": c2}, None
    d2, n = pick("D1", "D2")
    err = None if d2 is not None else f"{n} D2 candidates match {mangled}"
    d0 = None
    if virtual:
        d0, n0 = pick("D1", "D0")
        if d0 is None and err is None:
            err = f"{n0} D0 candidates match {mangled}"
    return {"D1": mangled, "D2": d2, "D0": d0}, err
