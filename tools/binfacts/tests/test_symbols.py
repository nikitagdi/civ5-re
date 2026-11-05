import unittest

from binfacts import symbols
from binfacts.api import BinfactsError


class SplitEntity(unittest.TestCase):
    def check(self, text, comps, params=None, cv=None):
        self.assertEqual(symbols.split_entity(text), (comps, params, cv), text)

    def test_method(self):
        self.check("ns::Class::method(int, char const*) const", ["ns", "Class", "method"], "int, char const*", "const")

    def test_free_function_without_params(self):
        self.check("fixture_use()", ["fixture_use"], "")

    def test_template_class_scope(self):
        self.check("std::__1::vector<int, std::__1::allocator<int>>::push_back(int const&)",
                   ["std", "__1", "vector<int, std::__1::allocator<int>>", "push_back"], "int const&")

    def test_operators(self):
        self.check("ns::Vec::operator<(ns::Vec const&) const", ["ns", "Vec", "operator<"], "ns::Vec const&", "const")
        self.check("ns::Vec::operator()(int) const", ["ns", "Vec", "operator()"], "int", "const")
        self.check("operator<<(FDataStream&, int const&)", ["operator<<"], "FDataStream&, int const&")
        self.check("Foo::operator->() const", ["Foo", "operator->"], "", "const")
        self.check("Foo::operator[](unsigned int)", ["Foo", "operator[]"], "unsigned int")

    def test_operator_new_delete_and_conversion(self):
        self.check("operator new[](unsigned int)", ["operator new[]"], "unsigned int")
        self.check("Foo::operator bool() const", ["Foo", "operator bool"], "", "const")
        self.check("Foo::operator unsigned int() const", ["Foo", "operator unsigned int"], "", "const")
        self.check("Foo::operator Bar<int>*()", ["Foo", "operator Bar<int>*"], "")

    def test_anonymous_namespace(self):
        self.check("(anonymous namespace)::helper(int)", ["(anonymous namespace)", "helper"], "int")
        self.check("ns::(anonymous namespace)::Cls::run()", ["ns", "(anonymous namespace)", "Cls", "run"], "")

    def test_statics_in_functions(self):
        self.check("ns::counter()::c", ["ns", "counter()", "c"])
        self.check("Foo::bar(int) const::x", ["Foo", "bar(int) const", "x"])

    def test_return_type_is_dropped(self):
        self.check("int ns::tmax<int>(int, int)", ["ns", "tmax<int>"], "int, int")
        self.check("std::__1::basic_string<char> ns::f<int>(int)", ["ns", "f<int>"], "int")
        self.check("unsigned int const* ns::g()", ["ns", "g"], "")

    def test_function_local_data_with_return_type(self):
        self.check("std::__1::basic_string<char> FSerialization::toString<MissionTypes>(MissionTypes const&)::result",
                   ["FSerialization", "toString<MissionTypes>(MissionTypes const&)", "result"])

    def test_function_pointer_parameter(self):
        self.check("ns::f(void (*)(int), int)", ["ns", "f"], "void (*)(int), int")

    def test_lambda_and_unnamed(self):
        self.check("ns::Outer::run()::'lambda'(int)::operator()(int) const",
                   ["ns", "Outer", "run()", "'lambda'(int)", "operator()"], "int", "const")
        self.check("ns::f()::{lambda(int)#1}::operator()(int) const",
                   ["ns", "f()", "{lambda(int)#1}", "operator()"], "int", "const")

    def test_dtor_and_abi_tag(self):
        self.check("ns::Foo::~Foo()", ["ns", "Foo", "~Foo"], "")
        self.check("ns::name[abi:cxx11]()", ["ns", "name[abi:cxx11]"], "")

    def test_strip_templates(self):
        st = symbols.strip_templates
        self.assertEqual(st("vector<FStringA, FSTL_Tagged_Allocator<FStringA, 1u>>"), "vector")
        self.assertEqual(st("operator<"), "operator<")
        self.assertEqual(st("operator<<"), "operator<<")
        self.assertEqual(st("tmax<int>"), "tmax")
        self.assertEqual(st("(anonymous namespace)"), "(anonymous namespace)")


class Variants(unittest.TestCase):
    def test_find_variant(self):
        f = symbols.find_variant
        self.assertEqual(f("_ZN2ns3FooC1Ei"), ("C1", 10))
        self.assertEqual(f("_ZN2ns3FooD0Ev"), ("D0", 10))
        self.assertEqual(f("_ZNSt3__110shared_ptrIcEC2IcvEEPT_")[0], "C2")
        self.assertIsNone(f("_ZN2ns3Foo3barEv"))
        self.assertIsNone(f("_Z3foov"))
        self.assertIsNone(f("_ZTVN2ns3FooE"))

    def test_identifier_that_looks_like_a_variant(self):
        self.assertIsNone(symbols.find_variant("_ZN4XC1E3barEv"))
        self.assertEqual(symbols.find_variant("_ZN4XC1EC2Ev")[0], "C2")
        self.assertEqual(symbols.find_variant("_ZN2ns4XD0EC1ERK4XC1E")[0], "C1")

    def test_function_type_in_template_arguments(self):
        self.assertEqual(symbols.find_variant("_ZN4FuncIFviEEC2Ev")[0], "C2")
        self.assertEqual(symbols.find_variant("_ZN4FuncIFviEED0Ev")[0], "D0")

    def test_local_class(self):
        self.assertEqual(symbols.find_variant("_ZZ3foovEN5LocalC1Ev")[0], "C1")

    def test_entity_key(self):
        p = symbols.classify("_ZN2ns3FooC1Ei", "ns::Foo::Foo(int)")
        self.assertEqual(symbols.entity_key(p), "_ZN2ns3FooC*Ei")
        q = symbols.classify("_ZN2ns3FooD2Ev", "ns::Foo::~Foo()")
        self.assertEqual(symbols.entity_key(q), "_ZN2ns3FooD*Ev")


class Classify(unittest.TestCase):
    def test_ctor_and_dtor(self):
        p = symbols.classify("_ZN2ns3FooC1Ei", "ns::Foo::Foo(int)")
        self.assertEqual((p.kind, p.variant, p.scope, p.name, p.params), ("ctor", "C1", "ns::Foo", "Foo", "int"))
        d = symbols.classify("_ZN2ns3FooD0Ev", "ns::Foo::~Foo()")
        self.assertEqual((d.kind, d.variant, d.name, d.params), ("dtor", "D0", "~Foo", ""))

    def test_template_class_ctor(self):
        p = symbols.classify("_ZNSt3__110shared_ptrIcEC2IcvEEPT_", "std::__1::shared_ptr<char>::shared_ptr<char, void>(char*)")
        self.assertEqual((p.kind, p.variant), ("ctor", "C2"))
        self.assertEqual(p.key, "std::__1::shared_ptr::shared_ptr")

    def test_method_with_cv(self):
        p = symbols.classify("_ZNK2ns3Foo3barEv", "ns::Foo::bar() const")
        self.assertEqual((p.kind, p.scope, p.name, p.cv, p.variant), ("function", "ns::Foo", "bar", "const", None))

    def test_thunks(self):
        p = symbols.classify("_ZThn8_N2ns3Mix1bEv", "non-virtual thunk to ns::Mix::b()")
        self.assertEqual((p.kind, p.scope, p.name), ("thunk", "ns::Mix", "b"))
        v = symbols.classify("_ZTv0_n12_N2ns2DVD1Ev", "virtual thunk to ns::DV::~DV()")
        self.assertEqual((v.kind, v.variant), ("thunk", None))

    def test_class_level_names(self):
        self.assertEqual(symbols.classify("_ZTVN2ns5ShapeE", "vtable for ns::Shape").kind, "vtable")
        t = symbols.classify("_ZTIN2ns5ShapeE", "typeinfo for ns::Shape")
        self.assertEqual((t.kind, t.scope, t.name, t.entity, t.params), ("typeinfo", "ns", "Shape", "ns::Shape", None))
        self.assertEqual(symbols.classify("_ZTSN2ns5ShapeE", "typeinfo name for ns::Shape").kind, "typeinfo_name")
        c = symbols.classify("_ZTCN2ns2DDE0_NS_2DVE", "construction vtable for ns::DV-in-ns::DD")
        self.assertEqual((c.kind, c.entity), ("ctor_vtable", "ns::DV"))

    def test_guard_and_local_static(self):
        g = symbols.classify("_ZGVZN2ns7counterEvE1c", "guard variable for ns::counter()::c")
        self.assertEqual((g.kind, g.scope, g.name), ("guard", "ns::counter()", "c"))
        d = symbols.classify("_ZZN2ns7counterEvE1c", "ns::counter()::c")
        self.assertEqual((d.kind, d.params), ("data", None))

    def test_c_names(self):
        p = symbols.classify("FSBAAllocate", "FSBAAllocate")
        self.assertEqual((p.kind, p.scope, p.name, p.key), ("c", "", "FSBAAllocate", "FSBAAllocate"))

    def test_static_data_member(self):
        p = symbols.classify("_ZN2ns4Stat5countE", "ns::Stat::count")
        self.assertEqual((p.kind, p.scope, p.name), ("data", "ns::Stat", "count"))

    def test_ctor_name_must_match_scope(self):
        with self.assertRaises(ValueError):
            symbols.classify("_ZN2ns3FooC1Ev", "ns::Foo::Bar()")

    def test_unsupported_special_name_is_an_error_with_a_list(self):
        with self.assertRaises(ValueError):
            symbols.classify("_ZTTN2ns3FooE", "VTT for ns::Foo")
        with self.assertRaises(BinfactsError) as cm:
            symbols.parse_all(["_ZTTN2ns3FooE"], ["VTT for ns::Foo"])
        self.assertIn("_ZTTN2ns3FooE", str(cm.exception))


if __name__ == "__main__":
    unittest.main()
