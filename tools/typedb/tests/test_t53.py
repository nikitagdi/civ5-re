"""Probe naming for template instances, SPEC for nested classes and operator templates, pure destructors."""
import unittest

from typedb import markers as mk
from typedb import probes, schema

from .util import built, needs_llvm


def _rec(db, name):
    return next(r for r in db["records"] if r["name"] == name)


def _fn(db, qualified):
    return next(f for f in db["functions"] if f["qualified"] == qualified)


def _inst(f, spec):
    return next(i for i in f["instances"] if i["spec"] == spec)


class SpecText(unittest.TestCase):
    def test_nested_spec_parses(self):
        self.assertEqual(mk.parse_body("size 0x8 CvEvent<int>::Handle"), ("size", "0x8", False, "CvEvent<int>::Handle"))
        self.assertEqual(mk.parse_body("0x0a000100 fastcc ns::A<int, char>::B::C")[3], "ns::A<int,char>::B::C")

    def test_nested_spec_must_follow_a_template_id(self):
        for bad in ("size 0x8 CvEvent::Handle", "size 0x8 CvEvent<int>::", "size 0x8 CvEvent<int> Handle",
                    "size 0x8 CvEvent<int>::Handle<int>x"):
            with self.assertRaises(mk._Bad, msg=bad):
                mk.parse_body(bad)

    def test_operator_spec(self):
        self.assertEqual(mk.parse_body("0x0a000100 operator<< <int>")[3], "operator<< <int>")
        self.assertEqual(mk.parse_body("0x0a000100 operator<<<int>")[3], "operator<< <int>")
        self.assertEqual(mk.parse_body("0x0a000100 operator< <int>")[3], "operator< <int>")
        self.assertEqual(mk.parse_body("0x0a000100 operator>><int>")[3], "operator>><int>")
        self.assertEqual(mk.parse_body("0x0a000100 FOut::operator()<int>")[3], "FOut::operator()<int>")
        self.assertEqual(mk.parse_body("0x0a000100 Foo<A<int>>::operator-> <int>")[3], "Foo<A<int>>::operator-><int>")

    def test_operator_spec_unbalanced(self):
        with self.assertRaises(mk._Bad):
            mk.parse_body("0x0a000100 operator<< <int")

    def test_split_top_ignores_arguments_and_operators(self):
        self.assertEqual(mk.split_top("A<B::C, D<E::F>>::g<H::I>", "::"), ["A<B::C, D<E::F>>", "g<H::I>"])
        self.assertEqual(mk.split_top("Foo<int>::operator<< <X::Y>", "::"), ["Foo<int>", "operator<< <X::Y>"])
        self.assertEqual(probes.last_component("ns::Foo<std::vector<int>>::Bar<X::Y>"), "Bar<X::Y>")
        self.assertEqual(mk.strip_args("operator<< <int>"), "operator<<")
        self.assertEqual(mk.strip_args("Foo<int>"), "Foo")


@needs_llvm
class ProbeOverloads(unittest.TestCase):
    """Member and namespace function templates with overloads, `::` and pointers in their arguments."""

    @classmethod
    def setUpClass(cls):
        cls.path, cls.db = built("probes-overload")

    def test_no_diagnostics(self):
        self.assertEqual(self.db["diagnostics"], [])
        self.assertEqual(schema.validate(self.db), [])

    def test_member_overloads(self):
        fs = [f for f in self.db["functions"] if f["qualified"] == "FXml::SetVal"]
        mangled = sorted(i["mangled"] for f in fs for i in f["instances"])
        self.assertEqual(mangled, [
            "_ZN4FXml6SetValIKcEEbPS1_jPT_",
            "_ZN4FXml6SetValIN2ns3Key6MethodEEEbPKcjT_",
            "_ZN4FXml6SetValIN2ns4PairINS1_3Key6MethodEiEEEEbPKcjT_",
            "_ZN4FXml6SetValIPcEEbPKcjT_"])
        put = [f for f in self.db["functions"] if f["qualified"] == "FXml::Put"]
        self.assertEqual(sorted(i["mangled"] for f in put for i in f["instances"]),
                         ["_ZN4FXml3PutIPcEEvRT_i", "_ZN4FXml3PutIiEEvRKT_"])

    def test_member_instance_signature(self):
        (f,) = [f for f in self.db["functions"] if f["qualified"] == "FXml::SetVal" and
                any(i["spec"] == "FXml::SetVal<char*>" for i in f["instances"])]
        i = _inst(f, "FXml::SetVal<char*>")
        self.assertEqual((i["return"], [p["type"] for p in i["params"]]),
                         ("bool", ["const char *", "unsigned int", "char *"]))

    def test_same_spec_on_two_overloads(self):
        got = sorted(i["mangled"] for f in self.db["functions"] if f["qualified"] == "Each" for i in f["instances"])
        self.assertEqual(got, ["_Z4EachIN2ns3Key6MethodEEvR4FXmlRT_",
                               "_Z4EachIN2ns3Key6MethodEEvRNS0_4PairIiiEERT_"])

    def test_const_ref_of_pointer_argument(self):
        (f,) = [f for f in self.db["functions"] if f["qualified"] == "ns::toStr" and
                any(i["spec"] == "ns::toStr<char*>" for i in f["instances"])]
        i = _inst(f, "ns::toStr<char*>")
        self.assertEqual([p["type"] for p in i["params"]], ["char *const &"])
        self.assertEqual(i["mangled"], "_ZN2ns5toStrIPcEEiRKT_")


@needs_llvm
class EnumTemplateArguments(unittest.TestCase):
    """A non-type enum argument is named with its cast; a bare integer in a SPEC is accepted."""

    @classmethod
    def setUpClass(cls):
        cls.path, cls.db = built("enum-arg")

    def test_clean(self):
        self.assertEqual(self.db["diagnostics"], [])
        self.assertEqual(schema.validate(self.db), [])

    def test_canonical_names(self):
        names = {r["name"] for r in self.db["records"]}
        self.assertTrue({"FArr<char,(eMPoolType)3>", "FArr<int,(eMPoolType)3>", "FArr<short,(eMPoolType)3>"} <= names)
        f = _fn(self.db, "FArr::Add")
        self.assertEqual(sorted(i["spec"] for i in f["instances"]),
                         ["FArr<char,(eMPoolType)3>", "FArr<short,(eMPoolType)3>"])


@needs_llvm
class NestedInTemplate(unittest.TestCase):
    """`/// size 0x8 CvEvent<int>::Handle`: SPEC ends with the nested classes down to the entity's class."""

    @classmethod
    def setUpClass(cls):
        cls.path, cls.db = built("nested-template")

    def test_clean(self):
        self.assertEqual(self.db["diagnostics"], [])
        self.assertEqual(schema.validate(self.db), [])

    def test_nested_pattern_and_instances(self):
        pat = _rec(self.db, "CvEvent::Handle")
        self.assertEqual(pat["instance_markers"], [
            {"kind": "size", "spec": "CvEvent<char>::Handle", "value": "0x8"},
            {"kind": "size", "spec": "CvEvent<int>::Handle", "value": "0x8"}])
        h = _rec(self.db, "CvEvent<int>::Handle")
        self.assertEqual((h["size"], h["template"]["pattern"], h["template"]["args"]), (8, "CvEvent::Handle", ["int"]))
        self.assertEqual([(f["name"], f["offset"]) for f in h["fields"]], [("m_value", 0), ("m_tag", 4)])
        t = _rec(self.db, "CvEvent<int>::Handle::Tag")
        self.assertEqual((t["size"], t["size_marker"]), (1, "0x1"))

    def test_methods_of_nested_class(self):
        f = _fn(self.db, "CvEvent::Handle::Open")
        self.assertEqual({i["spec"]: (i["addr"], i["mangled"]) for i in f["instances"]}, {
            "CvEvent<int>::Handle": ("0x0a000100", "_ZN7CvEventIiE6Handle4OpenEv"),
            "CvEvent<char>::Handle": ("0x0a000110", "_ZN7CvEventIcE6Handle4OpenEv")})
        self.assertEqual(sorted(i["spec"] for i in _fn(self.db, "CvEvent::Fire")["instances"]),
                         ["CvEvent<char>", "CvEvent<int>"])

    def test_dynamic_nested_class(self):
        s = _rec(self.db, "CvEvent<int>::Sink")
        self.assertTrue(s["dynamic"])
        self.assertEqual([x["kind"] for x in s["vtable"]["groups"][0]["slots"]], ["D1", "D0", "pure"])
        d = _fn(self.db, "CvEvent::Sink::~Sink")
        (i,) = d["instances"]
        self.assertEqual((i["spec"], i["addr"], i["deleting"]), ("CvEvent<int>::Sink", "0x0a000200", "0x0a000210"))


@needs_llvm
class NestedSpecErrors(unittest.TestCase):
    def test_spec_must_name_the_class_of_the_entity(self):
        _p, db = built("nested-template-bad")
        codes = {(d["code"], d["line"]) for d in db["diagnostics"]}
        for line in (8, 11, 15):
            self.assertIn(("marker-misplaced", line), codes)


@needs_llvm
class OperatorTemplates(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.path, cls.db = built("operator-template")

    def test_clean(self):
        self.assertEqual(self.db["diagnostics"], [])
        self.assertEqual(schema.validate(self.db), [])

    def test_namespace_operators(self):
        got = {i["spec"]: i["mangled"] for f in self.db["functions"] if f["qualified"].startswith("operator")
               for i in f["instances"]}
        self.assertEqual(got, {
            "operator<< <int>": "_ZlsIiER7FStreamS1_RKT_",
            "operator<< <char>": "_ZlsIcER7FStreamS1_RKT_",
            "operator< <int>": "_ZltIiEbRK7FStreamRKT_",
            "operator>><FStream*>": "_ZrsIP7FStreamERS0_S2_RT_"})

    def test_member_operators(self):
        got = {i["spec"]: (i["mangled"], [p["type"] for p in i["params"]])
               for q in ("FOut::operator<<", "FOut::operator()")
               for f in self.db["functions"] if f["qualified"] == q for i in f["instances"]}
        self.assertEqual(got["FOut::operator<< <int>"], ("_ZN4FOutlsIiEERS_T_", ["int"]))
        self.assertEqual(got["FOut::operator()<int>"], ("_ZNK4FOutclIiEEiT_", ["int"]))


@needs_llvm
class PureDestructorWithBody(unittest.TestCase):
    """`virtual ~X() = 0;` with a body: the class's own vtable holds __cxa_pure_virtual, the destructor is a function."""

    @classmethod
    def setUpClass(cls):
        cls.path, cls.db = built("pure-dtor")

    def test_clean(self):
        self.assertEqual(self.db["diagnostics"], [])
        self.assertEqual(schema.validate(self.db), [])

    def test_own_vtable_slots_are_pure(self):
        slots = _rec(self.db, "FAutoArchive")["vtable"]["groups"][0]["slots"]
        self.assertEqual([(s["kind"], s["symbol"]) for s in slots[:3]], [("pure", "__cxa_pure_virtual")] * 3)
        self.assertEqual(slots[0]["function"], "_ZN12FAutoArchiveD1Ev")
        self.assertEqual(slots[3]["kind"], "method")

    def test_destructor_function_keeps_its_variants(self):
        f = next(f for f in self.db["functions"] if f["key"] == "_ZN12FAutoArchiveD1Ev")
        self.assertEqual(f["variants"], {"D0": "_ZN12FAutoArchiveD0Ev", "D1": "_ZN12FAutoArchiveD1Ev",
                                         "D2": "_ZN12FAutoArchiveD2Ev"})
        self.assertEqual((f["markers"]["addr"], f["markers"]["base"], f["markers"]["deleting"], f["has_body"]),
                         ("0x0a000500", "0x0a000510", "0x0a000520", True))

    def test_derived_class_is_unchanged(self):
        slots = _rec(self.db, "FConcrete")["vtable"]["groups"][0]["slots"]
        self.assertEqual([s["kind"] for s in slots], ["D1", "D0", "method", "method"])


if __name__ == "__main__":
    unittest.main()
