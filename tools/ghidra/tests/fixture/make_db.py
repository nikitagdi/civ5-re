"""Writes the hand-made typedb.json and binfacts.json of the t.cpp fixture (addresses of t.elf)."""
import hashlib
import json
from pathlib import Path

HERE = Path(__file__).resolve().parent
H = "Civ5Expansion2/Fixture/t.h"


def node_types():
    b = lambda name, size, signed=True: {"kind": "builtin", "name": name, "signed": signed, "size": size}
    rec = lambda key, size: {"key": key, "kind": "record", "size": size}
    ptr = lambda to: {"kind": "pointer", "size": 4, "to": to}
    return {
        "void": b("void", None, False), "int": b("int", 4), "unsigned int": b("unsigned int", 4, False),
        "UNK_RET": {"kind": "unknown", "name": "UNK_RET", "size": None},
        "Vec2": rec("Vec2", 8), "Base": rec("Base", 8), "Derived": rec("Derived", 20), "Bits": rec("Bits", 4),
        "Removed": rec("Removed", 4), "Base *": ptr("Base"), "Derived *": ptr("Derived"),
        "Color": {"key": "Color", "kind": "enum", "size": 4},
        "DerivedPtr": {"kind": "typedef", "name": "DerivedPtr", "of": "Derived *"},
        "FwdFn": {"kind": "function", "ret": "int", "params": ["int"], "variadic": False},
        "FwdFn *": ptr("FwdFn"),
        # sorts before /civ5re/_fn/: typedef -> pointer -> funcdef created later (forward reference)
        "AFwdFnPtr": {"kind": "typedef", "name": "AFwdFnPtr", "of": "FwdFn *"},
    }


def field(name, type_, off, size, bit_offset=None, bit_size=None):
    return {"bit_offset": bit_offset, "bit_size": bit_size, "line": 1, "name": name, "offset": off,
            "offset_marker": hex(off), "size": size, "type": type_}


def record(name, size, fields, *, kind="struct", bases=(), dynamic=False, trivial=True, vtable=None,
           vtable_marker=None):
    return {"align": 4, "bases": list(bases), "comment": None, "complete": True, "dsize": size, "dynamic": dynamic,
            "fields": fields, "file": H, "instance_markers": [], "kind": kind, "line": 1, "markers": {"lua": []},
            "name": name, "origin": "tree", "size": size, "size_marker": "?" if name == "Bits" else hex(size),
            "template": None, "trivial_for_calls": trivial, "typeinfo_symbol": None,
            "vptrs": [0] if dynamic else [], "vtable": vtable, "vtable_marker": vtable_marker,
            "vtable_symbol": ("_ZTV%d%s" % (len(name), name)) if vtable else None}


def slot(fn):
    return {"function": fn, "kind": "method", "symbol": None, "thunk": None}


def vtable(sub, fns):
    return {"groups": [{"offset_to_top": 0, "slots": [slot(f) for f in fns], "subobject": sub, "vbase_offsets": []}]}


def markers(addr, fastcc=False):
    return {"absent": False, "addr": addr, "base": None, "deleting": None, "fastcc": fastcc, "inline": False,
            "lua": [], "pure": False, "thunks": []}


def fn(key, qualified, kind, ret, params, addr, *, scope="", static=False, virtual=False, indirect=False,
       unknown=False, fastcc=False, variants=None, const=False, linkage="external"):
    name = qualified.split("::")[-1]
    is_class = bool(scope)
    return {"access": "public", "comment": None, "const": const, "file": H, "has_body": False, "instances": [],
            "key": key, "kind": kind, "line": 1, "linkage": linkage, "mangled": key, "markers": markers(addr, fastcc),
            "name": name, "params": [{"name": n, "type": t} for n, t in params], "params_unknown": unknown,
            "pure": False, "qualified": qualified, "redecls": [], "return": ret, "return_indirect": indirect,
            "scope": scope, "scope_kind": "class" if is_class else "namespace", "static": static,
            "this": (scope + " *") if is_class and not static else None, "variadic": False, "variants": variants,
            "virtual": virtual, "vtable_index": None}


def typedb():
    records = [
        record("Base", 8, [field("m_iCount", "int", 4, 4)], dynamic=True, trivial=False,
               vtable=vtable("Base", ["_ZN4Base6UpdateEi", "_ZN4Base5ExtraEii"]), vtable_marker="0x080484c4"),
        record("Bits", 4, [field("a", "int", 0, 4, 0, 3), field("b", "int", 0, 4, 3, 5),
                           field("c", "unsigned int", 0, 4, 8, 10)]),
        record("Derived", 20, [field("m_iExtra", "int", 8, 4), field("m_pos", "Vec2", 12, 8)],
               bases=[{"access": "public", "name": "Base", "offset": 0, "virtual": False}], dynamic=True,
               trivial=False, vtable=vtable("Derived", ["_ZN7Derived6UpdateEi", "_ZN4Base5ExtraEii",
                                                        "_ZN7Derived5ScaleEi"]), vtable_marker="0x080484d4"),
        record("Removed", 4, [field("v", "int", 0, 4)]),
        record("Vec2", 8, [field("x", "int", 0, 4), field("y", "int", 4, 4)], kind="struct"),
    ]
    functions = [
        fn("_Z10CallUpdateP4Basei", "CallUpdate", "function", "int", [("p", "Base *"), ("dt", "int")], "0x08049780"),
        fn("_Z9CallExtraP4Base", "CallExtra", "function", "int", [("p", "Base *")], "0x080497a0"),
        fn("_ZL11hidden_funcP7Derivedi", "hidden_func", "function", "int", [("d", "Derived *"), ("k", "int")],
           "0x08049800", fastcc=True, linkage="internal"),
        fn("_ZN4Base5ExtraEii", "Base::Extra", "method", "int", [("a", "int"), ("b", "int")], "0x080496d0",
           scope="Base", virtual=True),
        fn("_ZN4Base6UpdateEi", "Base::Update", "method", "int", [("dt", "int")], "0x080496c0", scope="Base",
           virtual=True),
        fn("_ZN4BaseC1Ev", "Base::Base", "ctor", "void", [], "0x080496a0", scope="Base",
           variants={"C1": "_ZN4BaseC1Ev", "C2": "_ZN4BaseC2Ev"}),
        fn("_ZN7Derived3SumEii", "Derived::Sum", "method", "int", [], "0x08049760", scope="Derived", unknown=True),
        fn("_ZN7Derived4PeekEv", "Derived::Peek", "method", "int", [], "0x08049770", scope="Derived"),
        fn("_ZN7Derived5ScaleEi", "Derived::Scale", "method", "UNK_RET", [("k", "int")], "0x08049730",
           scope="Derived", virtual=True),
        fn("_ZN7Derived6UpdateEi", "Derived::Update", "method", "int", [("dt", "int")], "0x08049720",
           scope="Derived", virtual=True),
        fn("_ZN7DerivedC1Ev", "Derived::Derived", "ctor", "void", [], "0x080496f0", scope="Derived",
           variants={"C1": "_ZN7DerivedC1Ev", "C2": "_ZN7DerivedC2Ev"}),
        fn("_ZNK7Derived6GetPosEv", "Derived::GetPos", "method", "Vec2", [], "0x08049740", scope="Derived",
           indirect=True, const=True),
    ]
    return {
        "cpp_defs": [], "diagnostics": [],
        "enums": [{"comment": None, "file": H, "line": 1, "markers": {"lua": []}, "qualified": "Color", "size": 4,
                   "values": [{"name": "Red", "value": 0}, {"name": "Green", "value": 1},
                              {"name": "Blue", "value": 2}]}],
        "files": [{"kind": "h", "origin": "guess", "path": H, "sha256": "0" * 64}],
        "functions": functions,
        "globals": [{"addr_marker": "0x0804b8dc", "comment": None, "file": H, "inline_marker": False, "line": 1,
                     "linkage": "external", "mangled": "g_counter", "qualified": "g_counter", "size": 4,
                     "static_member": False, "type": "int"}],
        "inputs": {}, "markers": "strict", "ok": True, "records": records, "schema": "civ5re.typedb/1",
        "stats": {}, "tool": {},
        "typedefs": [{"comment": None, "file": H, "line": 1, "markers": {"lua": []}, "qualified": "DerivedPtr",
                      "type": "Derived *"},
                     {"comment": None, "file": H, "line": 2, "markers": {"lua": []}, "qualified": "AFwdFnPtr",
                      "type": "FwdFn *"}],
        "types": node_types(),
    }


SYMS = [  # mangled, addr, size, type, kind, variant
    ("_ZN4BaseC1Ev", 0x080496a0, 18, "func", "ctor", "C1"), ("_ZN4BaseC2Ev", 0x080496a0, 18, "func", "ctor", "C2"),
    ("_ZN4Base6UpdateEi", 0x080496c0, 12, "func", "method", None),
    ("_ZN4Base5ExtraEii", 0x080496d0, 17, "func", "method", None),
    ("_ZN7DerivedC1Ev", 0x080496f0, 48, "func", "ctor", "C1"),
    ("_ZN7DerivedC2Ev", 0x080496f0, 48, "func", "ctor", "C2"),
    ("_ZN7Derived6UpdateEi", 0x08049720, 14, "func", "method", None),
    ("_ZN7Derived5ScaleEi", 0x08049730, 16, "func", "method", None),
    ("_ZNK7Derived6GetPosEv", 0x08049740, 20, "func", "method", None),
    ("_ZN7Derived3SumEii", 0x08049760, 16, "func", "method", None),
    ("_ZN7Derived4PeekEv", 0x08049770, 11, "func", "method", None),
    ("_Z10CallUpdateP4Basei", 0x08049780, 27, "func", "function", None),
    ("_Z9CallExtraP4Base", 0x080497a0, 30, "func", "function", None),
    ("_Z6UsePosP7Derivedi", 0x080497c0, 58, "func", "function", None),
    ("_start", 0x08049810, 131, "func", "c", None),
    ("_ZTV4Base", 0x080484c4, 16, "object", "vtable", None),
    ("_ZTV7Derived", 0x080484d4, 20, "object", "vtable", None),
    ("g_counter", 0x0804b8dc, 4, "object", "c", None),
]
FDES = [(0x080496a0, 0x12), (0x080496c0, 0xc), (0x080496d0, 0x11), (0x080496f0, 0x30), (0x08049720, 0xe),
        (0x08049730, 0x10), (0x08049740, 0x14), (0x08049760, 0x10), (0x08049770, 0xb), (0x08049780, 0x1b),
        (0x080497a0, 0x1e), (0x080497c0, 0x3a), (0x08049800, 0xe), (0x08049810, 0x83)]


def binfacts():
    h = lambda a: "0x%08x" % a
    elf = HERE / "t.elf"
    syms = [{"addr": h(a), "bind": "global", "cv": None, "demangled": m, "group": "firaxis", "kind": k,
             "mangled": m, "name": m, "params": None, "scope": None, "scope_is_class": None,
             "section": ".text" if t == "func" else (".data" if m == "g_counter" else ".rodata"), "size": s,
             "type": t, "variant": v} for m, a, s, t, k, v in SYMS]
    syms.sort(key=lambda s: (s["addr"], s["mangled"]))
    vt = lambda sym, cls, addr, size, slots: {
        "addr": h(addr), "class": cls, "construction": False, "size": size, "symbol": sym,
        "groups": [{"address_point": h(addr + 8), "offset_to_top": 0, "typeinfo": None, "vbase_offsets": [],
                    "slots": [{"kind": "func", "symbols": [], "target": h(t)} for t in slots]}]}
    return {
        "binary": {"build_id": None, "comment": [], "entry": h(0x08049810), "name": "Civ5XP",
                   "sha256": hashlib.sha256(elf.read_bytes()).hexdigest(), "size": elf.stat().st_size},
        "functions_eh": [{"addr": h(a), "size": s} for a, s in FDES],
        "plt": [], "schema": 1,
        "sections": [{"addr": h(0x080484c4), "flags": "A", "name": ".rodata", "size": 0x24},
                     {"addr": h(0x080496a0), "flags": "AX", "name": ".text", "size": 0x1f3},
                     {"addr": h(0x0804b8dc), "flags": "WA", "name": ".data", "size": 4}],
        "stats": {}, "symbols": syms, "tool": {}, "typeinfo": [],
        "vtables": [vt("_ZTV4Base", "Base", 0x080484c4, 16, [0x080496c0, 0x080496d0]),
                    vt("_ZTV7Derived", "Derived", 0x080484d4, 20, [0x08049720, 0x080496d0, 0x08049730])],
    }


def dump(path, obj):
    path.write_text(json.dumps(obj, indent=1, sort_keys=True) + "\n", encoding="utf-8", newline="\n")


if __name__ == "__main__":
    dump(HERE / "typedb.json", typedb())
    dump(HERE / "binfacts.json", binfacts())
