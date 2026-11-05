"""`build_db`: tree → `<build>/typedb.json`; JSON writing and cache."""
from __future__ import annotations

import hashlib
import json
import os
import re
from collections import defaultdict
from pathlib import Path

from civ5re import placeholders as ph
from typedb import attach as at
from typedb import clang_ast, cppdefs, llvm_ir, probes as pr, variants as vr
from typedb import compile as cc
from typedb import markers as mk
from typedb.schema import SCHEMA
from typedb.typegraph import TypeGraph, parse_thunk

EXTS = {".h": "h", ".inl": "inl", ".cpp": "cpp"}


class Paths:
    """clang path → (relative output path, 'tree'|'third_party'|'other')."""

    def __init__(self, tree_root: Path, third_party: Path, wd: Path):
        self.tree = os.path.normcase(str(tree_root))
        self.tree_parent = os.path.normcase(str(tree_root.parent))
        self.tp = os.path.normcase(str(third_party))
        self.wd = wd
        self._real_tree = tree_root
        self._real_tp = third_party
        self._cache = {}

    def __call__(self, p):
        r = self._cache.get(p)
        if r is not None:
            return r
        q = p if os.path.isabs(p) else os.path.join(str(self.wd), p)
        q = os.path.normpath(q)
        nq = os.path.normcase(q)
        if nq.startswith(self.tree + os.sep):
            r = (os.path.relpath(q, str(self._real_tree.parent)).replace("\\", "/"), "tree")
        elif nq.startswith(self.tp + os.sep):
            r = ("third_party/" + os.path.relpath(q, str(self._real_tp)).replace("\\", "/"), "third_party")
        else:
            r = (q.replace("\\", "/"), "other")
        self._cache[p] = r
        return r


def _collect(tree_root: Path):
    out = []
    for root, dirs, files in os.walk(tree_root):
        dirs.sort()
        for name in files:
            ext = os.path.splitext(name)[1]
            if ext in EXTS:
                p = Path(root) / name
                rel = tree_root.name + "/" + p.relative_to(tree_root).as_posix()
                out.append((rel, p, EXTS[ext]))
    out.sort(key=lambda t: t[0].encode("utf-8"))
    return out


def _sha(*parts) -> str:
    h = hashlib.sha256()
    for p in parts:
        h.update(p.encode("utf-8") if isinstance(p, str) else p)
        h.update(b"\0")
    return h.hexdigest()


def _dumps(obj) -> str:
    return json.dumps(obj, sort_keys=True, indent=1, ensure_ascii=False) + "\n"


def _read_cache(wd):
    try:
        return json.loads((wd / "cache.json").read_text(encoding="utf-8"))
    except (OSError, ValueError):
        return {}


def _deps(path: Path):
    try:
        text = path.read_text(encoding="utf-8", errors="replace")
    except OSError:
        return None
    text = text.replace("\\\n", " ").replace("\\\r\n", " ")
    _t, _c, body = text.partition(":")
    files, cur, i = [], [], 0
    while i < len(body):
        c = body[i]
        if c == "\\" and i + 1 < len(body) and body[i + 1] == " ":
            cur.append(" ")
            i += 2
            continue
        if c.isspace():
            if cur:
                files.append("".join(cur))
                cur = []
        else:
            cur.append(c)
        i += 1
    if cur:
        files.append("".join(cur))
    return files


class Build:
    def __init__(self, tree_root, build_dir, markers="strict", third_party=None, force=False, keep=False):
        if markers not in ("strict", "off"):
            raise ValueError("markers must be 'strict' or 'off'")
        self.tree = Path(tree_root).resolve()
        if not self.tree.is_dir():
            raise cc.EnvError(f"tree not found: {self.tree}")
        self.build = Path(build_dir).resolve()
        self.wd = cc.work_dir(self.build)
        self.mode = markers
        self.strict = markers == "strict"
        self.force = force
        self.keep = keep
        self.env = cc.load_env(third_party)
        self.paths = Paths(self.tree, self.env.third_party, self.wd)
        self.diags = at.Diags()
        self.clang_invoked = False

    # ------------------------------------------------------------------------------------------
    def run(self) -> Path:
        self.wd.mkdir(parents=True, exist_ok=True)
        out = self.build / "typedb.json"
        files = _collect(self.tree)
        self.files = []
        for rel, p, kind in files:
            data = p.read_bytes()
            self.files.append((rel, p, kind, cc.sha256_bytes(data), data.decode("utf-8", "replace")))
        version = cc.clang_version(self.env, self.wd)
        self.version = version
        pk = cc.pch_key(self.env, version, self.build)
        tk = _sha(pk, self.mode, *[f"{r}\0{s}" for r, _p, _k, s, _t in self.files])
        cache = _read_cache(self.wd)
        if not self.force and cache.get("tree_key") == tk and out.is_file():
            try:
                if cc.sha256_file(out) == cache.get("json_sha"):
                    return out
            except OSError:
                pass
        pch = self.wd / "third_party.pch"
        if self.force or cache.get("pch_key") != pk or not pch.is_file():
            rc, err = cc.run(self.env, cc.clang_args("pch", self.env, build=self.build), cwd=str(self.wd))
            self.clang_invoked = True
            if rc != 0 or not pch.is_file():
                raise cc.EnvError(f"third_party/pch.h does not compile into a PCH:\n{err[-3000:]}")
        db = self._build_db()
        text = _dumps(db)
        tmp = out.with_suffix(".json.tmp")
        with open(tmp, "w", encoding="utf-8", newline="\n") as f:
            f.write(text)
        os.replace(tmp, out)
        cache = {"pch_key": pk, "tree_key": tk, "json_sha": cc.sha256_file(out),
                 "headers_key": getattr(self, "headers_key", None)}
        (self.wd / "cache.json").write_text(json.dumps(cache, sort_keys=True, indent=1), encoding="utf-8")
        return out

    # ------------------------------------------------------------------------------------------
    def _build_db(self):
        self.scans = {}
        for rel, _p, _kind, _sha_, text in self.files:
            scan = mk.scan(text, strict=self.strict)
            self.scans[rel] = scan
            if self.strict:
                for d in scan.diags:
                    self.diags.add(d.code, rel, d.line, d.message)
                if scan.origin is None:
                    self.diags.add("marker-missing", rel, 1, "no /// origin on line 1")
        import time
        self.timings = {}
        t0 = time.perf_counter()

        def lap(name):
            nonlocal t0
            now = time.perf_counter()
            self.timings[name] = round(now - t0, 3)
            t0 = now

        lap("scan")
        headers = [(rel, p) for rel, p, kind, _s, _t in self.files if kind == "h"]
        unity = self.wd / "unity.cpp"
        text = "\n".join(self._unity_lines(headers)) + "\n"
        # Rewrite only on change: a newer unity.cpp invalidates headers.pch for pass C.
        if not unity.is_file() or unity.read_text(encoding="utf-8") != text:
            _write_retry(unity, text)
        pool = None
        c_future = None
        if any(k == "cpp" for _r, _p, k, _s, _t in self.files):
            from concurrent.futures import ThreadPoolExecutor
            pool = ThreadPoolExecutor(max_workers=1)
            c_future = pool.submit(self._pass_c, headers)
        self.a_ok, model = self._pass_a(headers)
        lap("pass_a")
        self.model = model
        decls = model.decls if model else []
        by_file = defaultdict(list)
        for d in decls:
            by_file[d.file].append(d)
        scans_h = {r: s for r, s in self.scans.items() if not r.endswith(".cpp")}
        self.attacher = at.Attacher(by_file, scans_h, self.diags, self.strict)
        self.attacher.run()
        self._spec_names(decls)
        self.tg = None
        self.probe_by = {}
        lap("attach")
        if self.a_ok and model is not None:
            self._pass_b(model)
        lap("pass_b")
        cpp_results = {}
        if c_future is not None:
            res, c_diags = c_future.result()
            pool.shutdown()
            if self.a_ok and model is not None:
                cpp_results = res
                for d in c_diags:
                    self.diags.add(*d)
        lap("pass_c")
        db = self._assemble(decls, cpp_results)
        lap("assemble")
        return db

    # -- pass A ----------------------------------------------------------------------------------
    def _unity_lines(self, headers):
        return [f"#include <{rel.split('/', 1)[1]}>" for rel, _p in headers]

    def _pass_a(self, headers):
        unity = self.wd / "unity.cpp"
        ast_path = self.wd / "pass_a.json"
        dep = self.wd / "unity.d"
        args = cc.clang_args("tree", self.env, tree=self.tree, build=self.build) + cc.PASS_A + \
            cc.DEPS + [cc._posix(dep), cc._posix(unity)]
        rc, err = cc.run(self.env, args, cwd=str(self.wd), stdout_path=str(ast_path))
        self.clang_invoked = True
        ok = True
        for f, line, _col, sev, msg in clang_ast.parse_diagnostics(err):
            if sev in ("error", "fatal error"):
                ok = False
                rel, origin = self.paths(f)
                self.diags.add("clang-error", rel, line, msg)
        if rc != 0 and ok:
            ok = False
            self.diags.add("clang-error", "unity.cpp", 1, err.strip()[-500:] or f"clang exit {rc}")
        model = None
        try:
            root = clang_ast.load_json(ast_path)
            texts = {rel: t for rel, _p, _k, _s, t in self.files}
            model = clang_ast.extract(root, self.paths, texts.get)
        except (OSError, ValueError) as e:
            if ok:
                raise cc.EnvError(f"cannot read the AST of pass A: {e}") from None
        if not self.keep:
            _unlink(ast_path)
        deps = _deps(dep)
        if deps is not None:
            seen = {self.paths(x)[0] for x in deps}
            for rel, _p, kind, _s, _t in self.files:
                if kind == "inl" and rel not in seen:
                    self.diags.add("file-unreached", rel, 1, "file is not included by any header of the unity")
        return ok, model

    def _spec_names(self, decls):
        for d in decls:
            if d.ent == "record_template" and d.template in ("pattern", "partial", "nested"):
                names = set()
                owner = self._spec_owner(d)
                for m in d.markers:
                    if m.spec:
                        names.add(self.canon_spec(owner, m.spec))
                for c in d.children:
                    if c.ent == "function" and c.template != "function":
                        for m in c.markers:
                            if m.spec:
                                names.add(self.canon_spec(owner, m.spec))
                d.spec_names = sorted(names)
            elif d.ent == "function" and d.template == "function" and d.primary is None:
                owner = self._spec_owner(d)
                d.spec_names = sorted({self.canon_spec(owner, m.spec) for m in d.markers if m.spec})
            elif d.ent == "global" and d.in_pattern and d.primary is None:
                d.spec_names = sorted(self._global_specs(d))

    def _global_specs(self, d):
        """Canonical SPEC -> marker for a static member of a class template: lines on the in-class declaration
        and on its out-of-class definitions (the first line of a SPEC in source order is kept)."""
        owner = self._spec_owner(d)
        out = {}
        for x in [d] + sorted(d.redecls, key=lambda r: (r.file, r.line)):
            for m in getattr(x, "markers", ()):
                if m.spec and m.kind == "addr":
                    out.setdefault(self.canon_spec(owner, m.spec), m)
        return out

    @staticmethod
    def canon_spec(d, spec):
        spec = mk.normalize_spec(spec)
        if getattr(d, "nt_types", None) and any(d.nt_types):
            spec = mk.normalize_spec(clang_ast.cast_nt_args(spec, d.nt_types, d.enum_names))
        head = mk.strip_args(spec)
        scope = d.scope
        if scope and not head.startswith(scope + "::") and "::" not in head:
            return f"{scope}::{spec}"
        return spec

    # -- pass B ----------------------------------------------------------------------------------
    def _dynamic_hint(self, d):
        n = d.node
        dd = n.get("definitionData", {})
        if dd.get("isPolymorphic") or n.get("bases"):
            final = any(c.get("kind") == "FinalAttr" for c in n.get("inner", ()))
            return not final or bool(dd.get("isPolymorphic"))
        return False

    def _pass_b(self, model):
        b = pr.plan(model, self._dynamic_hint, [])
        includes = self._unity_lines([(r, p) for r, p, k, _s, _t in self.files if k == "h"])
        skip = set()
        failed = []
        res = self._compile_b(b, includes, skip, failed)
        if res is None:
            return
        mod, tg = res
        extra = self._incomplete_by_value(mod, tg)
        if extra:
            # by-value parameters and returns: the record must be complete, not a declaration in the DI
            top = max([p.n for p in b.probes] + [0])
            added = []
            for i, name in enumerate(extra, top + 1):
                p = b.add("complete", name, None, name, pr.complete_probe(i, name))
                p.n = i
                added.append(p)
            res2 = self._compile_b(b, includes, skip, failed)
            if res2 is not None:
                mod, tg = res2
            else:
                b.probes = [p for p in b.probes if p not in added]
        for p, msg in failed:
            d = p.decl
            if d is not None:
                self.diags.add("probe-failed", d.file, d.line, f"probe for {p.key} does not compile: {msg}")
        self.mod, self.tg = mod, tg
        self.probe_globals = {}
        for gname, g in self.mod.globals.items():
            if gname.startswith(("_ZTV", "_ZTI", "_ZTS", "_ZTT", "_ZTC")):
                continue
            mm = re.search(r"__typedb_([rsgfijtuy]\d+)", gname)
            if mm:
                self.probe_globals.setdefault(mm.group(1), g)
        self.probe_by = defaultdict(dict)
        self.probe_decl = {}
        for p in b.probes:
            if id(p) not in skip and p.decl is not None:
                self.probe_by[p.kind].setdefault((p.key, id(p.decl)) if p.kind == "function_instance" else p.key, p)
                self.probe_decl[(p.kind, id(p.decl))] = p

    def _compile_b(self, b, includes, skip, failed):
        """Compile the probes (skipping the ones that fail, once) -> (Module, TypeGraph) or None."""
        ll_path = self.wd / "pass_b.ll"
        for attempt in range(2):
            text, by_line = b.render(includes, skip)
            src = self.wd / "probes.cpp"
            _write_retry(src, text)
            args = cc.clang_args("tree", self.env, tree=self.tree, build=self.build) + cc.PASS_B + \
                [cc._posix(src), "-o", cc._posix(ll_path)]
            rc, err = cc.run(self.env, args, cwd=str(self.wd))
            if rc == 0:
                break
            lines = pr.failed_lines(err)
            bad = [by_line[ln] for ln in sorted(lines) if ln in by_line]
            if not bad or attempt == 1:
                if not any(p.kind == "complete" for p in b.probes):
                    for f, line, _col, sev, msg in clang_ast.parse_diagnostics(err):
                        if sev in ("error", "fatal error"):
                            self.diags.add("clang-error", self.paths(f)[0], line, f"pass B: {msg}")
                return None
            for p in bad:
                skip.add(id(p))
                if p.kind != "complete":
                    failed.append((p, _first_error(err, p.line)))
        ll = ll_path.read_text(encoding="utf-8", errors="replace")
        if not self.keep:
            _unlink(ll_path)
        mod = llvm_ir.parse(ll)
        return mod, TypeGraph(mod, self.paths)

    def _incomplete_by_value(self, mod, tg):
        """Class template specializations that a tree function takes or returns by value but the DI only declares.

        Candidates are read from the signatures of the tree's own subprograms and from the signatures of
        its probed free functions (their types are in the probe globals' debug info).
        """
        subs = []
        for n in mod.meta.select(lambda r: r[0] == "DISubprogram"):
            f = tg.file_of(n)
            if f is not None and f[1] == "tree" and n.get("type") is not None:
                subs.append(n.get("type"))
        for gname, g in mod.globals.items():
            mm = re.search(r"__typedb_[ft]\d+", gname)
            if mm and g.dbg is not None:
                gve = mod.meta.get(g.dbg)
                var = tg.node(gve.get("var")) if gve is not None else None
                fn = self._strip_to_function_in(tg, var.get("type") if var is not None else None)
                if fn is not None:
                    subs.append(fn)
        out = set()
        for st in subs:
            sn = tg.node(st)
            if sn is None or sn.kind != "DISubroutineType":
                continue
            for t in tg.tuple(sn.get("types")):
                n = tg.node(t) if t is not None else None
                while n is not None and n.kind == "DIDerivedType" and n.get("tag") in (
                        "DW_TAG_typedef", "DW_TAG_const_type", "DW_TAG_volatile_type"):
                    n = tg.node(n.get("baseType"))
                if n is None or n.kind != "DICompositeType" or n.get("tag") not in (
                        "DW_TAG_structure_type", "DW_TAG_class_type", "DW_TAG_union_type"):
                    continue
                name = tg.full_name(n)
                if "DIFlagFwdDecl" in llvm_ir.flags(n) and name not in tg.records and "<" in name \
                        and "<local>" not in name and "(anon " not in name:
                    out.add(name)
        return sorted(out)

    @staticmethod
    def _strip_to_function_in(tg, ref):
        n = tg.node(ref)
        while n is not None:
            if n.kind == "DISubroutineType":
                return ref
            if n.kind == "DIDerivedType":
                ref = n.get("baseType")
                n = tg.node(ref)
                continue
            return None
        return None

    def _probe_global(self, kind, letter, key):
        p = self.probe_by.get(kind, {}).get(key)
        if p is None:
            return None
        return self.probe_globals.get(f"{letter}{p.n}")

    def _probe_di_type(self, g):
        if g is None or g.dbg is None:
            return None
        gve = self.mod.meta.get(g.dbg)
        if gve is None:
            return None
        var = self.tg.node(gve.get("var"))
        return var.get("type") if var is not None else None

    # -- pass C ----------------------------------------------------------------------------------
    def _pass_c(self, headers):
        unity = self.wd / "unity.cpp"
        hpch = self.wd / "headers.pch"
        hk = _sha("headers", *[f"{r}\0{s}" for r, _p, k, s, _t in self.files if k != "cpp"],
                  cc.sha256_file(self.wd / "third_party.pch"))
        self.headers_key = hk
        cache = _read_cache(self.wd)
        if self.force or cache.get("headers_key") != hk or not hpch.is_file() \
                or hpch.stat().st_mtime < unity.stat().st_mtime:
            args = cc.clang_args("tree", self.env, tree=self.tree, build=self.build) + cc.EMIT_PCH + \
                [cc._posix(unity), "-o", cc._posix(hpch)]
            rc, err = cc.run(self.env, args, cwd=str(self.wd))
            if rc != 0:
                return {}, []
        cpps = [(rel, p, s, t) for rel, p, k, s, t in self.files if k == "cpp"]
        res = cppdefs.run_all(self.env, self.tree, self.build, cpps, hk, self.paths)
        diags = []
        for rel in sorted(res):
            for f, line, msg in res[rel]["errors"]:
                diags.append(("clang-error", f or rel, line, msg))
        return res, diags

    # -- assembly --------------------------------------------------------------------------------
    def _assemble(self, decls, cpp_results):
        tg = self.tg
        prim = [d for d in decls if d.primary is None]
        records_ast = [d for d in prim if d.ent in ("record", "record_template")]
        funcs_ast = [d for d in prim if d.ent == "function"]
        self._di_methods = {}
        rec_out = {}
        dyn = {}
        for d in records_ast:
            dyn[id(d)] = self._record_dynamic(d)
        # functions first pass: DI info, virtual flags
        finfo = {}
        for d in funcs_ast:
            finfo[id(d)] = self._function_di(d)
            sp = finfo[id(d)].get("sp")
            d.virtual = bool(d.node.get("virtual")) or (sp is not None and "DISPFlagVirtual" in str(sp.get("spFlags"))
                                                         or sp is not None and "DISPFlagPureVirtual" in str(sp.get("spFlags")))
        self._inst_keys = {}
        for d in funcs_ast:
            for inst in finfo[id(d)].get("instances", {}).values():
                if inst.get("mangled"):
                    self._inst_keys[inst["mangled"]] = f"{d.qualified}@{d.file}:{d.line}"
        cpp_defs, cpp_bodies = self._cpp_defs(cpp_results, funcs_ast, decls)

        def has_body(x):
            return bool(getattr(x, "has_body", False)) or any(getattr(r, "has_body", False) for r in x.redecls) \
                or id(x) in cpp_bodies

        insts = {id(d): sorted(set(d.instantiations) | set(getattr(d, "spec_names", ())))
                 for d in records_ast if d.ent == "record_template"}
        self.attacher.obligations([d for d in decls], {r: s for r, s in self.scans.items()
                                                       if not r.endswith(".cpp")},
                                  lambda x: dyn.get(id(x)), has_body, insts)
        # variants
        reqs = []
        for d in funcs_ast:
            m = d.node.get("mangledName")
            if m and d.ast_kind in ("CXXConstructorDecl", "CXXDestructorDecl"):
                reqs.append((m, "ctor" if d.ast_kind == "CXXConstructorDecl" else "dtor", d.virtual))
            for inst in finfo[id(d)].get("instances", {}).values():
                if inst.get("mangled") and d.ast_kind in ("CXXConstructorDecl", "CXXDestructorDecl"):
                    reqs.append((inst["mangled"], "ctor" if d.ast_kind == "CXXConstructorDecl" else "dtor", d.virtual))
        self.demangled = vr.demangle_all(self.env.cxxfilt, vr.plan(reqs)) if reqs else {}

        functions = [self._function_out(d, finfo[id(d)], has_body(d)) for d in funcs_ast]
        records = self._records_out(records_ast, dyn, functions)
        globals_ = [self._global_out(d) for d in prim if d.ent == "global"]
        enums = [self._enum_out(d) for d in prim if d.ent == "enum"]
        typedefs = [self._typedef_out(d) for d in prim if d.ent == "typedef"]
        files = []
        for rel, _p, kind, sha, _t in self.files:
            o = self.scans[rel].origin
            files.append({"path": rel, "kind": kind, "origin": o.value if o is not None else None, "sha256": sha})
        functions.sort(key=lambda f: f["key"])
        records.sort(key=lambda r: r["name"])
        globals_.sort(key=lambda g: (g["qualified"], g["file"], g["line"]))
        enums.sort(key=lambda e: (e["qualified"], e["file"], e["line"]))
        typedefs.sort(key=lambda t: (t["qualified"], t["file"], t["line"]))
        cpp_defs.sort(key=lambda c: (c["file"], c["line"], c["qualified"] or ""))
        diags = sorted({json.dumps(x, sort_keys=True) for x in self.diags.items})
        diags = sorted((json.loads(x) for x in diags), key=lambda x: (x["file"] or "", x["line"] or 0, x["code"], x["message"]))
        types = dict(sorted(tg.types.items())) if tg else {}
        flags = cc.relative_flags(cc.clang_args("tree", self.env, tree=self.tree, build=self.build),
                                  self.env, tree=self.tree, build=self.build)
        tp_files = []
        for p in cc.third_party_files(self.env):
            tp_files.append(f"{p.relative_to(self.env.third_party).as_posix()}\0{cc.sha256_file(p)}")
        db = {
            "schema": SCHEMA,
            "ok": not any(x["severity"] == "error" for x in diags),
            "markers": self.mode,
            "tool": {"typedb": cc.TYPEDB_VERSION, "clang": self.version, "flags_sha": _sha(*flags)},
            "inputs": {"tree_root": self.tree.name,
                       "tree_sha": _sha(*[f"{r}\0{s}" for r, _p, _k, s, _t in self.files]),
                       "third_party_sha": _sha(*tp_files),
                       "prelude_sha": cc.sha256_file(self.env.prelude)},
            "files": files,
            "types": types,
            "records": records,
            "functions": functions,
            "globals": globals_,
            "enums": enums,
            "typedefs": typedefs,
            "cpp_defs": cpp_defs,
            "diagnostics": diags,
        }
        db["stats"] = stats(db)
        return db

    # -- records ---------------------------------------------------------------------------------
    def _record_dynamic(self, d):
        if self.tg is not None:
            n = self.tg.records.get(d.qualified)
            if n is not None:
                return self.tg.dynamic(n)
            if d.ent == "record_template":
                for inst in sorted(set(d.instantiations) | set(getattr(d, "spec_names", ()))):
                    n = self.tg.records.get(inst)
                    if n is not None:
                        return self.tg.dynamic(n)
        dd = d.node.get("definitionData", {})
        if dd.get("isPolymorphic"):
            return True
        return any(b.get("isVirtual") for b in d.node.get("bases", ())) or None if d.node.get("bases") else False

    def _records_out(self, records_ast, dyn, functions=()):
        tg = self.tg
        out = {}
        by_name = {d.qualified: d for d in records_ast}
        pattern_of = {}
        for d in records_ast:
            if d.ent == "record_template" and d.template in ("pattern", "partial", "nested"):
                for inst in sorted(set(d.instantiations) | set(getattr(d, "spec_names", ()))):
                    pattern_of[inst] = d
        for d in records_ast:
            out[d.qualified] = self._record_tree(d, dyn.get(id(d)))
        if tg is None:
            return list(out.values())
        # instantiations and reachable records
        todo = []
        for name, n in tg.records.items():
            if name in out:
                continue
            f = tg.file_of(n)
            if f is not None and f[1] == "tree":
                todo.append(name)
        reach = set()
        stack = [r for r in list(out) if r in tg.records] + todo + \
            [r for r in self._by_value_records(functions) if r in tg.records]
        while stack:
            name = stack.pop()
            if name in reach:
                continue
            reach.add(name)
            n = tg.records.get(name)
            if n is None:
                continue
            fields, bases, _m = tg.record_layout(n)
            for f in fields:
                for dep in self._value_records(f.get("baseType")):
                    stack.append(dep)
            for b in bases:
                stack.append(b[0])
        for name in sorted(reach):
            if name in out or name not in tg.records:
                continue
            out[name] = self._record_di(name, tg.records[name], pattern_of.get(name), by_name)
        return list(out.values())

    def _by_value_records(self, functions):
        """Records that functions of the tree take or return by value (through typedefs and cv)."""
        types = self.tg.types
        out = set()

        def add(tid):
            while tid in types:
                t = types[tid]
                if t["kind"] in ("typedef", "cv"):
                    tid = t["of"]
                    continue
                if t["kind"] == "record" and t.get("key"):
                    out.add(t["key"])
                break

        for f in functions:
            for r in [f["return"]] + [p["type"] for p in f["params"]]:
                add(r)
            for inst in f["instances"]:
                for r in [inst["return"]] + [p["type"] for p in inst["params"] or ()]:
                    add(r)
        return sorted(out)

    def _value_records(self, ref):
        tg = self.tg
        n = tg.node(ref)
        while n is not None:
            if n.kind == "DICompositeType":
                if n.get("tag") == "DW_TAG_array_type":
                    n = tg.node(n.get("baseType"))
                    continue
                if n.get("tag") in ("DW_TAG_structure_type", "DW_TAG_class_type", "DW_TAG_union_type"):
                    return [tg.full_name(n)]
                return []
            if n.kind == "DIDerivedType" and n.get("tag") in ("DW_TAG_typedef", "DW_TAG_const_type",
                                                                "DW_TAG_volatile_type", "DW_TAG_member"):
                n = tg.node(n.get("baseType"))
                continue
            return []
        return []

    def _record_markers(self, d):
        size_m = vt_m = None
        inst = []
        lua = []
        for m in getattr(d, "markers", []):
            if m.kind in ("size", "vtable"):
                if m.spec is None:
                    if m.kind == "size":
                        size_m = m.value
                    else:
                        vt_m = m.value
                else:
                    inst.append({"spec": self.canon_spec(self._spec_owner(d), m.spec), "kind": m.kind, "value": m.value})
            elif m.kind == "lua":
                lua.append({"text": m.value, "file": d.file, "line": m.line})
        inst.sort(key=lambda x: (x["spec"], x["kind"]))
        return size_m, vt_m, inst, lua

    def _comment(self, d):
        scan = self.scans.get(d.file)
        if scan is None:
            return None
        key = d.block.first_line if getattr(d, "block", None) is not None else d.line
        c = scan.comments.get(key)
        if c is None:
            return None
        return {"line": c[0], "text": c[1]}

    def _record_tree(self, d, dynamic):
        tg = self.tg
        size_m, vt_m, inst_m, lua = self._record_markers(d)
        dd = d.node.get("definitionData", {})
        tmpl = None
        if d.template == "explicit":
            tmpl = {"pattern": d.pattern_name, "args": [mk.normalize_spec(a) for a in d.template_args]}
        elif d.template == "partial":
            tmpl = {"pattern": d.qualified, "args": None, "partial_of": d.pattern_name}
        elif d.ent == "record_template":
            tmpl = {"pattern": d.qualified, "args": None}
        rec = {
            "name": d.qualified, "kind": getattr(d, "tag", "struct"), "file": d.file, "line": d.line,
            "origin": "tree", "complete": True, "template": tmpl,
            "size": None, "align": None, "dsize": None, "dynamic": dynamic,
            "trivial_for_calls": bool(dd.get("canPassInRegisters")) if d.ent == "record" else None,
            "size_marker": size_m, "vtable_marker": vt_m, "instance_markers": inst_m,
            "vtable_symbol": None, "typeinfo_symbol": None, "bases": [], "vptrs": [], "fields": [],
            "vtable": None, "markers": {"lua": lua}, "comment": self._comment(d),
            "accessed_from": None,
        }
        ast_fields = [c for c in d.children if c.ent == "field" or (c.ast_kind == "FieldDecl")]
        n = tg.records.get(d.qualified) if tg is not None else None
        if n is not None and d.ent == "record":
            self._fill_layout(rec, n, d.qualified, ast_fields, d, "record")
        else:
            rec["fields"] = [self._ast_field(c) for c in ast_fields if c.ent == "field"]
            if d.ent == "record":
                rec["bases"] = [{"name": b.get("type", {}).get("qualType"), "offset": None,
                                 "virtual": bool(b.get("isVirtual")), "access": b.get("access", "public")}
                                for b in d.node.get("bases", ())]
        if getattr(d, "anonymous", False) and d.parent is not None:
            rec["accessed_from"] = self._accessed_from(d)
        return rec

    def _accessed_from(self, d):
        """Unnamed anonymous member records: `///<` offsets count from the enclosing named record."""
        if self.tg is None:
            return None
        off = 0
        cur = d
        while getattr(cur, "anonymous", False) and cur.parent is not None:
            pn = self.tg.records.get(cur.parent.qualified)
            if pn is None:
                return None
            fields, _b, _m = self.tg.record_layout(pn)
            hit = None
            for f in fields:
                bn = self.tg.node(f.get("baseType"))
                if bn is not None and not f.get("name") and bn.kind == "DICompositeType" \
                        and self.tg.full_name(bn) == cur.qualified:
                    hit = f
            if hit is None:
                return None
            off += (hit.get("offset") or 0) // 8
            cur = cur.parent
        return {"record": cur.qualified, "offset": off}

    def _ast_field(self, c):
        return {"name": c.name, "type": None, "offset": None, "size": None, "bit_offset": None,
                "bit_size": None, "line": c.line,
                "offset_marker": c.trailing.value if getattr(c, "trailing", None) is not None else None}

    def _fill_layout(self, rec, n, name, ast_fields, d, origin):
        tg = self.tg
        fields, bases, _methods = tg.record_layout(n)
        rec["size"] = (n.get("size") or 0) // 8
        if n.get("align"):
            rec["align"] = n.get("align") // 8
        flg = llvm_ir.flags(n)
        if rec.get("trivial_for_calls") is None and origin != "tree-pattern":
            if "DIFlagTypePassByValue" in flg:
                rec["trivial_for_calls"] = True
            elif "DIFlagTypePassByReference" in flg:
                rec["trivial_for_calls"] = False
        g = None
        if d is not None and d.ent == "record":
            g = self._probe_global("record", "s", d.qualified)
        elif d is not None:
            g = self._probe_global("instance", "j", name)
        if g is not None:
            vals = llvm_ir.int_array(g)
            if vals and len(vals) == 3:
                if vals[0] != rec["size"]:
                    self.diags.add("internal", rec["file"], rec["line"],
                                   f"sizeof({name}) = {vals[0]}, DI size = {rec['size']}")
                rec["size"], rec["align"], rec["dsize"] = vals
        rec["dynamic"] = tg.dynamic(n)
        by_name = {}
        for c in ast_fields:
            if c.name:
                by_name.setdefault(c.name, c)
        out_f = []
        for f in fields:
            fl = llvm_ir.flags(f)
            bit = "DIFlagBitField" in fl
            off_bits = f.get("offset") or 0
            fname = f.get("name")
            ac = by_name.get(fname) if fname else None
            t = tg.tid(f.get("baseType"))
            tnode = tg.types.get(t, {})
            size = (f.get("size") or 0) // 8 if not bit else tnode.get("size")
            out_f.append({
                "name": fname, "type": t, "offset": off_bits // 8, "size": size,
                "bit_offset": off_bits if bit else None, "bit_size": f.get("size") if bit else None,
                "line": ac.line if ac is not None else f.get("line"),
                "offset_marker": ac.trailing.value if ac is not None and getattr(ac, "trailing", None) is not None else None,
            })
        out_f.sort(key=lambda x: (x["bit_offset"] if x["bit_offset"] is not None else x["offset"] * 8, x["line"] or 0))
        rec["fields"] = out_f
        ast_bases = d.node.get("bases", []) if d is not None and d.ent == "record" else []
        groups = None
        if rec["dynamic"] and d is not None:
            kind = "dynamic" if d.ent == "record" else "instance_dynamic"
            letter = "P" if d.ent == "record" else "Q"
            p = self.probe_by.get(kind, {}).get(name if d.ent != "record" else d.qualified)
            if p is not None:
                groups = self._vtable(rec, n, p.n, letter)
        vb_off = {}
        if groups is not None:
            vb_off = groups[1]
            rec["vtable"] = {"groups": groups[0]}
            rec["vptrs"] = sorted({-g["offset_to_top"] for g in groups[0]})
        out_b = []
        for i, (bname, off, virtual, bfl, bn) in enumerate(bases):
            access = None
            if i < len(ast_bases):
                access = ast_bases[i].get("access")
            if access is None:
                access = "public" if "DIFlagPublic" in bfl else "protected" if "DIFlagProtected" in bfl else \
                    "private" if "DIFlagPrivate" in bfl else ("private" if n.get("tag") == "DW_TAG_class_type" else "public")
            out_b.append({"name": bname, "offset": vb_off.get(bname) if virtual else off // 8,
                          "virtual": virtual, "access": access})
        rec["bases"] = out_b

    def _record_di(self, name, n, pattern, by_name):
        tg = self.tg
        f = tg.file_of(n) or (None, "other")
        origin = "tree" if f[1] == "tree" else "third_party"
        tmpl = None
        if "<" in name:
            pat = name.split("<", 1)[0]
            args = None
            if pattern is not None:
                pat = pattern.qualified
            tmpl = {"pattern": pat, "args": self._template_args(name)}
        rec = {
            "name": name, "kind": {"DW_TAG_structure_type": "struct", "DW_TAG_class_type": "class",
                                   "DW_TAG_union_type": "union"}[n.get("tag")],
            "file": f[0], "line": n.get("line"), "origin": origin, "complete": True, "template": tmpl,
            "size": None, "align": None, "dsize": None, "dynamic": tg.dynamic(n), "trivial_for_calls": None,
            "size_marker": None, "vtable_marker": None, "instance_markers": [], "vtable_symbol": None,
            "typeinfo_symbol": None, "bases": [], "vptrs": [], "fields": [], "vtable": None,
            "markers": {"lua": []}, "comment": None, "accessed_from": None,
        }
        ast_fields = []
        if pattern is not None:
            size_m, vt_m, inst_m, _lua = self._record_markers(pattern)
            spec_size = [x["value"] for x in inst_m if x["spec"] == name and x["kind"] == "size"]
            spec_vt = [x["value"] for x in inst_m if x["spec"] == name and x["kind"] == "vtable"]
            rec["size_marker"] = spec_size[0] if spec_size else size_m
            rec["vtable_marker"] = spec_vt[0] if spec_vt else vt_m
            ast_fields = [c for c in pattern.children if c.ent == "field"]
        self._fill_layout(rec, n, name, ast_fields, pattern, "di")
        if pattern is None:
            rec["dsize"] = None
        return rec

    @staticmethod
    def _template_args(name):
        inner = name[name.index("<") + 1:name.rindex(">")]
        out, depth, cur = [], 0, []
        for ch in inner:
            if ch == "<":
                depth += 1
            elif ch == ">":
                depth -= 1
            if ch == "," and depth == 0:
                out.append("".join(cur).strip())
                cur = []
            else:
                cur.append(ch)
        out.append("".join(cur).strip())
        return out

    def _vtable(self, rec, n, num, letter):
        tg = self.tg
        mod = self.mod
        zv = mod.globals.get(f"_ZTV{len(f'__typedb_{letter}{num}')}__typedb_{letter}{num}")
        zi = mod.globals.get(f"_ZTI{len(f'__typedb_{letter}{num}')}__typedb_{letter}{num}")
        if zv is None or zi is None:
            return None
        arrays = llvm_ir.vtable_arrays(zv)
        base_ti = llvm_ir.typeinfo_base(zi)
        if not arrays or base_ti is None:
            return None
        tail = base_ti[4:]
        rec["typeinfo_symbol"] = base_ti
        rec["vtable_symbol"] = "_ZTV" + tail
        inner = tail[1:-1] if tail.startswith("N") and tail.endswith("E") else tail
        q_d1 = f"_ZN{inner}D1Ev"
        q_d0 = f"_ZN{inner}D0Ev"
        probe_name = f"{len(f'__typedb_{letter}{num}')}__typedb_{letter}{num}"
        # `virtual ~Q() = 0;` with a body: the probe's own destructor overrides it, but the slots of Q's vtable
        # are __cxa_pure_virtual; the destructor function itself keeps its D1/D0 symbols
        dtor_pure = any(m.kind == "DISubprogram" and str(m.get("name") or "").startswith("~")
                        and "DISPFlagPureVirtual" in str(m.get("spFlags")) for m in tg.members(n))
        groups = []
        for gi, arr in enumerate(arrays):
            ti = next((i for i, e in enumerate(arr) if e[0] == "sym" and e[1].startswith("_ZTI")), None)
            if ti is None or ti == 0:
                continue
            prefix = [e[1] if e[0] == "int" else None for e in arr[:ti]]
            ott = prefix[-1]
            vbo = prefix[:-1]
            slots = arr[ti + 1:]
            if gi == 0 and slots:
                slots = slots[:-1]
            groups.append({"offset_to_top": ott, "vbase_offsets": vbo, "raw": slots, "ti": ti})
        # subobjects for group naming and virtual base offsets
        g_by_off = {-g["offset_to_top"]: g for g in groups}
        vb_off = {}
        subobjects = []

        def walk(node, name, off, depth):
            subobjects.append((off, depth, name, node))
            _f, bases, _m = tg.record_layout(node)
            for bname, boff, virtual, _fl, bn in bases:
                if bn is None:
                    continue
                if virtual:
                    if bname in vb_off:
                        continue
                    g = g_by_off.get(off)
                    if g is None:
                        continue
                    idx = g["ti"] + 1 - boff // 4
                    entries = g["vbase_offsets"] + [g["offset_to_top"]]
                    if 0 <= idx < len(entries) and entries[idx] is not None:
                        vb_off[bname] = off + entries[idx]
                        if depth == 0:
                            pass
                        walk(bn, bname, off + entries[idx], depth + 1)
                else:
                    walk(bn, bname, off + boff // 8, depth + 1)

        walk(n, rec["name"], 0, 0)
        direct_vb = {}
        for bname, boff, virtual, _fl, bn in tg.record_layout(n)[1]:
            if virtual and bname in vb_off:
                direct_vb[bname] = vb_off[bname]
        out = []
        for g in groups:
            off = -g["offset_to_top"]
            cands = [s for s in subobjects if s[0] == off and tg.dynamic(s[3])]
            cands.sort(key=lambda s: s[1])
            sub = cands[0] if cands else (off, 0, rec["name"], n)
            slots = []
            for j, e in enumerate(g["raw"]):
                slots.append(self._slot(e, j, sub, subobjects, probe_name, q_d1, q_d0, dtor_pure))
            out.append({"subobject": sub[2], "offset_to_top": g["offset_to_top"],
                        "vbase_offsets": g["vbase_offsets"], "slots": slots})
        return out, vb_off

    def _slot(self, e, j, sub, subobjects, probe_name, q_d1, q_d0, dtor_pure=False):
        if e[0] != "sym":
            return {"kind": "method", "function": None, "symbol": None, "thunk": None}
        sym = e[1]
        thunk = None
        target = sym
        prefix = ""
        t = parse_thunk(sym)
        if t is not None:
            target, thunk, prefix = t
        if sym == "__cxa_pure_virtual":
            fn = self._pure_at(sub, subobjects, j)
            return {"kind": "pure", "function": fn, "symbol": sym, "thunk": None}
        pn = f"_ZN{probe_name}D"
        if target.startswith(pn):
            variant = target[len(pn):len(pn) + 1]
            want = q_d0 if variant == "0" else q_d1
            fn = self._fkey(want)
            symbol = (prefix + want[2:]) if prefix else want
            if dtor_pure:
                return {"kind": "pure", "function": fn or self._fkey(q_d1), "symbol": "__cxa_pure_virtual",
                        "thunk": None}
            return {"kind": "D0" if variant == "0" else "D1", "function": fn or self._fkey(q_d1),
                    "symbol": symbol, "thunk": thunk}
        return {"kind": "method", "function": self._fkey(target), "symbol": sym, "thunk": thunk}

    def _fkey(self, mangled):
        keys = getattr(self, "_fkeys", None)
        if keys is None:
            keys = self._fkeys = {d.node.get("mangledName") for d in self.model.decls
                                  if d.ent == "function" and d.primary is None and d.node.get("mangledName")}
        if mangled in keys:
            return mangled
        return getattr(self, "_inst_keys", {}).get(mangled)

    def _pure_at(self, sub, subobjects, j):
        tg = self.tg
        off = sub[0]
        chain = [s for s in subobjects if s[0] == off]
        chain.sort(key=lambda s: s[1])
        for s in chain:
            for m in tg.members(s[3]):
                if m.kind == "DISubprogram" and m.get("virtualIndex") == j and \
                        "DISPFlagPureVirtual" in str(m.get("spFlags")):
                    ln = m.get("linkageName")
                    if ln:
                        self._fkey(ln)
                        u = _unify_structor(ln, self._fkeys)
                        return self._fkey(u) or u
        return None

    # -- functions -------------------------------------------------------------------------------
    def _method_index(self, rec_name):
        idx = self._di_methods.get(rec_name)
        if idx is not None:
            return idx
        idx = {"by_link": {}, "by_name": defaultdict(list)}
        n = self.tg.records.get(rec_name) if self.tg else None
        if n is not None:
            for m in self.tg.members(n):
                if m.kind != "DISubprogram":
                    continue
                ln = m.get("linkageName")
                if ln:
                    for c in [ln] + vr.candidates(ln, "C4", "C1") + vr.candidates(ln, "D4", "D1"):
                        idx["by_link"].setdefault(c, m)
                idx["by_name"][m.get("name")].append(m)
        self._di_methods[rec_name] = idx
        return idx

    def _function_di(self, d):
        """→ {sp, ret, params, variadic, this, instances{spec: {...}}}"""
        info = {"sp": None, "ret": None, "params": None, "variadic": bool(d.node.get("variadic")),
                "this": None, "instances": {}}
        if self.tg is None:
            return info
        mangled = d.node.get("mangledName")
        tg = self.tg
        if d.scope_kind == "class" and self._probe_for(d, "function"):
            g = self._decl_probe_global(d, "function", "f")
            sym = llvm_ir.ptr_target(g) if g is not None else None
            if sym is not None and mangled and sym != mangled:
                self.diags.add("internal", d.file, d.line, f"mangledName {mangled} != probe symbol {sym}")
            fn = self._strip_to_function(self._probe_di_type(g)) if g is not None else None
            if fn is not None:
                ret, params, variadic, this = tg.signature(fn)
                # an explicit specialization has no storage class of its own: no `this` means static
                info.update(ret=ret, params=params, variadic=variadic, this=this, static=this is None)
        elif d.scope_kind == "class" and d.parent is not None and not d.in_pattern:
            idx = self._method_index(d.parent.qualified)
            sp = idx["by_link"].get(mangled) if mangled else None
            if sp is None and mangled:
                same = [m for m in idx["by_name"].get(d.name, []) if m.get("line") == (d.node.get("_loc") or d.begin).line]
                if same and same[0].get("linkageName"):
                    self.diags.add("internal", d.file, d.line,
                                   f"mangledName {mangled} != DI linkageName {same[0].get('linkageName')}")
            if sp is not None:
                info["sp"] = sp
                ret, params, variadic, this = tg.signature(sp.get("type"))
                info.update(ret=ret, params=params, variadic=variadic, this=this)
        elif d.template is None and d.scope_kind == "namespace":
            g = self._decl_probe_global(d, "function", "f")
            if g is not None:
                sym = llvm_ir.ptr_target(g)
                if sym is not None and mangled and sym != mangled:
                    self.diags.add("internal", d.file, d.line, f"mangledName {mangled} != probe symbol {sym}")
                t = self._probe_di_type(g)
                fn = self._strip_to_function(t)
                if fn is not None:
                    ret, params, variadic, _this = tg.signature(fn, skip_this=False)
                    info.update(ret=ret, params=params, variadic=variadic)
        # instances
        for m in d.markers if hasattr(d, "markers") else ():
            if m.spec is None:
                continue
            spec = self.canon_spec(self._spec_owner(d), m.spec)
            if spec in info["instances"]:
                continue
            inst = {"mangled": None, "ret": None, "params": None, "variadic": False, "this": None}
            if d.in_pattern and d.template != "function":
                rec = self._pattern_root(d)
                inst_name = spec
                idx = self._method_index(inst_name)
                line = (d.node.get("_loc") or d.begin).line
                cands = [x for x in idx["by_name"].get(mk.strip_args(d.name), []) if x.get("line") == line]
                if len(cands) > 1:
                    nparams = sum(1 for c in d.node.get("inner", ()) if c.get("kind") == "ParmVarDecl")
                    cands = [x for x in cands if len(tg.signature(x.get("type"))[1]) == nparams] or cands
                if cands:
                    sp = cands[0]
                    ret, params, variadic, this = tg.signature(sp.get("type"))
                    inst.update(mangled=_unify_structor(sp.get("linkageName")) if sp.get("linkageName") else None,
                                ret=ret, params=params, variadic=variadic, this=this)
            else:
                g = self._probe_global("function_instance", "t", (spec, id(d)))
                if g is not None:
                    inst["mangled"] = llvm_ir.ptr_target(g)
                    fn = self._strip_to_function(self._probe_di_type(g))
                    if fn is not None:
                        ret, params, variadic, this = tg.signature(fn)
                        inst.update(ret=ret, params=params, variadic=variadic, this=this)
            info["instances"][spec] = inst
        return info

    def _probe_for(self, d, kind):
        return (kind, id(d)) in getattr(self, "probe_decl", {})

    def _decl_probe_global(self, d, kind, letter):
        p = getattr(self, "probe_decl", {}).get((kind, id(d)))
        return self.probe_globals.get(f"{letter}{p.n}") if p is not None else None

    def _spec_owner(self, d):
        if d.in_pattern:
            r = self._pattern_root(d)
            if r is not None:
                return r
        return d

    def _pattern_root(self, d):
        p = d.parent
        while p is not None and p.template not in ("pattern", "partial"):
            p = p.parent
        return p

    def _strip_to_function(self, ref):
        tg = self.tg
        n = tg.node(ref)
        while n is not None:
            if n.kind == "DISubroutineType":
                return ref
            if n.kind == "DIDerivedType":
                ref = n.get("baseType")
                n = tg.node(ref)
                continue
            return None
        return None

    def _return_indirect(self, tid):
        tg = self.tg
        t = tg.types.get(tid) if tg else None
        while t is not None:
            k = t["kind"]
            if k == "record":
                return True
            if k == "typedef":
                t = tg.types.get(t["of"])
            elif k == "cv":
                t = tg.types.get(t["of"])
            else:
                return False
        return False

    def _function_out(self, d, info, body):
        n = d.node
        mangled = n.get("mangledName")
        kind = {"CXXConstructorDecl": "ctor", "CXXDestructorDecl": "dtor",
                "CXXConversionDecl": "conversion"}.get(d.ast_kind) or ("method" if d.scope_kind == "class" else "function")
        key = mangled if mangled else f"{d.qualified}@{d.file}:{d.line}"
        variants = None
        if mangled and kind in ("ctor", "dtor"):
            variants, err = vr.resolve(mangled, kind, d.virtual, self.demangled)
            if err:
                self.diags.add("internal", d.file, d.line, f"structor variants: {err}")
        linkage = _linkage(mangled) if mangled else ("internal" if d.anon_ns else "external")
        qt = n.get("type", {}).get("qualType", "")
        is_const = bool(re.search(r"\)\s*const\b", qt))
        text = self._decl_text(d)
        params_unknown = bool(re.search(r"\(\s*UNK_PARAMS\s*\)", text))
        names = [c.get("name") for c in n.get("inner", ()) if c.get("kind") == "ParmVarDecl"]
        ptypes = info.get("params")
        params = []
        if params_unknown:
            params = []
        elif ptypes is not None:
            for i, t in enumerate(ptypes):
                params.append({"name": names[i] if i < len(names) else None, "type": t})
        else:
            params = [{"name": nm, "type": None} for nm in names]
        variadic = bool(info.get("variadic")) and not params_unknown
        ret = info.get("ret")
        sp = info.get("sp")
        vindex = sp.get("virtualIndex") if sp is not None else None
        mk_ = self._fmarkers(d)
        instances = []
        by_spec = defaultdict(list)
        owner = self._spec_owner(d)
        for m in d.markers:
            if m.spec is not None:
                by_spec[self.canon_spec(owner, m.spec)].append(m)
        for spec, inst in sorted(info["instances"].items()):
            im = by_spec.get(spec, [])
            iv = None
            if inst.get("mangled") and kind in ("ctor", "dtor"):
                iv, err = vr.resolve(inst["mangled"], kind, d.virtual, self.demangled)
                if err:
                    self.diags.add("internal", d.file, d.line, f"structor variants of {spec}: {err}")
            iparams = None
            if inst.get("params") is not None:
                iparams = [{"name": names[i] if i < len(names) else None, "type": t}
                           for i, t in enumerate(inst["params"])]
            instances.append({
                "spec": spec,
                "addr": next((m.value for m in im if m.kind == "addr"), None),
                "base": next((m.value for m in im if m.kind == "base"), None),
                "deleting": next((m.value for m in im if m.kind == "deleting"), None),
                "thunks": sorted(m.value for m in im if m.kind == "thunk"),
                "fastcc": any(m.fastcc for m in im if m.kind == "addr"),
                "mangled": inst.get("mangled"), "variants": iv,
                "return": inst.get("ret"), "params": iparams,
            })
        redecls = []
        for r in d.redecls:
            redecls.append({"file": r.file, "line": r.line, "markers": self._fmarkers(r)})
        redecls.sort(key=lambda x: (x["file"], x["line"]))
        return {
            "key": key, "qualified": d.qualified, "name": d.name, "scope": d.scope, "scope_kind": d.scope_kind,
            "file": d.file, "line": d.line, "kind": kind, "mangled": mangled, "variants": variants,
            "linkage": linkage, "static": n.get("storageClass") == "static" or bool(info.get("static")),
            "template": d.template == "function" or bool(d.in_pattern),
            "virtual": bool(d.virtual),
            "pure": bool(n.get("pure")), "const": is_const, "has_body": bool(body),
            "access": d.access if d.scope_kind == "class" else None,
            "return": ret, "return_indirect": self._return_indirect(ret) if ret else False,
            "params": params, "variadic": variadic, "params_unknown": params_unknown,
            "this": info.get("this"), "vtable_index": vindex, "markers": mk_,
            "instances": instances, "redecls": redecls, "comment": self._comment(d),
        }

    def _fmarkers(self, d):
        ms = [m for m in getattr(d, "markers", []) if m.spec is None]
        lua = [{"text": m.value, "file": d.file, "line": m.line} for m in ms if m.kind == "lua"]
        return {
            "addr": next((m.value for m in ms if m.kind == "addr"), None),
            "base": next((m.value for m in ms if m.kind == "base"), None),
            "deleting": next((m.value for m in ms if m.kind == "deleting"), None),
            "thunks": sorted(m.value for m in ms if m.kind == "thunk"),
            "inline": any(m.kind == "inline" for m in ms),
            "pure": any(m.kind == "pure" for m in ms),
            "absent": any(m.kind == "absent" for m in ms),
            "fastcc": any(m.fastcc for m in ms if m.kind == "addr"),
            "lua": lua,
        }

    def _decl_text(self, d):
        cache = self.__dict__.setdefault("_lines", {})
        lines = cache.get(d.file)
        if lines is None:
            lines = next((t.split("\n") for rel, _p, _k, _s, t in self.files if rel == d.file), [])
            cache[d.file] = lines
        a = max(1, d.line)
        b = max(a, d.end_line or a)
        return "\n".join(lines[a - 1:b])

    # -- globals, enums, typedefs ----------------------------------------------------------------
    def _global_out(self, d):
        n = d.node
        mangled = n.get("mangledName")
        t = size = None
        if self.tg is not None:
            g = self._decl_probe_global(d, "global", "g")
            ref = self._probe_di_type(g)
            pn = self.tg.node(ref)
            if pn is not None and pn.kind == "DIDerivedType" and pn.get("tag") == "DW_TAG_pointer_type":
                t = self.tg.tid(pn.get("baseType"))
                size = self.tg.types.get(t, {}).get("size")
                cur = self.tg.types.get(t)
                while cur is not None and cur["kind"] in ("typedef", "cv"):
                    cur = self.tg.types.get(cur["of"])
                    if cur is not None and cur.get("size") is not None:
                        size = cur["size"]
                        break
        ms = getattr(d, "markers", [])
        instances = []
        if d.in_pattern:
            for spec, m in sorted(self._global_specs(d).items()):
                g = self._probe_global("global_instance", "u", f"{spec}::{d.name}")
                instances.append({"spec": spec, "addr": m.value,
                                  "mangled": llvm_ir.ptr_target(g) if g is not None else None})
        redecls = sorted(({"file": r.file, "line": r.line} for r in d.redecls), key=lambda x: (x["file"], x["line"]))
        return {
            "qualified": d.qualified, "mangled": mangled, "linkage": _linkage(mangled) if mangled else "external",
            "type": t, "size": size, "file": d.file, "line": d.line, "static_member": d.scope_kind == "class",
            "addr_marker": next((m.value for m in ms if m.kind == "addr" and m.spec is None), None),
            "inline_marker": any(m.kind == "inline" for m in ms), "instances": instances, "redecls": redecls,
            "comment": self._comment(d),
        }

    def _enum_out(self, d):
        size = None
        if self.tg is not None:
            n = self.tg.enums.get(d.qualified)
            if n is not None and n.get("size") is not None:
                size = n.get("size") // 8
        ms = getattr(d, "markers", [])
        return {"qualified": d.qualified, "file": d.file, "line": d.line, "size": size,
                "values": list(getattr(d, "values", [])),
                "markers": {"lua": [{"text": m.value, "file": d.file, "line": m.line} for m in ms if m.kind == "lua"]},
                "comment": self._comment(d)}

    def _typedef_out(self, d):
        t = None
        if self.tg is not None:
            n = self.tg.typedefs.get(d.qualified)
            if n is not None:
                t = self.tg.tid(n.get("baseType"))
        ms = getattr(d, "markers", [])
        return {"qualified": d.qualified, "type": t, "file": d.file, "line": d.line,
                "template": d.template == "alias" or bool(d.in_pattern),
                "markers": {"lua": [{"text": m.value, "file": d.file, "line": m.line} for m in ms if m.kind == "lua"]},
                "comment": self._comment(d)}

    # -- cpp defs --------------------------------------------------------------------------------
    def _cpp_defs(self, cpp_results, funcs_ast, decls):
        by_mangled = {}
        by_qual = defaultdict(list)
        for d in funcs_ast:
            m = d.node.get("mangledName")
            if m:
                by_mangled[m] = d
                if d.ast_kind in ("CXXConstructorDecl", "CXXDestructorDecl"):
                    for src, dst in (("C1", "C2"), ("D1", "D2"), ("D1", "D0")):
                        for c in vr.candidates(m, src, dst):
                            by_mangled.setdefault(c, d)
            else:
                by_qual[d.qualified].append(d)
        globals_by = {d.node.get("mangledName"): d for d in decls
                      if d.ent == "global" and d.primary is None and d.node.get("mangledName")}
        out, bodies = [], set()
        for rel in sorted(cpp_results):
            res = cpp_results[rel]
            scan = self.scans.get(rel)
            starts = defaultdict(list)
            for df in res["defs"]:
                starts[df["line"]].append(df)
            other_lines = set(res.get("others", ()))
            bound = defaultdict(list)
            if self.strict and scan is not None:
                for b in scan.blocks:
                    line = b.last_line + 1
                    if starts.get(line):
                        bound[line].extend(b.markers)
                    elif line in other_lines:
                        for m in b.markers:
                            self.diags.add("marker-misplaced", rel, m.line,
                                           f"'{at.describe(m)}' above a declaration that is not a definition")
                    else:
                        self.diags.add("marker-orphan", rel, b.first_line,
                                       f"no definition starts on line {line}, right below the block")
                for line, m in sorted(scan.trailing.items()):
                    self.diags.add("marker-misplaced", rel, line, "'///<' in a .cpp")
            for line in sorted(starts):
                for df in starts[line]:
                    target = None
                    if df["kind"] == "function":
                        if df["mangled"]:
                            target = by_mangled.get(df["mangled"])
                        else:
                            cands = by_qual.get(df["qualified"], [])
                            if len(cands) > 1:
                                cands = [c for c in cands if sum(1 for x in c.node.get("inner", ())
                                                                 if x.get("kind") == "ParmVarDecl") == df["params"]] or cands
                            target = cands[0] if cands else None
                        if target is None:
                            self.diags.add("cpp-undeclared", rel, line,
                                           f"body of {df['qualified']} has no declaration in the tree's headers")
                            continue
                        bodies.add(id(target))
                        qualified = target.qualified
                        fkey = target.node.get("mangledName") or f"{target.qualified}@{target.file}:{target.line}"
                    else:
                        g = globals_by.get(df["mangled"])
                        if g is None:
                            continue
                        qualified, fkey = g.qualified, None
                    ms = bound.get(line, [])
                    if self.strict:
                        ms = self.attacher.definition(rel, line, ms, qualified, df["mangled"] is None)
                    plain = [m for m in ms if m.spec is None]
                    insts = defaultdict(list)
                    for m in ms:
                        if m.spec is not None:
                            insts[m.spec].append(m)
                    out.append({
                        "file": rel, "line": line, "qualified": qualified, "mangled": df["mangled"],
                        "function": fkey,
                        "markers": {"addr": next((m.value for m in plain if m.kind == "addr"), None),
                                    "base": next((m.value for m in plain if m.kind == "base"), None),
                                    "inline": any(m.kind == "inline" for m in plain),
                                    "fastcc": any(m.fastcc for m in plain if m.kind == "addr")},
                        "instances": [{"spec": self.canon_spec(self._spec_owner(target), s) if target is not None else s,
                                       "addr": next((m.value for m in v if m.kind == "addr"), None),
                                       "base": next((m.value for m in v if m.kind == "base"), None),
                                       "fastcc": any(m.fastcc for m in v if m.kind == "addr")}
                                      for s, v in sorted(insts.items())],
                    })
        return out, bodies


def _unify_structor(ln, known=None):
    """DI names structors C4/D4; map to the C1/D1 spelling (the one in `known` when given)."""
    if ln is None:
        return None
    cands = vr.candidates(ln, "C4", "C1") + vr.candidates(ln, "D4", "D1")
    if known is not None:
        for c in cands:
            if c in known:
                return c
        return ln
    return cands[-1] if cands else ln


def _linkage(mangled):
    if mangled.startswith("_ZL") or "_GLOBAL__N_" in mangled or _nested_internal(mangled):
        return "internal"
    return "external"


def _skip_nested_part(s, i):
    """Index after the template argument, literal or name that starts at s[i] inside a nested name, or -1."""
    n = len(s)
    depth = 0
    while i < n:
        c = s[i]
        if c.isdigit():
            j = i
            while j < n and s[j].isdigit():
                j += 1
            i = j + int(s[i:j])
            continue
        if c == "S":
            if i + 1 < n and s[i + 1] in "tabsiod":
                i += 2
            else:
                j = s.find("_", i)
                if j < 0:
                    return -1
                i = j + 1
            continue
        if c == "T":
            j = s.find("_", i)
            if j < 0:
                return -1
            i = j + 1
            continue
        if c == "L":
            # literal `L<type><value>E`, or `L_Z<encoding>E`
            if i + 1 < n and s[i + 1] == "_":
                depth += 1
                i += 3
                continue
            j = i + 1
            if j < n and s[j].isdigit():
                k = j
                while k < n and s[k].isdigit():
                    k += 1
                j = k + int(s[j:k])
            else:
                j += 1
            if j < n and s[j] == "n":
                j += 1
            while j < n and s[j].isdigit():
                j += 1
            if j >= n or s[j] != "E":
                return -1
            i = j + 1
            continue
        if c in "INXJF":
            depth += 1
        elif c == "E":
            depth -= 1
            if depth == 0:
                return i + 1
        i += 1
    return -1


def _nested_internal(mangled):
    """True when a nested name `_ZN…` has an `L<len><name>` component of its own (not inside template args)."""
    if not mangled.startswith("_ZN"):
        return False
    s, i, n = mangled, 3, len(mangled)
    while i < n and s[i] in "rVK":
        i += 1
    while i < n and s[i] != "E":
        c = s[i]
        if c == "L" and i + 1 < n and s[i + 1].isdigit():
            return True
        if c.isdigit():
            j = i
            while j < n and s[j].isdigit():
                j += 1
            i = j + int(s[i:j])
        elif c == "I":
            i = _skip_nested_part(s, i)
            if i < 0:
                return False
        elif c == "S" and i + 1 < n and s[i + 1] in "tabsiod":
            i += 2
        elif c in "ST":
            j = s.find("_", i)
            if j < 0:
                return False
            i = j + 1
        else:
            i += 1
    return False


def _first_error(err, line):
    for raw in err.splitlines():
        if f"probes.cpp:{line}:" in raw and "error" in raw:
            return raw.split("error:", 1)[-1].strip()
    for raw in err.splitlines():
        if "error:" in raw:
            return raw.split("error:", 1)[-1].strip()
    return "error"


def _unlink(p):
    try:
        os.remove(p)
    except OSError:
        pass


# ------------------------------------------------------------------------------------------------
def stats(db):
    """Entities and markers by kind; placeholders per tools/civ5re/placeholders.py."""
    tree_recs = [r for r in db["records"] if r["origin"] == "tree"]
    ents = {"files": len(db["files"]), "records": len(tree_recs), "functions": len(db["functions"]),
            "fields": sum(len(r["fields"]) for r in tree_recs), "globals": len(db["globals"]),
            "enums": len(db["enums"]), "typedefs": len(db["typedefs"]), "cpp_defs": len(db["cpp_defs"]),
            "types": len(db["types"]), "third_party_records": len(db["records"]) - len(tree_recs)}
    marks = defaultdict(int)
    for f in db["files"]:
        if f["origin"]:
            marks["origin"] += 1
    for r in tree_recs:
        if r["size_marker"] is not None and r["template"] is None or r["size_marker"] is not None and r["template"] and r["template"].get("args") is None:
            marks["size"] += 1
        if r["vtable_marker"] is not None and (r["template"] is None or r["template"].get("args") is None):
            marks["vtable"] += 1
        for im in r["instance_markers"]:
            marks[im["kind"]] += 1
        marks["lua"] += len(r["markers"]["lua"])
        if r["template"] is None or r["template"].get("args") is None:
            marks["offset"] += sum(1 for f in r["fields"] if f["offset_marker"] is not None)
    for fn in db["functions"]:
        m = fn["markers"]
        for k in ("addr", "base", "deleting"):
            if m[k] is not None:
                marks[k] += 1
        marks["thunk"] += len(m["thunks"])
        for k in ("inline", "pure", "absent", "fastcc"):
            if m[k]:
                marks[k] += 1
        marks["lua"] += len(m["lua"])
        for i in fn["instances"]:
            for k in ("addr", "base", "deleting"):
                if i[k] is not None:
                    marks[k] += 1
            marks["thunk"] += len(i["thunks"])
    for g in db["globals"]:
        if g["addr_marker"]:
            marks["addr"] += 1
        marks["addr"] += sum(1 for i in g["instances"] if i["addr"])
        if g["inline_marker"]:
            marks["inline"] += 1
    for e in db["enums"] + db["typedefs"]:
        marks["lua"] += len(e["markers"]["lua"])
    holders = {k: 0 for k in ph.kinds()}
    for k in ph.MARKER_PLACEHOLDERS:
        holders[k] = 0

    def count(name, allowed):
        if not name:
            return
        mm = ph.classify(name)
        if mm is not None and mm.kind in allowed:
            holders[mm.kind] += 1

    for r in tree_recs:
        count(r["name"].split("::")[-1], ("class",))
        if r["template"] is None or r["template"].get("args") is None:
            for f in r["fields"]:
                count(f["name"], ("field", "unk"))
        if r["size_marker"] == "?":
            holders["size ?"] += 1
        if r["vtable_marker"] == "?":
            holders["vtable ?"] += 1
        for im in r["instance_markers"]:
            if im["value"] == "?":
                holders[f"{im['kind']} ?"] += 1
    lua_q = 0
    for fn in db["functions"]:
        count(fn["name"], ("fun", "vf"))
        for p in fn["params"]:
            count(p["name"], ("param",))
        if fn["return"] == "UNK_RET":
            holders["unk_ret"] += 1
        if fn["params_unknown"]:
            holders["unk_params"] += 1
        lua_q += sum(1 for x in fn["markers"]["lua"] if x["text"].strip() == "?")
    for g in db["globals"]:
        count(g["qualified"].split("::")[-1], ("global",))
        t = db["types"].get(g["type"]) if g["type"] else None
        while t is not None and t["kind"] in ("array", "cv"):
            t = db["types"].get(t["of"])
        if t is not None and t["kind"] == "unknown" and t["name"] == "UNK_DATA":
            holders["unk_data"] += 1
    for coll in (tree_recs, db["enums"], db["typedefs"]):
        for e in coll:
            lua_q += sum(1 for x in e["markers"]["lua"] if x["text"].strip() == "?")
    holders["lua ?"] = lua_q
    diag = defaultdict(int)
    for d in db["diagnostics"]:
        diag[d["code"]] += 1
    return {"entities": ents, "markers": dict(sorted(marks.items())), "placeholders": holders,
            "placeholders_total": sum(holders.values()), "diagnostics": dict(sorted(diag.items()))}


def build_db(tree_root, build_dir, markers="strict", third_party=None, *, force=False, keep=False):
    """Tree → `<build_dir>/typedb.json` (R3). Raises compile.EnvError on tool/environment failure."""
    # Parallel workers share build/: one build at a time, the others wait and then hit the cache.
    with _BuildLock(Path(build_dir) / "typedb.lock"):
        return Build(tree_root, build_dir, markers, third_party, force, keep).run()


def _write_retry(path, text, tries=50):
    """Windows keeps a just-closed file busy for a moment (clang exiting, antivirus scan): retry."""
    import time
    for i in range(tries):
        try:
            Path(path).write_text(text, encoding="utf-8", newline="\n")
            return
        except PermissionError:
            if i == tries - 1:
                raise
            time.sleep(0.1)


class _BuildLock:
    """Exclusive cross-process lock on a file; waits up to `timeout` seconds."""

    def __init__(self, path, timeout=600.0):
        self.path, self.timeout, self.fh = Path(path), timeout, None

    def __enter__(self):
        import time
        self.path.parent.mkdir(parents=True, exist_ok=True)
        self.fh = open(self.path, "a+b")
        end = time.monotonic() + self.timeout
        while True:
            try:
                if os.name == "nt":
                    import msvcrt
                    self.fh.seek(0)
                    msvcrt.locking(self.fh.fileno(), msvcrt.LK_NBLCK, 1)
                else:
                    import fcntl
                    fcntl.flock(self.fh.fileno(), fcntl.LOCK_EX | fcntl.LOCK_NB)
                return self
            except OSError:
                if time.monotonic() > end:
                    self.fh.close()
                    raise cc.EnvError(f"typedb build lock {self.path} is held for over {self.timeout:.0f} s")
                time.sleep(0.2)

    def __exit__(self, *exc):
        try:
            if os.name == "nt":
                import msvcrt
                self.fh.seek(0)
                msvcrt.locking(self.fh.fileno(), msvcrt.LK_UNLCK, 1)
        finally:
            self.fh.close()
