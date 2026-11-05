import unittest

from binfacts import groups, symbols
from binfacts.api import BinfactsError
from binfacts.tests.support import FIXTURES

REPO_CFG = groups.__file__.replace("groups.py", "groups.cfg")


def sym(mangled, demangled):
    return symbols.classify(mangled, demangled)


class Rules(unittest.TestCase):
    def test_first_match_wins(self):
        rules = groups.parse_rules("std cpp ^std(::|$)\nfiraxis cpp .*\n")
        got = groups.assign(rules, [sym("_ZNSt3__16vectorIiE4sizeEv", "std::__1::vector<int>::size()"),
                                    sym("_ZN2ns3Foo3barEv", "ns::Foo::bar()")])
        self.assertEqual(got, ["std", "firaxis"])

    def test_template_arguments_do_not_take_part(self):
        rules = groups.parse_rules("firaxis cpp ^FFastVector(::|$)\nstd cpp ^std(::|$)\n")
        p = sym("_ZNSt3__16vectorI11FFastVectorIiEE4sizeEv", "std::__1::vector<FFastVector<int>>::size()")
        self.assertEqual(p.key, "std::__1::vector::size")
        self.assertEqual(groups.assign(rules, [p]), ["std"])

    def test_targets(self):
        rules = groups.parse_rules("lua c ^lua_\ncrt cpp ^never\nfiraxis mangled ^_ZN2ns\nthirdparty demangled bar\n")
        self.assertEqual(groups.assign(rules, [sym("lua_gettop", "lua_gettop")]), ["lua"])
        self.assertEqual(groups.assign(rules, [sym("_ZN2ns3Foo3barEv", "ns::Foo::bar()")]), ["firaxis"])
        self.assertEqual(groups.assign(rules, [sym("_Z3barv", "bar()")]), ["thirdparty"])

    def test_c_target_does_not_match_cpp_names(self):
        rules = groups.parse_rules("lua c lua\nfiraxis cpp .*\n")
        self.assertEqual(groups.assign(rules, [sym("_Z3lua_xv", "lua_x()")]), ["firaxis"])

    def test_unclassified_is_an_error_with_a_list(self):
        rules = groups.parse_rules("std cpp ^std\n")
        with self.assertRaises(BinfactsError) as cm:
            groups.assign(rules, [sym("_ZN2ns3Foo3barEv", "ns::Foo::bar()"), sym("c_name", "c_name")])
        self.assertIn("2 symbols have no group", str(cm.exception))
        self.assertIn("_ZN2ns3Foo3barEv", str(cm.exception))
        self.assertIn("c_name", str(cm.exception))

    def test_bad_lines(self):
        for text, msg in (("nogroup cpp x\n", "unknown group"), ("std nowhere x\n", "unknown target"),
                          ("std cpp\n", "expected"), ("std cpp (\n", "bad regex")):
            with self.assertRaisesRegex(BinfactsError, msg):
                groups.parse_rules(text)

    def test_comments_and_blank_lines(self):
        self.assertEqual(len(groups.parse_rules("# c\n\n  \nstd cpp ^std # not a comment\n")), 1)

    def test_shipped_configs_parse(self):
        for path in (REPO_CFG, FIXTURES / "groups.cfg"):
            rules = groups.load_rules(path)
            self.assertTrue(rules)
            self.assertEqual(rules[-1].pattern, ".*")

    def test_shipped_config_spot_checks(self):
        rules = groups.load_rules(REPO_CFG)
        cases = [
            ("_ZNSt3__16vectorIiNS_9allocatorIiEEE9push_backERKi",
             "std::__1::vector<int, std::__1::allocator<int>>::push_back(int const&)", "std"),
            ("_ZN7icu_446UnicodeStringC1Ev", "icu_44::UnicodeString::UnicodeString()", "icu"),
            ("_ZN6granny3FooE", "granny::Foo", "granny"),
            ("_ZN15cvUnitVisSystem10InitHandleEv", "cvUnitVisSystem::InitHandle()", "firaxis"),
            ("_ZN3ASL6FooBarEv", "ASL::FooBar()", "aspyr"),
            ("_ZN4Json5ValueC1Ev", "Json::Value::Value()", "thirdparty"),
            ("GrannyGetTrackGroup", "GrannyGetTrackGroup", "granny"), ("lua_pushnil", "lua_pushnil", "lua"),
            ("SDL_Init", "SDL_Init", "thirdparty"), ("u_strlen_44", "u_strlen_44", "icu"),
            ("_init", "_init", "crt"), ("GetACP", "GetACP", "aspyr"), ("g_cvUnitSimSystem", "g_cvUnitSimSystem", "firaxis"),
        ]
        for mangled, demangled, want in cases:
            self.assertEqual(groups.assign(rules, [sym(mangled, demangled)]), [want], mangled)


if __name__ == "__main__":
    unittest.main()
