"""Live tests: explicit and partial specializations, member templates (found on the Firaxis SDK)."""
import unittest

from typedb import schema

from .util import built, needs_llvm


def _rec(db, name):
    return next(r for r in db["records"] if r["name"] == name)


def _fn(db, key):
    return next(f for f in db["functions"] if f["key"] == key)


@needs_llvm
class ProbeNames(unittest.TestCase):
    """Probes name entities as clang resolves them (FEndian.h, FLuaCommon.h, FSTLBlockAllocator.h)."""

    @classmethod
    def setUpClass(cls):
        cls.path, cls.db = built("probes")

    def test_no_probe_failed(self):
        self.assertEqual(self.db["diagnostics"], [])
        self.assertEqual(schema.validate(self.db), [])

    def test_member_explicit_specialization_out_of_class(self):
        f = _fn(self.db, "_ZN7FEndian10SwapEndianIsEEvPT_NS_11EEndiannessE")
        self.assertEqual((f["qualified"], f["scope"], f["scope_kind"], f["kind"]),
                         ("FEndian::SwapEndian", "FEndian", "class", "method"))
        self.assertEqual([p["type"] for p in f["params"]], ["short *", "FEndian::EEndianness"])
        self.assertTrue(f["static"])
        self.assertTrue(f["markers"]["inline"])
        c = _fn(self.db, "_ZNK7FEndian5CountIcEEiT_NS_11EEndiannessE")
        self.assertEqual((c["qualified"], c["static"], c["this"], c["return"]),
                         ("FEndian::Count", False, "const FEndian *", "int"))
        self.assertEqual([p["type"] for p in c["params"]], ["char", "FEndian::EEndianness"])

    def test_namespace_explicit_specializations(self):
        for key in ("_ZN4FLua7DetailsL12DescribeTypeIbEEPKcv", "_ZN4FLua7DetailsL12DescribeTypeIKbEEPKcv"):
            f = _fn(self.db, key)
            self.assertEqual((f["qualified"], f["scope_kind"], f["return"], f["params"]),
                             ("FLua::Details::DescribeType", "namespace", "const char *", []))
            self.assertEqual(f["linkage"], "internal")

    def test_template_static_member_definition(self):
        gs = [g for g in self.db["globals"] if g["qualified"].endswith("s_pFreeStack")]
        self.assertEqual([g["qualified"] for g in gs], ["FBlockAllocator::s_pFreeStack"])

    def test_template_static_member_instances(self):
        (g,) = [g for g in self.db["globals"] if g["qualified"] == "FBlockAllocator::s_pFreeStack"]
        self.assertEqual(g["instances"], [
            {"spec": "FBlockAllocator<char,8u,0u>", "addr": "0x0a000004",
             "mangled": "_ZN15FBlockAllocatorIcLj8ELj0EE12s_pFreeStackE"},
            {"spec": "FBlockAllocator<int,4u,0u>", "addr": "0x0a000000",
             "mangled": "_ZN15FBlockAllocatorIiLj4ELj0EE12s_pFreeStackE"}])
        self.assertEqual((g["addr_marker"], g["linkage"], g["redecls"]),
                         (None, "external", [{"file": g["file"], "line": 56}]))


@needs_llvm
class PartialSpecializations(unittest.TestCase):
    """A primary template and its partial specializations are distinct records (FAutoVariable, LuaAnalog)."""

    @classmethod
    def setUpClass(cls):
        cls.path, cls.db = built("partial")

    def test_clean(self):
        self.assertEqual(self.db["diagnostics"], [])
        self.assertEqual(schema.validate(self.db), [])

    def test_own_records(self):
        prim = _rec(self.db, "FAutoVariable")
        part = _rec(self.db, "FAutoVariable<ElementType*,ClassContainer>")
        self.assertEqual((prim["line"], prim["dynamic"], prim["size_marker"], prim["vtable_marker"]),
                         (7, True, "?", "?"))
        self.assertEqual((part["line"], part["dynamic"], part["size_marker"], part["vtable_marker"]),
                         (19, False, "0x8", None))
        self.assertEqual(part["template"], {"pattern": "FAutoVariable<ElementType*,ClassContainer>", "args": None,
                                            "partial_of": "FAutoVariable"})
        self.assertEqual([(f["name"], f["offset_marker"]) for f in part["fields"]],
                         [("m_pData", "0x0"), ("m_iCount", "0x4")])
        self.assertEqual([r["name"] for r in self.db["records"] if r["name"].startswith("LuaAnalog")],
                         ["LuaAnalog", "LuaAnalog<T&>", "LuaAnalog<const T>", "LuaAnalog<int&>"])

    def test_instantiations_follow_their_pattern(self):
        a = _rec(self.db, "FAutoVariable<int,char>")
        b = _rec(self.db, "FAutoVariable<int*,char>")
        self.assertEqual((a["template"]["pattern"], a["size"], a["size_marker"], a["vtable_symbol"]),
                         ("FAutoVariable", 8, "?", "_ZTV13FAutoVariableIicE"))
        self.assertEqual((b["template"]["pattern"], b["size"], b["size_marker"], b["vtable"]),
                         ("FAutoVariable<ElementType*,ClassContainer>", 8, "0x8", None))
        self.assertEqual({f["name"]: f["offset"] for f in b["fields"]}, {"m_pData": 0, "m_iCount": 4})
        self.assertEqual(_rec(self.db, "LuaAnalog<int&>")["template"]["pattern"], "LuaAnalog<T&>")

    def test_members_are_functions(self):
        quals = {f["qualified"] for f in self.db["functions"]}
        self.assertIn("FAutoVariable<ElementType*,ClassContainer>::size", quals)
        self.assertIn("FAutoVariable::get", quals)
        self.assertIn("FDebugHelper::Plain", quals)


@needs_llvm
class MemberTemplateDefinitions(unittest.TestCase):
    """Member function templates of a class template carry SPEC `Class<CArgs>::name<MArgs>` (CvLuaMethodWrapper.h)."""

    def test_spec_on_the_declaration(self):
        _p, db = built("member-template")
        self.assertEqual(db["diagnostics"], [])
        got = {i["spec"]: (i["addr"], i["mangled"]) for f in db["functions"]
               if f["qualified"] == "CvLuaMethodWrapper::BasicLuaMethod" for i in f["instances"]}
        self.assertEqual(got, {
            "CvLuaMethodWrapper<CvLuaUnit,CvUnit>::BasicLuaMethod<int>": (
                "0x0a000100", "_ZN18CvLuaMethodWrapperI9CvLuaUnit6CvUnitE14BasicLuaMethodIiEEiiMS1_KFT_vE"),
            "CvLuaMethodWrapper<CvLuaUnit,CvUnit>::BasicLuaMethod<int,int>": (
                "0x0a000110", "_ZN18CvLuaMethodWrapperI9CvLuaUnit6CvUnitE14BasicLuaMethodIiiEEiiMS1_KFT_T0_E")})
        push = {i["spec"]: i["mangled"] for f in db["functions"] if f["qualified"] == "CvLuaMethodWrapper::Push"
                for i in f["instances"]}
        self.assertEqual(sorted(push.values()), [
            "_ZN18CvLuaMethodWrapperI9CvLuaUnit6CvUnitE4PushIPcEEvRKT_",
            "_ZN18CvLuaMethodWrapperI9CvLuaUnit6CvUnitE4PushIsEEvRKT_"])

    def test_member_class_template_marker_is_unsupported(self):
        _p, db = built("marker-unsupported")
        self.assertEqual(sorted((d["code"], d["line"]) for d in db["diagnostics"]), [("marker-unsupported", 8)])


if __name__ == "__main__":
    unittest.main()
