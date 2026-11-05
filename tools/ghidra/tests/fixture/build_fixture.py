"""Rebuild t.elf from t.cpp: python build_fixture.py [--out DIR] [--check]."""
import argparse
import hashlib
import subprocess
import sys
import tempfile
from pathlib import Path

HERE = Path(__file__).resolve().parent
sys.path.insert(0, str(HERE.parents[2]))

CXXFLAGS = ["--target=i386-pc-linux-gnu", "-O1", "-fno-pic", "-ffreestanding", "-nostdlib",
            "-fno-rtti", "-fno-exceptions", "-fasynchronous-unwind-tables", "-std=gnu++11"]


def tools():
    from civ5re import config
    cfg = config.load()
    return cfg.tool("clang++"), cfg.tool("ld.lld")


def build(out_dir):
    clang, lld = tools()
    out_dir = Path(out_dir)
    with tempfile.TemporaryDirectory() as tmp:
        tmp = Path(tmp)
        (tmp / "empty.c").write_text("", encoding="ascii")
        subprocess.run([str(clang), *CXXFLAGS, "-c", str(HERE / "t.cpp"), "-o", str(tmp / "t.o")], check=True)
        subprocess.run([str(clang), "--target=i386-pc-linux-gnu", "-x", "c", "-c", str(tmp / "empty.c"),
                        "-o", str(tmp / "empty.o")], check=True)
        subprocess.run([str(lld), "-shared", "-soname", "libempty.so", str(tmp / "empty.o"),
                        "-o", str(tmp / "libempty.so")], check=True)
        elf = out_dir / "t.elf"
        subprocess.run([str(lld), "-m", "elf_i386", "--export-dynamic", "-s", "--build-id=none",
                        "--image-base=0x08048000", str(tmp / "t.o"), str(tmp / "libempty.so"),
                        "-o", str(elf)], check=True)
    return elf


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--out", default=str(HERE))
    ap.add_argument("--check", action="store_true", help="build into a temp dir and compare with t.elf.sha256")
    a = ap.parse_args()
    if a.check:
        with tempfile.TemporaryDirectory() as tmp:
            sha = hashlib.sha256(build(tmp).read_bytes()).hexdigest()
        want = (HERE / "t.elf.sha256").read_text(encoding="ascii").split()[0]
        print(sha, "ok" if sha == want else "MISMATCH " + want)
        return 0 if sha == want else 1
    elf = build(a.out)
    sha = hashlib.sha256(elf.read_bytes()).hexdigest()
    (Path(a.out) / "t.elf.sha256").write_text(f"{sha}  t.elf\n", encoding="ascii", newline="\n")
    print(sha)
    return 0


if __name__ == "__main__":
    sys.exit(main())
