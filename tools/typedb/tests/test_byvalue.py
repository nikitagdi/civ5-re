"""Live tests: records reachable by value from signatures, class-scope typedefs, template flags (fixture byvalue)."""
import unittest

from typedb import schema

from .util import built, needs_llvm

VEC = "std::__1::vector<%s>"


def _rec(db, name):
    return next((r for r in db["records"] if r["name"] == name), None)


def _fn(db, qualified):
    return next(f for f in db["functions"] if f["qualified"] == qualified)


@needs_llvm
class ByValueRecords(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.path, cls.db = built("byvalue")

    def test_clean(self):
        self.assertEqual(self.db["diagnostics"], [])
        self.assertEqual(schema.validate(self.db), [])

    def test_returned_and_passed_by_value_are_complete_records(self):
        for arg in ("int", "char", "short", "unsigned char"):
            r = _rec(self.db, VEC % arg)
            self.assertIsNotNone(r, arg)
            self.assertEqual((r["origin"], r["size"], r["complete"]), ("third_party", 12, True), arg)
            self.assertEqual([f["name"] for f in r["fields"]], ["__begin_", "__end_", "__cap_"], arg)

    def test_reference_and_pointer_do_not_pull_the_record(self):
        for arg in ("long", "double"):
            self.assertIsNone(_rec(self.db, VEC % arg), arg)
            self.assertIsNone(self.db["types"][VEC % arg]["size"])

    def test_signatures_use_the_complete_record(self):
        self.assertEqual(_fn(self.db, "FHolder::Make")["return"], VEC % "int")
        self.assertEqual(_fn(self.db, "FreeMake")["return"], VEC % "short")
        self.assertEqual([p["type"] for p in _fn(self.db, "FreeMake")["params"]], [VEC % "unsigned char"])
        self.assertTrue(_fn(self.db, "FreeMake")["return_indirect"])


@needs_llvm
class TypedefsAndTemplates(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.path, cls.db = built("byvalue")
        cls.td = {t["qualified"]: t for t in cls.db["typedefs"]}

    def test_class_scope_typedefs_are_resolved(self):
        self.assertEqual(self.td["FHolder::Count"]["type"], "int")
        self.assertEqual(self.td["FHolder::Ints"]["type"], VEC % "int")
        self.assertFalse(self.td["FHolder::Count"]["template"])

    def test_pattern_and_alias_template_typedefs_are_flagged_null(self):
        for q in ("FBox::value_type", "FBox::Items", "FItems"):
            self.assertEqual((self.td[q]["type"], self.td[q]["template"]), (None, True), q)

    def test_function_templates_are_flagged_and_only_they_have_null_types(self):
        for q in ("FBox::Get", "FBox::Put", "FreeTemplate"):
            f = _fn(self.db, q)
            self.assertTrue(f["template"], q)
            self.assertIsNone(f["return"], q)
        for f in self.db["functions"]:
            if not f["template"]:
                self.assertIsNotNone(f["return"], f["qualified"])
                self.assertTrue(all(p["type"] is not None for p in f["params"]), f["qualified"])

    def test_pattern_record_fields_are_null(self):
        box = _rec(self.db, "FBox")
        self.assertEqual((box["template"], box["size"]), ({"pattern": "FBox", "args": None}, None))
        self.assertEqual([f["type"] for f in box["fields"]], [None])
        self.assertTrue(all(f["type"] for f in _rec(self.db, "FHolder")["fields"]))


if __name__ == "__main__":
    unittest.main()
