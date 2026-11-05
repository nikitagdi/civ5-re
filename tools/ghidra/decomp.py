"""civ5re ghidra decomp: selecting functions, dump freshness and build/decomp/index.tsv. No Ghidra here."""

import os
import re
from pathlib import Path

HEX = re.compile(r"^(?:0[xX][0-9a-fA-F]{1,8}|[0-9a-fA-F]{8})$")
INDEX = "index.tsv"
FORMAT = "6"  # dump layout version, keep equal to FORMAT in Civ5reDecomp.java; part of freshness
INDEX_HEADER = "addr\tname\tsize\tplan_sha256\tformat"
RUN_FILE = ".run.tsv"
NO_PLAN = "-"


class SelectError(Exception):
    pass


def addr8(text):
    return f"{int(text, 16):08x}"


def is_func(sym):
    return sym.get("type") == "func" and sym.get("section") not in ("UND", "*ABS*", None) and \
        int(sym["addr"], 16) != 0


def qualified(sym):
    """Demangled name without the parameter list: 'Class::Method'."""
    d = sym["demangled"]
    i = d.find("(")
    return d if i < 0 else d[:i]


def by_name(symbols, sel):
    funcs = [s for s in symbols if is_func(s)]
    exact = [s for s in funcs if sel in (s["demangled"], qualified(s), s["mangled"])]
    if exact:
        return exact
    try:
        rx = re.compile(sel)
    except re.error as e:
        raise SelectError(f"bad regex {sel!r}: {e}") from None
    return [s for s in funcs if rx.search(s["demangled"]) or rx.search(s["mangled"])]


def by_class(symbols, cls):
    pre = cls + "::"
    return [s for s in symbols if is_func(s) and (s.get("scope") == cls or s["demangled"].startswith(pre))]


def read_list(path):
    """One selector per line; blank lines and '#' lines are ignored."""
    out = []
    for line in Path(path).read_text(encoding="utf-8").splitlines():
        line = line.strip()
        if line and not line.startswith("#"):
            out.append(line)
    return out


def select(symbols, selectors=(), classes=()):
    """Sorted unique 8-digit addresses. Raises SelectError listing every selector that matched nothing."""
    addrs, empty = set(), []
    for sel in selectors:
        if HEX.match(sel):
            addrs.add(addr8(sel))
            continue
        hits = by_name(symbols, sel)
        if not hits:
            empty.append(sel)
        addrs.update(addr8(s["addr"]) for s in hits)
    for cls in classes:
        hits = by_class(symbols, cls)
        if not hits:
            empty.append(f"--class {cls}")
        addrs.update(addr8(s["addr"]) for s in hits)
    if empty:
        raise SelectError("nothing matches: " + ", ".join(empty))
    return sorted(addrs)


# ---- index ----------------------------------------------------------------------------------------------------

def read_index(out_dir):
    """{addr8: (name, size, plan_sha256, format)}; rows of an old index count as format 1."""
    p = Path(out_dir) / INDEX
    rows = {}
    if not p.is_file():
        return rows
    for line in p.read_text(encoding="utf-8").splitlines()[1:]:
        f = line.split("\t")
        if len(f) in (4, 5):
            rows[f[0]] = (f[1], f[2], f[3], f[4] if len(f) == 5 else "1")
    return rows


def write_index(out_dir, rows):
    p = Path(out_dir) / INDEX
    p.parent.mkdir(parents=True, exist_ok=True)
    tmp = p.with_name(p.name + ".tmp")
    text = INDEX_HEADER + "\n" + "".join(f"{a}\t{n}\t{s}\t{h}\t{v}\n" for a, (n, s, h, v) in sorted(rows.items()))
    tmp.write_text(text, encoding="utf-8", newline="\n")
    os.replace(tmp, p)


def is_fresh(out_dir, index, addr, applied_sha):
    """A dump is fresh when its file exists and was taken at the plan the project currently holds."""
    row = index.get(addr)
    return bool(applied_sha) and row is not None and row[2] == applied_sha and row[3] == FORMAT and \
        (Path(out_dir) / f"{addr}.txt").is_file()


def split_fresh(out_dir, addrs, applied_sha, force=False):
    """(todo, fresh) address lists."""
    if force:
        return list(addrs), []
    index = read_index(out_dir)
    todo, fresh = [], []
    for a in addrs:
        (fresh if is_fresh(out_dir, index, a, applied_sha) else todo).append(a)
    return todo, fresh


def symbol_sizes(symbols):
    """{addr8: largest symbol size} of the function symbols: the range the script disassembles."""
    sizes = {}
    for s in symbols:
        if is_func(s) and s.get("size"):
            a = addr8(s["addr"])
            sizes[a] = max(sizes.get(a, 0), int(s["size"]))
    return sizes


def write_request(path, addrs, sizes=None):
    """One line per function: `0xADDR` or `0xADDR 0xSYMBOL_SIZE`."""
    path = Path(path)
    path.parent.mkdir(parents=True, exist_ok=True)
    sizes = sizes or {}
    path.write_text("".join(f"0x{a} 0x{sizes[a]:x}\n" if sizes.get(a) else f"0x{a}\n" for a in addrs),
                    encoding="utf-8", newline="\n")


def read_run(out_dir):
    """Rows the script wrote: [(addr8, name, size_hex, status)]."""
    p = Path(out_dir) / RUN_FILE
    if not p.is_file():
        return None
    rows = []
    for line in p.read_text(encoding="utf-8").splitlines():
        f = line.split("\t")
        if len(f) == 4:
            rows.append((f[0], f[1], f[2], f[3]))
    return rows


def merge_run(out_dir, run_rows, applied_sha):
    """Adds the dumped functions to the index (size in hex). Returns (dumped, missing address list)."""
    index = read_index(out_dir)
    dumped, missing = 0, []
    for addr, name, size, status in run_rows:
        if status != "ok":
            missing.append(addr)
            index.pop(addr, None)
            continue
        index[addr] = (name, f"0x{int(size):x}", applied_sha or NO_PLAN, FORMAT)
        dumped += 1
    write_index(out_dir, index)
    return dumped, missing
