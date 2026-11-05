import re
from collections import namedtuple

ADDR = r"[0-9a-f]{8}"
OFF = r"(?:0|[1-9a-f][0-9a-f]*)"

Placeholder = namedtuple("Placeholder", "kind where pattern check")
Match = namedtuple("Match", "kind value")

# `where` is the only place the placeholder may appear; `check` is what rule M5 verifies against.
PLACEHOLDERS = (
    Placeholder("fun", "function name", re.compile(rf"Fun_({ADDR})"), "own address marker"),
    Placeholder("global", "global name", re.compile(rf"g_({ADDR})"), "own address marker"),
    Placeholder("class", "class name", re.compile(rf"Class_({ADDR})"),
                "own vtable marker; error if that vtable has a name in RTTI"),
    Placeholder("field", "field name", re.compile(rf"field_({OFF})"), "own ///< offset"),
    Placeholder("unk", "field name", re.compile(rf"unk_({OFF})"), "own ///< offset"),
    Placeholder("vf", "virtual method name", re.compile(r"vf_(\d{2,})"), "vtable_index, decimal"),
    Placeholder("param", "parameter name", re.compile(r"a([1-9]\d*)"),
                "parameter number from 1, not counting this"),
    Placeholder("unk_ret", "return type", re.compile(r"UNK_RET"), "none"),
    Placeholder("unk_params", "whole parameter list", re.compile(r"UNK_PARAMS"),
                "forbidden if a C++ symbol is exported at the function's address"),
    Placeholder("unk_data", "element type of a global array", re.compile(r"UNK_DATA"), "array size, rule A9"),
)

MARKER_PLACEHOLDERS = ("size ?", "vtable ?", "lua ?")

GHIDRA_STYLE = re.compile(r"(?:FUN|DAT|PTR|LAB|SUB|UNK)_[0-9a-fA-F]{6,8}|field_0x[0-9a-fA-F]+")


def kinds():
    return [p.kind for p in PLACEHOLDERS]


def classify(name):
    for p in PLACEHOLDERS:
        m = p.pattern.fullmatch(name)
        if m:
            return Match(p.kind, m.group(1) if m.groups() else None)
    return None


def is_placeholder(name):
    return classify(name) is not None


def is_ghidra_style(name):
    return GHIDRA_STYLE.fullmatch(name) is not None


def fun_name(addr):
    return f"Fun_{addr:08x}"


def global_name(addr):
    return f"g_{addr:08x}"


def class_name(vtable_addr):
    return f"Class_{vtable_addr:08x}"


def field_name(offset):
    return f"field_{offset:x}"


def unk_name(offset):
    return f"unk_{offset:x}"


def vf_name(index):
    return f"vf_{index:02d}"


def param_name(number):
    return f"a{number}"


def zero_based_params(names):
    """True for a parameter list that uses the original 0-based names (a0, a1, ...): its a<N> are names, not
    placeholders (Firaxis SDK, FLuaTypes.h)."""
    return any(n == "a0" for n in names)
