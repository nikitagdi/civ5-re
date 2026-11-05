"""Class templates: members of a family = union over instantiations, parameter types
generalized by replacing argument k with T<k>; the replacement must reproduce every instantiation's mangling."""
from collections import defaultdict

from seed import mangle as M
from seed.model import Fn


def binding(targs):
    return {M.TParam(k): a for k, a in enumerate(targs)}


def _abstract(t, targs, which):
    mp = {}
    for k, a in enumerate(targs):
        if which == "full" or (which == "class" and not isinstance(a, (M.Builtin, M.Lit))):
            mp.setdefault(a, M.TParam(k))
    return M.substitute(t, mp) if mp else t


def _cands(params, targs):
    out = []
    for which in ("full", "class", "none"):
        g = tuple(_abstract(p, targs, which) for p in params)
        if g not in out:
            out.append(g)
    return out


def generalize(model):
    for sc in model.scopes.values():
        if sc.kind != "class" or (sc.tparams is None and sc.in_template is None) or not sc.insts:
            continue
        _family(model, sc)


def _family(model, sc):
    if sc.inst_bases:
        votes = defaultdict(list)
        for spec, bases in sc.inst_bases.items():
            targs = sc.insts[spec]
            g = tuple((M.substitute(M.Named(bn), {a: M.TParam(k) for k, a in enumerate(targs)}), off, pub, virt)
                      for _, bn, off, pub, virt in bases)
            votes[g].append(spec)
        best = max(votes, key=lambda g: (len(votes[g]), repr(g)))
        if len(votes) > 1:
            model.decisions["template_bases_differ"].append("::".join(sc.path))
        sc.tbases = best
    groups = defaultdict(list)       # (part, kind, const, arity, variadic, virtual-ness) -> [(fn, inst)]
    keep = []
    for f in sc.fns:
        if not any(i.spec for i in f.insts) or f.params is None:
            keep.append(f)
            continue
        for i in f.insts:
            groups[(f.part, f.kind, f.const, len(f.params), f.variadic)].append((f, i))
    out, seen = [], {}
    for f in keep:
        k = (f.part, f.kind, f.const, f.params)
        if f.params is None and any(i.spec for i in f.insts) and k in seen:
            g = seen[k]
            g.insts.extend(f.insts)
            g.virtual |= f.virtual
            g.pure |= f.pure
            if f.slot is not None:
                g.slot = f.slot if g.slot is None else min(g.slot, f.slot)
            continue
        seen[k] = f
        out.append(f)
    for gk, rows in sorted(groups.items(), key=lambda kv: repr(kv[0])):
        # concrete signatures present per instantiation
        present = defaultdict(set)
        for f, i in rows:
            present[i.spec].add(f.params)
        support = defaultdict(int)
        cand_of = {}
        for f, i in rows:
            targs = sc.insts.get(i.spec)
            if targs is None:
                continue
            cs = _cands(f.params, targs)
            cand_of[(id(f), i.spec)] = cs
        for cs in cand_of.values():
            for g in cs:
                if g in support:
                    continue
                n = 0
                for spec, sigs in present.items():
                    b = binding(sc.insts[spec])
                    if tuple(M.substitute(p, b) for p in g) in sigs:
                        n += 1
                support[g] = n
        merged = {}
        for f, i in rows:
            cs = cand_of.get((id(f), i.spec)) or [f.params]
            best = max(cs, key=lambda g: (support.get(g, 0), -cs.index(g)))
            g = merged.get(best)
            if g is None:
                g = merged[best] = Fn(sc.path, f.part, f.kind, best, f.variadic, f.const, None, f.ret_text,
                                      f.static, f.virtual, f.pure, None, [], f.lua, f.order, f.slot)
                for attr in ("_vkey", "_vf"):
                    if hasattr(f, attr):
                        setattr(g, attr, getattr(f, attr))
            g.virtual |= f.virtual
            g.pure |= f.pure
            g.lua |= f.lua
            g.slot = f.slot if g.slot is None else (min(g.slot, f.slot) if f.slot is not None else g.slot)
            g.order = min(g.order, f.order) if g.order else f.order
            g.insts.append(i)
        out.extend(merged.values())
        if len(merged) > 1 and len({g for g in merged}) != len(merged):
            model.decisions["template_conflict"].append(f"{'::'.join(sc.path)}::{gk[0].name}")
    sc.fns = out
    for f in sc.fns:
        f.insts.sort(key=lambda i: (i.spec or "", i.addr or i.base or ""))
