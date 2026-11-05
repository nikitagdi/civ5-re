import copy
import json
import threading
from http.server import BaseHTTPRequestHandler, ThreadingHTTPServer
from pathlib import Path

from ghidra import headless, plan as planmod

DATA = Path(__file__).resolve().parent / "data"
SHA = "cc06b647821ec5e7cca3c397f6b0d4726f0106cdd67bcf074d494bea2607a8ca"


def fixture():
    typedb = json.loads((DATA / "typedb.json").read_text(encoding="utf-8"))
    binfacts = json.loads((DATA / "binfacts.json").read_text(encoding="utf-8"))
    meta, funcs = planmod.read_functions(DATA / "ghidra-functions.txt")
    return copy.deepcopy(typedb), copy.deepcopy(binfacts), dict(funcs)


def build(typedb=None, binfacts=None, funcs=None):
    t, b, f = fixture()
    return planmod.build_plan(typedb or t, binfacts or b, f if funcs is None else funcs,
                              {"typedb": "a" * 64, "binfacts": "b" * 64, "binary": SHA, "functions": "c" * 64})


def by_id(plan):
    return {t["id"]: t for t in plan["types"]}


def funcs_by_addr(plan):
    return {f["addr"]: f for f in plan["functions"]}


def state_from_plan(plan):
    """What a perfect Civ5reApply would read back."""
    kinds = {tuple(s["path"]): s["kind"] for s in plan["scopes"]}
    funcs = []
    for f in plan["functions"]:
        funcs.append({
            "addr": f["addr"], "exists": True, "owned": True, "name": f["name"], "scope": list(f["scope"]),
            "scope_kinds": [kinds[tuple(f["scope"][:i + 1])] for i in range(len(f["scope"]))],
            "cc": f["cc"], "custom": f["custom"], "ret": f["ret"], "ret_storage": f["ret_storage"],
            "params": [] if f["params"] is None else copy.deepcopy(f["params"]),
            "auto_this": f["this_type"] if f["cc"] == "__thiscall" else None,
            "auto_retptr": f["auto_retptr"], "varargs": f["varargs"], "name_source": "IMPORTED",
            "sig_source": "IMPORTED"})
    data = [dict(d, exists=True, owned=True) for d in plan["data"]]
    labels = [dict(lb, exists=True, owned=True) for lb in plan["labels"]]
    for lb in labels:
        lb.pop("kind", None)
    return {"schema": "civ5re.state/1", "script_version": planmod.SCRIPT_VERSION,
            "plan_sha256": plan["header"]["plan_sha256"], "binary_sha256": plan["header"]["binary_sha256"],
            "time": "2026-10-19T00:00:00+00:00", "types": copy.deepcopy(plan["types"]), "funcs": funcs,
            "data": data, "labels": labels,
            "bodies": [{"addr": b["addr"], "size": b["size"], "end": hex(int(b["addr"], 16) + b["size"]),
                        "status": "ok"} for b in plan.get("bodies", [])]}


def report_for(plan, dry_run, changes=0, conflicts=0, aborted=None):
    return {"schema": "civ5re.apply-report/1", "script_version": planmod.SCRIPT_VERSION,
            "plan_sha256": plan["header"]["plan_sha256"], "dry_run": dry_run, "aborted": aborted,
            "counters": {"changes": changes, "conflicts": conflicts, "manual_overwrites": 0, "stale": 0,
                         "stale_limit": 50},
            "entries": []}


class FakeHeadless:
    """Stands in for headless.Headless: does what Civ5reApply/Civ5reExport would do, without Ghidra."""

    def __init__(self, ctx, changes=0, sha=SHA, drift=None):
        self.ctx = ctx
        self.calls = []
        self.changes = changes
        self.sha = sha
        self.drift = drift

    def _write(self, rel, obj):
        p = self.ctx.gdir / rel
        p.parent.mkdir(parents=True, exist_ok=True)
        p.write_text(json.dumps(obj), encoding="utf-8")

    def apply(self, project_dir, name=headless.PROJECT, dry_run=False, prune_all=False, both=False, stage="apply"):
        self.calls.append(("apply", str(project_dir), dry_run, prune_all, both))
        plan = json.loads(self.ctx.plan_json.read_text(encoding="utf-8"))
        if not dry_run:
            state = state_from_plan(plan)
            if self.drift:
                self.drift(state)
            self._write("state.json", state)
            self._write("apply-report.json", report_for(plan, False, self.changes))
        if dry_run or both:
            self._write("apply-report.dry.json", report_for(plan, True, self.changes if dry_run else 0))
        return headless.StageResult(stage, 0, 0.1, self.ctx.logs / "fake.log")

    def build_cache(self, project_dir, k12, binary, force=False, seeds_text=None):
        self.calls.append(("cache", k12))
        cdir = headless.cache_dir(project_dir)
        cdir.mkdir(parents=True, exist_ok=True)
        (cdir / (headless.cache_name(k12) + ".gpr")).write_text("gpr")
        (cdir / (headless.cache_name(k12) + ".rep")).mkdir(exist_ok=True)
        text = (DATA / "ghidra-functions.txt").read_text(encoding="utf-8").replace(SHA, self.sha)
        headless.cache_functions(project_dir, k12).write_text(text, encoding="utf-8")
        return headless.StageResult("cache", 0, 0.1, self.ctx.logs / "fake.log")

    def export_functions(self, project_dir, k12):
        self.calls.append(("export", k12))
        text = (DATA / "ghidra-functions.txt").read_text(encoding="utf-8").replace(SHA, self.sha)
        headless.cache_functions(project_dir, k12).write_text(text, encoding="utf-8")
        return headless.StageResult("export", 0, 0.1, self.ctx.logs / "fake.log")

    def paramid(self, project_dir, name=headless.PROJECT):
        self.calls.append(("paramid", str(project_dir)))
        return headless.StageResult("paramid", 0, 0.1, self.ctx.logs / "fake.log")


class FakePlugin:
    """A ghidra-mcp stand-in on 127.0.0.1: /mcp/instance_info and /run_ghidra_script."""

    def __init__(self, project_path, port=0, status=200, body="{}", on_run=None, programs=("Civ5XP",)):
        self.requests = []
        outer = self

        class H(BaseHTTPRequestHandler):
            def log_message(self, *a):
                pass

            def do_GET(self):
                if self.path == "/mcp/instance_info":
                    data = json.dumps({"project_path": project_path, "programs": list(programs)}).encode()
                    self.send_response(200)
                else:
                    data = b"not found"
                    self.send_response(404)
                self.send_header("Content-Length", str(len(data)))
                self.end_headers()
                self.wfile.write(data)

            def do_POST(self):
                n = int(self.headers.get("Content-Length", 0))
                payload = json.loads(self.rfile.read(n) or b"{}")
                outer.requests.append((self.path, payload))
                if status == 200 and on_run:
                    on_run(payload)
                data = (body if status == 200 else "Script execution disabled").encode()
                self.send_response(status)
                self.send_header("Content-Length", str(len(data)))
                self.end_headers()
                self.wfile.write(data)

        self.server = ThreadingHTTPServer(("127.0.0.1", port), H)
        self.port = self.server.server_address[1]
        self.thread = threading.Thread(target=self.server.serve_forever, daemon=True)
        self.thread.start()

    def close(self):
        self.server.shutdown()
        self.server.server_close()
