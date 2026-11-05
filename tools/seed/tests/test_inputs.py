import os
import shutil
import subprocess
import tempfile
import unittest
from concurrent.futures import ThreadPoolExecutor
from pathlib import Path

from seed import inputs
from seed.inputs import InputsError

LEKMOD = inputs.default_lekmod_dir()
HAVE_LEKMOD = LEKMOD is not None and (LEKMOD / ".git").exists() and shutil.which("git") is not None


def git(cwd, *args):
    subprocess.run(["git", "-C", str(cwd), *args], check=True, capture_output=True)


class LockFormatTests(unittest.TestCase):
    def test_roundtrip(self):
        lock = inputs.load_lock()
        again = inputs.parse_lock(inputs.format_lock(lock))
        self.assertEqual(again.commits, lock.commits)
        self.assertEqual(sorted(again.trees), sorted(lock.trees))
        self.assertEqual(again.files, lock.files)

    def test_spec_tree_hashes(self):
        lock = inputs.load_lock()
        got = {p.rsplit("/", 2)[-2] if p.endswith("/include") else p.rsplit("/", 1)[-1]: h[:8] for _, p, h in lock.trees}
        self.assertEqual(got["CvGameCoreDLLUtil"], "017ae238")
        self.assertEqual(got["FirePlace"], "6b7307dd")
        self.assertEqual(got["CvGameDatabase"], "59f03026")
        self.assertEqual(got["CvLocalization"], "37b51bb1")
        self.assertEqual(got["CvWorldBuilderMap"], "dbf41a5a")
        self.assertEqual(got["Lua51"], "2c961846")
        self.assertEqual(got["Win32"], "0bec44e8")

    def test_bad_line_is_error(self):
        with self.assertRaises(InputsError):
            inputs.parse_lock("tree\tonly-two\n")

    def test_evidence_hashes_match_files(self):
        lock = inputs.load_lock()
        for rel in inputs.evidence_paths():
            self.assertEqual(lock.files[rel], inputs.file_sha256(inputs.ROOT / rel), rel)

    def test_text_sha256_ignores_crlf(self):
        self.assertEqual(inputs.text_sha256(b"a\r\nb\n"), inputs.text_sha256(b"a\nb\n"))

    def test_git_wrapper_is_read_only(self):
        with self.assertRaises(InputsError):
            inputs.git(".", "checkout", "HEAD")
        with self.assertRaises(InputsError):
            inputs.git(".", "reset")


class SdkMapTests(unittest.TestCase):
    def test_counts(self):
        entries = inputs.sdk_entries()
        self.assertEqual(len(entries), 98)
        self.assertEqual(inputs.origin_counts(entries), {"binary": 67, "guess": 31})

    def test_destinations_are_unique_ignoring_case(self):
        dests = [e.dest.lower() for e in inputs.sdk_entries()]
        self.assertEqual(len(dests), len(set(dests)))

    def test_per_library_counts(self):
        counts = {}
        for e in inputs.sdk_entries():
            key = e.dest.rsplit("/", 1)[0]
            counts[key] = counts.get(key, 0) + 1
        self.assertEqual(counts["FirePlace/FireWorks"], 49 + 0)
        self.assertEqual(counts["FirePlace/FireWorks/EventSystem"], 1)
        self.assertEqual(counts["FirePlace/FireWorks/FLua/Include"], 8)
        self.assertEqual(counts["FirePlace/FireWorks/Win32"], 3)
        self.assertEqual(counts["Civ5/Src/App/CvGameCoreDLLUtil"], 20)
        self.assertEqual(counts["Civ5/Src/App/CvWorldBuilderMap"], 6)
        self.assertEqual(counts["Civ5/Src/CvGameDatabase"], 8)
        self.assertEqual(counts["Civ5/Src/CvLocalization"], 3)

    def test_origin_matches_evidence(self):
        d = inputs.DATA
        proven = set()
        for line in (d / "source_paths_unique.txt").read_text(encoding="utf-8").splitlines():
            p, tag = line.split("\t")
            if tag != "P" and "<unresolved>" not in p:
                proven.add(p.lower().removeprefix("root/"))
        for line in (d / "sdk_lib_paths.txt").read_text(encoding="utf-8").splitlines():
            if not line.startswith("#"):
                proven.add(line.split("\t")[0])
        for e in inputs.sdk_entries():
            self.assertEqual(e.origin == "binary", e.dest.lower() in proven, e.dest)

    def test_database_and_localization_are_guess(self):
        for e in inputs.sdk_entries():
            if e.dest.startswith(("Civ5/Src/CvGameDatabase/", "Civ5/Src/CvLocalization/")):
                self.assertEqual(e.origin, "guess")


class CompatTests(unittest.TestCase):
    def test_templates_exist(self):
        files = inputs.compat_files()
        self.assertEqual(sorted(files), ["compat/Windows.h", "compat/civ5re_prelude.h", "compat/gnu/stubs-32.h",
                                         "compat/hash_map", "compat/hash_set", "compat/sqlite3.h", "compat/tbb.h"])
        self.assertIn("class hash_compare", files["compat/hash_map"])
        self.assertIn("Pred comp;", files["compat/hash_map"])
        self.assertEqual(files["compat/gnu/stubs-32.h"], "")

    def test_prelude_requirements(self):
        text = inputs.compat_template("civ5re_prelude.h.in")
        for needle in ("typedef void UNK_RET;", "typedef unsigned char UNK_DATA;", "#define UNK_PARAMS ...",
                       "#define WIN32 1", "#define _WINPC 1", "#define FINAL_RELEASE 1"):
            self.assertIn(needle, text)
        for name in ("__stdcall", "__cdecl", "__fastcall", "__thiscall", "WINAPI", "CALLBACK"):
            self.assertRegex(text, rf"(?m)^#define {name}$")
        self.assertNotRegex(text, r"(?m)^#define (_WIN32|_MSC_VER)\b")

    def test_templates_are_not_headers(self):
        for p in (inputs.DATA / "compat").rglob("*"):
            if p.is_file():
                self.assertTrue(p.name.endswith(".h.in"), p)


@unittest.skipUnless(shutil.which("git"), "git not available")
class TempRepoTests(unittest.TestCase):
    def setUp(self):
        self.tmp = Path(tempfile.mkdtemp())
        self.repo = self.tmp / "repo"
        self.repo.mkdir()
        git(self.repo, "init", "-q")
        git(self.repo, "config", "user.email", "t@example.com")
        git(self.repo, "config", "user.name", "t")
        (self.repo / "d").mkdir()
        (self.repo / "d" / "f.h").write_bytes(b"int x;\r\n")
        git(self.repo, "add", ".")
        git(self.repo, "commit", "-qm", "c")
        self.commit = subprocess.run(["git", "-C", str(self.repo), "rev-parse", "HEAD"], capture_output=True, text=True).stdout.strip()
        self.tree = subprocess.run(["git", "-C", str(self.repo), "rev-parse", "HEAD:d"], capture_output=True, text=True).stdout.strip()
        self.evidence = self.tmp / "e.txt"
        self.evidence.write_text("x\n", encoding="utf-8", newline="\n")

    def tearDown(self):
        shutil.rmtree(self.tmp, ignore_errors=True)

    def lock(self, **over):
        lock = inputs.Lock({"sdk": self.commit}, [("sdk", "d", self.tree)], {"e.txt": inputs.file_sha256(self.evidence)})
        for k, v in over.items():
            setattr(lock, k, v)
        return lock

    def test_ok(self):
        inputs.verify(self.repo, self.tmp, self.lock())

    def test_tree_mismatch(self):
        lock = self.lock(trees=[("sdk", "d", "0" * 40)])
        with self.assertRaises(InputsError) as cm:
            inputs.verify(self.repo, self.tmp, lock)
        self.assertIn("tree d", str(cm.exception))
        self.assertEqual(cm.exception.exit_code, 2)

    def test_commit_mismatch(self):
        lock = self.lock(commits={"sdk": "1" * 40})
        with self.assertRaises(InputsError):
            inputs.verify(self.repo, self.tmp, lock)

    def test_evidence_mismatch_and_missing(self):
        with self.assertRaises(InputsError):
            inputs.verify(self.repo, self.tmp, self.lock(files={"e.txt": "0" * 64}))
        with self.assertRaises(InputsError):
            inputs.verify(self.repo, self.tmp, self.lock(files={"gone.txt": "0" * 64}))

    def test_third_party_checked_only_when_present(self):
        lock = self.lock(files={"third_party/a.h": "0" * 64})
        inputs.verify(self.repo, self.tmp, lock, third_party_dir=self.tmp / "third_party")
        (self.tmp / "third_party").mkdir()
        (self.tmp / "third_party" / "a.h").write_text("x\n", encoding="utf-8")
        with self.assertRaises(InputsError):
            inputs.verify(self.repo, self.tmp, lock, third_party_dir=self.tmp / "third_party")

    def test_read_blob_and_list(self):
        self.assertEqual(inputs.read_blob(self.repo, self.commit, "d/f.h"), b"int x;\r\n")
        self.assertEqual(inputs.list_files(self.repo, self.commit, "d"), ["d/f.h"])


@unittest.skipUnless(HAVE_LEKMOD, "Lekmod checkout not available")
class LekmodTests(unittest.TestCase):
    def test_locked_inputs_verify(self):
        inputs.verify(LEKMOD)

    def test_compute_lock_equals_stored(self):
        stored = inputs.load_lock()
        fresh = inputs.compute_lock(LEKMOD)
        self.assertEqual(fresh.commits, stored.commits)
        self.assertEqual(sorted(fresh.trees), sorted(stored.trees))
        self.assertEqual(fresh.files, {k: v for k, v in stored.files.items() if not k.startswith("third_party/")})

    def test_sdk_map_matches_git_objects(self):
        lock = inputs.load_lock()
        entries = inputs.sdk_entries()
        with ThreadPoolExecutor(8) as ex:
            blobs = list(ex.map(lambda e: inputs.read_blob(LEKMOD, lock.commits[e.ref], e.src), entries))
        for e, blob in zip(entries, blobs):
            self.assertTrue(blob, e.src)

    def test_sdk_map_covers_every_header_in_the_trees(self):
        lock = inputs.load_lock()
        mapped = {(e.ref, e.src) for e in inputs.sdk_entries()}
        found = set()
        for ref, path, _ in lock.trees:
            if "Lua51" in path:
                continue
            for p in inputs.list_files(LEKMOD, lock.commits[ref], path):
                found.add((ref, p))
        self.assertEqual(mapped, found)

    def test_lua_files(self):
        names = [Path(p).name for p in inputs.lua_files(LEKMOD)]
        self.assertEqual(names, ["lauxlib.h", "lua.h", "lua.hpp", "luaconf.h", "lualib.h"])

    def test_verify_exit_code_for_wrong_dir(self):
        with self.assertRaises(InputsError):
            inputs.verify(Path(tempfile.gettempdir()) / "no-such-lekmod")


if __name__ == "__main__":
    unittest.main()
