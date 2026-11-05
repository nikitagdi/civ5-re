"""Schema `civ5re.typedb/1` and a structural validator.

Top level: schema, ok, markers (strict|off), tool{typedb, clang, flags_sha},
inputs{tree_root, tree_sha, third_party_sha, prelude_sha}, files[], types{}, records[], functions[],
globals[], enums[], typedefs[], cpp_defs[], diagnostics[], stats{}.

Field lists below are the contract. Additions beyond the contract:
records[].accessed_from — {record, offset} for an unnamed anonymous member record (`union { … };`):
its fields[].offset count from its own start, while their `///<` count from `record` (add `offset`).
records[].template.partial_of — only on a class template partial specialization: the primary template's
name. Such a record is a pattern of its own: name = primary name + the arguments as written, normalized
(`FAutoVariable<std::vector<ElementType>,ClassContainer>`), template = {pattern: that name, args: null,
partial_of: primary}; its instantiations name it in template.pattern.
functions[].template — true for a function template and for a member of a class template pattern: its
parameter and return types depend on template parameters, so they are null; only instances[] carry types.
Any other null type in functions[] is a defect. records[] of a pattern (template.args null) have null field
types for the same reason, and typedefs[].template (alias template or inside a pattern) has a null type.
globals[].instances — [{spec, addr, mangled}] of a static data member of a class template, from the SPEC
address lines on the in-class declaration and on its out-of-class definitions; mangled comes from a probe.
globals[].redecls — [{file, line}] of the out-of-class definitions (seed puts the SPEC lines above them).
"""
from __future__ import annotations

SCHEMA = "civ5re.typedb/1"

TOP = ("schema", "ok", "markers", "tool", "inputs", "files", "types", "records", "functions", "globals",
       "enums", "typedefs", "cpp_defs", "diagnostics", "stats")
FIELDS = {
    "tool": ("typedb", "clang", "flags_sha"),
    "inputs": ("tree_root", "tree_sha", "third_party_sha", "prelude_sha"),
    "files": ("path", "kind", "origin", "sha256"),
    "records": ("name", "kind", "file", "line", "origin", "complete", "template", "size", "align", "dsize",
                "dynamic", "trivial_for_calls", "size_marker", "vtable_marker", "instance_markers",
                "vtable_symbol", "typeinfo_symbol", "bases", "vptrs", "fields", "vtable", "markers", "comment"),
    "record_fields": ("name", "type", "offset", "size", "bit_offset", "bit_size", "line", "offset_marker"),
    "functions": ("key", "qualified", "name", "scope", "scope_kind", "file", "line", "kind", "mangled",
                  "variants", "linkage", "static", "template", "virtual", "pure", "const", "has_body", "access", "return",
                  "return_indirect", "params", "variadic", "params_unknown", "this", "vtable_index", "markers",
                  "instances", "redecls", "comment"),
    "function_markers": ("addr", "base", "deleting", "thunks", "inline", "pure", "absent", "fastcc", "lua"),
    "instances": ("spec", "addr", "base", "deleting", "thunks", "fastcc", "mangled", "variants", "return",
                  "params"),
    "globals": ("qualified", "mangled", "linkage", "type", "size", "file", "line", "static_member",
                "addr_marker", "inline_marker", "instances", "redecls", "comment"),
    "enums": ("qualified", "file", "line", "size", "values", "markers", "comment"),
    "typedefs": ("qualified", "type", "template", "file", "line", "markers", "comment"),
    "cpp_defs": ("file", "line", "qualified", "mangled", "function", "markers", "instances"),
    "diagnostics": ("severity", "code", "file", "line", "message"),
    "slots": ("kind", "function", "symbol", "thunk"),
    "groups": ("subobject", "offset_to_top", "vbase_offsets", "slots"),
}
TYPE_KINDS = {
    "builtin": ("name", "size", "signed"),
    "pointer": ("to", "size"),
    "ref": ("to", "size", "rvalue"),
    "cv": ("of", "const", "volatile"),
    "array": ("of", "count", "size"),
    "function": ("ret", "params", "variadic"),
    "member_pointer": ("class", "to", "size"),
    "record": ("key", "size"),
    "enum": ("key", "size"),
    "typedef": ("name", "of"),
    "unknown": ("name", "size"),
}


class SchemaError(Exception):
    pass


def _need(obj, keys, where, errs):
    for k in keys:
        if k not in obj:
            errs.append(f"{where}: missing {k}")


def validate(db) -> list:
    """Return a list of contract violations (empty = valid)."""
    errs = []
    if db.get("schema") != SCHEMA:
        return [f"schema is {db.get('schema')!r}, expected {SCHEMA}"]
    _need(db, TOP, "top", errs)
    for k in ("tool", "inputs"):
        _need(db.get(k, {}), FIELDS[k], k, errs)
    for coll in ("files", "records", "functions", "globals", "enums", "typedefs", "cpp_defs", "diagnostics"):
        for i, x in enumerate(db.get(coll, ())):
            _need(x, FIELDS[coll], f"{coll}[{i}]", errs)
    types = db.get("types", {})
    for tid, t in types.items():
        kind = t.get("kind")
        if kind not in TYPE_KINDS:
            errs.append(f"types[{tid}]: bad kind {kind!r}")
            continue
        _need(t, TYPE_KINDS[kind], f"types[{tid}]", errs)
    for r in db.get("records", ()):
        for f in r.get("fields", ()):
            _need(f, FIELDS["record_fields"], f"records[{r.get('name')}].fields", errs)
            if f.get("type") is not None and f["type"] not in types:
                errs.append(f"records[{r.get('name')}].{f.get('name')}: unknown type {f['type']}")
        vt = r.get("vtable")
        if vt:
            for g in vt["groups"]:
                _need(g, FIELDS["groups"], f"records[{r.get('name')}].vtable", errs)
                for s in g["slots"]:
                    _need(s, FIELDS["slots"], f"records[{r.get('name')}].slots", errs)
    for fn in db.get("functions", ()):
        _need(fn.get("markers", {}), FIELDS["function_markers"], f"functions[{fn.get('key')}].markers", errs)
        for i in fn.get("instances", ()):
            _need(i, FIELDS["instances"], f"functions[{fn.get('key')}].instances", errs)
        for t in [fn.get("return"), fn.get("this")] + [p.get("type") for p in fn.get("params", ())]:
            if t is not None and t not in types:
                errs.append(f"functions[{fn.get('key')}]: unknown type {t}")
    for coll in ("files", "functions", "records"):
        keys = [x.get({"files": "path", "functions": "key", "records": "name"}[coll]) for x in db.get(coll, ())]
        if keys != sorted(keys):
            errs.append(f"{coll} not sorted")
        if len(set(keys)) != len(keys):
            errs.append(f"{coll}: duplicate keys")
    return errs
