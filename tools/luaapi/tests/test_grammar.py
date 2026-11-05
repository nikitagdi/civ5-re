import sys
import unittest
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[2]))

from luaapi import grammar as g  # noqa: E402

VALID = {
    "function": [
        "UI.GetHeadSelectedUnit() -> Unit?",
        "ControlBase:SetHide(hide: boolean)",
        "ControlBase:SetSize(size: Vector2)",
        "print(...: any)",
        "math.clamp(v: number, lo: number, hi: number) -> number",
        "UI.Foo(a: integer, b?: string) -> integer, name: string",
        "UI.Foo(a: integer, ...: any)",
        "Event:__call(EventArgs)",
        "Event:Add(handler: fun(EventArgs)) -> Handle",
        "LuaEvent:__index(key: string) -> any",
        "A.B:C() -> table<string, integer[]>",
        "UI.Cb(f: fun(x: integer, y: integer) -> boolean, n: integer)",
        "UI.Get() -> T[]?",
        "UI.Get() -> (A|B)[]",
        "UI.Get() -> {x: number, y?: number}",
        "UI.Get(a: A|B?) -> A?|nil",
        "UI.Get(a: (fun() -> A)?) -> (A|B)?",
        "UI.Get() -> fun() -> A, B",
    ],
    "?": ["?"],
    "none": ["none"],
    "table": ["table UI", "table Database.Scripting"],
    "class": [
        "class ControlBase",
        "class ButtonControl : ButtonControlBase xml Button",
        "class Image : ControlBase xml Image, AlphaAnim",
        "class Event",
    ],
    "global": [
        "global ContextPtr: LuaContext",
        "global Controls: table<string, ControlBase>",
        "global F: fun(a: integer) -> integer, string",
        "global X: integer|nil",
    ],
    "enum": ["enum Keys", "enum PopupPriority"],
    "event": ["event Events.SerialEventUnitCreated", "event LuaEvents.Foo"],
    "push": [
        "push ()",
        "push (playerID: integer, unitID: integer, hex: Vector3)",
    ],
    "pull": ["pull (a: integer, b?: string)", "pull ()"],
    "shape": [
        "shape Vector2 {x: number, y: number}",
        "shape Opt {a?: integer}",
        "shape Nest {pos: {x: number, y: number}, tags: string[]}",
    ],
}

INVALID = [
    "",
    "UI.GetX",
    "UI.GetX(",
    "UI.GetX(a)",
    "UI.GetX(a: )",
    "UI.GetX(a: integer",
    "UI.GetX() ->",
    "UI.GetX() -> A,",
    "UI.GetX(a: integer,)",
    "UI.GetX(...: any, b: integer)",
    "UI.GetX(EventArgs, a: integer)",
    "UI.GetX(a: A?[])",
    "UI.GetX(a: A[]?[])",
    "UI.GetX(a: table<string>)",
    "UI.GetX(a: (A)",
    "UI.GetX(a: A|)",
    "UI.GetX(a: (fun() -> A, B))",
    "UI:A.B()",
    "UI.GetX() extra",
    "UI.GetX(a: 1)",
    "UI-GetX()",
    "?x",
    "none x",
    "table",
    "table 1",
    "class",
    "class A :",
    "class A : B xml",
    "class A xml B,",
    "global X",
    "global X:",
    "global X integer",
    "enum",
    "event",
    "push",
    "push a: integer",
    "push (a integer)",
    "shape V",
    "shape V {}",
    "shape V {x: number",
    "shape V x: number",
]


class Kinds(unittest.TestCase):
    def test_all_valid_lines_parse_canonically(self):
        n = 0
        for kind, lines in VALID.items():
            for line in lines:
                with self.subTest(line=line):
                    m = g.parse(line)
                    self.assertEqual(m.kind, kind)
                    self.assertTrue(g.is_canonical(line))
                    n += 1
        self.assertGreaterEqual(n, 35)

    def test_all_eleven_kinds_covered(self):
        kinds = {g.parse(l).kind for ls in VALID.values() for l in ls}
        self.assertEqual(kinds, {"function", "?", "none", "table", "class", "global", "enum", "event",
                                 "push", "pull", "shape"})

    def test_invalid_lines_raise(self):
        for line in INVALID:
            with self.subTest(line=line):
                with self.assertRaises(g.GrammarError) as cm:
                    g.parse(line, strict=False)
                self.assertGreaterEqual(cm.exception.col, 1)
                self.assertLessEqual(cm.exception.col, len(line) + 1)

    def test_at_least_sixty_lines(self):
        total = sum(len(v) for v in VALID.values()) + len(INVALID)
        self.assertGreaterEqual(total, 60)


class Function(unittest.TestCase):
    def test_dot(self):
        m = g.parse("UI.GetHeadSelectedUnit() -> Unit?")
        self.assertEqual((m.owner, m.name, m.call, m.path), ("UI", "GetHeadSelectedUnit", "dot", "UI.GetHeadSelectedUnit"))
        self.assertEqual(m.params, [])
        self.assertEqual(m.returns, [{"name": None, "type": "Unit?",
                                      "t": {"k": "nullable", "inner": {"k": "name", "name": "Unit"}}}])
        self.assertEqual([r.name for r in m.refs], ["Unit"])

    def test_method(self):
        m = g.parse("ControlBase:SetHide(hide: boolean)")
        self.assertEqual((m.owner, m.call, m.path), ("ControlBase", "method", "ControlBase:SetHide"))
        self.assertEqual(m.params, [{"name": "hide", "optional": False, "vararg": False, "type": "boolean",
                                     "t": {"k": "prim", "name": "boolean"}}])
        self.assertEqual(m.refs, ())

    def test_global(self):
        m = g.parse("print(...: any)")
        self.assertEqual((m.owner, m.call, m.path), (None, "global", "print"))
        self.assertTrue(m.params[0]["vararg"])
        self.assertEqual(m.params[0]["name"], "...")

    def test_nested_owner(self):
        m = g.parse("A.B:C()")
        self.assertEqual((m.owner, m.name, m.call), ("A.B", "C", "method"))
        m = g.parse("A.B.C()")
        self.assertEqual((m.owner, m.name, m.call), ("A.B", "C", "dot"))

    def test_metamethod(self):
        m = g.parse("Event:__call(EventArgs)")
        self.assertEqual(m.name, "__call")
        self.assertTrue(m.is_eventargs)
        self.assertEqual(m.params[0]["t"], {"k": "eventargs"})
        self.assertEqual(m.eventargs, (len("Event:__call(") + 1,))

    def test_eventargs_nested_in_fun(self):
        m = g.parse("Event:Add(h: fun(EventArgs))")
        self.assertFalse(m.is_eventargs)
        self.assertEqual(len(m.eventargs), 1)

    def test_optional_param(self):
        m = g.parse("UI.F(a: integer, b?: string)")
        self.assertEqual([p["optional"] for p in m.params], [False, True])

    def test_named_returns(self):
        m = g.parse("UI.F() -> a: integer, string")
        self.assertEqual([r["name"] for r in m.returns], ["a", None])

    def test_overloads_are_separate_lines(self):
        a = g.parse("UI.F(a: integer)")
        b = g.parse("UI.F(a: string)")
        self.assertEqual(a.path, b.path)
        self.assertNotEqual(a, b)


class Types(unittest.TestCase):
    def t(self, s):
        return g.parse_type(s)

    def test_prims_and_names(self):
        for p in g.PRIMS:
            self.assertEqual(self.t(p), {"k": "prim", "name": p})
        self.assertEqual(self.t("Unit"), {"k": "name", "name": "Unit"})

    def test_array_then_nullable(self):
        self.assertEqual(self.t("T[]?"), {"k": "nullable", "inner": {"k": "array", "elem": {"k": "name", "name": "T"}}})
        self.assertEqual(self.t("T[][]")["elem"]["k"], "array")

    def test_nullable_binds_to_term(self):
        t = self.t("A|B?")
        self.assertEqual([i["k"] for i in t["items"]], ["name", "nullable"])

    def test_map(self):
        t = self.t("table<string, integer[]>")
        self.assertEqual(t["k"], "map")
        self.assertEqual(t["key"], {"k": "prim", "name": "string"})
        self.assertEqual(t["value"]["k"], "array")
        self.assertEqual(self.t("table"), {"k": "prim", "name": "table"})

    def test_union_flattened(self):
        t = self.t("(A|B)|C")
        self.assertEqual(t["k"], "union")
        self.assertEqual(len(t["items"]), 3)
        self.assertEqual(g.format_type(t), "A|B|C")

    def test_parens(self):
        self.assertEqual(self.t("(A)"), {"k": "name", "name": "A"})
        self.assertEqual(g.format_type(self.t("(A|B)[]")), "(A|B)[]")
        self.assertEqual(g.format_type(self.t("(A|B)?")), "(A|B)?")
        self.assertEqual(g.format_type(self.t("(A?)[]")), "(A?)[]")
        self.assertEqual(g.format_type(self.t("(A?)?")), "(A?)?")
        self.assertEqual(g.format_type(self.t("(A[])[]")), "A[][]")
        self.assertEqual(g.format_type(self.t("(fun() -> A)?")), "(fun() -> A)?")
        self.assertEqual(g.format_type(self.t("fun()?")), "fun()?")

    def test_record(self):
        t = self.t("{x: number, y?: Vec}")
        self.assertEqual(t["k"], "record")
        self.assertEqual([(f["name"], f["optional"], f["type"]) for f in t["fields"]],
                         [("x", False, "number"), ("y", True, "Vec")])

    def test_fun(self):
        t = self.t("fun(a: integer, b?: string) -> boolean")
        self.assertEqual(t["k"], "fun")
        self.assertEqual(len(t["params"]), 2)
        self.assertEqual(t["returns"][0]["type"], "boolean")

    def test_nested_fun_takes_one_return(self):
        m = g.parse("UI.F(cb: fun(x: integer) -> boolean, n: integer)")
        self.assertEqual([p["name"] for p in m.params], ["cb", "n"])
        m = g.parse("UI.F() -> fun() -> A, B")
        self.assertEqual([r["type"] for r in m.returns], ["fun() -> A", "B"])

    def test_global_fun_takes_all_returns(self):
        m = g.parse("global F: fun() -> A, B")
        self.assertEqual(len(m.t["returns"]), 2)
        self.assertEqual(m.type, "fun() -> A, B")

    def test_fun_in_union_needs_parens_when_not_last(self):
        t = self.t("(fun() -> A)|nil")
        self.assertEqual(g.format_type(t), "(fun() -> A)|nil")
        self.assertEqual(g.format_type(self.t("fun()|nil")), "fun()|nil")

    def test_refs_with_columns(self):
        m = g.parse("UI.F(a: Foo, b: table<Bar, Baz[]>) -> Qux?")
        self.assertEqual([r.name for r in m.refs], ["Foo", "Bar", "Baz", "Qux"])
        for r in m.refs:
            self.assertEqual(m.text.index(r.name) + 1 if r.name != "Qux" else m.text.rindex("Qux") + 1, r.col)

    def test_class_base_is_ref(self):
        m = g.parse("class A : B xml C, D")
        self.assertEqual((m.name, m.base, m.xml), ("A", "B", ("C", "D")))
        self.assertEqual([r.name for r in m.refs], ["B"])

    def test_shape_and_global_refs(self):
        self.assertEqual([r.name for r in g.parse("shape S {a: X, b: integer}").refs], ["X"])
        self.assertEqual([r.name for r in g.parse("global G: table<string, Y>").refs], ["Y"])
        self.assertEqual([r.name for r in g.parse("push (a: Z)").refs], ["Z"])


class Kind(unittest.TestCase):
    def test_table_event_enum(self):
        self.assertEqual(g.parse("table A.B").path, "A.B")
        self.assertEqual(g.parse("event Events.X").path, "Events.X")
        self.assertEqual(g.parse("enum Keys").name, "Keys")

    def test_push_pull(self):
        m = g.parse("push (a: integer, b?: Unit)")
        self.assertEqual(m.kind, "push")
        self.assertEqual([f["name"] for f in m.fields], ["a", "b"])
        self.assertEqual(g.parse("pull ()").fields, [])

    def test_shape(self):
        m = g.parse("shape Vector2 {x: number, y: number}")
        self.assertEqual((m.name, [f["name"] for f in m.fields]), ("Vector2", ["x", "y"]))

    def test_keyword_named_function_still_a_function(self):
        m = g.parse("table.insert(t: table, v: any)")
        self.assertEqual((m.kind, m.owner, m.name), ("function", "table", "insert"))
        m = g.parse("none()")
        self.assertEqual(m.kind, "function")

    def test_unknown_and_none(self):
        self.assertEqual(g.parse("?").kind, "?")
        self.assertEqual(g.parse("none").kind, "none")

    def test_global_function_and_field_named_fun(self):
        self.assertEqual(g.parse("UI.F(fun: integer)").params[0]["name"], "fun")


class Canonical(unittest.TestCase):
    def err(self, text, col=None, frag=None):
        with self.assertRaises(g.GrammarError) as cm:
            g.parse(text)
        if col is not None:
            self.assertEqual(cm.exception.col, col, str(cm.exception))
        if frag:
            self.assertIn(frag, cm.exception.message)
        return cm.exception

    def test_missing_space_after_comma(self):
        text = "UI.F(a: integer,b: integer)"
        self.err(text, text.index("b") + 1, "needs a space after ','")
        self.assertIn(f"(column {text.index('b') + 1})", str(g.check(text)))

    def test_missing_space_after_colon(self):
        text = "UI.F(a:integer)"
        self.err(text, text.index("integer") + 1, "needs a space after ':'")

    def test_space_before_colon(self):
        text = "UI.F(a : integer)"
        self.err(text, text.index(" :") + 1, "unexpected space before ':'")

    def test_space_before_comma(self):
        text = "UI.F(a: integer , b: integer)"
        self.err(text, text.index(" ,") + 1, "unexpected space before ','")

    def test_double_space(self):
        text = "UI.F(a:  integer)"
        self.err(text, text.index("  ") + 1, "single space")

    def test_arrow_spacing(self):
        text = "UI.F()->integer"
        self.err(text, text.index("->") + 1, "needs a space after ')'")
        text = "UI.F() ->integer"
        self.err(text, text.index("integer") + 1, "needs a space after '->'")
        text = "UI.F()  -> integer"
        self.err(text, text.index("  ") + 1, "single space")

    def test_leading_trailing_space(self):
        self.err(" UI.F()", 1, "leading")
        text = "UI.F() "
        self.err(text, len(text), "trailing")
        self.err("none ", 5, "trailing")

    def test_space_around_union_and_suffix(self):
        text = "UI.F(a: A | B)"
        self.err(text, text.index(" |") + 1, "unexpected space before '|'")
        text = "UI.F(a: A[ ])"
        self.err(text, None)
        text = "UI.F(a: A ?)"
        self.err(text, text.index(" ?") + 1)

    def test_map_spacing(self):
        text = "UI.F(a: table<string,integer>)"
        self.err(text, text.index("integer") + 1, "needs a space after ','")
        text = "UI.F(a: table<string, integer >)"
        self.err(text, text.index(" >") + 1)

    def test_keyword_spacing(self):
        self.err("table  UI", 6, "single space")
        self.err("class A: B", 8, "needs a space after 'A'")
        self.err("class A : B xml C,D", 19, "needs a space after ','")
        self.err("global X : integer", 9, "unexpected space before ':'")
        self.err("global X:integer", 10, "needs a space after ':'")
        self.err("shape V{x: number}", 8, "needs a space after 'V'")
        self.err("push(a: integer)", 5, "needs a space after 'push'")
        self.err("shape V { x: number}", 10, "unexpected space before 'x'")

    def test_paren_spacing(self):
        self.err("UI.F( a: integer)", 6, "unexpected space before 'a'")
        self.err("UI.F (a: integer)", 5, "unexpected space before '('")
        self.err("UI.F(a: integer )", 16, "unexpected space before ')'")

    def test_vararg_must_be_last(self):
        text = "UI.F(...: any, b: integer)"
        self.err(text, 6, "last parameter")
        self.err("fun_marker_is_not_a_kind(...: any, b: integer)", 26)

    def test_eventargs_rules(self):
        self.err("UI.F(EventArgs, a: integer)")

    def test_lenient_parse_accepts_and_normalizes(self):
        m = g.parse("UI.F(a:integer,b : string)->  integer ,string", strict=False)
        self.assertEqual(m.text, "UI.F(a: integer, b: string) -> integer, string")

    def test_error_col_offset(self):
        e = g.check("UI.F(a:integer)", col_offset=10)
        self.assertEqual(e.col, 10 + "UI.F(a:integer)".index("integer") + 1)

    def test_unexpected_character(self):
        e = self.err("UI.F(a: integer) -> #", 21, "unexpected character")
        self.assertEqual(e.col, 21)


class Normalize(unittest.TestCase):
    CASES = [
        "UI.F(a:integer,b : string)->  integer ,string",
        "class  A:B  xml C ,D",
        "global  F :fun( a:integer )->A,B",
        "shape V {x:number,y :number}",
        "push (a:integer , b ?: Unit)",
        "UI.F( a : (A|B)[] ? )",
        "table  UI",
        "UI.F(a: ((A)))",
        "UI.F(a: (A|B)|C)",
        "UI.F(a: A | (fun() -> B) | nil)",
        "UI.F(a: (A?)?)",
        "UI.F() -> fun(x: integer) -> boolean, n: integer",
    ]

    def test_idempotent(self):
        for c in self.CASES:
            with self.subTest(c=c):
                n = g.normalize(c)
                self.assertEqual(g.normalize(n), n)
                self.assertTrue(g.is_canonical(n))
                self.assertEqual(g.parse(n), g.parse(c, strict=False))

    def test_canonical_text_unchanged(self):
        for ls in VALID.values():
            for line in ls:
                with self.subTest(line=line):
                    self.assertEqual(g.normalize(line), line)

    def test_normalization_drops_redundant_parens(self):
        self.assertEqual(g.normalize("UI.F(a: ((A)))"), "UI.F(a: A)")
        self.assertEqual(g.normalize("UI.F(a: (A|B)|C)"), "UI.F(a: A|B|C)")

    def test_roundtrip_through_format_marker(self):
        for ls in VALID.values():
            for line in ls:
                with self.subTest(line=line):
                    self.assertEqual(g.format_marker(g.parse(line)), line)


class Imports(unittest.TestCase):
    def test_no_checks_import(self):
        src = (Path(g.__file__)).read_text(encoding="utf-8")
        self.assertNotIn("checks", src.replace("# ", ""))


if __name__ == "__main__":
    unittest.main()
