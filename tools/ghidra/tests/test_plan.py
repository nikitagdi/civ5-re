import unittest

from ghidra import plan as pm
from ghidra.tests.helpers import SHA, build, by_id, fixture, funcs_by_addr

R = "/civ5re/"


def field(t, name):
    return next(f for f in t["fields"] if f["name"] == name)


class Names(unittest.TestCase):
    def test_normalization(self):
        self.assertEqual(pm.norm_name("Foo<int, char>"), "Foo<int,char>")
        self.assertEqual(pm.norm_name("Foo<Bar<int> >"), "Foo<Bar<int>>")
        self.assertEqual(pm.norm_name("Foo<A<B<int> > >"), "Foo<A<B<int>>>")
        self.assertEqual(pm.norm_name("Foo<const char *>"), "Foo<const_char*>")
        self.assertEqual(pm.norm_name("Foo<int &>"), "Foo<int&>")
        self.assertEqual(pm.norm_name("operator new"), "operator_new")
        self.assertEqual(pm.norm_type_name("operator/"), "operator_")

    def test_long_name_truncated_with_hash(self):
        long = "A" * 2500
        n = pm.norm_name(long)
        self.assertEqual(len(n), 2000)
        self.assertEqual(n, pm.norm_name(long))
        self.assertNotEqual(n, pm.norm_name("A" * 2400))

    def test_split_qualified_ignores_template_args(self):
        self.assertEqual(pm.split_qualified("A::B<x::y, z>::C"), ["A", "B<x::y, z>", "C"])
        self.assertEqual(pm.split_qualified(""), [])
        self.assertEqual(pm.type_path("A::B<int, char>::C"), "/civ5re/A/B<int,char>/C")

    def test_stale_threshold(self):
        self.assertEqual(pm.stale_threshold(10), 50)
        self.assertEqual(pm.stale_threshold(2500), 50)
        self.assertEqual(pm.stale_threshold(10000), 200)


class Fixture(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.plan = build()
        cls.types = by_id(cls.plan)
        cls.funcs = funcs_by_addr(cls.plan)

    def f(self, addr):
        return self.funcs["0x%08x" % addr]

    def test_valid_and_deterministic(self):
        self.assertEqual(pm.validate(self.plan), [])
        self.assertEqual(self.plan, build())
        self.assertEqual(self.plan["conflicts"], [])
        self.assertEqual(self.plan["header"]["binary_sha256"], SHA)
        self.assertEqual(self.plan["header"]["script_version"], pm.SCRIPT_VERSION)

    def test_plan_sha_follows_content(self):
        t, b, fl = fixture()
        t["records"][0]["size_marker"] = "?"
        self.assertNotEqual(build(t, b, fl)["header"]["plan_sha256"], self.plan["header"]["plan_sha256"])

    def test_scopes_class_and_namespace(self):
        scopes = {tuple(s["path"]): s for s in self.plan["scopes"]}
        self.assertEqual(scopes[("Base",)]["kind"], "class")
        self.assertEqual(scopes[("Base",)]["type"], R + "Base")
        self.assertEqual(scopes[("Util",)]["kind"], "namespace")
        self.assertIsNone(scopes[("Util",)]["type"])
        self.assertEqual(scopes[("FFastVector<int>",)]["kind"], "class")

    def test_scope_parents_come_first(self):
        t, b, fl = fixture()
        for f in t["functions"]:
            if f["name"] == "CopyNT":
                f["scope"] = "Outer::Inner"
                f["qualified"] = "Outer::Inner::CopyNT"
        paths = [tuple(s["path"]) for s in build(t, b, fl)["scopes"]]
        self.assertLess(paths.index(("Outer",)), paths.index(("Outer", "Inner")))

    def test_primary_base_is_expanded_and_empty_base_has_no_field(self):
        d = self.types[R + "Derived"]
        self.assertEqual(d["size"], 20)
        self.assertEqual([(f["off"], f["name"]) for f in d["fields"]],
                         [(0, "_vptr"), (4, "m_iCount"), (8, "m_iExtra"), (12, "m_pos")])
        self.assertEqual(field(d, "_vptr")["type"], {"p": {"t": R + "Derived_vtbl"}})
        self.assertEqual(field(d, "m_pos")["type"], {"t": R + "Vec2"})
        self.assertNotIn("Empty", [f["name"] for f in d["fields"]])

    def test_secondary_base_in_tail_padding_is_expanded_with_prefix(self):
        m = self.types[R + "Mix"]
        self.assertEqual([(f["off"], f["name"]) for f in m["fields"]],
                         [(0, "_vptr"), (4, "m_iCount"), (8, "TailBase_m_iCount"), (12, "m_c"), (13, "m_cMix")])
        self.assertNotIn("TailBase", [f["name"] for f in m["fields"]])

    def test_secondary_base_without_overlap_is_a_field(self):
        e = self.types[R + "Embed"]
        self.assertEqual(field(e, "Side"), {"off": 8, "name": "Side", "type": {"t": R + "Side"},
                                            "comment": None, "bit": None})
        m = self.types[R + "Multi"]
        self.assertEqual(field(m, "Iface")["type"], {"t": R + "Iface"})
        self.assertEqual(field(m, "Iface")["off"], 8)

    def test_size_unknown_keeps_clang_layout_with_description(self):
        o = self.types[R + "Opaque"]
        self.assertEqual((o["size"], o["desc"], len(o["fields"])), (12, "size ?", 2))
        self.assertIsNone(self.types[R + "Derived"]["desc"])

    def test_incomplete_record_is_an_empty_structure(self):
        f = self.types[R + "Fwd"]
        self.assertEqual((f["size"], f["fields"], f["desc"]), (0, [], "incomplete"))

    def test_bit_fields(self):
        t = self.types[R + "Bits"]
        a, b, c = field(t, "m_a"), field(t, "m_b"), field(t, "m_c")
        self.assertEqual((a["off"], a["bit"]), (0, {"size": 3, "shift": 0}))
        self.assertEqual((b["off"], b["bit"]), (0, {"size": 5, "shift": 3}))
        self.assertEqual((c["off"], c["bit"]), (4, {"size": 1, "shift": 0}))
        self.assertEqual(a["type"], {"b": "uint"})

    def test_types_pod(self):
        p = self.types[R + "Pod"]
        self.assertEqual(field(p, "m_bFlag")["type"], {"t": R + "BOOL"})
        self.assertEqual(self.types[R + "BOOL"], {"id": R + "BOOL", "kind": "typedef", "to": {"b": "int"}})
        self.assertEqual(field(p, "m_pmf")["type"], {"t": R + "_pmf"})
        self.assertEqual(self.types[R + "_pmf"]["size"], 8)
        self.assertEqual(field(p, "unk_18")["type"], {"a": {"b": "undefined1"}, "n": 8})
        self.assertEqual(field(p, "m_name")["type"], {"a": {"b": "char"}, "n": 4})
        cb = self.types[R + "Callback"]["to"]
        fd = self.types[cb["p"]["t"]]
        self.assertEqual((fd["kind"], fd["ret"], fd["cc"]), ("funcdef", {"b": "void"}, "__cdecl"))
        self.assertTrue(fd["id"].startswith(R + "_fn/"))
        e = self.types[R + "Mode"]
        self.assertEqual(e["values"], [["MODE_NONE", -1], ["MODE_A", 0], ["MODE_B", 1]])

    def test_only_reachable_third_party_types(self):
        self.assertIn(R + "ThirdRef", self.types)
        self.assertNotIn(R + "ThirdUnused", self.types)

    def test_template_instantiation(self):
        v = self.types[R + "FFastVector<int>"]
        self.assertEqual(v["size"], 8)
        self.assertEqual(v["desc"], None)
        f = self.f(0x08000800)
        self.assertEqual((f["scope"], f["name"], f["cc"]), (["FFastVector<int>"], "push_back", "__thiscall"))
        self.assertEqual(f["params"][0]["type"], {"p": {"b": "int"}})
        self.assertEqual(f["this_type"], {"p": {"t": R + "FFastVector<int>"}})

    def test_vtable_with_inherited_slots(self):
        vt = self.types[R + "Derived_vtbl"]
        self.assertEqual([(f["off"], f["name"]) for f in vt["fields"]],
                         [(0, "dtor"), (4, "dtor_deleting"), (8, "Update"), (12, "Extra"), (16, "Scale"),
                          (20, "vf_05")])
        self.assertEqual(vt["size"], 24)
        extra = self.types[field(vt, "Extra")["type"]["p"]["t"]]
        self.assertEqual(extra["id"], R + "Derived_vfn/Extra")
        self.assertEqual(extra["params"][0], {"name": "this", "type": {"p": {"t": R + "Derived"}}})
        self.assertEqual([p["name"] for p in extra["params"]], ["this", "a", "b"])
        self.assertEqual(extra["ret"], {"b": "int"})
        self.assertEqual(extra["cc"], "__cdecl")

    def test_secondary_vtable_group_with_thunk(self):
        vt = self.types[R + "Multi_vtbl_Iface"]
        self.assertEqual([f["name"] for f in vt["fields"]], ["OnEvent"])
        fd = self.types[vt["fields"][0]["type"]["p"]["t"]]
        self.assertEqual(fd["params"][0]["type"], {"p": {"t": R + "Iface"}})
        main = self.types[R + "Multi_vtbl"]
        self.assertEqual(len(main["fields"]), 5)
        thunk = self.f(0x08000410)
        self.assertEqual((thunk["kind"], thunk["name"], thunk["scope"], thunk["cc"]),
                         ("thunk", "thunk_OnEvent", ["Multi"], "__cdecl"))
        self.assertEqual(thunk["params"][0], {"name": "this", "type": {"p": {"t": R + "Iface"}}, "storage": None})
        self.assertEqual(thunk["params"][1]["name"], "code")

    def test_vtable_data_at_address_points(self):
        data = {d["addr"]: d for d in self.plan["data"]}
        self.assertEqual(data["0x0a000208"]["name"], "Derived_vtbl")
        self.assertEqual(data["0x0a000208"]["scope"], ["Derived"])
        self.assertEqual(data["0x0a000208"]["type"], {"t": R + "Derived_vtbl"})
        self.assertEqual(data["0x0a000208"]["size"], 24)
        self.assertEqual(data["0x0a000330"]["name"], "Multi_vtbl_Iface")
        self.assertIn("0x0a000108", data)

    def test_vtable_question_mark_gives_type_only(self):
        self.assertIn(R + "Mix_vtbl", self.types)
        self.assertNotIn("Mix_vtbl", [d["name"] for d in self.plan["data"]])

    def test_pure_and_absent_and_inline_are_not_applied(self):
        names = {(tuple(f["scope"]), f["name"]) for f in self.plan["functions"]}
        self.assertNotIn((("Iface",), "OnEvent"), names)
        self.assertNotIn((("Derived",), "GetCount"), names)
        self.assertNotIn((("Util",), "Missing"), names)
        self.assertIn(R + "Iface_vfn/OnEvent", self.types)
        self.assertEqual(self.types[R + "Iface_vtbl"]["size"], 4)

    def test_method_is_thiscall_without_this_in_plan(self):
        f = self.f(0x08000240)
        self.assertEqual((f["cc"], f["custom"], f["auto_retptr"]), ("__thiscall", False, False))
        self.assertEqual([p["name"] for p in f["params"]], ["dt"])
        self.assertEqual(f["this_type"], {"p": {"t": R + "Derived"}})
        self.assertEqual(f["ret"], {"b": "int"})

    def test_record_return_is_cdecl_with_explicit_this(self):
        f = self.f(0x08000260)
        self.assertEqual((f["cc"], f["auto_retptr"], f["custom"]), ("__cdecl", True, False))
        self.assertEqual(f["params"], [{"name": "this", "type": {"p": {"t": R + "Derived"}}, "storage": None}])
        self.assertEqual(f["ret"], {"t": R + "Vec2"})
        self.assertIsNone(f["this_type"])

    def test_references_and_non_trivial_values_become_pointers(self):
        f = self.f(0x08000600)
        self.assertEqual(f["cc"], "__cdecl")
        self.assertEqual([p["type"] for p in f["params"]],
                         [{"p": {"t": R + "NT"}}, {"p": {"b": "int"}}, {"p": {"t": R + "Vec2"}},
                          {"t": R + "Vec2"}])

    def test_unknown_return_is_undefined_and_slot_is_undefined4(self):
        f = self.f(0x08000300)
        self.assertEqual(f["ret"], {"b": "undefined"})
        self.assertEqual(f["name"], "Fun_08000300")
        fd = self.types[R + "Derived_vfn/vf_05"]
        self.assertEqual(fd["ret"], {"b": "undefined4"})

    def test_params_unknown(self):
        m = self.f(0x080002a0)
        self.assertTrue(m["partial"] and m["drop_this"])
        self.assertIsNone(m["params"])
        self.assertEqual((m["cc"], m["name"], m["scope"], m["ret"]), ("__thiscall", "vf_05", ["Derived"],
                                                                   {"b": "undefined"}))
        c = self.f(0x08000700)
        self.assertTrue(c["partial"])
        self.assertFalse(c["drop_this"])
        self.assertEqual(c["cc"], "")
        fd = self.types[R + "Derived_vfn/vf_05"]
        self.assertEqual(fd["params"], [{"name": "this", "type": {"p": {"t": R + "Derived"}}}])
        self.assertTrue(fd["varargs"])

    def test_fastcc_method_and_free_function(self):
        peek = self.f(0x08000280)
        self.assertTrue(peek["custom"])
        self.assertEqual(peek["params"], [{"name": "this", "type": {"p": {"t": R + "Derived"}},
                                           "storage": {"reg": "ECX"}}])
        self.assertEqual(peek["ret_storage"], {"reg": "EAX"})
        h = self.f(0x08000500)
        self.assertEqual([p["storage"] for p in h["params"]], [{"reg": "ECX"}, {"reg": "EDX"}])
        self.assertEqual(h["scope"], [])

    def test_fastcc_stack_float_and_record_return(self):
        t, b, fl = fixture()
        for f in t["functions"]:
            if f["key"] == "_ZL11hidden_funcP7Derivedi":
                f["params"] = [{"name": "d", "type": "Derived *"}, {"name": "x", "type": "double"},
                               {"name": "k", "type": "int"}, {"name": "j", "type": "int"}]
                f["return"] = "Vec2"
                f["return_indirect"] = True
        h = funcs_by_addr(build(t, b, fl))["0x08000500"]
        self.assertEqual([(p["name"], p["storage"]) for p in h["params"]],
                         [("__return_storage_ptr__", {"reg": "ECX"}), ("d", {"reg": "EDX"}),
                          ("x", {"stack": 4}), ("k", {"stack": 12}), ("j", {"stack": 16})])
        self.assertEqual(h["ret"], {"p": {"t": R + "Vec2"}})
        self.assertEqual(h["ret_storage"], {"reg": "EAX"})
        self.assertFalse(h["auto_retptr"])

    def test_opaque_record_return_gets_explicit_return_pointer(self):
        # Ghidra adds no hidden return pointer for a size-0 (incomplete) struct: it must be spelled out
        t, b, fl = fixture()
        for r in t["records"]:
            if r["name"] == "Vec2":
                r["complete"] = False
        for f in t["functions"]:
            if f["key"] == "_ZL11hidden_funcP7Derivedi":
                f["markers"]["fastcc"] = False
                f["return"] = "Vec2"
                f["return_indirect"] = True
        plan = build(t, b, fl)
        self.assertEqual(pm.validate(plan), [])
        h = funcs_by_addr(plan)["0x08000500"]
        self.assertTrue(h["custom"])
        self.assertFalse(h["auto_retptr"])
        self.assertEqual([(p["name"], p["storage"]) for p in h["params"]],
                         [("__return_storage_ptr__", {"stack": 4}), ("d", {"stack": 8}), ("k", {"stack": 12})])
        self.assertEqual(h["ret"], {"p": {"t": R + "Vec2"}})
        self.assertEqual(h["ret_storage"], {"reg": "EAX"})

    def test_overlapping_empty_base_wrapper_is_dropped(self):
        # libc++ __compressed_pair: two bases at +0, one of them only wraps an empty class
        t, b, fl = fixture()
        proto = [r for r in t["records"] if r["name"] == "Empty"][0]

        def rec(name, size, fields=(), bases=()):
            r = dict(proto, name=name, size=size, dsize=size, fields=list(fields),
                     bases=[{"access": "public", "name": n, "offset": 0, "virtual": False} for n in bases])
            t["records"].append(r)
            t["types"][name] = {"key": name, "kind": "record", "size": size}

        def fld(name, type_, off, size):
            return {"bit_offset": None, "bit_size": None, "line": 1, "name": name, "offset": off,
                    "offset_marker": hex(off), "size": size, "type": type_}
        rec("WrapEmpty", 1, [fld("e", "Empty", 0, 1)])
        rec("WrapInt", 4, [fld("v", "int", 0, 4)])
        rec("PairEbo", 4, bases=["WrapEmpty", "WrapInt"])
        plan = build(t, b, fl)
        self.assertEqual(pm.validate(plan), [])
        pair = by_id(plan)[R + "PairEbo"]
        self.assertEqual([(f["off"], f["name"], f["type"]) for f in pair["fields"]],
                         [(0, "WrapInt", {"t": R + "WrapInt"})])
        self.assertFalse([w for w in plan["warnings"] if w["kind"] == "field-overlap"])

    def test_unk_data_global_is_label_only(self):
        self.assertIn({"addr": "0x0a100100", "name": "g_0a100100", "scope": [], "kind": "data"},
                      self.plan["labels"])
        self.assertNotIn("0x0a100100", [d["addr"] for d in self.plan["data"]])

    def test_globals(self):
        data = {d["addr"]: d for d in self.plan["data"]}
        self.assertEqual(data["0x0a100000"]["type"], {"b": "int"})
        self.assertEqual(data["0x0a100200"]["scope"], ["Derived"])
        self.assertEqual(data["0x0a100200"]["name"], "s_iStatic")
        self.assertNotIn("kInline", [d["name"] for d in self.plan["data"]])

    def test_ctor_alias_and_destructor_variants(self):
        base = [f for f in self.plan["functions"] if f["scope"] == ["Base"] and f["name"] == "Base"]
        self.assertEqual([f["addr"] for f in base], ["0x08000100"])
        dt = sorted(f["addr"] for f in self.plan["functions"] if f["scope"] == ["Derived"] and f["name"] == "~Derived")
        self.assertEqual(dt, ["0x08000220", "0x08000224", "0x08000230"])
        self.assertEqual({f["name"] for f in self.plan["functions"] if f["addr"] in dt}, {"~Derived"})
        c2 = [f["addr"] for f in self.plan["functions"] if f["scope"] == ["Derived"] and f["name"] == "Derived"]
        self.assertEqual(sorted(c2), ["0x08000200", "0x08000210"])

    def test_one_function_per_address_sorted(self):
        addrs = [f["addr"] for f in self.plan["functions"]]
        self.assertEqual(addrs, sorted(set(addrs)))


class Templates(unittest.TestCase):
    """Patterns (types depend on template parameters) never reach Ghidra; only instantiations do."""

    def test_pattern_function_is_not_planned_but_its_instances_are(self):
        t, b, fl = fixture()
        pat = next(f for f in t["functions"] if f["key"] == "FFastVector::push_back@Fx.h:1")
        self.assertTrue(pat["instances"])
        pat.update(template=True, **{"return": None})
        pat["params"] = [{"name": "kValue", "type": None}]
        pat["markers"]["addr"] = "0x08000900"
        plan = build(t, b, fl)
        self.assertEqual(plan["conflicts"], [])
        self.assertEqual([w for w in plan["warnings"] if w["kind"] != "no-function-list"], [])
        addrs = funcs_by_addr(plan)
        self.assertNotIn("0x08000900", addrs)
        self.assertEqual(addrs["0x08000800"]["scope"], ["FFastVector<int>"])

    def test_pattern_record_is_not_a_type(self):
        t, b, fl = fixture()
        proto = next(r for r in t["records"] if r["name"] == "Pod")
        field0 = dict(proto["fields"][0], type=None, offset=None, size=None)
        t["records"].append(dict(proto, name="Boxed", template={"pattern": "Boxed", "args": None}, size=None,
                                 align=None, dsize=None, complete=True, fields=[field0], bases=[]))
        t["records"].sort(key=lambda r: r["name"])
        plan = build(t, b, fl)
        self.assertNotIn(R + "Boxed", by_id(plan))
        self.assertEqual([w for w in plan["warnings"] if w["kind"] != "no-function-list"], [])

    def test_pattern_and_alias_typedefs_are_skipped_silently(self):
        t, b, fl = fixture()
        tdef = dict(t["typedefs"][0], qualified="Boxed::value_type", type=None, template=True)
        t["typedefs"].append(tdef)
        plan = build(t, b, fl)
        self.assertNotIn(R + "Boxed/value_type", by_id(plan))
        self.assertEqual([w for w in plan["warnings"] if w["kind"] != "no-function-list"], [])

    def test_null_typedef_that_is_no_pattern_is_named_in_a_warning(self):
        t, b, fl = fixture()
        t["typedefs"].append(dict(t["typedefs"][0], qualified="Holder::Count", type=None, template=False))
        plan = build(t, b, fl)
        self.assertEqual([(w["kind"], w["key"]) for w in plan["warnings"] if w["kind"] != "no-function-list"],
                         [("typedef-unresolved", "Holder::Count")])
        self.assertNotIn(R + "Holder/Count", by_id(plan))

    def test_unexpected_null_type_has_a_key(self):
        t, b, fl = fixture()
        f = next(f for f in t["functions"] if f["key"] == "_ZN4Base6UpdateEi")
        f["params"][0]["type"] = None
        plan = build(t, b, fl)
        self.assertEqual({(w["kind"], w["key"]) for w in plan["warnings"] if w["kind"] != "no-function-list"},
                         {("type-null", "<null>")})

    def test_unresolved_instance_is_skipped_with_a_warning(self):
        t, b, fl = fixture()
        pat = next(f for f in t["functions"] if f["key"] == "FFastVector::push_back@Fx.h:1")
        pat["instances"][0]["params"] = None
        plan = build(t, b, fl)
        self.assertNotIn("0x08000800", funcs_by_addr(plan))
        self.assertEqual([w["kind"] for w in plan["warnings"] if w["kind"] != "no-function-list"],
                         ["instance-unresolved"])


class ThirdPartyByValue(unittest.TestCase):
    def _plan(self, origin, size=8):
        t, b, fl = fixture()
        for r in t["records"]:
            if r["name"] == "Vec2":
                r["origin"] = origin
                r["size"] = size
                r["complete"] = size is not None
        for f in t["functions"]:
            if f["key"] == "_ZL11hidden_funcP7Derivedi":
                f["markers"]["fastcc"] = False
                f["return"] = "Vec2"
                f["return_indirect"] = True
        return build(t, b, fl)

    def test_complete_third_party_record_returned_by_value_has_layout_and_automatic_return_pointer(self):
        plan = self._plan("third_party")
        self.assertEqual(pm.validate(plan), [])
        vec = by_id(plan)[R + "Vec2"]
        self.assertEqual((vec["size"], vec["desc"], len(vec["fields"])), (8, None, 2))
        h = funcs_by_addr(plan)["0x08000500"]
        self.assertFalse(h["custom"])
        self.assertTrue(h["auto_retptr"])
        self.assertEqual(h["ret"], {"t": R + "Vec2"})

    def test_explicit_return_pointer_only_for_a_declared_only_record(self):
        plan = self._plan("third_party", None)
        self.assertEqual(by_id(plan)[R + "Vec2"]["desc"], "incomplete")
        h = funcs_by_addr(plan)["0x08000500"]
        self.assertTrue(h["custom"])
        self.assertEqual(h["ret"], {"p": {"t": R + "Vec2"}})


class TemplateReturn(unittest.TestCase):
    def test_instance_returning_a_record_has_the_automatic_return_pointer_although_the_pattern_says_no(self):
        t, b, fl = fixture()
        for f in t["functions"]:
            if f["key"] == "_ZL11hidden_funcP7Derivedi":
                f["markers"]["fastcc"] = False
                f["return"] = "Vec2"
                f["return_indirect"] = False    # typedb computes it on the unresolved `T` of a template
        h = funcs_by_addr(build(t, b, fl))["0x08000500"]
        self.assertTrue(h["auto_retptr"])
        self.assertEqual(h["ret"], {"t": R + "Vec2"})


class SplitBodies(unittest.TestCase):
    def funcs(self):
        funcs = fixture()[2]
        funcs[0x08000800] = (0x08000805, "FUN_08000800", "-")    # call next; pop
        funcs[0x08000805] = (0x08000810, "FUN_08000805", "-")
        return funcs

    def test_symbol_longer_than_the_ghidra_body_is_a_body_repair(self):
        plan = build(funcs=self.funcs())
        self.assertEqual(plan["bodies"], [{"addr": "0x08000800", "size": 16}])
        self.assertEqual(pm.validate(plan), [])

    def test_whole_functions_need_no_repair(self):
        self.assertEqual(build()["bodies"], [])

    def test_a_cut_at_a_named_address_is_not_repaired(self):
        t, b, fl = fixture()
        fl = self.funcs()
        b["symbols"].append(dict(next(s for s in b["symbols"] if s["addr"] == "0x08000800"), addr="0x08000805",
                                 mangled="_Z5otherv", name="other", demangled="other()"))
        self.assertEqual(build(t, b, fl)["bodies"], [])

    def test_a_cut_at_a_vtable_slot_target_is_not_repaired(self):
        t, b, fl = fixture()
        b["vtables"][0]["groups"][0]["slots"].append({"kind": "func", "target": "0x08000805", "symbols": []})
        self.assertEqual(build(t, b, self.funcs())["bodies"], [])

    def test_no_function_start_at_the_end_is_not_repaired(self):
        funcs = self.funcs()
        del funcs[0x08000805]
        self.assertEqual(build(funcs=funcs)["bodies"], [])


class BaseDestructorAlias(unittest.TestCase):
    def test_derived_d2_aliasing_a_base_destructor_is_skipped_without_conflict(self):
        t, b, fl = fixture()
        for s in b["symbols"]:
            if s["addr"] == "0x08000224":
                s["mangled"] = s["name"] = s["demangled"] = "_ZN4BaseD2Ev"
        plan = build(t, b, fl)
        by = funcs_by_addr(plan)
        self.assertNotIn("0x08000224", by)
        self.assertIn("0x08000220", by)
        self.assertEqual([c for c in plan["conflicts"] if c.get("addr") == "0x08000224"], [])

    def test_a_dtor_symbol_of_the_same_class_is_not_an_alias(self):
        plan = build()
        self.assertIn("0x08000224", funcs_by_addr(plan))


class Conflicts(unittest.TestCase):
    def conflicts(self, mutate, funcs=None):
        t, b, fl = fixture()
        mutate(t, b, fl)
        return build(t, b, fl)

    def test_mangled_name_must_match_the_exported_symbol(self):
        def m(t, b, fl):
            for s in b["symbols"]:
                if s["mangled"] == "_ZN7Derived3SumEii":
                    s["mangled"] = "_ZN7Derived3SumEi"
        plan = self.conflicts(m)
        self.assertEqual([(c["kind"], c["addr"]) for c in plan["conflicts"]], [("address", "0x08000270")])
        self.assertNotIn("0x08000270", funcs_by_addr(plan))

    def test_non_exported_address_must_be_a_known_function_start(self):
        def m(t, b, fl):
            del fl[0x08000300]
        plan = self.conflicts(m)
        self.assertEqual([c["addr"] for c in plan["conflicts"]], ["0x08000300"])
        self.assertNotIn("0x08000300", funcs_by_addr(plan))

    def test_functions_eh_is_enough(self):
        def m(t, b, fl):
            del fl[0x080002a0]
        plan = self.conflicts(m)
        self.assertEqual(plan["conflicts"], [])
        self.assertIn("0x080002a0", funcs_by_addr(plan))

    def test_without_function_list_a_warning(self):
        t, b, _ = fixture()
        plan = pm.build_plan(t, b, None, {})
        self.assertEqual(plan["conflicts"], [])
        self.assertIn("no-function-list", [w["kind"] for w in plan["warnings"]])

    def test_vtable_marker_must_match_binfacts(self):
        def m(t, b, fl):
            for r in t["records"]:
                if r["name"] == "Derived":
                    r["vtable_marker"] = "0x0a000204"
        plan = self.conflicts(m)
        self.assertIn("vtable-address", [c["kind"] for c in plan["conflicts"]])
        self.assertNotIn("Derived_vtbl", [d["name"] for d in plan["data"]])

    def test_data_symbol_mismatch(self):
        def m(t, b, fl):
            for s in b["symbols"]:
                if s["mangled"] == "g_counter":
                    s["mangled"] = "g_other"
        plan = self.conflicts(m)
        self.assertEqual([c["kind"] for c in plan["conflicts"]], ["address"])
        self.assertNotIn("0x0a100000", [d["addr"] for d in plan["data"]])

    def test_typedb_with_errors_is_refused(self):
        t, b, fl = fixture()
        t["ok"] = False
        with self.assertRaises(pm.PlanError):
            pm.build_plan(t, b, fl)

    def test_shared_address_prefers_non_placeholder_name_and_keeps_label(self):
        def m(t, b, fl):
            f = dict(next(x for x in t["functions"] if x["key"] == "_ZN7Derived3SumEii"))
            f.update(key="_Z9Fun_8000270v", name="Fun_08000270", qualified="Fun_08000270", scope="",
                     scope_kind="namespace", kind="function", mangled="_Z9Fun_8000270v", params=[],
                     markers=dict(f["markers"]))
            t["functions"].append(f)
            b["symbols"].append(dict(next(s for s in b["symbols"] if s["mangled"] == "_ZN7Derived3SumEii"),
                                     mangled="_Z9Fun_8000270v"))
        plan = self.conflicts(m)
        fn = funcs_by_addr(plan)["0x08000270"]
        self.assertEqual(fn["name"], "Sum")
        self.assertIn({"addr": "0x08000270", "name": "Fun_08000270", "scope": [], "kind": "secondary"},
                      plan["labels"])

    def test_virtual_base_is_laid_out_at_its_offset(self):
        def m(t, b, fl):
            r = next(x for x in t["records"] if x["name"] == "Side")
            v = dict(r, name="VDer", size=12, bases=[{"name": "Side", "offset": 8, "virtual": True,
                                                      "access": "public"}],
                     fields=[dict(r["fields"][0], name="m_v", offset=0)], size_marker="0xc")
            t["records"].append(v)
            t["types"]["VDer"] = {"kind": "record", "key": "VDer", "size": 12}
        plan = self.conflicts(m)
        v = by_id(plan)[R + "VDer"]
        self.assertEqual([(f["off"], f["name"]) for f in v["fields"]], [(0, "m_v"), (8, "Side")])


class Validate(unittest.TestCase):
    def test_catches_dangling_type_and_hash(self):
        plan = build()
        self.assertEqual(pm.validate(plan), [])
        bad = build()
        bad["types"][0]["id"] = "/other/x"
        errs = pm.validate(bad)
        self.assertTrue(any("plan_sha256" in e for e in errs))
        bad2 = build()
        bad2["header"]["plan_sha256"] = "0" * 64
        self.assertIn("header.plan_sha256 does not match the body", pm.validate(bad2))
        with self.assertRaises(pm.PlanError):
            pm.assert_valid(bad2)

    def test_dangling_ref(self):
        plan = build()
        plan["functions"][0]["ret"] = {"t": "/civ5re/Nope"}
        plan["header"]["plan_sha256"] = pm.body_sha(pm.body_of(plan))
        self.assertTrue(any("Nope" in e for e in pm.validate(plan)))


if __name__ == "__main__":
    unittest.main()
