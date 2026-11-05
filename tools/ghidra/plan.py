"""typedb + binfacts + ghidra-functions -> build/ghidra/plan.json.

This docstring is the authoritative contract of plan.json, state.json and apply-report.json
(SCRIPT_VERSION = 2). Java decides nothing: every name, type, convention and storage below is final.
JSON is UTF-8 with LF, addresses are strings "0x%08x". Everything under /civ5re is "ours".

FILES (all under <repo>/build/ghidra/)
  plan.json          Python -> Java
  state.json         Java -> Python (written by a real apply only, NEVER by --dry-run); read by verify.py
  apply-report.json  Java -> Python, REAL apply: written LAST (also on abort). Python deletes it before each run and
                     treats "no report" as failed. apply-report.txt = the same as human lines.
  apply-report.dry.json / apply-report.dry.txt   same content for --dry-run (separate names: rebuild runs a real
                     apply and a dry run as two -postScript in ONE headless call and reads both).

HEADER (plan.json)
  {"header": {schema:"civ5re.plan/1", script_version:2, typedb_sha256, binfacts_sha256, binary_sha256,
              functions_sha256 (null without ghidra-functions.txt), plan_sha256, stale_min:50, stale_pct:2},
   "types":[...], "scopes":[...], "functions":[...], "data":[...], "labels":[...], "bodies":[...], "conflicts":[...],
   "warnings":[...]}
  plan_sha256 = sha256 of json.dumps(body, sort_keys=True, separators=(",",":"), ensure_ascii=False).encode("utf-8"),
  body = the object without "header". Java compares typedb/binfacts sha256 with the files on disk and
  script_version with its own constant (stale -> abort "plan is stale: civ5re ghidra apply").
  "conflicts"/"warnings" are for the report only; Java ignores them.

TYPE REFERENCES (ref)
  {"b": name}         Ghidra built-in type: void bool char schar uchar short ushort int uint long ulong longlong
                      ulonglong float double longdouble wchar32 wchar16 undefined undefined1 undefined2 undefined4
                      undefined8 ("char" signed, "long" 4 bytes, "wchar32" = wchar_t, "undefined" = DataType.DEFAULT)
  {"p": ref}          pointer (4 bytes)
  {"a": ref, "n": N}  array, N >= 1
  {"t": id}           type from types[] by id = full Ghidra path, e.g. "/civ5re/FireWorks/cvUnitVisSystem"
  Function pointer = {"p": {"t": "<funcdef id>"}}; pointer to method = {"t": "/civ5re/_pmf"}.

types[]  (sorted by id; every {"t": id} is defined here; create all empty shells first, then fill)
  struct/union  {id, kind:"struct"|"union", size, desc:null|"size ?"|"incomplete",
                 fields:[{off, name, type:ref, comment:null|str, bit:null|{size, shift}}]}
      length = size, packing off, fields at exact offsets, sorted by off, no overlaps, gaps stay undefined.
      union: every off = 0. desc = structure description. incomplete = size 0, no fields.
      bit: off = offset of the storage unit (aligned to sizeof(type)), type = integer base type,
           shift = bit offset inside the unit (little endian), size = width.
  enum          {id, kind:"enum", size, values:[[name, value], ...]}   sorted by (value, name)
  typedef       {id, kind:"typedef", to:ref}
  funcdef       {id, kind:"funcdef", cc:"__cdecl", ret:ref, params:[{name, type:ref}], varargs}
                (return always locked; params explicit; no automatic this)

scopes[]  parents before children (createNameSpace / createClass, convertNamespaceToClass for demangler ones)
  {path:[str,...], kind:"class"|"namespace", type:id|null}

functions[]  sorted by addr; exactly one entry per address (other names at the address are labels[] "secondary")
  {addr, key, name, scope:[str], kind:"function"|"thunk",
   cc:"__thiscall"|"__cdecl"|"" ("" = leave the convention alone: partial free function),
   custom:bool,            CUSTOM_STORAGE, cc "__cdecl" is the base name
   this_type:null|ref,     type of the automatic this ({"p":{"t":class}}), __thiscall only
   auto_retptr:bool,       __cdecl with a record return: Ghidra adds __return_storage_ptr__ itself, not in params
   ret:ref,                formal return type; {"b":"undefined"} = unknown, not locked
   ret_storage:null|{reg:"EAX"|"EDX:EAX"|"ST0"}|{none:true},   custom only; null = unknown return, not locked
   params:null|[{name, type:ref, storage:null|{reg:"ECX"|"EDX"}|{stack:int}}],
        null = partial signature; else the full explicit list EXCLUDING automatic parameters (the automatic this of
        __thiscall is not in it; __cdecl methods and thunks list this first; custom lists everything incl.
        this and __return_storage_ptr__)
   varargs:bool,
   partial:bool,           params_unknown: apply name+scope+return(+__thiscall); keep Param ID parameters unlocked
   drop_this:bool}         partial method: Param ID's first parameter is this; remove it
  Name and signature source: IMPORTED.

data[]    {addr, name, scope:[str], type:ref, size}       label + createData (clear foreign data/strings in range)
labels[]  {addr, name, scope:[str], kind:"secondary"|"data"}   label only
bodies[]  {addr, size}   a function symbol whose Ghidra body (ghidra-functions.txt) ends before addr+size at a function
      start that no symbol names: Ghidra cut it at `call next; pop`. Java joins the pieces while the cut is exactly a
      call to the next instruction (deletes the bogus function, sets one body over the symbol, flow override
      BRANCH when the call still is one); anything else is skipped with a reason. Not owned, never stale.
conflicts[] {kind, key, addr:null|str, message}   found while planning; those entities are NOT in the plan
warnings[]  {kind, key, message}
      Template patterns (typedb functions[].template, records with template.args null, typedefs[].template) are
      never planned: their types depend on template parameters. Only instantiations are (functions via instances[],
      records by name). A null type anywhere else is a defect: "type-null" / "typedef-unresolved" /
      "instance-unresolved" warnings. Third-party records taken or returned by value are complete records in
      typedb; a record returned by value that is only declared (size null) gets an explicit return pointer.

STATE (state.json, real apply only, after read-back)
  {schema:"civ5re.state/1", script_version, plan_sha256, binary_sha256, time,
   types:[...]  EVERY type under /civ5re in the program; same entry shapes as plan types[] with actual values
                (struct fields = defined components only, desc = structure description or null)
   funcs:[{addr, exists, owned, name, scope:[str], scope_kinds:["class"|"namespace",..] (len = len(scope)),
           cc, custom, ret:ref, ret_storage, params:[{name,type,storage}] (EXCLUDING automatic parameters),
           auto_this:null|ref, auto_retptr, varargs, name_source, sig_source (IMPORTED|USER_DEFINED|ANALYSIS|DEFAULT)}]
        one per plan function address and per owned (civ5re map) function address
   data:[{addr, exists, owned, name, scope:[str], type:ref|null, size}]    plan data addrs + owned
   labels:[{addr, exists, owned, name, scope:[str]}]                       NON-primary symbols only (the symbol that
        names a function or data item is in funcs/data, not here): plan label addrs + owned labels,
   bodies:[{addr, size, end:null|str (first address after the function's entry range), status:"ok"|"skipped: <why>"}]
        one per plan bodies[] entry; ok = the body reaches addr+size}
  owned = address is in the civ5re StringPropertyMap; owned entries outside the plan are reported "extra".

APPLY REPORT
  {schema:"civ5re.apply-report/1", script_version, plan_sha256, dry_run, aborted:null|str,
   counters:{changes, conflicts, manual_overwrites, stale, stale_limit, types_created, types_changed, types_deleted,
             functions_created, functions_changed, data_changed, labels_changed, bodies_repaired,
             bodies_skipped},
   entries:[{kind:"change"|"conflict"|"manual"|"stale", entity:"type"|"function"|"data"|"label", id, message}]}
  changes = writes made (dry run: that would be made); a repeated apply gives 0.
  aborted != null: nothing was changed. Stale safeguard: stale (ours, not in plan) > max(stale_min, stale_pct% of
  ours) aborts unless --prune-all.

Java args: --repo=<repo> [--dry-run] [--prune-all]; headless also gets env CIV5RE_REPO.
Export: Civ5reExport gets --out=<file> --cache-key=<key12>; Civ5reOptions and Civ5reParamId get --profile=<path>.
"""

import hashlib
import json
import re
from collections import defaultdict, deque
from pathlib import Path

from civ5re import placeholders as ph

SCRIPT_VERSION = 2
SCHEMA = "civ5re.plan/1"
ROOT_CAT = "/civ5re"
STALE_MIN = 50
STALE_PCT = 2
MAX_NAME = 2000

BUILTIN = {
    "void": "void", "bool": "bool", "_Bool": "bool", "char": "char", "signed char": "schar",
    "unsigned char": "uchar", "short": "short", "short int": "short", "signed short": "short",
    "unsigned short": "ushort", "unsigned short int": "ushort", "int": "int", "signed int": "int",
    "unsigned int": "uint", "unsigned": "uint", "long": "long", "long int": "long",
    "unsigned long": "ulong", "unsigned long int": "ulong", "long long": "longlong",
    "long long int": "longlong", "unsigned long long": "ulonglong",
    "unsigned long long int": "ulonglong", "float": "float", "double": "double",
    "long double": "longdouble", "wchar_t": "wchar32", "char32_t": "wchar32", "char16_t": "wchar16",
}
BUILTIN_VOCAB = frozenset(BUILTIN.values()) | {
    "undefined", "undefined1", "undefined2", "undefined4", "undefined8"}
REG_INT = frozenset({"int", "uint", "long", "ulong", "wchar32"})
CCS = ("__thiscall", "__cdecl", "")
HEX8 = re.compile(r"0x[0-9a-f]{8}")
PMF = ROOT_CAT + "/_pmf"


class PlanError(Exception):
    pass


def stale_threshold(ours):
    return max(STALE_MIN, ours * STALE_PCT // 100)


def hex8(addr):
    return "0x%08x" % addr


def _sha1(s):
    return hashlib.sha1(s.encode("utf-8")).hexdigest()


def norm_name(s):
    s = re.sub(r",\s+", ",", s)
    while True:
        t = re.sub(r">\s+>", ">>", s)
        if t == s:
            break
        s = t
    s = re.sub(r"\s*([*&])\s*", r"\1", s)
    s = re.sub(r"\s+", "_", s.strip())
    if len(s) > MAX_NAME:
        s = s[:MAX_NAME - 9] + "_" + _sha1(s)[:8]
    return s


def norm_type_name(s):
    return norm_name(s).replace("/", "_")


def split_qualified(s):
    if not s:
        return []
    if isinstance(s, (list, tuple)):
        return list(s)
    parts, depth, cur, i = [], 0, [], 0
    while i < len(s):
        c = s[i]
        if c in "<(":
            depth += 1
        elif c in ">)" and depth > 0:
            depth -= 1
        if depth == 0 and s.startswith("::", i):
            parts.append("".join(cur))
            cur = []
            i += 2
            continue
        cur.append(c)
        i += 1
    parts.append("".join(cur))
    return [p for p in parts if p]


def type_path(qualified):
    return ROOT_CAT + "".join("/" + norm_type_name(p) for p in split_qualified(qualified))


def short_name(qualified):
    parts = split_qualified(qualified)
    return parts[-1] if parts else qualified


def sha256_bytes(data):
    return hashlib.sha256(data).hexdigest()


def sha256_file(path):
    return sha256_bytes(Path(path).read_bytes())


def body_sha(body):
    raw = json.dumps(body, sort_keys=True, separators=(",", ":"), ensure_ascii=False)
    return sha256_bytes(raw.encode("utf-8"))


def read_functions(path):
    meta, funcs = {}, {}
    for n, line in enumerate(Path(path).read_text(encoding="utf-8").splitlines(), 1):
        if n == 1:
            if line.strip() != "# civ5re ghidra-functions 2":
                raise PlanError(f"{Path(path).name}: unsupported header {line!r}")
            continue
        if line.startswith("#"):
            for tok in line[1:].split():
                if "=" in tok:
                    k, v = tok.split("=", 1)
                    meta[k] = v
            continue
        if not line.strip():
            continue
        cols = line.split("\t")
        if len(cols) != 4:
            raise PlanError(f"{Path(path).name}:{n}: expected 4 columns")
        funcs[int(cols[0], 16)] = (int(cols[1], 16), cols[2], cols[3])
    return meta, funcs


class _Ent:
    __slots__ = ("addr", "key", "name", "scope", "kind", "mangled", "variant", "sig", "scope_kind")

    def __init__(self, **kw):
        for k in self.__slots__:
            setattr(self, k, kw.get(k))


class Builder:
    def __init__(self, typedb, binfacts, functions=None, hashes=None):
        self.tdb = typedb
        self.nodes = typedb.get("types", {})
        self.records = {r["name"]: r for r in typedb.get("records", [])}
        self.enums = {e["qualified"]: e for e in typedb.get("enums", [])}
        self.fns = {f["key"]: f for f in typedb.get("functions", [])}
        self.hashes = hashes or {}
        self.functions = functions
        self.out = {}
        self.claims = {}
        self.queue = deque()
        self.fd_paths = {}
        self.conflicts = []
        self.warnings = []
        self.data = []
        self.labels = []
        self.ents = defaultdict(list)
        self.scope_kinds = {}
        self._empty = {}
        self._index_binfacts(binfacts)

    def warn(self, kind, key, message):
        self.warnings.append({"kind": kind, "key": key, "message": message})

    def conflict(self, kind, key, addr, message):
        self.conflicts.append({"kind": kind, "key": key, "addr": addr, "message": message})

    def _index_binfacts(self, bf):
        self.sym_func = defaultdict(list)
        self.sym_obj = defaultdict(list)
        for s in bf.get("symbols", []):
            a = int(s["addr"], 16)
            (self.sym_func if s.get("type") == "func" else self.sym_obj)[a].append(s)
        self.eh = {int(f["addr"], 16) for f in bf.get("functions_eh", [])}
        self.bf_vt = {v["symbol"]: v for v in bf.get("vtables", []) if v.get("symbol")}
        self.vtable_targets = {int(sl["target"], 16) for v in bf.get("vtables", []) for g in v.get("groups", [])
                               for sl in g.get("slots", []) if sl.get("target")}

    def under(self, tid):
        while True:
            node = self.nodes.get(tid)
            if node is None:
                return tid, None
            if node["kind"] == "cv":
                tid = node["of"]
            elif node["kind"] == "typedef":
                tid = node["of"]
            else:
                return tid, node

    def rec_of(self, tid):
        _, node = self.under(tid)
        if node and node["kind"] == "record":
            return self.records.get(node.get("key") or node.get("name"))
        return None

    def sizeof(self, tid):
        node = self.nodes.get(tid)
        if node is None:
            return 4
        k = node["kind"]
        if k in ("pointer", "ref"):
            return 4
        if k in ("cv", "typedef"):
            return self.sizeof(node["of"])
        if node.get("size") is not None:
            return node["size"]
        if k == "array":
            return (node.get("count") or 0) * self.sizeof(node["of"])
        if k == "record":
            rec = self.records.get(node.get("key") or node.get("name"))
            return (rec or {}).get("size") or 0
        return 4

    def _claim(self, path, key):
        c = self.claims.get(path)
        if c is None:
            self.claims[path] = key
            return path, True
        if c == key:
            return path, False
        alt = path + "_" + _sha1(repr(key))[:6]
        self.warn("type-path-collision", repr(key), f"{path} is taken, using {alt}")
        if self.claims.get(alt) == key:
            return alt, False
        self.claims[alt] = key
        return alt, True

    @staticmethod
    def is_pattern(rec):
        """A class template pattern (also a nested one): types depend on template parameters, never emitted."""
        t = rec.get("template") if rec else None
        return bool(t) and t.get("args") is None

    def need_record(self, name):
        path, fresh = self._claim(type_path(name), ("record", name))
        if fresh:
            self.queue.append(("record", name, path))
        return path

    def need_enum(self, key):
        path, fresh = self._claim(type_path(key), ("enum", key))
        if fresh:
            e = self.enums.get(key)
            if e is None:
                self.warn("enum-missing", key, "enum is not in typedb.enums")
                values, size = [], 4
            else:
                values = sorted(([v["name"], v["value"]] for v in e.get("values", [])), key=lambda v: (v[1], v[0]))
                size = e.get("size") or 4
            self.out[path] = {"id": path, "kind": "enum", "size": size, "values": values}
        return path

    def pmf(self):
        if PMF not in self.out:
            self.claims[PMF] = ("pmf",)
            self.out[PMF] = {"id": PMF, "kind": "struct", "size": 8, "desc": None, "fields": [
                {"off": 0, "name": "ptr", "type": {"p": {"b": "void"}}, "comment": None, "bit": None},
                {"off": 4, "name": "adj", "type": {"b": "int"}, "comment": None, "bit": None}]}
        return {"t": PMF}

    def funcdef(self, tid, node):
        path = self.fd_paths.get(tid)
        if path:
            return path
        base = ROOT_CAT + "/_fn/"
        path = base + norm_type_name(tid)
        if len(path) > 400:
            path = base + norm_type_name(tid)[:300] + "_" + _sha1(tid)[:8]
        path, _ = self._claim(path, ("fn", tid))
        self.fd_paths[tid] = path
        entry = {"id": path, "kind": "funcdef", "cc": "__cdecl", "ret": None, "params": [], "varargs": False}
        self.out[path] = entry
        entry["ret"] = self.ret_ref(node.get("ret", "void"))
        entry["params"] = [{"name": ph.param_name(i + 1), "type": self.param_ref(self._ptid(p))}
                           for i, p in enumerate(node.get("params", []))]
        entry["varargs"] = bool(node.get("variadic"))
        return path

    @staticmethod
    def _ptid(p):
        return p["type"] if isinstance(p, dict) else p

    def _builtin(self, node):
        name = BUILTIN.get(node["name"])
        if name:
            return {"b": name}
        size = node.get("size")
        if size in (1, 2, 4, 8):
            return {"b": "undefined%d" % size}
        return {"a": {"b": "undefined1"}, "n": max(1, size or 1)}

    def ref(self, tid, in_ptr=False):
        if tid == "UNK_RET":
            return {"b": "undefined"}
        if tid is None:
            self.warn("type-null", "<null>", "a type that must be known is null in typedb")
            return {"b": "undefined4"}
        node = self.nodes.get(tid)
        if node is None:
            self.warn("type-missing", tid, "type id is not in typedb.types")
            return {"b": "undefined4"}
        k = node["kind"]
        if k == "builtin":
            return self._builtin(node)
        if k == "unknown":
            return {"b": "undefined" if node.get("name") == "UNK_RET" else "undefined1"}
        if k in ("pointer", "ref"):
            return {"p": self.ref(node["to"], True)}
        if k == "cv":
            return self.ref(node["of"], in_ptr)
        if k == "array":
            n = node.get("count")
            if n is None and in_ptr:
                return self.ref(node["of"], True)
            return {"a": self.ref(node["of"]), "n": n or 0}
        if k == "function":
            return {"t": self.funcdef(tid, node)}
        if k == "member_pointer":
            _, tnode = self.under(node["to"])
            return self.pmf() if tnode and tnode["kind"] == "function" else {"b": "int"}
        if k == "record":
            return {"t": self.need_record(node.get("key") or node.get("name"))}
        if k == "enum":
            return {"t": self.need_enum(node.get("key") or node.get("name"))}
        if k == "typedef":
            return self.typedef_ref(node)
        self.warn("type-kind", tid, f"unknown type kind {k}")
        return {"b": "undefined4"}

    def typedef_ref(self, node):
        path = type_path(node["name"])
        key = ("typedef", node["name"])
        if self.claims.get(path) == key:
            return {"t": path}
        to = self.ref(node["of"])
        if to == {"t": path}:
            return to
        if path in self.claims and self.claims[path] != key:
            self.warn("typedef-collision", node["name"], f"{path} is taken, typedef not emitted")
            return to
        self.claims[path] = key
        self.out[path] = {"id": path, "kind": "typedef", "to": to}
        return {"t": path}

    def param_ref(self, tid):
        _, node = self.under(tid)
        if node is not None:
            k = node["kind"]
            if k == "ref":
                return {"p": self.ref(node["to"], True)}
            if k == "array":
                return {"p": self.ref(node["of"])}
            if k == "record":
                rec = self.records.get(node.get("key") or node.get("name"))
                if rec is not None and rec.get("trivial_for_calls") is False:
                    return {"p": self.ref(tid)}
        return self.ref(tid)

    def ret_ref(self, tid):
        if tid in (None, "UNK_RET"):
            return {"b": "undefined"}
        _, node = self.under(tid)
        if node is not None and node["kind"] == "ref":
            return {"p": self.ref(node["to"], True)}
        return self.ref(tid)

    def is_unknown_ret(self, tid):
        if tid in (None, "UNK_RET"):
            return True
        _, node = self.under(tid)
        return bool(node and node["kind"] == "unknown" and node.get("name") == "UNK_RET")

    def is_record_value(self, tid):
        _, node = self.under(tid)
        return bool(node and node["kind"] == "record")

    def eff_marker(self, rec, kind):
        val = rec.get("size_marker" if kind == "size" else "vtable_marker")
        if val is not None:
            return val
        t = rec.get("template")
        if t:
            pat = self.records.get(t.get("pattern"))
            if pat is not None:
                for im in pat.get("instance_markers", []):
                    if im.get("kind") == kind and im.get("spec") == rec["name"]:
                        return im["value"]
                return pat.get("size_marker" if kind == "size" else "vtable_marker")
        return None

    def has_vtable(self, rec):
        return bool(rec and rec.get("vtable") and rec["vtable"].get("groups"))

    def is_empty(self, rec):
        if rec is None:
            return False
        name = rec["name"]
        if name not in self._empty:
            self._empty[name] = False
            self._empty[name] = (not rec.get("dynamic") and
                                 all(self._empty_field(f) for f in rec.get("fields", [])) and
                                 all(self.is_empty(self.records.get(b["name"])) for b in rec.get("bases", [])))
        return self._empty[name]

    def _empty_field(self, f):
        """A member of an empty class type: an empty-base-optimised wrapper (libc++ __compressed_pair_elem)."""
        if f.get("bit_size"):
            return False
        _, node = self.under(f["type"])
        if node is None or node["kind"] != "record":
            return False
        return self.is_empty(self.records.get(node.get("key") or node.get("name")))

    def is_opaque_value(self, tid):
        """A record returned by value that is emitted as an empty shell (declared only): Ghidra adds no hidden return pointer."""
        _, node = self.under(tid)
        if node is None or node["kind"] != "record":
            return False
        rec = self.records.get(node.get("key") or node.get("name"))
        return rec is None or not rec.get("size") or rec.get("complete") is False

    # ---- records -------------------------------------------------------------------------------------------

    def emit_record(self, name, path):
        rec = self.records.get(name)
        if rec is None or rec.get("size") is None or rec.get("complete") is False:
            self.out[path] = {"id": path, "kind": "struct", "size": 0, "desc": "incomplete", "fields": []}
            return
        desc = "size ?" if self.eff_marker(rec, "size") == "?" else None
        if rec.get("kind") == "union":
            fields = []
            for f in rec.get("fields", []):
                r = self.ref(f["type"])
                if r.get("n") == 0 and "a" in r:
                    continue
                fields.append({"off": 0, "name": f["name"] or ph.field_name(f.get("offset", 0)),
                               "type": r, "comment": None, "bit": None})
            self.out[path] = {"id": path, "kind": "union", "size": rec["size"], "desc": desc, "fields": fields}
        else:
            lay = _Layout()
            vptr = self.vptr_ref(rec, path)
            self._fill(rec, 0, lay, vptr, None)
            fields = sorted(lay.fields, key=lambda f: f["off"])
            self.out[path] = {"id": path, "kind": "struct", "size": rec["size"], "desc": desc, "fields": fields}
        if self.has_vtable(rec):
            self.emit_vtable(rec, path)

    def vptr_ref(self, rec, path):
        if self.has_vtable(rec):
            return {"p": {"t": path + "_vtbl"}}
        return {"p": {"b": "void"}}

    def _primary(self, rec):
        for b in sorted(rec.get("bases", []), key=lambda b: b.get("offset") or 0):
            brec = self.records.get(b["name"])
            if not b.get("virtual") and b.get("offset") == 0 and brec is not None and brec.get("dynamic"):
                return b, brec
        return None, None

    def _fill(self, rec, base, lay, vptr, comment):
        owner = short_name(rec["name"])
        if rec.get("dynamic"):
            pb, prec = self._primary(rec)
            if prec is not None:
                self._fill(prec, base, lay, vptr, short_name(prec["name"]))
            else:
                lay.add(base, 4, "_vptr", vptr, comment, None, owner, ("vptr",))
        own = [(f.get("offset") or 0) for f in rec.get("fields", [])]
        pb, _ = self._primary(rec)
        for b in sorted(rec.get("bases", []), key=lambda b: (b.get("offset") or 0, b["name"])):
            if b is pb:
                continue
            brec = self.records.get(b["name"])
            if brec is None or self.is_empty(brec):
                continue
            if b.get("offset") is None:
                self.warn("vbase-offset", rec["name"], f"base {b['name']} has no offset, skipped")
                continue
            off = b["offset"]
            end = off + (brec.get("size") or 0)
            bname = short_name(b["name"])
            if any(off <= o < end for o in own):
                self._fill(brec, base + off, lay, self.vptr_ref(brec, self.need_record(b["name"])), bname)
            else:
                bpath = self.need_record(b["name"])
                lay.add(base + off, brec.get("size") or 1, norm_type_name(bname), {"t": bpath}, None, None,
                        owner, ("base", base + off, b["name"]))
        for f in rec.get("fields", []):
            self._own_field(rec, f, base, lay, comment, owner)

    def _own_field(self, rec, f, base, lay, comment, owner):
        off = (f.get("offset") or 0)
        name = f.get("name") or ph.field_name(off)
        tid = f["type"]
        if f.get("bit_size"):
            u = max(1, self.sizeof(tid))
            bo = base * 8 + (f.get("bit_offset") or 0)
            unit = (bo // (8 * u)) * u
            shift = bo - unit * 8
            if shift + f["bit_size"] > 8 * u:
                self.warn("bitfield-straddle", f"{rec['name']}::{name}", "bit field crosses its storage unit, skipped")
                return
            lay.add(unit, u, name, self.ref(tid), comment, {"size": f["bit_size"], "shift": shift}, owner,
                    ("bits", unit))
            return
        r = self.ref(tid)
        if "a" in r and r.get("n") == 0:
            return
        size = f.get("size")
        if not size:
            size = self.sizeof(tid)
        if not lay.add(base + off, max(size, 1), name, r, comment, None, owner, None):
            self.warn("field-overlap", f"{rec['name']}::{name}", f"field at +0x{base + off:x} overlaps, skipped")

    # ---- vtables -------------------------------------------------------------------------------------------

    def emit_vtable(self, rec, path):
        cat, short = path.rsplit("/", 1)
        groups = rec["vtable"]["groups"]
        bf = None
        marker = self.eff_marker(rec, "vtable")
        if marker and marker != "?" and rec.get("origin") != "third_party":
            bf = self.bf_vt.get(rec.get("vtable_symbol"))
            if bf is None:
                self.conflict("vtable-not-found", rec["name"], marker,
                              f"{rec.get('vtable_symbol')} is not in binfacts.vtables")
            elif int(bf["addr"], 16) != int(marker, 16):
                self.conflict("vtable-address", rec["name"], marker,
                              f"vtable marker {marker} != binfacts {bf['addr']}")
                bf = None
            elif len(bf["groups"]) != len(groups):
                self.conflict("vtable-groups", rec["name"], marker,
                              f"{len(groups)} groups in typedb, {len(bf['groups'])} in binfacts")
                bf = None
        used = set()
        scope = split_qualified(rec["name"])
        for g, grp in enumerate(groups):
            sub = grp.get("subobject") or rec["name"]
            suffix = "" if g == 0 else "_" + norm_type_name(short_name(sub))
            if suffix in used:
                suffix += "_%d" % g
            used.add(suffix)
            gpath = f"{cat}/{short}_vtbl{suffix}"
            fcat = f"{cat}/{short}_vfn{suffix}"
            this_ref = {"p": {"t": self.need_record(sub)}}
            seen, fields = {}, []
            for i, slot in enumerate(grp.get("slots", [])):
                fn = self.fns.get(slot.get("function")) if slot.get("function") else None
                fname = self._slot_name(slot, fn, i, seen)
                fd = self._slot_funcdef(fcat, fname, fn, this_ref)
                fields.append({"off": 4 * i, "name": fname, "type": {"p": {"t": fd}}, "comment": None, "bit": None})
            self.claims[gpath] = ("vtbl", rec["name"], g)
            self.out[gpath] = {"id": gpath, "kind": "struct", "size": 4 * len(fields), "desc": None,
                               "fields": fields}
            if bf is not None:
                ap = bf["groups"][g].get("address_point")
                if ap:
                    self.data.append({"addr": ap, "name": f"{short}_vtbl{suffix}", "scope": scope,
                                      "type": {"t": gpath}, "size": 4 * len(fields)})

    def _slot_name(self, slot, fn, i, seen):
        kind = slot.get("kind")
        if kind == "D1" or (fn is not None and fn.get("kind") == "dtor" and kind != "D0"):
            name = "dtor"
        elif kind == "D0":
            name = "dtor_deleting"
        elif fn is not None:
            name = norm_type_name(fn["name"])
        else:
            name = ph.vf_name(i)
        n = seen.get(name, 0)
        seen[name] = n + 1
        return name if n == 0 else f"{name}_{i}"

    def _slot_funcdef(self, fcat, fname, fn, this_ref):
        path = f"{fcat}/{fname}"
        self.claims[path] = ("vfn", path)
        entry = {"id": path, "kind": "funcdef", "cc": "__cdecl", "ret": {"b": "undefined4"},
                 "params": [{"name": "this", "type": this_ref}], "varargs": False}
        self.out[path] = entry
        if fn is None or fn.get("params_unknown") or fn.get("template"):
            entry["varargs"] = True
            if fn is not None and not fn.get("template") and not self.is_unknown_ret(fn.get("return")):
                entry["ret"] = self.ret_ref(fn.get("return"))
            return path
        if not self.is_unknown_ret(fn.get("return")):
            entry["ret"] = self.ret_ref(fn.get("return"))
        for i, p in enumerate(fn.get("params", [])):
            entry["params"].append({"name": p.get("name") or ph.param_name(i + 1),
                                    "type": self.param_ref(p["type"])})
        entry["varargs"] = bool(fn.get("variadic"))
        return path

    # ---- functions -----------------------------------------------------------------------------------------

    def _base_dtor_alias(self, addr, mangled):
        """The address holds only destructors of other classes and none named `mangled`."""
        syms = self.sym_func.get(addr) or []
        return bool(syms) and mangled not in {s["mangled"] for s in syms} \
            and all(s.get("kind") == "dtor" for s in syms)

    def check_addr(self, addr, mangled, kind):
        syms = self.sym_func.get(addr)
        if syms:
            if kind == "thunk":
                return None if any(s.get("kind") == "thunk" for s in syms) else \
                    f"no thunk symbol at {hex8(addr)} ({', '.join(s['mangled'] for s in syms)})"
            if mangled is not None and mangled not in {s["mangled"] for s in syms}:
                return f"binary symbol at {hex8(addr)} is {syms[0]['mangled']}, typedb says {mangled}"
            return None
        if self.functions is None:
            return None
        if addr in self.functions or addr in self.eh:
            return None
        return f"{hex8(addr)} is neither an export nor in ghidra-functions.txt/functions_eh"

    def collect_functions(self):
        for key in sorted(self.fns):
            f = self.fns[key]
            m = f.get("markers") or {}
            if m.get("inline") or m.get("pure") or m.get("absent"):
                continue
            if not f.get("template"):  # a pattern has no address and no types of its own, only its instances
                self._function(f, None, m, f.get("return", "UNK_RET"), f.get("params", []), f.get("mangled"),
                               f.get("variants"))
            for inst in f.get("instances", []) or []:
                if inst.get("params") is None and not f.get("params_unknown"):
                    self.warn("instance-unresolved", f"{key}<{inst.get('spec')}>",
                              "typedb has no signature for this instance, skipped")
                    continue
                im = {"addr": inst.get("addr"), "base": inst.get("base"), "deleting": inst.get("deleting"),
                      "thunks": inst.get("thunks") or [], "fastcc": inst.get("fastcc")}
                self._function(f, inst["spec"], im, inst.get("return", f.get("return", "UNK_RET")),
                               inst.get("params", f.get("params", [])), inst.get("mangled"), inst.get("variants"))

    def _function(self, f, spec, m, ret, params, mangled, variants):
        skind = f.get("scope_kind")
        if spec is None:
            scope = split_qualified(f.get("scope"))
            name = f["name"]
        elif skind == "class":
            scope, name = split_qualified(spec), f["name"]
        else:
            parts = split_qualified(spec)
            scope, name = parts[:-1], parts[-1] if parts else f["name"]
        kind = f.get("kind", "function")
        if kind == "ctor":
            name = short_name("::".join(scope)) if scope else name
        elif kind == "dtor":
            name = "~" + (short_name("::".join(scope)) if scope else name.lstrip("~"))
        is_method = skind == "class" and not f.get("static")
        variants = variants or {}
        todo = []
        a = m.get("addr")
        if a:
            todo.append((a, mangled, "C1" if kind == "ctor" else "D1" if kind == "dtor" else None, False,
                         bool(m.get("fastcc"))))
        if m.get("base"):
            vk = "C2" if kind == "ctor" else "D2"
            todo.append((m["base"], variants.get(vk), vk, False, False))
        if m.get("deleting"):
            todo.append((m["deleting"], variants.get("D0"), "D0", False, False))
        for t in m.get("thunks") or []:
            taddr = t["addr"] if isinstance(t, dict) else t
            todo.append((taddr, None, "thunk", True, False))
        thunk_addrs = sorted(int(t["addr"] if isinstance(t, dict) else t, 16) for t in (m.get("thunks") or []))
        for addr_s, mg, variant, is_thunk, fastcc in todo:
            addr = int(addr_s, 16)
            ref = f["key"] if spec is None else f"{f['key']}<{spec}>"
            if kind == "dtor" and variant in ("D1", "D2") and self._base_dtor_alias(addr, mg):
                continue    # alias of a base destructor (checks A2): the function keeps the base's name
            why = self.check_addr(addr, mg, "thunk" if is_thunk else "function")
            if why:
                self.conflict("address", ref, addr_s, why)
                continue
            ename = name
            thunk_base = None
            if is_thunk:
                thunk_base = self._thunk_base(f, spec, scope, addr, thunk_addrs) or "::".join(scope)
                ename = "thunk_" + name
            sig = self._signature(f, ref, scope, is_method, ret, params, fastcc, thunk_base)
            self.ents[addr].append(_Ent(addr=addr, key=ref, name=ename, scope=scope,
                                        kind="thunk" if is_thunk else "function", mangled=mg,
                                        variant=variant, sig=sig, scope_kind=skind))

    def _thunk_base(self, f, spec, scope, addr, thunk_addrs):
        cls = spec if spec is not None else "::".join(scope)
        rec = self.records.get(cls)
        if rec is None or not rec.get("vtable"):
            return None
        keys = {f["key"]}
        cand = [gi for gi, grp in enumerate(rec["vtable"]["groups"])
                if any(s.get("thunk") and s.get("function") in keys for s in grp.get("slots", []))]
        bf = self.bf_vt.get(rec.get("vtable_symbol"))
        pick = None
        if bf is not None:
            for gi in cand:
                if gi < len(bf["groups"]) and any(
                        int(s["target"], 16) == addr for s in bf["groups"][gi].get("slots", [])):
                    pick = gi
                    break
        if pick is None and cand:
            idx = thunk_addrs.index(addr) if addr in thunk_addrs else 0
            pick = cand[min(idx, len(cand) - 1)]
        if pick is None:
            return None
        return rec["vtable"]["groups"][pick].get("subobject") or rec["name"]

    def _signature(self, f, ref, scope, is_method, ret, params, fastcc, thunk_base):
        cls = "::".join(scope)
        unknown_ret = self.is_unknown_ret(ret)
        # A template's return_indirect is computed on the unresolved `T`: the instance's own return decides too.
        indirect = (not unknown_ret) and (bool(f.get("return_indirect")) or self.is_record_value(ret))
        sig = {"cc": "__cdecl", "custom": False, "this_type": None, "auto_retptr": False,
               "ret": self.ret_ref(ret), "ret_storage": None, "params": None,
               "varargs": bool(f.get("variadic")), "partial": False, "drop_this": False}
        class_ref = {"p": {"t": self.need_record(cls)}} if is_method and cls else None
        if f.get("params_unknown"):
            if thunk_base is not None:
                this = {"name": "this", "type": {"p": {"t": self.need_record(thunk_base)}}, "storage": None}
                sig.update(params=[this], varargs=True)
                return sig
            sig.update(partial=True, varargs=False, cc="__thiscall" if is_method else "", drop_this=is_method,
                       this_type=class_ref)
            if fastcc:
                self.warn("fastcc-partial", ref, "fastcc ignored on a function with unknown parameters")
            return sig
        plist = [{"name": p.get("name") or ph.param_name(i + 1), "type": self.param_ref(p["type"]),
                  "storage": None} for i, p in enumerate(params)]
        if thunk_base is not None:
            this = {"name": "this", "type": {"p": {"t": self.need_record(thunk_base)}}, "storage": None}
            sig.update(params=[this] + plist, auto_retptr=indirect)
            return sig
        this = {"name": "this", "type": class_ref, "storage": None} if is_method else None
        explicit_retptr = indirect and not fastcc and self.is_opaque_value(ret)
        if fastcc or explicit_retptr:
            ps, tids = [], []
            if indirect:
                ps.append({"name": "__return_storage_ptr__", "type": {"p": self.ret_ref(ret)}, "storage": None})
                tids.append(None)
            if this:
                ps.append(this)
                tids.append(None)
            ps += plist
            tids += [p["type"] for p in params]
            self._assign_storage(ps, tids, ["ECX", "EDX"] if fastcc else [])
            sig.update(custom=True, params=ps, ret_storage=self._ret_storage(ret, indirect, unknown_ret))
            if indirect:
                sig["ret"] = {"p": self.ret_ref(ret)}
            return sig
        if is_method and not indirect:
            sig.update(cc="__thiscall", this_type=class_ref, params=plist)
        elif is_method:
            sig.update(params=[this] + plist, auto_retptr=True)
        else:
            sig.update(params=plist, auto_retptr=indirect)
        return sig

    def _reg_ok(self, tid):
        if tid is None:
            return True
        _, node = self.under(tid)
        if node is None:
            return False
        k = node["kind"]
        if k in ("pointer", "ref"):
            return True
        if k == "builtin":
            return BUILTIN.get(node["name"]) in REG_INT
        if k == "enum":
            return (node.get("size") or 4) == 4
        if k == "record":
            rec = self.records.get(node.get("key") or node.get("name"))
            return bool(rec is not None and rec.get("trivial_for_calls") is False)
        return False

    def _stack_size(self, tid):
        _, node = self.under(tid)
        if node is not None and node["kind"] == "record":
            rec = self.records.get(node.get("key") or node.get("name"))
            if rec is not None and rec.get("trivial_for_calls") is False:
                return 4
        return max(self.sizeof(tid), 1)

    def _assign_storage(self, ps, tids, regs):
        stack = 4
        for p, tid in zip(ps, tids):
            if regs and self._reg_ok(tid):
                p["storage"] = {"reg": regs.pop(0)}
            else:
                p["storage"] = {"stack": stack}
                stack += (self._stack_size(tid) + 3) // 4 * 4

    def _ret_storage(self, ret, indirect, unknown):
        if unknown:
            return None
        if indirect:
            return {"reg": "EAX"}
        _, node = self.under(ret)
        if node is not None and node["kind"] == "builtin":
            if node["name"] == "void":
                return {"none": True}
            if node["name"] in ("float", "double", "long double"):
                return {"reg": "ST0"}
        if self.sizeof(ret) == 8:
            return {"reg": "EDX:EAX"}
        return {"reg": "EAX"}

    # ---- globals -------------------------------------------------------------------------------------------

    def collect_globals(self):
        for g in sorted(self.tdb.get("globals", []), key=lambda g: g["qualified"]):
            if not g.get("addr_marker"):
                continue
            addr_s = g["addr_marker"]
            addr = int(addr_s, 16)
            syms = self.sym_obj.get(addr) or self.sym_func.get(addr)
            mg = g.get("mangled")
            if syms and mg and mg not in {s["mangled"] for s in syms}:
                self.conflict("address", g["qualified"], addr_s,
                              f"binary symbol at {addr_s} is {syms[0]['mangled']}, typedb says {mg}")
                continue
            parts = split_qualified(g["qualified"])
            scope, name = parts[:-1], parts[-1]
            if self._label_only(g["type"]):
                self.labels.append({"addr": addr_s, "name": name, "scope": scope, "kind": "data"})
            else:
                self.data.append({"addr": addr_s, "name": name, "scope": scope, "type": self.ref(g["type"]),
                                  "size": g.get("size") or self.sizeof(g["type"])})

    def _label_only(self, tid):
        while True:
            _, node = self.under(tid)
            if node is None:
                return False
            if node["kind"] == "array":
                tid = node["of"]
                continue
            return node["kind"] == "unknown"

    # ---- assembly ------------------------------------------------------------------------------------------

    def _scope_entries(self, entries):
        paths = {}
        for scope, kind in entries:
            for i in range(1, len(scope) + 1):
                paths.setdefault(tuple(scope[:i]), None)
            if scope and kind:
                paths[tuple(scope)] = kind
        out = []
        for p in sorted(paths, key=lambda p: (len(p), [norm_name(x) for x in p])):
            qname = "::".join(p)
            rec = self.records.get(qname)
            kind = "class" if rec is not None else paths[p] or "namespace"
            out.append({"path": [norm_name(x) for x in p], "kind": kind,
                        "type": self.need_record(qname) if rec is not None else None})
        return out

    def build(self):
        self.collect_functions()
        self.collect_globals()
        funcs = []
        scope_src = []
        for addr in sorted(self.ents):
            ents = sorted(self.ents[addr], key=lambda e: (ph.is_placeholder(e.name),
                                                         "::".join(e.scope + [e.name]), e.key, e.variant or ""))
            first = ents[0]
            funcs.append(self._func_entry(first))
            scope_src.append((first.scope, first.scope_kind))
            for e in ents[1:]:
                if (e.scope, e.name) == (first.scope, first.name):
                    continue
                self.labels.append({"addr": hex8(addr), "name": e.name, "scope": e.scope, "kind": "secondary"})
        for rec in self.tdb.get("records", []):
            if rec.get("origin") == "tree" and rec.get("size") is not None and not self.is_pattern(rec):
                self.need_record(rec["name"])
        for e in self.tdb.get("enums", []):
            if not e["file"].startswith("third_party/"):
                self.need_enum(e["qualified"])
        for t in self.tdb.get("typedefs", []):
            if t["file"].startswith("third_party/") or t.get("template"):
                continue
            if t["type"] is None:
                self.warn("typedef-unresolved", t["qualified"], "typedb has no type for this typedef, skipped")
                continue
            self.typedef_ref({"name": t["qualified"], "of": t["type"]})
        while True:
            while self.queue:
                kind, name, path = self.queue.popleft()
                self.emit_record(name, path)
            scopes = self._scope_entries(
                scope_src + [(d["scope"], None) for d in self.data] + [(lb["scope"], None) for lb in self.labels])
            if not self.queue:
                break
        for d in self.data:
            d["scope"] = [norm_name(x) for x in d["scope"]]
        for lb in self.labels:
            lb["scope"] = [norm_name(x) for x in lb["scope"]]
        data = self._unique(self.data, lambda d: d["addr"])
        labels = sorted(self.labels, key=lambda d: (d["addr"], d["scope"], d["name"], d["kind"]))
        return {
            "types": [self.out[k] for k in sorted(self.out)],
            "scopes": scopes,
            "functions": funcs,
            "data": data,
            "labels": labels,
            "bodies": self._bodies(),
            "conflicts": sorted(self.conflicts, key=lambda c: (c["kind"], c["key"], c["addr"] or "")),
            "warnings": sorted(self.warnings, key=lambda c: (c["kind"], c["key"], c["message"])),
        }

    def _bodies(self):
        """Symbols whose Ghidra function ends inside the symbol at a function start that nothing names."""
        if not self.functions:
            return []
        named = set(self.sym_func) | self.vtable_targets
        out = []
        for addr in sorted(self.sym_func):
            size = max((s.get("size") or 0 for s in self.sym_func[addr]), default=0)
            f = self.functions.get(addr)
            if not size or f is None:
                continue
            end = f[0]
            if addr < end < addr + size and end in self.functions and end not in named:
                out.append({"addr": hex8(addr), "size": size})
        return out

    def _unique(self, items, keyf):
        seen, out = {}, []
        for d in sorted(items, key=lambda d: (d["addr"], d["scope"], d["name"])):
            k = keyf(d)
            if k in seen:
                self.conflict("data-duplicate", d["name"], d["addr"], f"second data entry at {d['addr']}")
                continue
            seen[k] = d
            out.append(d)
        return out

    def _func_entry(self, e):
        ent = {"addr": hex8(e.addr), "key": e.key, "name": norm_name(e.name),
               "scope": [norm_name(x) for x in e.scope], "kind": e.kind}
        ent.update(e.sig)
        return ent


class _Layout:
    def __init__(self):
        self.fields = []
        self.used = []
        self.names = set()

    def add(self, off, size, name, typ, comment, bit, owner, tag):
        for s, e, t in self.used:
            if off < e and s < off + size and not (tag is not None and t == tag):
                return False
        self.used.append((off, off + size, tag))
        base = name
        if name in self.names:
            name = f"_vptr_{owner}" if name == "_vptr" else f"{owner}_{name}"
        n = 1
        while name in self.names:
            n += 1
            name = f"{base}_{off:x}" if n == 2 else f"{base}_{off:x}_{n}"
        self.names.add(name)
        self.fields.append({"off": off, "name": name, "type": typ, "comment": comment, "bit": bit})
        return True


def build_plan(typedb, binfacts, functions=None, hashes=None):
    if not typedb.get("ok", True):
        raise PlanError("typedb.json has errors (ok=false)")
    b = Builder(typedb, binfacts, functions, hashes)
    body = b.build()
    if functions is None:
        body["warnings"].append({"kind": "no-function-list", "key": "",
                                 "message": "ghidra-functions.txt missing: non-exported addresses unchecked"})
    hashes = hashes or {}
    header = {"schema": SCHEMA, "script_version": SCRIPT_VERSION,
              "typedb_sha256": hashes.get("typedb"), "binfacts_sha256": hashes.get("binfacts"),
              "binary_sha256": hashes.get("binary"), "functions_sha256": hashes.get("functions"),
              "plan_sha256": body_sha(body), "stale_min": STALE_MIN, "stale_pct": STALE_PCT}
    plan = {"header": header}
    plan.update(body)
    return plan


def build_from_files(typedb_path, binfacts_path, functions_path=None, binary_sha256=None):
    typedb = json.loads(Path(typedb_path).read_text(encoding="utf-8"))
    binfacts = json.loads(Path(binfacts_path).read_text(encoding="utf-8"))
    funcs, fsha = None, None
    if functions_path and Path(functions_path).is_file():
        meta, funcs = read_functions(functions_path)
        fsha = sha256_file(functions_path)
        have = meta.get("binary_sha256")
        want = binary_sha256 or binfacts.get("binary", {}).get("sha256")
        if have and want and have != want:
            raise PlanError("ghidra-functions.txt belongs to another binary "
                            f"({have[:12]} != {want[:12]}): run civ5re ghidra functions")
    funcs = None if funcs is None else {a: v for a, v in funcs.items()}
    hashes = {"typedb": sha256_file(typedb_path), "binfacts": sha256_file(binfacts_path),
              "binary": binary_sha256 or binfacts.get("binary", {}).get("sha256"), "functions": fsha}
    return build_plan(typedb, binfacts, funcs, hashes)


def write(plan, path):
    path = Path(path)
    path.parent.mkdir(parents=True, exist_ok=True)
    tmp = path.with_name(path.name + ".tmp")
    tmp.write_text(json.dumps(plan, sort_keys=True, indent=1, ensure_ascii=False) + "\n", encoding="utf-8",
                   newline="\n")
    tmp.replace(path)


def load(path):
    return json.loads(Path(path).read_text(encoding="utf-8"))


def body_of(plan):
    return {k: v for k, v in plan.items() if k != "header"}


def _walk_refs(r, out):
    if isinstance(r, dict):
        if "t" in r:
            out.append(r["t"])
        elif "p" in r:
            _walk_refs(r["p"], out)
        elif "a" in r:
            _walk_refs(r["a"], out)


def validate(plan):
    errs = []
    h = plan.get("header", {})
    if h.get("schema") != SCHEMA:
        errs.append(f"header.schema {h.get('schema')!r}")
    if h.get("script_version") != SCRIPT_VERSION:
        errs.append(f"header.script_version {h.get('script_version')!r}")
    if h.get("plan_sha256") != body_sha(body_of(plan)):
        errs.append("header.plan_sha256 does not match the body")
    for k in ("types", "scopes", "functions", "data", "labels", "bodies", "conflicts", "warnings"):
        if not isinstance(plan.get(k), list):
            errs.append(f"{k} is not a list")
    if errs:
        return errs
    ids = [t["id"] for t in plan["types"]]
    idset = set(ids)
    if ids != sorted(ids) or len(idset) != len(ids):
        errs.append("types are not sorted by unique id")
    scopes = set()
    for s in plan["scopes"]:
        p = tuple(s["path"])
        if len(p) > 1 and p[:-1] not in scopes:
            errs.append(f"scope {'::'.join(p)} precedes its parent")
        if s["kind"] not in ("class", "namespace"):
            errs.append(f"scope {'::'.join(p)}: kind {s['kind']!r}")
        if s.get("type") and s["type"] not in idset:
            errs.append(f"scope {'::'.join(p)}: type {s['type']} undefined")
        scopes.add(p)

    def check_ref(r, where):
        if not isinstance(r, dict) or len(set(r) - {"n"}) != 1:
            errs.append(f"{where}: malformed ref {r!r}")
            return
        if "b" in r:
            if r["b"] not in BUILTIN_VOCAB:
                errs.append(f"{where}: unknown builtin {r['b']!r}")
        elif "p" in r:
            check_ref(r["p"], where)
        elif "a" in r:
            if not isinstance(r.get("n"), int) or r["n"] < 1:
                errs.append(f"{where}: array without n >= 1")
            check_ref(r["a"], where)
        elif "t" in r:
            if r["t"] not in idset:
                errs.append(f"{where}: type {r['t']} is not defined")
        else:
            errs.append(f"{where}: malformed ref {r!r}")

    for t in plan["types"]:
        tid = t["id"]
        if not tid.startswith(ROOT_CAT + "/"):
            errs.append(f"{tid}: outside {ROOT_CAT}")
        k = t["kind"]
        if k in ("struct", "union"):
            last = -1
            names = set()
            for f in t["fields"]:
                check_ref(f["type"], f"{tid}.{f['name']}")
                if f["name"] in names:
                    errs.append(f"{tid}: duplicate field {f['name']}")
                names.add(f["name"])
                if k == "union":
                    if f["off"] != 0:
                        errs.append(f"{tid}.{f['name']}: union member off != 0")
                    continue
                if f["off"] < last:
                    errs.append(f"{tid}.{f['name']}: fields not sorted")
                last = f["off"]
                if f["off"] >= max(t["size"], 1):
                    errs.append(f"{tid}.{f['name']}: offset beyond size")
        elif k == "enum":
            pass
        elif k == "typedef":
            check_ref(t["to"], tid)
        elif k == "funcdef":
            check_ref(t["ret"], tid)
            for p in t["params"]:
                check_ref(p["type"], tid)
        else:
            errs.append(f"{tid}: kind {k!r}")
    seen = set()
    for f in plan["functions"]:
        w = f"function {f['addr']}"
        if not HEX8.fullmatch(f["addr"]):
            errs.append(f"{w}: bad address")
        if f["addr"] in seen:
            errs.append(f"{w}: duplicate address")
        seen.add(f["addr"])
        if f["cc"] not in CCS:
            errs.append(f"{w}: cc {f['cc']!r}")
        if tuple(f["scope"]) and any(tuple(f["scope"][:i]) not in scopes for i in range(1, len(f["scope"]) + 1)):
            errs.append(f"{w}: scope not in scopes[]")
        check_ref(f["ret"], w)
        if f["partial"] != (f["params"] is None):
            errs.append(f"{w}: partial/params mismatch")
        for p in f["params"] or []:
            check_ref(p["type"], w)
            if f["custom"] and not p.get("storage"):
                errs.append(f"{w}: custom storage without param storage")
            if not f["custom"] and p.get("storage"):
                errs.append(f"{w}: storage on a non-custom function")
        if f["custom"] and f["params"] is None:
            errs.append(f"{w}: custom without params/ret_storage")
        if f["cc"] == "__thiscall" and not f["partial"] and not f["this_type"]:
            errs.append(f"{w}: __thiscall without this_type")
    if [f["addr"] for f in plan["functions"]] != sorted(f["addr"] for f in plan["functions"]):
        errs.append("functions are not sorted by addr")
    for kind in ("data", "labels"):
        for d in plan[kind]:
            w = f"{kind} {d['addr']}"
            if not HEX8.fullmatch(d["addr"]):
                errs.append(f"{w}: bad address")
            if kind == "data":
                check_ref(d["type"], w)
            if d["scope"] and any(tuple(d["scope"][:i]) not in scopes for i in range(1, len(d["scope"]) + 1)):
                errs.append(f"{w}: scope not in scopes[]")
    return errs


def assert_valid(plan):
    errs = validate(plan)
    if errs:
        raise PlanError("invalid plan: " + "; ".join(errs[:5]) + (" ..." if len(errs) > 5 else ""))
