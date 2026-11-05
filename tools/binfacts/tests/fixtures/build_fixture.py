"""Rebuilds fixture.elf, fixture.elf.sha256 and expected.json from the sources in this directory."""
import hashlib
import os
import shutil
import subprocess
import sys
import tempfile
from pathlib import Path

HERE = Path(__file__).resolve().parent
TOOLS = HERE.parents[2]
if str(TOOLS) not in sys.path:
    sys.path.insert(0, str(TOOLS))

COMPILE = ["--target=i386-pc-linux-gnu", "-O1", "-ffreestanding", "-nostdlib", "-fno-exceptions", "-funwind-tables"]


def _run(cmd, **kw):
    p = subprocess.run([str(c) for c in cmd], cwd=HERE, capture_output=True, text=True, **kw)
    if p.returncode:
        raise SystemExit(f"{cmd[0]} failed:\n{p.stdout}{p.stderr}")
    return p.stdout


def tool(llvm_bin, name):
    return Path(llvm_bin) / (name + (".exe" if os.name == "nt" else ""))


def clang_version(llvm_bin):
    out = _run([tool(llvm_bin, "clang++"), "--version"])
    return out.splitlines()[0].strip()


def build(llvm_bin, out_dir):
    """Builds the fixture into out_dir; returns the path of fixture.elf."""
    out_dir = Path(out_dir)
    clang, lld = tool(llvm_bin, "clang++"), tool(llvm_bin, "ld.lld")
    stub_o, stub_so = out_dir / "stub.o", out_dir / "libstub.so"
    fixture_o, elf = out_dir / "fixture.o", out_dir / "fixture.elf"
    _run([clang, *COMPILE, "-fPIC", "-c", "stub.cpp", "-o", stub_o])
    _run([lld, "-m", "elf_i386", "-shared", "-soname=libstub.so", "-o", stub_so, stub_o])
    _run([clang, *COMPILE, "-fno-pic", "-c", "fixture.cpp", "-o", fixture_o])
    _run([lld, "-m", "elf_i386", "--export-dynamic", "--version-script", "fixture.ver", "--eh-frame-hdr", "-s",
          "-o", elf, fixture_o, stub_so])
    return elf


def sha256_of(path):
    return hashlib.sha256(Path(path).read_bytes()).hexdigest()


def write_expected(elf, llvm_bin):
    from binfacts import build as bf_build, groups, symbols
    data = Path(elf).read_bytes()
    rules = groups.load_rules(HERE / "groups.cfg")
    dem = symbols.Demangler(tool(llvm_bin, "llvm-cxxfilt"))
    facts = bf_build.extract(data, "fixture.elf", sha256_of(elf), dem, rules, "")
    facts["tool"] = {"binfacts": bf_build.MODULE_VERSION, "cxxfilt": "", "groups_cfg": ""}
    (HERE / "expected.json").write_bytes(bf_build.dumps(facts).encode("ascii"))


def main():
    from civ5re import config
    cfg = config.load()
    llvm_bin = cfg.tool("llvm-cxxfilt").parent
    with tempfile.TemporaryDirectory() as tmp:
        elf = build(llvm_bin, tmp)
        shutil.copyfile(elf, HERE / "fixture.elf")
    digest = sha256_of(HERE / "fixture.elf")
    (HERE / "fixture.elf.sha256").write_text(f"{digest}  fixture.elf\n# {clang_version(llvm_bin)}\n", encoding="utf-8")
    write_expected(HERE / "fixture.elf", llvm_bin)
    print(f"fixture.elf {(HERE / 'fixture.elf').stat().st_size} bytes, sha256 {digest}")


if __name__ == "__main__":
    main()
