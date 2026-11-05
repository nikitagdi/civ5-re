"""The patched SDK headers compile with MSVC (x86), as the Win32 SDK users build them.

Every `.h` is compiled alone with `cl /Zs` after msvc_prelude.h (what the game's PCH provides), using MSVC's own
standard library, not third_party. A header that fails also on the unpatched SDK (steps 1-4 only) is not a
regression of sdk_port.patch. Skipped when Visual Studio, the Lekmod checkout or third_party/lua is missing.

Exploration: python -m seed.tests.test_msvc [patched|unpatched]   (run from tools/)
"""
import glob
import os
import shutil
import subprocess
import sys
import tempfile
import unittest
from concurrent.futures import ThreadPoolExecutor
from pathlib import Path

from seed import inputs, sdk

LEKMOD = inputs.default_lekmod_dir()
HAVE_LEKMOD = LEKMOD is not None and (LEKMOD / ".git").exists() and shutil.which("git") is not None
HERE = Path(__file__).resolve().parent
PRELUDE = HERE / "msvc_prelude.h"
LUA = HERE.parents[2] / "third_party" / "lua-5.1.4" / "include"
VS_GLOB = r"C:\Program Files*\Microsoft Visual Studio\*\*\VC\Auxiliary\Build\vcvarsall.bat"


def find_vcvarsall():
    """vcvarsall.bat of the newest installed Visual Studio, or None."""
    if os.name != "nt":
        return None
    found = sorted(glob.glob(VS_GLOB))
    return Path(found[-1]) if found else None


def msvc_env(vcvarsall, arch="x86"):
    """The environment `vcvarsall <arch>` sets up (cl.exe, INCLUDE, LIB), or None."""
    r = subprocess.run(f'cmd /c ""{vcvarsall}" {arch} >nul && set"', capture_output=True, text=True, shell=False)
    if r.returncode != 0:
        return None
    env = dict(os.environ)
    for line in r.stdout.splitlines():
        k, sep, v = line.partition("=")
        if sep and k:
            env[k] = v
    return env if shutil.which("cl", path=env.get("PATH")) else None


def compile_alone(env, tree, headers, work, jobs=8):
    """cl /Zs on `#include "<header>"` stubs; returns {header: (returncode, output)}."""
    work = Path(work)
    work.mkdir(parents=True, exist_ok=True)
    cl = shutil.which("cl", path=env["PATH"])

    def one(dest):
        stub = work / (dest.replace("/", "_") + ".cpp")
        stub.write_text(f'#include "{dest}"\n', encoding="utf-8", newline="\n")
        cmd = [cl, "/nologo", "/Zs", "/TP", "/EHsc", "/W0", "/FI", str(PRELUDE), "/DWIN32", "/D_WINPC",
               "/DFINAL_RELEASE", "/DCV_WORLD_BUILDER_MAP_IN_GAME=1", "/I", str(tree), "/I", str(LUA), str(stub)]
        r = subprocess.run(cmd, capture_output=True, text=True, env=env, cwd=work)
        return dest, (r.returncode, r.stdout + r.stderr)
    with ThreadPoolExecutor(jobs) as ex:
        return dict(ex.map(one, headers))


def check_sdk(env, files, work):
    """Writes `files` ({dest: text}) under work/tree and compiles every .h; returns {header: (rc, output)}."""
    tree = Path(work) / "tree"
    sdk.write_sdk(files, tree)
    headers = sorted(d for d in files if d.endswith(".h"))
    return compile_alone(env, tree, headers, Path(work) / "stubs")


def errors(output, limit=6):
    rows = [ln for ln in output.splitlines() if " error " in ln]
    return rows[:limit]


@unittest.skipUnless(find_vcvarsall(), "Visual Studio not found")
@unittest.skipUnless(HAVE_LEKMOD, "Lekmod checkout not available")
@unittest.skipUnless((LUA / "lua.h").is_file(), "third_party/lua-5.1.4 not generated (run seed env)")
class MsvcTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.env = msvc_env(find_vcvarsall())
        if cls.env is None:
            raise unittest.SkipTest("vcvarsall x86 failed")
        cls.tmp = tempfile.mkdtemp(prefix="civ5re-msvc-")
        cls.patched = check_sdk(cls.env, sdk.build_sdk(LEKMOD), Path(cls.tmp) / "patched")
        cls.original = None

    @classmethod
    def tearDownClass(cls):
        shutil.rmtree(cls.tmp, ignore_errors=True)

    def test_every_header_compiles_with_msvc_x86(self):
        self.assertEqual(len(self.patched), 94)
        failed = {h: r for h, r in self.patched.items() if r[0] != 0}
        if failed:
            type(self).original = check_sdk(self.env, sdk.stage(LEKMOD), Path(self.tmp) / "original")
        regress = {h: r for h, r in failed.items() if self.original[h][0] == 0}
        pre = sorted(h for h in failed if h not in regress)
        msg = "\n".join(f"{h}:\n  " + "\n  ".join(errors(r[1])) for h, r in sorted(regress.items()))
        self.assertEqual(regress, {}, "MSVC regressions of sdk_port.patch:\n" + msg
                         + (f"\n(also failing unpatched: {pre})" if pre else ""))


def main(argv):
    mode = argv[1] if len(argv) > 1 else "patched"
    env = msvc_env(find_vcvarsall())
    files = sdk.build_sdk(LEKMOD) if mode == "patched" else sdk.stage(LEKMOD)
    with tempfile.TemporaryDirectory(prefix="civ5re-msvc-") as tmp:
        res = check_sdk(env, files, tmp)
    bad = {h: r for h, r in res.items() if r[0] != 0}
    for h, r in sorted(bad.items()):
        print(h)
        for ln in errors(r[1], 4):
            print("   ", ln)
    print(f"{mode}: {len(res) - len(bad)} of {len(res)} headers pass")
    return 1 if bad else 0


if __name__ == "__main__":
    sys.exit(main(sys.argv))
