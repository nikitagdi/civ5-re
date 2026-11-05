import unittest

from checks import includes, plugins, staged


def graph(files):
    return includes.build(sorted(files), files.get, lambda p: p in files)


class IncludeGraph(unittest.TestCase):
    def test_quotes_only_and_resolution_order(self):
        files = {
            "Civ5Expansion2/A/a.h": '#include <vector>\n#include "x.h"\n  #  include "A/y.h"\n',
            "Civ5Expansion2/A/x.h": "",
            "Civ5Expansion2/x.h": "",
            "Civ5Expansion2/A/y.h": "",
        }
        e = graph(files).edges["Civ5Expansion2/A/a.h"]
        self.assertEqual([(x.line, x.target) for x in e],
                         [(2, "Civ5Expansion2/A/x.h"), (3, "Civ5Expansion2/A/y.h")])

    def test_reverse_dependencies_are_transitive(self):
        files = {"Civ5Expansion2/a.h": '#include "b.h"\n', "Civ5Expansion2/b.h": '#include "c.h"\n',
                 "Civ5Expansion2/c.h": "", "Civ5Expansion2/d.cpp": '#include "b.h"\n', "Civ5Expansion2/e.h": ""}
        g = graph(files)
        self.assertEqual(g.includers(["Civ5Expansion2/c.h"]),
                         {"Civ5Expansion2/c.h", "Civ5Expansion2/b.h", "Civ5Expansion2/a.h", "Civ5Expansion2/d.cpp"})
        self.assertEqual(g.includes(["Civ5Expansion2/a.h"]),
                         {"Civ5Expansion2/a.h", "Civ5Expansion2/b.h", "Civ5Expansion2/c.h"})

    def test_cycles(self):
        files = {"Civ5Expansion2/a.h": '#include "b.h"\n', "Civ5Expansion2/b.h": '#include "a.h"\n',
                 "Civ5Expansion2/c.h": '#include "a.h"\n', "Civ5Expansion2/s.h": '#include "s.h"\n'}
        cyc = sorted((s, e.line) for s, e in graph(files).cycle_edges())
        self.assertEqual(cyc, [("Civ5Expansion2/a.h", 1), ("Civ5Expansion2/b.h", 1), ("Civ5Expansion2/s.h", 1)])

    def test_third_party_is_a_leaf(self):
        files = {"Civ5Expansion2/a.h": '#include "lib.h"\n', "third_party/lib.h": '#include "a.h"\n'}
        g = graph(files)
        self.assertEqual(g.edges["Civ5Expansion2/a.h"][0].target, "third_party/lib.h")
        self.assertNotIn("third_party/lib.h", g.edges)
        self.assertEqual(g.cycle_edges(), [])


class Helpers(unittest.TestCase):
    def test_verify_functions_are_clean(self):
        self.assertEqual(includes.verify(), [])
        self.assertEqual(staged.verify(), [])
        self.assertEqual(plugins.verify(), [])
