"""Unit tests of rules B, O, M, P (no clang): stub inputs; the fixtures run in selftest."""
import unittest
from collections import namedtuple
from pathlib import Path
from unittest import mock

from checks import clangrun
from checks.api import ERROR, WARN
from checks.rules import b_build, m_markers, o_origin, p_cpp

WORLD = Path(__file__).resolve().parent.parent / "fixtures" / "_world"
ALL_DIAG_RULES = (m_markers.M0, m_markers.M1, m_markers.M2, m_markers.M3, m_markers.M4, o_origin.O1, p_cpp.P2)

TABLE = {
    "marker-syntax": "M1", "marker-orphan": "M1", "marker-ambiguous": "M1", "marker-duplicate": "M1",
    "marker-macro": "M1", "near-marker": "M2", "marker-missing": "M3",
    "marker-misplaced": "M4", "marker-conflict": "M4", "marker-unsupported": "M4",
    "cpp-undeclared": "P2",
    "file-unreached": "M0", "probe-failed": "M0", "clang-error": "M0", "internal": "M0",
}


class Stub:
    """The part of checks.context.Context the rules touch."""

    def __init__(self, typedb=None, binfacts=None, files=None, evidence=None, scope=None):
        self.typedb, self.binfacts = typedb or {}, binfacts or {}
        self.texts = files or {}
        self._evidence = evidence or {}
        self.scope = scope
        self._cache = {}

    def in_scope(self, path):
        return self.scope is None or path in self.scope

    def scoped(self, pred=None):
        return [p for p in sorted(self.texts) if self.in_scope(p) and (pred is None or pred(p))]

    def read(self, path):
        return self.texts[path]

    def evidence(self, name):
        return self._evidence.get(name, [])

    def _lazy(self, name, loader):
        if name not in self._cache:
            self._cache[name] = loader()
        return self._cache[name]


def world_evidence():
    return {"source_paths": (WORLD / "source_paths.txt").read_text(encoding="utf-8").splitlines(),
            "sdk_lib_paths": (WORLD / "sdk_lib_paths.txt").read_text(encoding="utf-8").splitlines(),
            "sdk_map": (WORLD / "sdk_map.txt").read_text(encoding="utf-8").splitlines()}


def diag(code, file="Civ5Expansion2/Fx/A.h", line=5, message="m", severity="error"):
    return {"severity": severity, "code": code, "file": file, "line": line, "message": message}


class DiagRouting(unittest.TestCase):
    def findings(self, d, scope=None):
        ctx = Stub(typedb={"diagnostics": [d]}, scope=scope)
        return [(r.id, f) for cls in ALL_DIAG_RULES for r in [cls()] for f in r.run(ctx)]

    def test_every_typedb_code_has_exactly_one_finding(self):
        from typedb.attach import CODES
        self.assertEqual(set(CODES), set(TABLE), "the M0 table drifted from typedb's code list")
        for code, rule in TABLE.items():
            got = self.findings(diag(code))
            self.assertEqual([r for r, _ in got], [rule], code)
            f = got[0][1]
            self.assertEqual((f.path, f.line, f.code, f.severity), ("Civ5Expansion2/Fx/A.h", 5, code, ERROR))
            self.assertTrue(f.message.startswith(code + ": "), f.message)

    def test_route_matches_the_table(self):
        for code, rule in TABLE.items():
            self.assertEqual(m_markers.route(diag(code)), rule)

    def test_unknown_code_goes_to_m0(self):
        got = self.findings(diag("brand-new-code", message="text"))
        self.assertEqual([r for r, _ in got], ["M0"])
        self.assertEqual(got[0][1].message, "brand-new-code: text")

    def test_missing_origin_goes_to_o1_not_m3(self):
        got = self.findings(diag("marker-missing", line=1, message="no /// origin on line 1"))
        self.assertEqual([r for r, _ in got], ["O1"])
        self.assertEqual(got[0][1].message, "no /// origin on the first line")
        got = self.findings(diag("marker-missing", line=1, message="field m_i has no ///<"))
        self.assertEqual([r for r, _ in got], ["M3"])

    def test_warning_stays_a_warning(self):
        got = self.findings(diag("near-marker", severity="warning"))
        self.assertEqual(got[0][1].severity, WARN)

    def test_scope_filters_files(self):
        self.assertEqual(self.findings(diag("near-marker"), scope={"other.h"}), [])

    def test_findings_are_not_duplicated_across_two_diagnostics(self):
        ctx = Stub(typedb={"diagnostics": [diag("near-marker", line=3), diag("near-marker", line=3)]})
        out = m_markers.M2().run(ctx)
        self.assertEqual(len(out), 2)


class Traces(unittest.TestCase):
    def run_m6(self, text):
        ctx = Stub(files={"Civ5Expansion2/A.h": text})
        return [(f.line, f.message) for f in m_markers.M6().run(ctx)]

    def test_each_trace_kind(self):
        for tok in ("undefined4", "param_1", "iVar3", "pcVar1", "ppcVar12", "local_10", "local_res0", "LAB_0812abcd",
                    "CONCAT44", "SUB41", "in_stack_00000004", "extraout_EAX", "unaff_EBX"):
            self.assertEqual(self.run_m6(f"int x = {tok};\n"), [(1, f"decompiler trace '{tok}'")], tok)

    def test_comments_strings_and_lookalikes_are_ignored(self):
        text = ('// iVar3\n/* param_1\n LAB_0812 */ int a;\nconst char* s = "undefined4 \\" iVar3";\n'
                "char c = 'x'; int Var3; int SUBTYPE; int param_x; int local; R\"(iVar3)\";\n")
        self.assertEqual(self.run_m6(text), [])

    def test_line_numbers_survive_multiline_comments_and_raw_strings(self):
        text = '/* a\n b\n c */\nconst char* s = R"x(\niVar3\n)x";\nint iVar9;\n'
        self.assertEqual(self.run_m6(text), [(7, "decompiler trace 'iVar9'")])

    def test_one_finding_per_token_per_line(self):
        self.assertEqual(len(self.run_m6("int iVar3 = iVar3 + iVar4;\n")), 2)


class Evidence(unittest.TestCase):
    def ctx(self, files):
        return Stub(files=files, evidence=world_evidence())

    def test_proof_tags(self):
        c = self.ctx({})
        self.assertEqual(o_origin.proof(c, "Civ5/Src/App/Audio/cvAudioManager.h"), ["L"])
        self.assertEqual(o_origin.proof(c, "Civ5/Src/App/Audio/CvAudioManager.h"), [])
        self.assertEqual(o_origin.proof(c, "Civ5/Src/App/Audio/CVAUDIOMANAGER.CPP"), ["Wr"])
        self.assertEqual(o_origin.proof(c, "Civ5/Src/App/Graphics/cvUnitVisSystem.cpp"), [])
        self.assertEqual(o_origin.proof(c, "Civ5/Src/App/CvGameCoreDLLUtil/CVASSERT.H"), ["S"])
        self.assertEqual(o_origin.proof(c, "Civ5/Src/App/CvGameCoreDLLUtil"), [])

    def test_tilde_question_unresolved_sdks_and_dir_lines_do_not_prove(self):
        ev = {"source_paths": ["ROOT/A/x.cpp\tWr~", "ROOT/A/y.cpp\tL,Wr?", "ROOT/<unresolved>/z.cpp\tL", "P/a.h\tP"],
              "sdk_lib_paths": ["sdks/foo/a.h\tx\tlib\tNEW", "civ5/dir\tx\tlib\tNEW(dir)"]}
        c = Stub(evidence=ev)
        for rel in ("A/x.cpp", "A/y.cpp", "<unresolved>/z.cpp", "SDKs/foo/a.h", "civ5/dir"):
            self.assertEqual(o_origin.proof(c, rel), [] if rel != "A/y.cpp" else ["L"], rel)

    def test_o2_and_o3(self):
        b, g = "/// origin binary\n", "/// origin guess\n"
        files = {"Civ5Expansion2/Civ5/Src/App/Audio/cvAudioManager.h": b,
                 "Civ5Expansion2/Civ5/Src/App/Audio/other.h": b,
                 "Civ5Expansion2/Civ5/Src/App/Audio/CVAUDIOMANAGER.cpp": g,
                 "Civ5Expansion2/Fx/Free.h": g,
                 "Civ5Expansion2/Fx/NoOrigin.h": "#pragma once\n"}
        c = self.ctx(files)
        self.assertEqual([f.path for f in o_origin.O2().run(c)], ["Civ5Expansion2/Civ5/Src/App/Audio/other.h"])
        o3 = o_origin.O3().run(c)
        self.assertEqual([(f.path, f.message) for f in o3],
                         [("Civ5Expansion2/Civ5/Src/App/Audio/CVAUDIOMANAGER.cpp",
                           "path is in evidence (Wr) - origin binary needed")])

    def test_origin_line_must_match_exactly(self):
        c = self.ctx({"Civ5Expansion2/Fx/A.h": "/// origin guess extra\n"})
        self.assertIsNone(o_origin.origin_of(c, "Civ5Expansion2/Fx/A.h"))


class OriginGuessNames(unittest.TestCase):
    REC = namedtuple("R", "name file origin")

    def db(self, *recs, files=()):
        return {"records": [{"name": n, "file": f, "origin": o} for n, f, o in recs],
                "files": [{"path": p, "origin": "guess", "kind": p.rsplit(".", 1)[1]} for p in files]}

    def test_top_level_names(self):
        db = self.db(("ns::Foo", "a.h", "tree"), ("ns::Foo::Inner", "a.h", "tree"), ("Tpl<int>", "b.h", "tree"),
                     ("Tpl", "b.h", "tree"), ("ns::Foo::(anon union L5)", "a.h", "tree"), ("std::vector<int>", "v", "third_party"))
        self.assertEqual(o_origin.top_level_names(db), {"a.h": {"foo"}, "b.h": {"tpl"}})

    def test_rule(self):
        T = "Civ5Expansion2/"
        db = self.db(("Foo", T + "Foo.h", "tree"), ("Bar", T + "Baz.h", "tree"), ("Q", T + "Q.h", "tree"),
                     files=[T + "Foo.h", T + "Foo.cpp", T + "Baz.h", T + "Baz.cpp", T + "Q.h", T + "Lone.cpp"])
        ctx = Stub(typedb=db, evidence={"sdk_map": ["# c", "sdk:x\tQ.h\tguess"]})
        got = sorted((f.path, f.message) for f in o_origin.O4().run(ctx))
        self.assertEqual(got, [
            (T + "Baz.cpp", "origin guess, but there is no class Baz in Baz.h"),
            (T + "Baz.h", "origin guess, but there is no class Baz in the file"),
            (T + "Lone.cpp", "origin guess, but there is no class Lone in Lone.h")])


def fn(name, line=10, **kw):
    f = {"name": name, "qualified": name, "file": "Civ5Expansion2/A.h", "line": line, "params": [], "return": "int",
         "params_unknown": False, "instances": [], "vtable_index": None, "virtual": False,
         "markers": {"addr": None, "base": None, "deleting": None}}
    f.update(kw)
    return f


class Placeholders(unittest.TestCase):
    BF = {"symbols": [{"addr": "0x0834a380", "mangled": "_ZN6cvBase4InitEv"},
                      {"addr": "0x0834a2c0", "mangled": "FSBAAllocate"}],
          "vtables": [{"addr": "0x0a1b2c00", "class": "cvBase"}]}

    def run_m5(self, **db):
        return [f.message for f in m_markers.M5().run(Stub(typedb=db, binfacts=self.BF))]

    def test_fun_over_wrong_address_and_without_address(self):
        got = self.run_m5(functions=[fn("Fun_0834a400", markers={"addr": "0x0834a440"}), fn("Fun_0834a500", line=11)])
        self.assertEqual(got, ["Fun_0834a400 placed over address 0x0834a440",
                               "Fun_0834a500 has no address marker; Fun_<addr> is its own address"])

    def test_vf_names(self):
        self.assertEqual(self.run_m5(functions=[fn("vf_03", virtual=True, vtable_index=3),
                                                fn("vf_100", line=11, virtual=True, vtable_index=100),
                                                fn("vf_5", line=12)]), [])
        self.assertEqual(self.run_m5(functions=[fn("vf_03", virtual=True, vtable_index=4)]),
                         ["vf_03 at vtable_index 4, expected vf_04"])
        self.assertEqual(self.run_m5(functions=[fn("vf_03")]), ["vf_03 is not a virtual method (no vtable_index)"])

    def test_param_numbers(self):
        got = self.run_m5(functions=[fn("F", params=[{"name": "a1", "type": "int"}, {"name": "a3", "type": "int"},
                                                     {"name": "count", "type": "int"}])])
        self.assertEqual(got, ["parameter a3 at position 2, expected a2"])

    def test_unk_params_only_where_no_cpp_symbol_is_exported(self):
        up = {"params_unknown": True}
        self.assertEqual(self.run_m5(functions=[fn("F", markers={"addr": "0x0834a2c0"}, **up),
                                                fn("G", line=11, markers={"addr": "0x08350000"}, **up),
                                                fn("H", line=12, markers={"addr": None, "base": None, "deleting": None}, **up)]),
                         [])
        got = self.run_m5(functions=[fn("F", markers={"addr": "0x08350000", "base": "0x0834a380"}, **up)])
        self.assertEqual(len(got), 1)
        self.assertIn("_ZN6cvBase4InitEv is exported at 0x0834a380", got[0])

    def test_class_vtable_and_rtti(self):
        rec = {"name": "ns::Class_0a1b2c00", "file": "Civ5Expansion2/A.h", "line": 3, "origin": "tree", "fields": [],
               "vtable_marker": "0x0a1b2c00"}
        self.assertIn("named cvBase in RTTI", self.run_m5(records=[rec])[0])
        rec = dict(rec, name="Class_0a1b2f00", vtable_marker="?")
        self.assertEqual(self.run_m5(records=[rec]), ["Class_0a1b2f00 has vtable marker ?, expected 0x0a1b2f00"])
        rec = dict(rec, vtable_marker="0x0a1b2f00")
        self.assertEqual(self.run_m5(records=[rec]), [])

    def test_field_offsets(self):
        fields = [{"name": "field_14", "line": 4, "offset_marker": "0x14", "type": "int"},
                  {"name": "unk_8", "line": 5, "offset_marker": "0x10", "type": "char[8]"},
                  {"name": "field_0", "line": 6, "offset_marker": "0x0", "type": "int"},
                  {"name": None, "line": 7, "offset_marker": None, "type": "int"}]
        rec = {"name": "R", "file": "Civ5Expansion2/A.h", "line": 3, "origin": "tree", "fields": fields}
        self.assertEqual(self.run_m5(records=[rec]), ["unk_8 placed at +0x10"])

    def test_globals(self):
        g = {"qualified": "g_0b000100", "file": "Civ5Expansion2/A.h", "line": 2, "addr_marker": "0x0b000100", "type": "int"}
        arr = dict(g, qualified="g_0a920000", addr_marker="0x0a920000", type="UNK_DATA[32]", line=3)
        bad = dict(g, qualified="g_0b000110", addr_marker="0x0b000110", type="UNK_DATA", line=4)
        self.assertEqual(self.run_m5(globals=[g, arr]), [])
        self.assertEqual(self.run_m5(globals=[bad]), ["UNK_DATA in the type of a global (UNK_DATA); "
                                                      "it belongs to global array element types"])

    def test_ghidra_style_and_dictionary_membership(self):
        got = self.run_m5(functions=[fn("FUN_0834a400", markers={"addr": "0x0834a400"}),
                                     fn("DAT_0a000000", line=11), fn("field_0x14", line=12)])
        self.assertEqual(len(got), 3)
        self.assertTrue(all("Ghidra style" in m for m in got))

    def test_non_tree_entities_are_skipped(self):
        f = fn("Fun_0834a400", markers={"addr": "0x00000001"}, file="third_party/x.h")
        self.assertEqual(self.run_m5(functions=[f]), [])


class CppMarkers(unittest.TestCase):
    def run_p1(self, funcs, cpp, globs=()):
        db = {"functions": funcs, "cpp_defs": cpp, "globals": list(globs)}
        return [f.message for f in p_cpp.P1().run(Stub(typedb=db))]

    def mk(self, **kw):
        m = {"addr": None, "base": None, "inline": False, "fastcc": False}
        m.update(kw)
        return m

    def test_matching_and_mismatching_primary_markers(self):
        f = {"key": "K", "file": "A.h", "line": 7, "instances": [],
             "markers": self.mk(addr="0x08360000", base="0x08360040")}
        c = {"file": "A.cpp", "line": 9, "function": "K", "mangled": "K", "qualified": "K", "instances": []}
        self.assertEqual(self.run_p1([f], [dict(c, markers=self.mk(addr="0x08360000"))]), [])
        self.assertEqual(self.run_p1([f], [dict(c, markers=self.mk(addr="0x08360010"))]),
                         ["0x08360010, in .h 0x08360000 (A.h:7)"])
        self.assertEqual(self.run_p1([f], [dict(c, markers=self.mk(addr="0x08360000", fastcc=True))]),
                         ["0x08360000 fastcc, in .h 0x08360000 (A.h:7)"])
        self.assertEqual(self.run_p1([f], [dict(c, markers=self.mk(inline=True))]),
                         ["inline, in .h 0x08360000 (A.h:7)"])

    def test_lone_base_and_missing_markers_are_not_p1(self):
        f = {"key": "K", "file": "A.h", "line": 7, "instances": [], "markers": self.mk(base="0x08360040")}
        c = {"file": "A.cpp", "line": 9, "function": "K", "mangled": "K", "qualified": "K", "instances": []}
        self.assertEqual(self.run_p1([f], [dict(c, markers=self.mk(base="0x08360040"))]), [])
        self.assertEqual(len(self.run_p1([f], [dict(c, markers=self.mk(base="0x08360048"))])), 1)
        self.assertEqual(self.run_p1([f], [dict(c, markers=self.mk())]), [])
        f2 = dict(f, markers=self.mk())
        self.assertEqual(self.run_p1([f2], [dict(c, markers=self.mk(base="0x08360048"))]), [])

    def test_instances_match_by_spec(self):
        f = {"key": "K", "file": "A.h", "line": 7, "markers": self.mk(),
             "instances": [{"spec": "Box<int>", "addr": "0x08360300", "base": None, "fastcc": False}]}
        c = {"file": "A.cpp", "line": 9, "function": "K", "mangled": None, "qualified": "K", "markers": self.mk(),
             "instances": [{"spec": "Box<int>", "addr": "0x08360308", "base": None, "fastcc": False}]}
        self.assertEqual(self.run_p1([f], [c]), ["0x08360308, in .h 0x08360300 for Box<int> (A.h:7)"])

    def test_globals(self):
        g = {"mangled": "G", "file": "A.h", "line": 3, "addr_marker": "0x0b000200", "inline_marker": False}
        c = {"file": "A.cpp", "line": 4, "function": None, "mangled": "G", "qualified": "G", "instances": [],
             "markers": self.mk(addr="0x0b000208")}
        self.assertEqual(self.run_p1([], [c], [g]), ["0x0b000208, in .h 0x0b000200 (A.h:3)"])


class Build(unittest.TestCase):
    class Ctx(Stub):
        def __init__(self, files):
            super().__init__(files={p: "" for p in files})
            self.includes = mock.Mock()
            self.includes.includes = lambda seeds: set(seeds)

    def run_rule(self, rule, files, errors):
        captured = []

        def fake_run(ctx, jobs):
            captured.extend(jobs)
            return {j.name: clangrun.Result(not errors.get(j.name), errors.get(j.name, []), False) for j in jobs}

        with mock.patch.object(b_build.clangrun, "run", fake_run):
            return rule.run(self.Ctx(files)), captured

    def test_b1_translation_unit_is_two_includes_of_the_relative_path(self):
        out, jobs = self.run_rule(b_build.B1(), ["Civ5Expansion2/Fx/A.h", "Civ5Expansion2/Fx/B.cpp", "third_party/z.h"], {})
        self.assertEqual(out, [])
        self.assertEqual([(j.name, j.text) for j in jobs],
                         [("Civ5Expansion2/Fx/A.h", '#include "Fx/A.h"\n#include "Fx/A.h"\n')])

    def test_b1_reports_the_first_error_and_keeps_the_rest_as_detail(self):
        d = [clangrun.Diag("Civ5Expansion2/Fx/Y.h", 40, 1, "unknown type name 'X'"), clangrun.Diag(None, None, None, "boom")]
        out, _ = self.run_rule(b_build.B1(), ["Civ5Expansion2/Fx/A.h"], {"Civ5Expansion2/Fx/A.h": d})
        self.assertEqual((out[0].path, out[0].line), ("Civ5Expansion2/Fx/A.h", 1))
        self.assertEqual(out[0].message, "does not compile alone: Civ5Expansion2/Fx/Y.h:40: unknown type name 'X'")
        self.assertEqual(out[0].detail, ["boom"])

    def test_b2_lines_come_from_the_cpp_or_fall_back_to_line_1(self):
        p = "Civ5Expansion2/Fx/A.cpp"
        d = [clangrun.Diag(p, 88, 3, "use of undeclared identifier 'm_iCount'"),
             clangrun.Diag(p, 88, 3, "use of undeclared identifier 'm_iCount'"),
             clangrun.Diag("Civ5Expansion2/Fx/A.h", 4, 1, "unknown type name 'T'")]
        out, jobs = self.run_rule(b_build.B2(), [p, "Civ5Expansion2/Fx/A.h"], {p: d})
        self.assertEqual(jobs[0].text, '#include "Fx/A.cpp"\n')
        self.assertEqual([(f.line, f.message) for f in out],
                         [(88, "use of undeclared identifier 'm_iCount'"),
                          (1, "Civ5Expansion2/Fx/A.h:4: unknown type name 'T'")])


class Fixtures(unittest.TestCase):
    def test_every_rule_ships_ok_and_bad_fixtures_and_typedb_codes_are_covered(self):
        root = Path(__file__).resolve().parent.parent / "fixtures"
        for rid in "B1 B2 O1 O2 O3 O4 M0 M1 M2 M3 M4 M5 M6 P1 P2".split():
            names = [d.name for d in (root / rid).iterdir() if d.is_dir()]
            self.assertTrue(any(n.startswith("ok-") for n in names), rid)
            self.assertTrue(any(n.startswith("bad-") for n in names), rid)
        for code, rule in TABLE.items():
            self.assertTrue((root / rule / ("bad-" + code)).is_dir(), f"{rule} has no fixture for {code}")
        self.assertTrue((root / "O1" / "bad-no-origin").is_dir())


if __name__ == "__main__":
    unittest.main()
