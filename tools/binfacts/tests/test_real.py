import time
import unittest
from collections import Counter

from binfacts import build, groups
from binfacts.tests import support
from civ5re import binlock, config

TARGET_SHA = "cc06b647821ec5e7cca3c397f6b0d4726f0106cdd67bcf074d494bea2607a8ca"


def _cfg():
    cfg = config.load()
    bin_dir = cfg.path("bin_dir")
    if not support.llvm_bin() or bin_dir is None:
        return None
    entry = binlock.target(binlock.load(cfg.root / "bins.lock"))
    return cfg if (bin_dir / entry.path).is_file() else None


CFG = _cfg()

VARIANTS = {
    "firaxis": ({"C1": 1024, "C2": 1025, "C3": 0, "D0": 201, "D1": 379, "D2": 638},
                dict(c1_eq_c2=1024, c1_ne_c2=0, c1_only=0, c2_only=1, d1_eq_d2=379, d1_ne_d2=0, d1_only=0,
                     d2_only=259, has_d0=201)),
    "aspyr": ({"C1": 108, "C2": 108, "C3": 0, "D0": 89, "D1": 107, "D2": 107},
              dict(c1_eq_c2=108, c1_ne_c2=0, c1_only=0, c2_only=0, d1_eq_d2=107, d1_ne_d2=0, d1_only=0,
                   d2_only=0, has_d0=89)),
    "icu": ({"C1": 152, "C2": 193, "C3": 0, "D0": 66, "D1": 71, "D2": 70},
            dict(c1_eq_c2=146, c1_ne_c2=6, c1_only=0, c2_only=41, d1_eq_d2=61, d1_ne_d2=9, d1_only=1,
                 d2_only=0, has_d0=66)),
    "std": ({"C1": 0, "C2": 118, "C3": 0, "D0": 4, "D1": 2, "D2": 460},
            dict(c1_eq_c2=0, c1_ne_c2=0, c1_only=0, c2_only=118, d1_eq_d2=0, d1_ne_d2=0, d1_only=2,
                 d2_only=460, has_d0=4)),
    "thirdparty": ({"C1": 38, "C2": 38, "C3": 0, "D0": 6, "D1": 6, "D2": 11},
                   dict(c1_eq_c2=38, c1_ne_c2=0, c1_only=0, c2_only=0, d1_eq_d2=6, d1_ne_d2=0, d1_only=0,
                        d2_only=5, has_d0=6)),
}


@unittest.skipUnless(CFG, "Civ5XP or LLVM is not available")
class RealBinary(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.facts, _action = build.ensure(CFG)

    def test_target_is_the_locked_binary(self):
        self.assertEqual(self.facts["binary"]["sha256"], TARGET_SHA)
        self.assertEqual(self.facts["binary"]["size"], 36536148)

    def test_symbol_counts(self):
        syms = self.facts["symbols"]
        self.assertEqual(len(syms), 42956)
        self.assertEqual(Counter(s["type"] for s in syms), {"func": 36213, "object": 6676, "notype": 67})

    def test_special_name_counts(self):
        syms = self.facts["symbols"]
        prefix = Counter(s["mangled"][:4] for s in syms if s["mangled"].startswith("_Z"))
        self.assertEqual((prefix["_ZTV"], prefix["_ZTI"], prefix["_ZTC"], prefix["_ZGV"]), (937, 1075, 4, 7))
        self.assertEqual(sum(1 for s in syms if s["kind"] == "thunk"), 1024)
        self.assertEqual(sum(1 for s in syms if s["mangled"].startswith("_ZTT")), 0)

    def test_eh_rtti_plt_counts(self):
        f = self.facts
        self.assertEqual(len(f["functions_eh"]), 34380)
        self.assertEqual(sum(len(t["bases"]) for t in f["typeinfo"]), 992)
        self.assertEqual(len(f["plt"]), 746)
        self.assertEqual(Counter(t["kind"] for t in f["typeinfo"]), {"si": 798, "class": 170, "vmi": 108})
        self.assertEqual(len(f["vtables"]), 938)
        self.assertEqual(sum(1 for v in f["vtables"] if v["construction"]), 4)

    def test_aliases(self):
        self.assertEqual(self.facts["stats"]["aliases"]["addresses"], 1850)
        names = Counter()
        for s in self.facts["symbols"]:
            if s["type"] == "func":
                names[s["addr"]] += 1
        sizes = Counter(n for n in names.values() if n > 1)
        self.assertEqual(sizes[2], 1846)
        self.assertEqual(sum(v for k, v in sizes.items() if k > 2), 4)
        self.assertTrue(all(4 <= k <= 16 for k in sizes if k > 2))

    def test_variants(self):
        v = self.facts["stats"]["variants"]
        for g in groups.GROUPS:
            symbols, entities = VARIANTS.get(g, ({k: 0 for k in ("C1", "C2", "C3", "D0", "D1", "D2")},
                                                 {k: 0 for k in v[g]["entities"]}))
            self.assertEqual(v[g]["symbols"], symbols, g)
            self.assertEqual(v[g]["entities"], entities, g)
        only_c2 = sum(v[g]["entities"]["c2_only"] for g in v)
        self.assertEqual(only_c2, 160)

    def test_vtable_groups_point_to_their_class(self):
        by_addr = {t["addr"]: t for t in self.facts["typeinfo"]}
        for vt in self.facts["vtables"]:
            for g in vt["groups"]:
                self.assertEqual(by_addr[g["typeinfo"]]["class"], vt["class"], vt["symbol"])

    def test_every_slot_has_a_known_kind(self):
        kinds = Counter(sl["kind"] for vt in self.facts["vtables"] for g in vt["groups"] for sl in g["slots"])
        self.assertEqual(set(kinds), {"func", "thunk", "pure", "import"})
        self.assertEqual(sum(kinds.values()), 12309)
        self.assertEqual((kinds["func"], kinds["thunk"], kinds["pure"], kinds["import"]), (10520, 1229, 547, 13))
        # unexported secondary-group bodies take the subobject this; none is an adjust+jmp thunk
        self.assertFalse(any("thunk_target" in sl for vt in self.facts["vtables"] for g in vt["groups"] for sl in g["slots"]))

    def test_every_ctor_and_dtor_has_a_variant(self):
        for s in self.facts["symbols"]:
            if s["kind"] in ("ctor", "dtor"):
                self.assertIn(s["variant"], ("C1", "C2", "C3", "D0", "D1", "D2"), s["mangled"])
            else:
                self.assertIsNone(s["variant"], s["mangled"])

    def test_groups(self):
        st = self.facts["stats"]
        self.assertGreater(st["groups"]["firaxis"], 0)
        self.assertEqual(sum(st["groups"].values()), 42956)
        self.assertTrue(all(s["group"] in groups.GROUPS for s in self.facts["symbols"]))

    def test_denominator(self):
        d = self.facts["stats"]["denominator"]
        self.assertEqual(d["total"], sum(d["by_kind"].values()))
        self.assertEqual(set(d["by_kind"]), {"function", "method", "ctor", "dtor", "data", "thunk", "vtable", "c"})
        firaxis = Counter(s["kind"] for s in self.facts["symbols"] if s["group"] == "firaxis")
        for k, n in d["by_kind"].items():
            self.assertEqual(n, firaxis[k], k)
        self.assertEqual(d["covered_by_class"],
                         {k: firaxis[k] for k in ("typeinfo", "typeinfo_name", "guard")})

    def test_forced_rebuild_is_identical_and_fast(self):
        path = build.out_path(CFG)
        before = path.read_bytes()
        t = time.perf_counter()
        _facts, action = build.ensure(CFG, force=True)
        seconds = time.perf_counter() - t
        self.assertEqual(action, "built")
        self.assertLess(seconds, 30)
        self.assertEqual(path.read_bytes(), before)
        self.assertEqual(build.ensure(CFG)[1], "fresh")

    def test_cvunitvissystem_vtable(self):
        vts = [v for v in self.facts["vtables"] if v["class"].startswith("cvUnitVisSystem::")]
        self.assertEqual(len(vts), 4)
        for v in vts:
            self.assertEqual(len(v["groups"]), 1)
            self.assertEqual(len(v["groups"][0]["slots"]), 1)
            self.assertTrue(v["groups"][0]["slots"][0]["symbols"][0].endswith("7ExecuteEj"))


if __name__ == "__main__":
    unittest.main()
