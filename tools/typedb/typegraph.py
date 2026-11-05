"""DI metadata → `types`, record layouts, vtable groups."""
from __future__ import annotations

import functools
import re

from typedb import llvm_ir
from typedb.llvm_ir import Ref
from typedb.markers import normalize_spec

RECORD_TAGS = {"DW_TAG_structure_type": "struct", "DW_TAG_class_type": "class", "DW_TAG_union_type": "union"}
UNKNOWN = {"UNK_RET": None, "UNK_DATA": 1}


@functools.lru_cache(maxsize=None)
def norm_name(s: str) -> str:
    return normalize_spec(s)


class TypeGraph:
    def __init__(self, mod: llvm_ir.Module, path_map):
        self.mod = mod
        self.path_map = path_map
        self.types = {}
        self._tid = {}
        self._nodes = {}
        self._names = {}
        self._files = {}
        self.records = {}
        self.enums = {}
        self.typedefs = {}
        self._index()

    def node(self, ref):
        if ref is None:
            return None
        key = int(ref)
        if key in self._nodes:
            return self._nodes[key]
        n = self.mod.meta.get(key)
        # `__typedb_id<T>` (probes spell types through it) is transparent: it is not a type of the tree
        while n is not None and n.kind == "DIDerivedType" and n.get("tag") == "DW_TAG_typedef"                 and str(n.get("name") or "").startswith("__typedb_id<") and n.get("baseType") is not None:
            n = self.mod.meta.get(int(n.get("baseType")))
        self._nodes[key] = n
        return n

    def tuple(self, ref):
        if ref is None:
            return []
        return self.mod.tuples.get(int(ref), [])

    # -- names ------------------------------------------------------------------------------
    def file_of(self, n):
        fr = n.get("file")
        if fr is None:
            return None
        key = int(fr)
        if key in self._files:
            return self._files[key]
        f = self.node(fr)
        res = None
        if f is not None:
            name = f.get("filename") or ""
            d = f.get("directory") or ""
            import os
            path = name if os.path.isabs(name) or not d else os.path.join(d, name)
            res = self.path_map(path)
        self._files[key] = res
        return res

    def scope_name(self, ref):
        n = self.node(ref)
        if n is None:
            return ""
        if n.kind == "DINamespace":
            name = n.get("name") or "(anonymous namespace)"
            outer = self.scope_name(n.get("scope"))
            return f"{outer}::{name}" if outer else name
        if n.kind == "DICompositeType":
            return self.full_name(n)
        if n.kind == "DISubprogram":
            return "<local>"
        return ""

    def full_name(self, n):
        k = n.id
        if k in self._names:
            return self._names[k]
        name = n.get("name")
        if not name:
            tag = n.get("tag")
            word = {"DW_TAG_union_type": "union", "DW_TAG_class_type": "class",
                    "DW_TAG_enumeration_type": "enum"}.get(tag, "struct")
            name = f"(anon {word} L{n.get('line') or 0})"
        outer = self.scope_name(n.get("scope"))
        full = norm_name(f"{outer}::{name}" if outer else name)
        self._names[k] = full
        return full

    # -- index ------------------------------------------------------------------------------
    def _index(self):
        for n in self.mod.meta.select(lambda r: r[0] == "DICompositeType" or (
                r[0] == "DIDerivedType" and "DW_TAG_typedef" in r[2])):
            if n.kind == "DICompositeType":
                tag = n.get("tag")
                if tag in RECORD_TAGS and "DIFlagFwdDecl" not in llvm_ir.flags(n):
                    name = self.full_name(n)
                    if "<local>" in name:
                        continue
                    self.records.setdefault(name, n)
                elif tag == "DW_TAG_enumeration_type":
                    self.enums.setdefault(self.full_name(n), n)
            elif n.kind == "DIDerivedType" and n.get("tag") == "DW_TAG_typedef":
                outer = self.scope_name(n.get("scope"))
                if "<local>" in outer:
                    continue
                name = n.get("name") or ""
                self.typedefs.setdefault(norm_name(f"{outer}::{name}" if outer else name), n)

    # -- types ------------------------------------------------------------------------------
    def tid(self, ref):
        """Register the type `ref` (None = void) and return its id."""
        key = None if ref is None else int(ref)
        if key in self._tid:
            return self._tid[key]
        tid, node = self._build(ref)
        self._tid[key] = tid
        if tid not in self.types:
            self.types[tid] = node
        return tid

    def _build(self, ref):
        if ref is None:
            return "void", {"kind": "builtin", "name": "void", "size": None, "signed": None}
        n = self.node(ref)
        if n is None:
            return "?", {"kind": "unknown", "name": "?", "size": None}
        spelled = self.spell(ref)
        k = n.kind
        if k == "DIBasicType":
            enc = n.get("encoding")
            signed = {"DW_ATE_signed": True, "DW_ATE_signed_char": True, "DW_ATE_unsigned": False,
                      "DW_ATE_unsigned_char": False, "DW_ATE_boolean": False, "DW_ATE_UTF": False}.get(enc)
            return spelled, {"kind": "builtin", "name": n.get("name"), "size": (n.get("size") or 0) // 8,
                             "signed": signed}
        if k == "DISubroutineType":
            ret, params, variadic, _this = self.signature(ref)
            return spelled, {"kind": "function", "ret": ret, "params": params, "variadic": variadic}
        if k == "DICompositeType":
            tag = n.get("tag")
            if tag == "DW_TAG_array_type":
                counts = self._counts(n)
                elem = n.get("baseType")
                of = self.tid(elem) if len(counts) <= 1 else self._subarray(elem, counts[1:])
                size = (n.get("size") or 0) // 8 if n.get("size") is not None else None
                return spelled, {"kind": "array", "of": of, "count": counts[0] if counts else None, "size": size}
            fwd = "DIFlagFwdDecl" in llvm_ir.flags(n)
            name = self.full_name(n)
            size = None if fwd else (n.get("size") or 0) // 8
            if not fwd and name in self.records and self.records[name] is not n:
                size = (self.records[name].get("size") or 0) // 8
            elif fwd and name in self.records:
                size = (self.records[name].get("size") or 0) // 8
            kind = "enum" if tag == "DW_TAG_enumeration_type" else "record"
            return spelled, {"kind": kind, "key": name, "size": size}
        if k == "DIDerivedType":
            tag = n.get("tag")
            base = n.get("baseType")
            if tag == "DW_TAG_typedef":
                name = spelled
                if name in UNKNOWN:
                    return name, {"kind": "unknown", "name": name, "size": UNKNOWN[name]}
                return spelled, {"kind": "typedef", "name": name, "of": self.tid(base)}
            if tag in ("DW_TAG_pointer_type",):
                return spelled, {"kind": "pointer", "to": self.tid(base), "size": (n.get("size") or 32) // 8}
            if tag in ("DW_TAG_reference_type", "DW_TAG_rvalue_reference_type"):
                return spelled, {"kind": "ref", "to": self.tid(base), "size": 4,
                                 "rvalue": tag == "DW_TAG_rvalue_reference_type"}
            if tag in ("DW_TAG_const_type", "DW_TAG_volatile_type", "DW_TAG_restrict_type"):
                c, v, inner = self._cv(ref)
                return spelled, {"kind": "cv", "of": self.tid(inner), "const": c, "volatile": v}
            if tag == "DW_TAG_ptr_to_member_type":
                bn = self.node(base)
                is_fn = bn is not None and bn.kind == "DISubroutineType"
                cls = self.node(n.get("extraData"))
                return spelled, {"kind": "member_pointer", "class": self.full_name(cls) if cls else None,
                                 "to": self.tid(base), "size": 8 if is_fn else 4}
            if tag in ("DW_TAG_atomic_type",):
                return self._build(base)
        return spelled, {"kind": "unknown", "name": spelled, "size": None}

    def _subarray(self, elem, counts):
        key = ("arr", int(elem) if elem is not None else None, tuple(counts))
        if key in self._tid:
            return self._tid[key]
        inner = self.tid(elem) if len(counts) == 1 else self._subarray(elem, counts[1:])
        es = self.types[inner].get("size") if inner in self.types else None
        spelled = self._spell_array(elem, counts, "")
        size = None
        if es is not None and all(c is not None for c in counts):
            size = es
            for c in counts[:1]:
                size *= c
        self.types.setdefault(spelled, {"kind": "array", "of": inner, "count": counts[0], "size": size})
        self._tid[key] = spelled
        return spelled

    def _counts(self, n):
        out = []
        for r in self.tuple(n.get("elements")):
            s = self.node(r)
            if s is None:
                continue
            c = s.get("count")
            out.append(c if isinstance(c, int) and not isinstance(c, Ref) and c >= 0 else None)
        return out

    def _cv(self, ref):
        c = v = False
        n = self.node(ref)
        while n is not None and n.kind == "DIDerivedType" and n.get("tag") in (
                "DW_TAG_const_type", "DW_TAG_volatile_type", "DW_TAG_restrict_type"):
            if n.get("tag") == "DW_TAG_const_type":
                c = True
            elif n.get("tag") == "DW_TAG_volatile_type":
                v = True
            ref = n.get("baseType")
            n = self.node(ref)
        return c, v, ref

    def signature(self, ref, skip_this=True):
        """DISubroutineType → (ret id, [param ids], variadic, this id|None)."""
        n = self.node(ref)
        types = self.tuple(n.get("types")) if n is not None else []
        if not types:
            return "void", [], False, None
        ret = self.tid(types[0])
        params, variadic, this = [], False, None
        rest = types[1:]
        for i, t in enumerate(rest):
            if t is None:
                if i == len(rest) - 1:
                    variadic = True
                continue
            tn = self.node(t)
            if skip_this and tn is not None and "DIFlagObjectPointer" in llvm_ir.flags(tn) and this is None and i == 0:
                this = self.tid(t)
                continue
            params.append(self.tid(t))
        return ret, params, variadic, this

    # -- spelling (clang style) -------------------------------------------------------------
    def spell(self, ref, inner=""):
        if ref is None:
            return _join("void", inner)
        n = self.node(ref)
        if n is None:
            return _join("?", inner)
        k = n.kind
        if k == "DIBasicType":
            return _join(n.get("name"), inner)
        if k == "DISubroutineType":
            types = self.tuple(n.get("types"))
            ps = []
            rest = types[1:] if types else []
            for i, t in enumerate(rest):
                if t is None:
                    if i == len(rest) - 1:
                        ps.append("...")
                    continue
                tn = self.node(t)
                if tn is not None and "DIFlagArtificial" in llvm_ir.flags(tn) and i == 0:
                    continue
                ps.append(self.spell(t))
            ret = types[0] if types else None
            return self.spell(ret, f"{inner}({', '.join(ps)})")
        if k == "DICompositeType":
            if n.get("tag") == "DW_TAG_array_type":
                return self._spell_array(n.get("baseType"), self._counts(n), inner)
            return _join(self.full_name(n), inner)
        if k == "DIDerivedType":
            tag = n.get("tag")
            base = n.get("baseType")
            if tag == "DW_TAG_typedef":
                outer = self.scope_name(n.get("scope"))
                name = n.get("name") or "?"
                return _join(norm_name(f"{outer}::{name}" if outer else name), inner)
            if tag in ("DW_TAG_pointer_type", "DW_TAG_reference_type", "DW_TAG_rvalue_reference_type",
                       "DW_TAG_ptr_to_member_type"):
                sym = {"DW_TAG_pointer_type": "*", "DW_TAG_reference_type": "&",
                       "DW_TAG_rvalue_reference_type": "&&"}.get(tag)
                if tag == "DW_TAG_ptr_to_member_type":
                    cls = self.node(n.get("extraData"))
                    sym = f"{self.full_name(cls) if cls else '?'}::*"
                new = sym + inner
                bn = self._strip_cv_node(base)
                if bn is not None and (bn.kind == "DISubroutineType" or (
                        bn.kind == "DICompositeType" and bn.get("tag") == "DW_TAG_array_type")):
                    new = f"({new})"
                return self.spell(base, new)
            if tag in ("DW_TAG_const_type", "DW_TAG_volatile_type", "DW_TAG_restrict_type"):
                c, v, inner_ref = self._cv(ref)
                q = " ".join(w for w, on in (("const", c), ("volatile", v)) if on)
                bn = self.node(inner_ref)
                if bn is not None and bn.kind == "DIDerivedType" and bn.get("tag") in (
                        "DW_TAG_pointer_type", "DW_TAG_ptr_to_member_type"):
                    return self.spell(inner_ref, "" if not q else (q + (" " + inner if inner else "")))
                if inner_ref is None:
                    return _join(f"{q} void", inner)
                base_s = self.spell(inner_ref)
                return _join(f"{q} {base_s}", inner)
            if tag == "DW_TAG_atomic_type":
                return self.spell(base, inner)
        return _join("?", inner)

    def _spell_array(self, elem, counts, inner):
        dims = "".join(f"[{c}]" if c is not None else "[]" for c in counts)
        if inner:
            inner = inner if inner.startswith("(") else inner
        return self.spell(elem, f"{inner}{dims}")

    def _strip_cv_node(self, ref):
        _c, _v, r = self._cv(ref)
        return self.node(r)

    # -- records ----------------------------------------------------------------------------
    def members(self, n):
        return [self.node(r) for r in self.tuple(n.get("elements")) if self.node(r) is not None]

    def record_layout(self, n):
        """Fields and direct bases of a DI composite (bases: (name, offset_bits_or_vbase_off, virtual, flags))."""
        fields, bases, methods = [], [], []
        for m in self.members(n):
            if m.kind == "DISubprogram":
                methods.append(m)
                continue
            if m.kind != "DIDerivedType":
                continue
            tag = m.get("tag")
            fl = llvm_ir.flags(m)
            if tag == "DW_TAG_inheritance":
                bn = self.node(m.get("baseType"))
                bases.append((self.full_name(bn) if bn else "?", m.get("offset") or 0,
                              "DIFlagVirtual" in fl, fl, bn))
            elif tag == "DW_TAG_member" and "DIFlagArtificial" not in fl and "DIFlagStaticMember" not in fl:
                fields.append(m)
        return fields, bases, methods

    def dynamic(self, n):
        return n.get("vtableHolder") is not None


def _join(base, inner):
    if not inner:
        return base
    if inner.startswith("["):
        return base + inner
    return f"{base} {inner}"


_CALL = re.compile(r"h(n?\d+)_|v(n?\d+)_(n?\d+)_")


def _num(s):
    return -int(s[1:]) if s.startswith("n") else int(s)


def parse_thunk(sym: str):
    """`_ZThn4_N…`, `_ZTv0_n12_N…`, `_ZTc…` → (target, {nv, vcall}) or None."""
    if not sym.startswith(("_ZTh", "_ZTv", "_ZTc")):
        return None
    covariant = sym.startswith("_ZTc")
    i = 4 if covariant else 3
    nv = vcall = None
    count = 2 if covariant else 1
    for _ in range(count):
        m = _CALL.match(sym, i)
        if not m:
            return None
        if m.group(1) is not None:
            off = {"nv": _num(m.group(1)), "vcall": None}
        else:
            off = {"nv": _num(m.group(2)), "vcall": _num(m.group(3))}
        if nv is None and vcall is None:
            nv, vcall = off["nv"], off["vcall"]
        i = m.end()
    return "_Z" + sym[i:], {"nv": nv, "vcall": vcall}, sym[:i]
