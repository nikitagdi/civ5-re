import tempfile
import unittest
from pathlib import Path

from civ5re import config
from civ5re.config import ConfigError, Key


def write(root, name, text):
    (Path(root) / name).write_text(text, encoding="utf-8")


class SourcePriority(unittest.TestCase):
    def setUp(self):
        self._tmp = tempfile.TemporaryDirectory()
        self.root = Path(self._tmp.name)
        self.addCleanup(self._tmp.cleanup)

    def test_defaults(self):
        cfg = config.load(self.root, environ={})
        self.assertEqual(cfg.get("ghidra", "heap"), "16G")
        self.assertEqual(cfg.get("llvm_dir"), "")
        self.assertEqual(cfg.source("ghidra", "heap"), "default")

    def test_priority_default_shared_local_env(self):
        write(self.root, "civ5re.cfg", "[ghidra]\nheap = 8G\nmcp_port = 9000\n")
        write(self.root, "civ5re.local.cfg", "[ghidra]\nheap = 4G\n")
        env = {"CIV5RE_GHIDRA_HEAP": "2G"}
        cfg = config.load(self.root, environ=env)
        self.assertEqual(cfg.get("ghidra", "heap"), "2G")
        self.assertEqual(cfg.source("ghidra", "heap"), "env")
        self.assertEqual(cfg.get("ghidra", "mcp_port"), "9000")
        self.assertEqual(cfg.source("ghidra", "mcp_port"), "civ5re.cfg")
        cfg = config.load(self.root, environ={})
        self.assertEqual(cfg.get("ghidra", "heap"), "4G")
        self.assertEqual(cfg.source("ghidra", "heap"), "civ5re.local.cfg")

    def test_flat_keys_before_first_section(self):
        write(self.root, "civ5re.local.cfg", "bin_dir = bins\n[ghidra]\nheap = 1G\n")
        cfg = config.load(self.root, environ={})
        self.assertEqual(cfg.get("bin_dir"), "bins")
        self.assertEqual(cfg.path("bin_dir"), self.root / "bins")

    def test_absolute_path_unchanged_and_empty_is_none(self):
        absolute = str(Path(self.root).resolve())
        cfg = config.load(self.root, environ={"CIV5RE_GAME_DIR": absolute})
        self.assertEqual(cfg.path("game_dir"), Path(absolute))
        self.assertIsNone(config.load(self.root, environ={}).path("ghidra_dir"))

    def test_env_names(self):
        self.assertEqual(config.env_name("", "llvm_dir"), "CIV5RE_LLVM_DIR")
        self.assertEqual(config.env_name("lua-api", "out"), "CIV5RE_LUA_API_OUT")
        self.assertEqual(config.env_name("checks", "export_coverage"), "CIV5RE_CHECKS_EXPORT_COVERAGE")
        cfg = config.load(self.root, environ={"CIV5RE_LUA_API_OUT": "x", "CIV5RE_OWNER": "ignored"})
        self.assertEqual(cfg.get("lua-api", "out"), "x")

    def test_tool(self):
        cfg = config.load(self.root, environ={"CIV5RE_LLVM_DIR": str(self.root)})
        self.assertEqual(cfg.tool("llvm-cxxfilt").parent, self.root / "bin")
        self.assertTrue(cfg.tool("llvm-cxxfilt").name.startswith("llvm-cxxfilt"))

    def test_missing_required(self):
        cfg = config.load(self.root, environ={})
        self.assertEqual([k.name for k in cfg.missing_required()], ["ghidra_project_dir"])
        cfg = config.load(self.root, environ={"CIV5RE_GHIDRA_PROJECT_DIR": "p"})
        self.assertEqual(cfg.missing_required(), [])


class Errors(unittest.TestCase):
    def setUp(self):
        self._tmp = tempfile.TemporaryDirectory()
        self.root = Path(self._tmp.name)
        self.addCleanup(self._tmp.cleanup)

    def test_unknown_key_in_shared(self):
        write(self.root, "civ5re.cfg", "[ghidra]\nnope = 1\n")
        with self.assertRaisesRegex(ConfigError, r"civ5re\.cfg: unknown key \[ghidra\] nope"):
            config.load(self.root, environ={})

    def test_unknown_key_in_local(self):
        write(self.root, "civ5re.local.cfg", "nope = 1\n")
        with self.assertRaisesRegex(ConfigError, r"civ5re\.local\.cfg: unknown key nope"):
            config.load(self.root, environ={})

    def test_unknown_section(self):
        write(self.root, "civ5re.cfg", "[nosuch]\nheap = 1\n")
        with self.assertRaisesRegex(ConfigError, "unknown key"):
            config.load(self.root, environ={})

    def test_local_key_in_shared_names_file_and_env(self):
        write(self.root, "civ5re.cfg", "ghidra_dir = x\n")
        with self.assertRaises(ConfigError) as cm:
            config.load(self.root, environ={})
        msg = str(cm.exception)
        for part in ("civ5re.cfg", "ghidra_dir", "CIV5RE_GHIDRA_DIR"):
            self.assertIn(part, msg)

    def test_shared_key_allowed_in_local(self):
        write(self.root, "civ5re.local.cfg", "[checks]\njobs = 4\n")
        self.assertEqual(config.load(self.root, environ={}).get("checks", "jobs"), "4")

    def test_malformed_and_duplicate(self):
        write(self.root, "civ5re.cfg", "[ghidra]\nheap = 1\nheap = 2\n")
        with self.assertRaises(ConfigError):
            config.load(self.root, environ={})

    def test_invalid_values(self):
        with self.assertRaisesRegex(ConfigError, "CIV5RE_CHECKS_JOBS"):
            config.load(self.root, environ={"CIV5RE_CHECKS_JOBS": "many"})
        with self.assertRaisesRegex(ConfigError, "report, required"):
            config.load(self.root, environ={"CIV5RE_CHECKS_EXPORT_COVERAGE": "maybe"})

    def test_unknown_lookup(self):
        cfg = config.load(self.root, environ={})
        with self.assertRaises(ConfigError):
            cfg.get("nosuch")


class Collisions(unittest.TestCase):
    def test_registry_is_clean(self):
        config.check_collisions(config.KEYS)

    def test_section_key_colliding_with_flat_key(self):
        keys = (Key("", "ghidra_dir", config.LOCAL), Key("ghidra", "dir", config.LOCAL))
        with self.assertRaisesRegex(ConfigError, "CIV5RE_GHIDRA_DIR"):
            config.check_collisions(keys)

    def test_hyphen_underscore_collision(self):
        keys = (Key("lua-api", "out", config.LOCAL), Key("lua_api", "out", config.LOCAL))
        with self.assertRaises(ConfigError):
            config.check_collisions(keys)

    def test_load_runs_the_check(self):
        keys = (Key("", "a_b", config.LOCAL), Key("a", "b", config.LOCAL))
        with tempfile.TemporaryDirectory() as d:
            with self.assertRaises(ConfigError):
                config.load(d, environ={}, keys=keys)


class SetKey(unittest.TestCase):
    def setUp(self):
        self._tmp = tempfile.TemporaryDirectory()
        self.addCleanup(self._tmp.cleanup)
        self.path = Path(self._tmp.name) / "civ5re.cfg"

    def test_replaces_value_and_keeps_comments(self):
        original = (
            "# header\n"
            "\n"
            "[checks]\n"
            "# jobs comment\n"
            "jobs = 0\n"
            "# coverage comment\n"
            "export_coverage = report\n"
            "\n"
            "[ghidra]\n"
            "heap = 16G\n"
        )
        self.path.write_text(original, encoding="utf-8", newline="")
        config.set_key(self.path, "checks", "export_coverage", "required")
        self.assertEqual(self.path.read_text(encoding="utf-8"),
                         original.replace("export_coverage = report", "export_coverage = required"))

    def test_adds_missing_key_to_existing_section(self):
        self.path.write_text("[checks]\njobs = 0\n\n[ghidra]\nheap = 1G\n", encoding="utf-8", newline="")
        config.set_key(self.path, "checks", "export_coverage", "required")
        self.assertEqual(self.path.read_text(encoding="utf-8"),
                         "[checks]\njobs = 0\nexport_coverage = required\n\n[ghidra]\nheap = 1G\n")

    def test_adds_missing_section(self):
        self.path.write_text("# only a comment\n", encoding="utf-8", newline="")
        config.set_key(self.path, "checks", "jobs", "2")
        self.assertEqual(self.path.read_text(encoding="utf-8"), "# only a comment\n\n[checks]\njobs = 2\n")

    def test_flat_key_stays_before_sections(self):
        self.path.write_text("# c\n[ghidra]\nheap = 1G\n", encoding="utf-8", newline="")
        config.set_key(self.path, "", "bin_dir", "x")
        self.assertEqual(self.path.read_text(encoding="utf-8"), "# c\nbin_dir = x\n[ghidra]\nheap = 1G\n")

    def test_preserves_crlf(self):
        self.path.write_bytes(b"[checks]\r\njobs = 0\r\n")
        config.set_key(self.path, "checks", "jobs", "3")
        self.assertEqual(self.path.read_bytes(), b"[checks]\r\njobs = 3\r\n")

    def test_roundtrip_through_load(self):
        root = self.path.parent
        self.path.write_text("[checks]\n# note\njobs = 0\n", encoding="utf-8", newline="")
        config.set_key(self.path, "checks", "jobs", "7")
        self.assertEqual(config.load(root, environ={}).get("checks", "jobs"), "7")

    def test_unknown_key_rejected(self):
        with self.assertRaises(ConfigError):
            config.set_key(self.path, "checks", "nope", "1")


class Template(unittest.TestCase):
    def test_template_has_every_local_key_with_comment_and_loads(self):
        text = config.local_template()
        for k in config.KEYS:
            if k.file == config.LOCAL:
                self.assertRegex(text, rf"(?m)^# .*{k.env}\n{k.name} = ")
            else:
                self.assertNotRegex(text, rf"(?m)^{k.name} = ")
        with tempfile.TemporaryDirectory() as d:
            write(d, "civ5re.local.cfg", text)
            cfg = config.load(d, environ={})
            self.assertEqual(cfg.get("ghidra_project_dir"), "")
            self.assertEqual(cfg.get("llvm_dir"), "")


if __name__ == "__main__":
    unittest.main()
