"""Pass C: definitions in `.cpp` (parallel clang runs, per-file cache)."""
from __future__ import annotations

import json
import os
import re
from concurrent.futures import ThreadPoolExecutor
from pathlib import Path

from typedb import clang_ast, compile as cc


def _qualifier(text_line, name_col, name):
    """`void FFastVector<T>::push_back(` → `FFastVector` (template args dropped)."""
    head = text_line[:max(0, name_col - 1)]
    m = re.search(r"((?:[A-Za-z_]\w*(?:<[^;{}()]*?>)?\s*::\s*)+)(?:~\s*)?$", head)
    if not m:
        return []
    parts = [re.sub(r"<.*", "", p).strip() for p in m.group(1).split("::") if p.strip()]
    return parts


def extract_defs(root, rel, path_map, text):
    model = clang_ast.extract(root, path_map)
    lines = text.split("\n")
    defs, others = [], []
    for d in model.decls:
        if d.file != rel:
            continue
        if d.ent == "function" and d.has_body:
            loc = d.node.get("_loc")
            qual_parts = []
            if loc is not None and 0 < loc.line <= len(lines) and not loc.macro:
                qual_parts = _qualifier(lines[loc.line - 1], loc.col or 1, d.name)
            ns = [p for p, k in d.scope_parts if k.startswith("ns")]
            cls = [p for p, k in d.scope_parts if not k.startswith("ns")]
            qualified = "::".join(ns + cls + qual_parts + [d.name])
            defs.append({"line": d.line, "kind": "function", "name": d.name, "qualified": qualified,
                         "mangled": d.node.get("mangledName"),
                         "params": sum(1 for c in d.node.get("inner", ()) if c.get("kind") == "ParmVarDecl")})
        elif d.ent == "global" and ("init" in d.node or d.node.get("storageClass") != "extern"):
            defs.append({"line": d.line, "kind": "global", "name": d.name, "qualified": d.qualified,
                         "mangled": d.node.get("mangledName"), "params": 0})
        else:
            others.append(d.line)
    return defs, sorted(set(others))


def run_one(env, args_base, src: Path, rel, out_json: Path, path_map, text):
    rc, err = cc.run(env, args_base + cc.PASS_C + [cc._posix(src)], cwd=str(out_json.parent), stdout_path=str(out_json))
    errors = []
    for f, line, _col, sev, msg in clang_ast.parse_diagnostics(err):
        if sev in ("error", "fatal error"):
            errors.append([path_map(f), line, msg])
    defs, others = [], []
    try:
        root = clang_ast.load_json(out_json)
        defs, others = extract_defs(root, rel, path_map, text)
    except (OSError, ValueError):
        # A .cpp with no definitions yet gives an empty filtered dump: that is not an error.
        empty = rc == 0 and out_json.is_file() and not out_json.read_bytes().strip()
        if not errors and not empty:
            tail = (err or "").strip().splitlines()[-1:] or [f"clang exit {rc}"]
            errors.append([(rel, "tree"), 1, f"no AST produced: {tail[0][:300]}"])
    try:
        os.remove(out_json)
    except OSError:
        pass
    return {"defs": defs, "others": others, "errors": [[e[0][0] if e[0] else None, e[1], e[2]] for e in errors]}


def run_all(env, tree, build, files, headers_key, path_map, jobs=None):
    """files: [(rel, abs Path, sha256, text)] → {rel: result}."""
    wd = cc.work_dir(build)
    cdir = wd / "cpp"
    cdir.mkdir(parents=True, exist_ok=True)
    base = cc.clang_args("cpp", env, tree=tree, build=build)
    results, todo = {}, []
    for rel, path, sha, text in files:
        key = cc.sha256_bytes(f"{sha}\0{headers_key}".encode())
        frag = cdir / (cc.sha256_bytes(rel.encode())[:24] + ".json")
        try:
            d = json.loads(frag.read_text(encoding="utf-8"))
            if d.get("key") == key:
                results[rel] = d["result"]
                continue
        except (OSError, ValueError, KeyError):
            pass
        todo.append((rel, path, text, key, frag))

    def work(item):
        rel, path, text, key, frag = item
        tmp = cdir / (frag.stem + ".ast.json")
        res = run_one(env, base, path, rel, tmp, path_map, text)
        frag.write_text(json.dumps({"key": key, "result": res}, sort_keys=True), encoding="utf-8")
        return rel, res

    if todo:
        with ThreadPoolExecutor(max_workers=jobs or os.cpu_count() or 4) as ex:
            for rel, res in ex.map(work, todo):
                results[rel] = res
    return results
