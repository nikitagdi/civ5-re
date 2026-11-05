"""Apply into an open Ghidra through the ghidra-mcp plugin (HTTP on 127.0.0.1)."""

import json
import os
import urllib.error
import urllib.request
from concurrent.futures import ThreadPoolExecutor

PROGRAM = "Civ5XP"
PORT_SPAN = 16
TIMEOUT = 1800
PROBE_TIMEOUT = 3

HINT_MENU = ("or use the menu: Script Manager > Manage Script Directories > <repo>\\tools\\ghidra\\java, "
             "then Tools > civ5re > Apply, then civ5re ghidra verify")
HINT_DISABLED = "set GHIDRA_MCP_ALLOW_SCRIPTS=1 in the environment of the Ghidra process and restart Ghidra; " + HINT_MENU
HINT_NO_INSTANCE = "open Civ5XP in CodeBrowser in the civ5re project; " + HINT_MENU


class GuiError(Exception):
    def __init__(self, message, hint=None):
        super().__init__(message)
        self.hint = hint

    def full(self):
        return f"{self.args[0]}: {self.hint}" if self.hint else str(self.args[0])


def _get_json(url, timeout=PROBE_TIMEOUT):
    with urllib.request.urlopen(url, timeout=timeout) as r:
        return json.loads(r.read().decode("utf-8", errors="replace"))


def _post_json(url, payload, timeout):
    req = urllib.request.Request(url, data=json.dumps(payload).encode("utf-8"), method="POST",
                                 headers={"Content-Type": "application/json"})
    with urllib.request.urlopen(req, timeout=timeout) as r:
        return r.status, r.read().decode("utf-8", errors="replace")


def _strings(v):
    if isinstance(v, str):
        yield v
    elif isinstance(v, dict):
        for x in v.values():
            yield from _strings(x)
    elif isinstance(v, list):
        for x in v:
            yield from _strings(x)


def _norm(p):
    return os.path.normcase(os.path.normpath(p)).rstrip("\\/")


def match_instance(info, project_dir, project_name="civ5re", program=PROGRAM):
    """info is the /mcp/instance_info JSON: our project path and the program among the open ones."""
    ours = {_norm(project_dir), _norm(os.path.join(project_dir, project_name)),
            _norm(os.path.join(project_dir, project_name + ".gpr"))}
    strings = list(_strings(info))
    project = info.get("project_path") if isinstance(info, dict) else None
    if isinstance(project, str):
        if _norm(project) not in ours:
            return False
    elif not any(_norm(s) in ours for s in strings if len(s) > 3):
        return False
    return any(s == program or s.replace("\\", "/").endswith("/" + program) for s in strings)


def find_instance(port0, project_dir, program=PROGRAM, span=PORT_SPAN, fetch=_get_json):
    ports = list(range(port0, port0 + span))

    def probe(port):
        try:
            return match_instance(fetch(f"http://127.0.0.1:{port}/mcp/instance_info"), project_dir, program=program)
        except Exception:
            return False

    pool = ThreadPoolExecutor(max_workers=span)
    try:
        for port, fut in zip(ports, [pool.submit(probe, p) for p in ports]):
            if fut.result():
                return port
        return None
    finally:
        pool.shutdown(wait=False, cancel_futures=True)


def run_script(port, script, repo, dry_run=False, prune_all=False, program=PROGRAM, timeout=TIMEOUT,
               post=_post_json):
    """Runs Civ5reApply.java by absolute path. Returns the response body; raises GuiError."""
    repo = str(repo)
    if " " in repo or " " in str(script):
        raise GuiError("the repository path contains spaces (script arguments are split on spaces)",
                       "move the repository to a path without spaces or " + HINT_MENU)
    args = f"--repo={repo}" + (" --dry-run" if dry_run else "") + (" --prune-all" if prune_all else "")
    url = f"http://127.0.0.1:{port}/run_ghidra_script?program={program}"
    try:
        status, body = post(url, {"script_name": str(script), "args": args}, timeout)
    except urllib.error.HTTPError as e:
        body = e.read().decode("utf-8", errors="replace") if hasattr(e, "read") else ""
        if e.code == 403 or "disabled" in body.lower():
            raise GuiError(f"the plugin refused to run the script (HTTP {e.code}): {body.strip()[:200]}",
                           HINT_DISABLED) from None
        raise GuiError(f"the plugin answered HTTP {e.code}: {body.strip()[:200]}", HINT_NO_INSTANCE) from None
    except (urllib.error.URLError, OSError) as e:
        raise GuiError(f"the plugin on port {port} is not reachable: {e}", HINT_NO_INSTANCE) from None
    if "script execution disabled" in body.lower():
        raise GuiError("the plugin refused to run the script: Script execution disabled", HINT_DISABLED)
    return body
