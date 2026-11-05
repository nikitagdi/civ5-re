import hashlib
import tempfile
import unittest
from pathlib import Path

from civ5re import binlock

REPO = Path(__file__).resolve().parents[3]


class Binlock(unittest.TestCase):
    def setUp(self):
        self._tmp = tempfile.TemporaryDirectory()
        self.addCleanup(self._tmp.cleanup)
        self.dir = Path(self._tmp.name)

    def entry(self, data):
        return binlock.Entry("sub/file.bin", "target", hashlib.sha256(data).hexdigest(), len(data), "test")

    def test_repo_lock_has_the_target(self):
        t = binlock.target(binlock.load(REPO / "bins.lock"))
        self.assertEqual(t.path, "linux_1.0.3.279/Civ5XP")
        self.assertEqual(t.size, 36536148)
        self.assertEqual(t.sha256, "cc06b647821ec5e7cca3c397f6b0d4726f0106cdd67bcf074d494bea2607a8ca")

    def test_verify_statuses(self):
        data = b"hello"
        e = self.entry(data)
        self.assertEqual(binlock.verify(e, self.dir)[0], "missing")
        (self.dir / "sub").mkdir()
        f = self.dir / "sub" / "file.bin"
        f.write_bytes(data)
        self.assertEqual(binlock.verify(e, self.dir)[0], "ok")
        f.write_bytes(b"hellO")
        status, msg = binlock.verify(e, self.dir)
        self.assertEqual(status, "mismatch")
        self.assertIn("sha256", msg)
        f.write_bytes(b"hello!")
        status, msg = binlock.verify(e, self.dir)
        self.assertEqual(status, "mismatch")
        self.assertIn("size", msg)

    def test_malformed_lock(self):
        p = self.dir / "bins.lock"
        p.write_text('[[file]]\npath = "x"\n', encoding="utf-8")
        with self.assertRaisesRegex(binlock.BinlockError, "lacks"):
            binlock.load(p)
        p.write_text("[[file", encoding="utf-8")
        with self.assertRaises(binlock.BinlockError):
            binlock.load(p)

    def test_target_must_be_unique(self):
        with self.assertRaises(binlock.BinlockError):
            binlock.target([])


if __name__ == "__main__":
    unittest.main()
