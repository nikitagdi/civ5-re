"""Synthetic marked-up tree for manual speed measurement (default: 1,400 headers, ~148k lines).

    .venv\\Scripts\\python tools/typedb/tests/gen_tree.py OUT_DIR [--headers 1400] [--cpp 20]
    .venv\\Scripts\\python tools/typedb/tests/gen_tree.py OUT_DIR --measure   # generate, full run, cached run

OUT_DIR/Civ5Expansion2/ is the tree; the fixture third_party is used by --measure.
"""
import argparse
import sys
import time
from pathlib import Path

RECORDS_PER_HEADER = 2
METHODS = 12
FIELDS = 8


class Addr:
    def __init__(self):
        self.n = 0x08100000

    def __call__(self):
        self.n += 0x40
        return f"0x{self.n:08x}"


def header(i, addr):
    lines = ["/// origin guess", "#pragma once", "#include <vector>"]
    if i > 0:
        lines.append(f'#include "Gen/M{(i - 1) // 100:02d}/H{i - 1:04d}.h"')
    lines.append("")
    lines.append(f"namespace Gen{i % 7} {{")
    for r in range(RECORDS_PER_HEADER):
        name = f"C{i:04d}_{r}"
        dynamic = r == 0
        base = f" : public ::Gen{(i - 1) % 7}::C{i - 1:04d}_0" if dynamic and i % 10 else ""
        lines.append("")
        lines.append("// Synthetic record.")
        lines.append("/// size ?")
        if dynamic:
            lines.append("/// vtable ?")
        lines.append(f"class {name}{base} {{")
        lines.append("public:")
        lines.append(f"    /// {addr()}")
        lines.append(f"    /// base {addr()}")
        lines.append(f"    {name}();")
        if dynamic:
            lines.append(f"    /// {addr()}")
            lines.append(f"    /// deleting {addr()}")
            lines.append(f"    virtual ~{name}();")
        for m in range(METHODS):
            lines.append(f"    /// {addr()}")
            virt = "virtual " if dynamic and m % 3 == 0 else ""
            const = " const" if m % 2 else ""
            lines.append(f"    {virt}int Method{m}(int a1, const {name}& a2, float a3){const};")
        lines.append("    /// inline")
        lines.append(f"    int Inline() const {{ return m_i0 + m_i1; }}")
        lines.append("private:")
        off = 4 if dynamic else 0
        for f in range(FIELDS):
            lines.append(f"    int m_i{f};  ///< +0x{off:x}")
            off += 4
        lines.append(f"    std::vector<int> m_v;  ///< +0x{off:x}")
        lines.append("};")
    lines.append("")
    lines.append(f"/// {addr()}")
    lines.append(f"int Free{i:04d}(int a1, Gen{i % 7}::C{i:04d}_1* a2);")
    lines.append("")
    lines.append(f"}}  // namespace Gen{i % 7}")
    return "\n".join(lines) + "\n"


def cpp(i, addr_of):
    name = f"C{i:04d}_1"
    return "\n".join([
        "/// origin guess",
        f'#include "Gen/M{i // 100:02d}/H{i:04d}.h"',
        "",
        f"namespace Gen{i % 7} {{",
        "",
        f"/// {addr_of}",
        f"int {name}::Method0(int a1, const {name}& a2, float a3) {{ return a1 + int(a3); }}",
        "",
        "}",
    ]) + "\n"


def generate(out: Path, headers=1400, cpps=20):
    tree = out / "Civ5Expansion2"
    addr = Addr()
    method0 = {}
    for i in range(headers):
        d = tree / "Gen" / f"M{i // 100:02d}"
        d.mkdir(parents=True, exist_ok=True)
        before = addr.n
        text = header(i, addr)
        # Method0 of record 1: after record 0 and the ctor/base lines of record 1.
        for line_no, line in enumerate(text.split("\n")):
            if f"Method0(int a1, const C{i:04d}_1&" in line:
                method0[i] = text.split("\n")[line_no - 1].split("/// ")[1]
        (d / f"H{i:04d}.h").write_text(text, encoding="utf-8", newline="\n")
        del before
    for i in range(min(cpps, headers)):
        d = tree / "Gen" / f"M{i // 100:02d}"
        (d / f"S{i:04d}.cpp").write_text(cpp(i, method0[i]), encoding="utf-8", newline="\n")
    return tree


def measure(out: Path):
    sys.path.insert(0, str(Path(__file__).resolve().parents[2]))
    from typedb.build import Build, build_db
    tp = Path(__file__).resolve().parent / "fixtures" / "third_party"
    t = time.perf_counter()
    b = Build(out / "Civ5Expansion2", out / "build", third_party=tp, force=True)
    p = b.run()
    full = time.perf_counter() - t
    print("phases", getattr(b, "timings", {}))
    t = time.perf_counter()
    build_db(out / "Civ5Expansion2", out / "build", third_party=tp)
    cached = time.perf_counter() - t
    import json
    db = json.loads(p.read_text(encoding="utf-8"))
    print(f"full {full:.2f} s, cached {cached:.2f} s, "
          f"json {p.stat().st_size / 2**20:.1f} MB, entities {db['stats']['entities']}, "
          f"diagnostics {len(db['diagnostics'])}")


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("out")
    ap.add_argument("--headers", type=int, default=1400)
    ap.add_argument("--cpp", type=int, default=20)
    ap.add_argument("--measure", action="store_true")
    a = ap.parse_args()
    out = Path(a.out)
    tree = generate(out, a.headers, a.cpp)
    n = sum(p.read_text(encoding="utf-8").count("\n") for p in tree.rglob("*.h"))
    print(f"{tree}: {a.headers} headers, {n} lines")
    if a.measure:
        measure(out)


if __name__ == "__main__":
    main()
