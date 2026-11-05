"""L1-L2: declared sizes and field offsets against the clang layout."""
from checks.api import Rule
from checks.rules.a_symbols import hexa


def layout_records(ctx):
    """Tree records with a layout: patterns (no size) are covered through their instantiations."""
    return [r for r in ctx.typedb.get("records", []) if r.get("origin") == "tree" and r.get("size") is not None]


class L1(Rule):
    id = "L1"
    title = "'/// size' equals the clang sizeof (templates: every instantiation, or the one named by SPEC)"
    needs = ("typedb",)
    scope = "entity"
    deep = True

    def run(self, ctx):
        out = []
        for r in layout_records(ctx):
            m = r.get("size_marker")
            if m is None or m == "?":
                continue
            if hexa(m) != r["size"]:
                out.append(self.finding(r["file"], r["line"], f"{r['name']}: size {m}, clang 0x{r['size']:x}",
                                        entity=r["name"], expected="0x%x" % r["size"], actual=m,
                                        key=f"L1:{r['name']}"))
        return out


def _plain(name):
    return "".join(name.split())


class L3(Rule):
    id = "L3"
    title = "declared class size is at least what the binary's constructors and destructors touch (warn)"
    needs = ("typedb", "binfacts")
    scope = "entity"
    deep = True

    def run(self, ctx):
        sizes = {_plain(k): v for k, v in (ctx.binfacts.get("sizes") or {}).items()}
        kinds = {}
        for s in ctx.binfacts.get("symbols", []):
            if s["kind"] in ("ctor", "dtor"):
                kinds.setdefault(s["addr"], s["kind"])
        out = []
        for r in layout_records(ctx):
            ev = sizes.get(_plain(r["name"]))
            if ev is None:
                continue
            m = r.get("size_marker")
            if m is None or m == "?":
                continue
            declared = hexa(m)
            if declared >= ev["min"]:
                continue
            via = ", ".join(f"{kinds.get(a, 'code')} {a}" for a in ev["from"][:2])
            out.append(self.warn(r["file"], r["line"],
                                 f"{r['name']}: size 0x{declared:x}, binary uses at least 0x{ev['min']:x} ({via})",
                                 entity=r["name"], expected=f">= 0x{ev['min']:x}", actual=f"0x{declared:x}",
                                 key=f"L3:{r['name']}"))
        return out


def field_base(rec_by_name, r, seen=()):
    """Offset to add so that a field's clang offset counts from the record it is named through."""
    a = r.get("accessed_from")
    if not a or r["name"] in seen:
        return 0
    outer = rec_by_name.get(a["record"])
    return a["offset"] + (field_base(rec_by_name, outer, seen + (r["name"],)) if outer else 0)


class L2(Rule):
    id = "L2"
    title = "'///<' equals the clang offset of the field (bit field: the byte of its first bit)"
    needs = ("typedb",)
    scope = "entity"
    deep = True

    def run(self, ctx):
        by_name = {r["name"]: r for r in ctx.typedb.get("records", [])}
        out = []
        for r in layout_records(ctx):
            base = field_base(by_name, r)
            for f in r.get("fields") or []:
                m = f.get("offset_marker")
                if m is None or f.get("offset") is None:
                    continue
                off = f["bit_offset"] // 8 if f.get("bit_size") is not None and f.get("bit_offset") is not None \
                    else f["offset"]
                off += base
                if hexa(m) != off:
                    out.append(self.finding(r["file"], f["line"],
                                            f"{f.get('name') or '<unnamed>'}: ///< +{m}, clang +0x{off:x}"
                                            + (f" in {r['name']}" if r.get("template") else ""),
                                            entity=f"{r['name']}.{f.get('name')}", expected=f"+0x{off:x}",
                                            actual=f"+{m}", key=f"L2:{r['name']}.{f.get('name')}"))
        return out
