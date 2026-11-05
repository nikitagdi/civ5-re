import hashlib
import json
import os
import time
from collections import Counter, defaultdict
from pathlib import Path

import bisect

from binfacts import classsize, ehframe, groups as groups_mod, rtti, symbols, vtables
from binfacts.api import SCHEMA, BinfactsError
from binfacts.elf import Elf, STB_NAMES, STT_NAMES

MODULE_VERSION = "2"
GROUPS_CFG = Path(__file__).with_name("groups.cfg")
DEFAULT_OUT = Path("build") / "binfacts.json"
KINDS = ("function", "method", "ctor", "dtor", "data", "vtable", "typeinfo", "typeinfo_name",
         "ctor_vtable", "guard", "thunk", "c")
DENOMINATOR_KINDS = ("function", "method", "ctor", "dtor", "data", "thunk", "vtable", "c")
COVERED_BY_CLASS = ("typeinfo", "typeinfo_name", "guard")
VARIANTS = ("C1", "C2", "C3", "D0", "D1", "D2")
ENTITY_KEYS = ("c1_eq_c2", "c1_ne_c2", "c1_only", "c2_only", "d1_eq_d2", "d1_ne_d2", "d1_only", "d2_only", "has_d0")
CLASS_KINDS = ("vtable", "typeinfo", "typeinfo_name", "ctor_vtable")


def hx(v):
    return "0x%08x" % v


def sha256_bytes(data):
    return hashlib.sha256(data).hexdigest()


def _section_name(elf, shndx):
    if shndx < len(elf.sections):
        return elf.sections[shndx].name
    return {0xFFF1: "*ABS*", 0xFFF2: "*COM*"}.get(shndx, f"*{shndx}*")


def _build_id(elf):
    if elf.section(".note.gnu.build-id") is None:
        return None
    data = elf.section_data(".note.gnu.build-id")
    namesz, descsz = int.from_bytes(data[0:4], "little"), int.from_bytes(data[4:8], "little")
    start = 12 + ((namesz + 3) & ~3)
    return data[start:start + descsz].hex()


def _comment(elf):
    if elf.section(".comment") is None:
        return []
    return [s.decode("latin-1") for s in elf.section_data(".comment").split(b"\0") if s]


def _class_evidence(parsed, typeinfos, vtable_list):
    classes = set()
    for p in parsed:
        if p.kind in CLASS_KINDS:
            classes.add(p.entity)
        elif p.kind in ("ctor", "dtor"):
            classes.add(p.scope)
        elif p.kind == "function" and p.cv is not None:
            classes.add(p.scope)
    for t in typeinfos:
        classes.add(t.class_)
        for b in t.bases:
            classes.add(b["class"])
    for v in vtable_list:
        classes.add(v["class"])
    classes.discard("")
    return classes


def _variant_stats(rows):
    out = {g: {"symbols": {v: 0 for v in VARIANTS}, "entities": {k: 0 for k in ENTITY_KEYS}}
           for g in groups_mod.GROUPS}
    ents = defaultdict(dict)
    for p, group, addr in rows:
        if p.variant is None:
            continue
        out[group]["symbols"][p.variant] += 1
        ents[(group, symbols.entity_key(p))][p.variant] = addr
    for (group, _key), vs in ents.items():
        e = out[group]["entities"]
        for a, b, name in (("C1", "C2", "c"), ("D1", "D2", "d")):
            if a in vs and b in vs:
                e[f"{name}1_{'eq' if vs[a] == vs[b] else 'ne'}_{name}2"] += 1
            elif a in vs:
                e[f"{name}1_only"] += 1
            elif b in vs:
                e[f"{name}2_only"] += 1
        if "D0" in vs:
            e["has_d0"] += 1
    return out


def _vtable_rows(vtable_list):
    rows = []
    for v in sorted(vtable_list, key=lambda v: (v["addr"], v["symbol"])):
        rows.append({
            "symbol": v["symbol"], "class": v["class"], "addr": hx(v["addr"]), "size": v["size"],
            "construction": v["construction"],
            "groups": [{
                "offset_to_top": g["offset_to_top"], "vbase_offsets": g["vbase_offsets"],
                "typeinfo": hx(g["typeinfo"]), "address_point": hx(g["address_point"]),
                "slots": [_slot_row(sl) for sl in g["slots"]]} for g in v["groups"]]})
    return rows


def _slot_row(sl):
    row = {"target": hx(sl["target"]), "symbols": sl["symbols"], "kind": sl["kind"]}
    if "thunk_target" in sl:
        row["thunk_target"] = hx(sl["thunk_target"])
        row["adjust"] = sl["adjust"]
    return row


CALL_OPCODE = b"\xe8"


def _call_sites(text, base, targets):
    """Addresses of `call rel32` byte patterns in a code section that land on one of targets (a filter only)."""
    out = []
    n = len(text)
    i = text.find(CALL_OPCODE)
    while i != -1:
        if i + 5 <= n:
            t = (base + i + 5 + int.from_bytes(text[i + 1:i + 5], "little", signed=True)) & 0xFFFFFFFF
            if t in targets:
                out.append(base + i)
        i = text.find(CALL_OPCODE, i + 1)
    return out


def class_sizes(elf, sym_rows, typeinfos, fde):
    """Lower bounds of class sizes proved by code: {class: {min, from[]}}."""
    new_addrs = {a for a, n in elf.plt_stubs() if n == "_Znwj"}
    structors = {}             # address -> [size, classes, classes with a constructor variant]
    for r in sym_rows:
        if r["kind"] in ("ctor", "dtor") and r["type"] == "func" and r["group"] == "firaxis" and r["size"] > 0 \
                and r["scope"]:
            ent = structors.setdefault(int(r["addr"], 16), [r["size"], set(), set()])
            ent[1].add(r["scope"])
            if r["variant"] in ("C1", "C2"):
                ent[2].add(r["scope"])
    # an address shared by structors of different classes (folded bodies) proves nothing about either
    structors = {a: v for a, v in structors.items() if len(v[1]) == 1}
    ctor_class = {a: next(iter(v[1])) for a, v in structors.items() if v[2]}
    derived = {b["class"] for t in typeinfos for b in t.bases}
    best = {}                  # class -> [bytes, [addresses]]
    new_min = {}               # class -> [bytes, [call sites]]

    for addr, (size, classes, _ctor) in sorted(structors.items()):
        res = classsize.scan(elf.read(addr, size) or b"", addr)
        if res.extent:
            _raise(best, next(iter(classes)), res.extent, addr)

    funcs = {}
    for r in sym_rows:
        if r["type"] == "func" and r["size"] > 0:
            funcs[int(r["addr"], 16)] = r["size"]
    for a, n in fde:
        funcs[a] = max(funcs.get(a, 0), n)
    starts = sorted(funcs)
    text = elf.section(".text")
    if new_addrs and text is not None:
        todo = set()
        for site in _call_sites(elf.section_data(".text"), text.addr, new_addrs):
            k = bisect.bisect_right(starts, site) - 1
            if k >= 0 and site < starts[k] + funcs[starts[k]]:
                todo.add(starts[k])
        for a in sorted(todo):
            res = classsize.scan(elf.read(a, funcs[a]) or b"", a, new_addrs, ctor_class)
            for site, cls, n in res.news:
                if cls not in derived:
                    cur = new_min.get(cls)
                    if cur is None or n < cur[0]:
                        new_min[cls] = [n, [site]]
                    elif n == cur[0]:
                        cur[1].append(site)
    for cls, (n, sites) in new_min.items():
        for site in sites:
            _raise(best, cls, n, site)
    return {cls: {"min": v, "from": [hx(a) for a in sorted(set(w))[:4]]} for cls, (v, w) in sorted(best.items())}


def _raise(best, cls, value, where):
    cur = best.get(cls)
    if cur is None or value > cur[0]:
        best[cls] = [value, [where]]
    elif value == cur[0]:
        cur[1].append(where)


def extract(data, name, sha256, demangler, rules, groups_sha_):
    """Builds the binfacts dict from the bytes of an ELF32 file."""
    elf = Elf(data)
    defined = elf.defined_symbols()
    names = [s.name for s in defined]
    parsed_by_name = dict(zip(names, symbols.parse_all(names, demangler.many(names))))

    roots = vtables.typeinfo_roots(elf, defined)
    typeinfos = rtti.parse(elf, defined, roots, demangler)
    vtable_list = vtables.build(elf, defined, typeinfos, demangler)

    classes = _class_evidence(list(parsed_by_name.values()), typeinfos, vtable_list)
    ordered = sorted(defined, key=lambda s: (s.value, s.name))
    parsed = [parsed_by_name[s.name] for s in ordered]
    group_of = groups_mod.assign(rules, parsed)

    sym_rows = []
    stat_rows = []
    kind_count = defaultdict(Counter)
    group_count = Counter()
    for s, p, group in zip(ordered, parsed, group_of):
        is_class = True if p.scope and p.scope in classes else None
        kind = "method" if p.kind == "function" and is_class else p.kind
        sym_rows.append({
            "mangled": s.name, "demangled": p.demangled, "addr": hx(s.value), "size": s.size,
            "type": STT_NAMES[s.type], "bind": STB_NAMES[s.bind], "section": _section_name(elf, s.shndx),
            "kind": kind, "variant": p.variant, "scope": p.scope, "scope_is_class": is_class,
            "name": p.name, "params": p.params, "cv": p.cv, "group": group})
        stat_rows.append((p, group, s.value))
        kind_count[group][kind] += 1
        group_count[group] += 1

    by_addr = defaultdict(list)
    for s in ordered:
        if s.type == 2:
            by_addr[s.value].append(s.name)
    aliased = [v for v in by_addr.values() if len(v) > 1]

    eh = elf.section(".eh_frame")
    fde = sorted(set(ehframe.parse(elf.section_data(".eh_frame"), eh.addr))) if eh else []

    ti_rows = [{"symbol": t.symbol, "addr": hx(t.addr), "class": t.class_, "kind": t.kind, "name": t.name,
                "flags": t.flags, "bases": t.bases} for t in typeinfos]

    by_kind = {k: kind_count["firaxis"][k] for k in DENOMINATOR_KINDS}
    stats = {
        "groups": {g: group_count[g] for g in groups_mod.GROUPS},
        "kinds": {g: {k: kind_count[g][k] for k in KINDS} for g in groups_mod.GROUPS},
        "variants": _variant_stats(stat_rows),
        "aliases": {"addresses": len(aliased), "names": sum(len(v) for v in aliased)},
        "denominator": {
            "by_kind": by_kind, "total": sum(by_kind.values()),
            "covered_by_class": {k: kind_count["firaxis"][k] for k in COVERED_BY_CLASS}},
    }
    return {
        "schema": SCHEMA,
        "tool": {"binfacts": MODULE_VERSION, "cxxfilt": demangler.get_version(), "groups_cfg": groups_sha_},
        "binary": {"name": name, "sha256": sha256, "size": len(data), "entry": hx(elf.entry),
                   "build_id": _build_id(elf), "comment": _comment(elf)},
        "sections": [{"name": s.name, "addr": hx(s.addr), "size": s.size, "flags": elf.flags_string(s)}
                     for s in elf.sections if s.name],
        "symbols": sym_rows,
        "functions_eh": [{"addr": hx(a), "size": n} for a, n in fde],
        "plt": [{"addr": hx(a), "name": n} for a, n in sorted(elf.plt_stubs())],
        "typeinfo": ti_rows,
        "vtables": _vtable_rows(vtable_list),
        "sizes": class_sizes(elf, sym_rows, typeinfos, fde),
        "stats": stats,
    }


def dumps(facts):
    """Deterministic JSON: sorted keys, one array element per line."""
    def enc(o):
        return json.dumps(o, sort_keys=True, separators=(",", ":"))

    parts = []
    for key in sorted(facts):
        val = facts[key]
        if isinstance(val, list) and val:
            parts.append(f"{enc(key)}:[\n" + ",\n".join(enc(x) for x in val) + "\n]")
        else:
            parts.append(f"{enc(key)}:{enc(val)}")
    return "{\n" + ",\n".join(parts) + "\n}\n"


def write(path, facts):
    path = Path(path)
    path.parent.mkdir(parents=True, exist_ok=True)
    tmp = path.with_name(path.name + ".tmp")
    tmp.write_bytes(dumps(facts).encode("ascii"))
    os.replace(tmp, path)


def read(path):
    try:
        with open(path, "rb") as f:
            data = json.load(f)
    except (OSError, ValueError) as e:
        raise BinfactsError(f"cannot read {path}: {e}") from None
    if not isinstance(data, dict) or data.get("schema") != SCHEMA:
        raise BinfactsError(f"{path}: schema is not {SCHEMA}")
    return data


def groups_sha(cfg_path=GROUPS_CFG):
    return sha256_bytes(Path(cfg_path).read_bytes())


def _target(cfg):
    from civ5re import binlock
    try:
        return binlock.target(binlock.load(cfg.root / "bins.lock"))
    except (OSError, binlock.BinlockError) as e:
        raise BinfactsError(f"bins.lock: {e}") from None


def out_path(cfg):
    return cfg.root / DEFAULT_OUT


def cxxfilt_exe(cfg):
    exe = cfg.tool("llvm-cxxfilt")
    return exe if exe.is_file() else None


def stale_reason(cfg, facts):
    """None if the cache matches bins.lock, module version, groups.cfg and (when available) llvm-cxxfilt."""
    if facts.get("binary", {}).get("sha256") != _target(cfg).sha256:
        return "binary sha256 differs from bins.lock"
    tool = facts.get("tool", {})
    if tool.get("binfacts") != MODULE_VERSION:
        return "module version changed"
    if tool.get("groups_cfg") != groups_sha():
        return "groups.cfg changed"
    exe = cxxfilt_exe(cfg)
    if exe is not None and tool.get("cxxfilt") != symbols.Demangler(exe).get_version():
        return "llvm-cxxfilt version changed"
    return None


def rebuild(cfg):
    from civ5re import binlock
    target = _target(cfg)
    bin_dir = cfg.path("bin_dir")
    if bin_dir is None:
        raise BinfactsError("bin_dir is not configured")
    status, msg = binlock.verify(target, bin_dir)
    if status != "ok":
        raise BinfactsError(msg)
    exe = cxxfilt_exe(cfg)
    if exe is None:
        raise BinfactsError(f"llvm-cxxfilt not found: {cfg.tool('llvm-cxxfilt')}")
    rules = groups_mod.load_rules(GROUPS_CFG)
    data = (Path(bin_dir) / target.path).read_bytes()
    facts = extract(data, Path(target.path).name, target.sha256, symbols.Demangler(exe), rules, groups_sha())
    write(out_path(cfg), facts)
    return facts


def ensure(cfg, force=False):
    """Returns (facts, action), action is `built` or `fresh`."""
    path = out_path(cfg)
    if not force and path.is_file():
        try:
            facts = read(path)
            reason = stale_reason(cfg, facts)
        except BinfactsError as e:
            facts, reason = None, str(e)
        if reason is None:
            return facts, "fresh"
        try:
            return rebuild(cfg), "built"
        except BinfactsError as e:
            raise BinfactsError(f"{path.name} is stale ({reason}) and cannot be rebuilt: {e}") from None
    return rebuild(cfg), "built"


def timed_ensure(cfg, force=False):
    t = time.perf_counter()
    facts, action = ensure(cfg, force)
    return facts, action, time.perf_counter() - t
