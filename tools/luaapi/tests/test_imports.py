import subprocess
import sys
import unittest
from pathlib import Path

TOOLS = Path(__file__).resolve().parents[2]


class NoChecks(unittest.TestCase):
    def test_core_modules_do_not_import_checks(self):
        code = ("import sys; sys.path.insert(0, %r); "
                "import luaapi.grammar, luaapi.model, luaapi.entries, luaapi.flua, luaapi.usage, "
                "luaapi.emit_json, luaapi.emit_stubs; "
                "bad = [m for m in sys.modules if m == 'checks' or m.startswith('checks.')]; "
                "sys.exit(1 if bad else 0)") % str(TOOLS)
        r = subprocess.run([sys.executable, "-I", "-c", code], capture_output=True, text=True)
        self.assertEqual(r.returncode, 0, r.stderr)

    def test_only_checkrules_names_checks(self):
        for p in sorted((TOOLS / "luaapi").glob("*.py")):
            text = p.read_text(encoding="utf-8")
            imports = [x for x in text.splitlines() if x.startswith(("import checks", "from checks"))]
            self.assertEqual(bool(imports), p.name == "checkrules.py", p.name)


if __name__ == "__main__":
    unittest.main()
