import re
import sys
from collections import Counter, defaultdict

from binfacts import build, groups, symbols
from binfacts.api import BinfactsError

TOP_PREFIXES = 12


def _prefix(sym):
    if sym["kind"] == "c":
        m = re.match(r"[?_]*[A-Za-z]+", sym["mangled"])
        return m.group(0) if m else sym["mangled"][:4]
    text = sym["scope"] or sym["name"]
    return symbols.strip_templates(symbols.split_entity(text)[0][0]) if text else "?"


def _facts(args):
    from civ5re import config
    facts, action, seconds = build.timed_ensure(config.load(), force=getattr(args, "force", False))
    return facts, action, seconds


def _cmd_build(args):
    facts, action, seconds = _facts(args)
    st = facts["stats"]
    print(f"binfacts: {action} in {seconds:.1f} s, {len(facts['symbols'])} symbols, "
          f"{len(facts['vtables'])} vtables, {len(facts['typeinfo'])} typeinfo, {len(facts['functions_eh'])} FDEs, "
          f"{len(facts['plt'])} PLT entries")
    print("denominator: " + ", ".join(f"{k} {v}" for k, v in st["denominator"]["by_kind"].items())
          + f" = {st['denominator']['total']} symbols (firaxis)")
    return 0


def _cmd_report(args):
    facts, _action, _s = _facts(args)
    st = facts["stats"]
    by_group = defaultdict(list)
    for s in facts["symbols"]:
        by_group[s["group"]].append(s)
    print(f"binary {facts['binary']['name']} sha256 {facts['binary']['sha256']}")
    print(f"symbols {len(facts['symbols'])}; unclassified 0\n")
    print("groups (symbols):")
    for g in groups.GROUPS:
        print(f"  {g:<11}{st['groups'][g]:>7}")
    for g in groups.GROUPS:
        print(f"\n{g}: top prefixes (first `::` component, or leading word for C names)")
        c = Counter()
        for s in by_group[g]:
            c[_prefix(s)] += 1
        for prefix, n in c.most_common(TOP_PREFIXES):
            print(f"  {n:>6}  {prefix}")
        kinds = ", ".join(f"{k} {n}" for k, n in st["kinds"][g].items() if n)
        print(f"  kinds: {kinds}")
    d = st["denominator"]
    print("\ncoverage denominator (symbols of group firaxis):")
    for k, v in d["by_kind"].items():
        print(f"  {k:<10}{v:>7}")
    print(f"  {'total':<10}{d['total']:>7}")
    print("  covered by class: " + ", ".join(f"{k} {v}" for k, v in d["covered_by_class"].items()))
    unnamed = [v for v in facts["vtables"] if any(not sl["symbols"] for g in v["groups"] for sl in g["slots"])]
    slots = sum(len(g["slots"]) for v in facts["vtables"] for g in v["groups"])
    nameless = sum(1 for v in facts["vtables"] for g in v["groups"] for sl in g["slots"] if not sl["symbols"])
    print(f"\nvtables with slots without a symbol name: {len(unnamed)} of {len(facts['vtables'])} "
          f"({nameless} of {slots} slots)")
    for v in unnamed[:10]:
        n = sum(1 for g in v["groups"] for sl in g["slots"] if not sl["symbols"])
        print(f"  {v['addr']}  {v['class']}  {n} slots")
    return 0


def _cmd_find(args):
    try:
        rx = re.compile(args.regex)
    except re.error as e:
        raise BinfactsError(f"bad regex: {e}") from None
    facts, _action, _s = _facts(args)
    n = 0
    for s in facts["symbols"]:
        if rx.search(s["demangled"]) or rx.search(s["mangled"]):
            print(f"{s['addr']} {s['size']:>7} {s['kind']:<13}{s['group']:<11}{s['demangled']}")
            n += 1
    print(f"{n} symbols", file=sys.stderr)
    return 0 if n else 1


def _slot_name(sl, demangled):
    if not sl["symbols"]:
        return "<no symbol>"
    return demangled.get(sl["symbols"][0], sl["symbols"][0])


def _cmd_vtable(args):
    facts, _action, _s = _facts(args)
    demangled = {s["mangled"]: s["demangled"] for s in facts["symbols"]}
    wanted = args.cls
    vts = [v for v in facts["vtables"] if v["class"] == wanted]
    if not vts:
        rx = re.compile(re.escape(wanted))
        vts = [v for v in facts["vtables"] if rx.search(v["class"])]
    if not vts:
        print(f"no vtable for {wanted}", file=sys.stderr)
        return 1
    groups_of = {s["mangled"]: s["group"] for s in facts["symbols"]}
    for v in vts:
        kind = "construction vtable" if v["construction"] else "vtable"
        print(f"## {v['class']}  {kind} {v['symbol']} at {v['addr']} size {v['size']}")
        for gi, g in enumerate(v["groups"]):
            print(f"  group {gi}: offset_to_top {g['offset_to_top']}, vbase_offsets {g['vbase_offsets']}, "
                  f"typeinfo {g['typeinfo']}, address_point {g['address_point']}, {len(g['slots'])} slots")
            for i, sl in enumerate(g["slots"]):
                sym = sl["symbols"][0] if sl["symbols"] else None
                grp = groups_of.get(sym, "-") if sl["kind"] not in ("pure", "import") else "-"
                print(f"   [{i}] {sl['kind']:<7}{sl['target']} {grp:<10}{_slot_name(sl, demangled)}")
    return 0


def _cmd_bytes(args):
    import struct
    from pathlib import Path
    from binfacts.elf import Elf
    from civ5re import config
    cfg = config.load()
    facts, _action, _s = _facts(args)
    bin_dir = cfg.path("bin_dir")
    if bin_dir is None:
        raise BinfactsError("bin_dir is not configured")
    elf = Elf.open(Path(bin_dir) / build._target(cfg).path)
    try:
        va = int(args.addr, 0)
    except ValueError:
        raise BinfactsError(f"bad address: {args.addr}") from None
    if args.len <= 0:
        raise BinfactsError("len must be positive")
    if args.as_ == "cstr":
        text = elf.cstring(va)
        if text is None:
            raise BinfactsError(f"0x{va:08x} is not in a loaded segment")
        print(f"0x{va:08x} {text!r}")
        return 0
    data = elf.read(va, args.len)
    if data is None:
        raise BinfactsError(f"0x{va:08x}+{args.len} is not in a loaded segment")
    if args.as_ is None:
        for i in range(0, len(data), 16):
            chunk = data[i:i + 16]
            asc = "".join(chr(b) if 32 <= b < 127 else "." for b in chunk)
            print(f"0x{va + i:08x}  {chunk.hex(' '):<47}  {asc}")
        return 0
    fmt = {"u32": "<I", "f32": "<f", "ptr": "<I"}[args.as_]
    for i in range(0, len(data) - 3, 4):
        (v,) = struct.unpack_from(fmt, data, i)
        if args.as_ == "u32":
            text = f"0x{v:08x} {v}"
        elif args.as_ == "f32":
            text = repr(v)
        else:
            s = elf.cstring(v) if v else None
            printable = s and len(s) < 80 and all(32 <= ord(c) < 127 for c in s)
            text = f"0x{v:08x}" + (f" {s!r}" if printable else "")
        print(f"0x{va + i:08x}  {text}")
    return 0


def _dispatch(args):
    sub = getattr(args, "sub", None)
    if sub == "bytes":
        return _cmd_bytes(args)
    if sub == "report":
        return _cmd_report(args)
    if sub == "find":
        return _cmd_find(args)
    if sub == "vtable":
        return _cmd_vtable(args)
    return _cmd_build(args)


def register(subparsers):
    p = subparsers.add_parser("binfacts", help="Civ5XP -> build/binfacts.json")
    p.add_argument("--force", action="store_true", help="rebuild even if the cache is fresh")
    p.set_defaults(func=_dispatch)
    sub = p.add_subparsers(dest="sub", metavar="<view>")
    sub.add_parser("report", help="groups, top prefixes, denominator, vtables without slot names")
    f = sub.add_parser("find", help="symbols by name: address, size, kind, group")
    f.add_argument("regex")
    b = sub.add_parser("bytes", help="read bytes of the binary at a virtual address (read-only)")
    b.add_argument("addr")
    b.add_argument("len", type=int)
    b.add_argument("--as", dest="as_", choices=("u32", "f32", "cstr", "ptr"), default=None)
    v = sub.add_parser("vtable", help="vtable slots of a class")
    v.add_argument("cls", metavar="class")
