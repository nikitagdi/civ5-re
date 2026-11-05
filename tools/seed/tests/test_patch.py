import unittest

from seed import patch
from seed.patch import PatchError

BASE = "a\nb\nc\nd\ne\nf\ng\nh\n"


def diff(old_path, new_path, hunks):
    return f"--- {old_path}\n+++ {new_path}\n" + hunks


class PatchTests(unittest.TestCase):
    def test_single_hunk(self):
        p = diff("a/x.h", "b/x.h", "@@ -2,3 +2,3 @@\n b\n-c\n+C\n d\n")
        out = patch.apply({"x.h": BASE}, p)
        self.assertEqual(out["x.h"], "a\nb\nC\nd\ne\nf\ng\nh\n")

    def test_two_hunks_and_line_shift(self):
        p = diff("a/x.h", "b/x.h",
                 "@@ -1,2 +1,3 @@\n a\n+a2\n b\n@@ -7,2 +8,2 @@\n g\n-h\n+H\n")
        out = patch.apply({"x.h": BASE}, p)
        self.assertEqual(out["x.h"], "a\na2\nb\nc\nd\ne\nf\ng\nH\n")

    def test_context_mismatch_is_error(self):
        p = diff("a/x.h", "b/x.h", "@@ -2,3 +2,3 @@\n b\n-X\n+C\n d\n")
        with self.assertRaises(PatchError):
            patch.apply({"x.h": BASE}, p)

    def test_no_offset_search(self):
        p = diff("a/x.h", "b/x.h", "@@ -3,3 +3,3 @@\n b\n-c\n+C\n d\n")
        with self.assertRaises(PatchError):
            patch.apply({"x.h": BASE}, p)

    def test_missing_file_is_error(self):
        p = diff("a/y.h", "b/y.h", "@@ -1,1 +1,1 @@\n-a\n+A\n")
        with self.assertRaises(PatchError):
            patch.apply({"x.h": BASE}, p)

    def test_new_file(self):
        p = diff("/dev/null", "b/n.h", "@@ -0,0 +1,2 @@\n+one\n+two\n")
        out = patch.apply({}, p)
        self.assertEqual(out["n.h"], "one\ntwo\n")

    def test_new_file_over_existing_is_error(self):
        p = diff("/dev/null", "b/x.h", "@@ -0,0 +1,1 @@\n+one\n")
        with self.assertRaises(PatchError):
            patch.apply({"x.h": BASE}, p)

    def test_insert_only_hunk(self):
        p = diff("a/x.h", "b/x.h", "@@ -2,0 +3,1 @@\n+new\n")
        out = patch.apply({"x.h": BASE}, p)
        self.assertEqual(out["x.h"].split("\n")[:4], ["a", "b", "new", "c"])

    def test_hunk_length_mismatch_is_error(self):
        p = diff("a/x.h", "b/x.h", "@@ -1,3 +1,3 @@\n a\n-b\n+B\n")
        with self.assertRaises(PatchError):
            patch.parse(p)

    def test_input_not_modified(self):
        files = {"x.h": BASE}
        patch.apply(files, diff("a/x.h", "b/x.h", "@@ -1,1 +1,1 @@\n-a\n+A\n"))
        self.assertEqual(files["x.h"], BASE)

    def test_stats(self):
        p = diff("a/x.h", "b/x.h", "@@ -1,1 +1,1 @@\n-a\n+A\n@@ -3,1 +3,1 @@\n-c\n+C\n")
        self.assertEqual(patch.stats(p), {"files": 1, "hunks": 2})


if __name__ == "__main__":
    unittest.main()
