"""SDK headers from Lekmod git objects (steps 1-5 of the seed pipeline).

build_sdk() returns {dest path under Civ5Expansion2/: text}; write_sdk() puts it
in a temporary folder (under build/). Markers (step 6) are added elsewhere.
"""
import posixpath
import re
from concurrent.futures import ThreadPoolExecutor
from pathlib import Path

from . import inputs, patch, unifdef

PATCH = inputs.DATA / "sdk_port.patch"
MARKER_KEYWORDS = ("origin", "size", "vtable", "base", "deleting", "thunk", "inline", "pure", "absent", "fastcc", "lua")
ROOT_PREFIX = "d:/firaxis_oldserver/civ5expansion2/"

_MARKER_LIKE = re.compile(r"^(?:0x|<\s*\+0x|(?:%s)\b)" % "|".join(MARKER_KEYWORDS))
_INCLUDE = re.compile(r'^(?P<pre>[ \t]*#[ \t]*include[ \t]*)(?P<open>["<])(?P<name>[^">\n]+)(?P<close>[">])(?P<post>.*)$')


class SdkError(Exception):
    pass


# step 1

def decode(data):
    try:
        text = data.decode("cp1252")
    except UnicodeDecodeError as e:
        raise SdkError(f"not cp1252: {e}") from None
    return text.replace("\r\n", "\n").replace("\r", "\n")


# step 3

def rewrite_comments(text):
    """`///` runs become `//`; line comments that look like markers get a neutral prefix."""
    out = []
    i, n = 0, len(text)
    state = None  # None | 'str' | 'chr' | 'block'
    while i < n:
        c = text[i]
        two = text[i:i + 2]
        if state is None:
            if two == "//":
                j = text.find("\n", i)
                j = n if j < 0 else j
                comment = text[i:j]
                k = len(comment) - len(comment.lstrip("/"))
                body = comment[k:]
                if k >= 3:
                    k = 2
                if _MARKER_LIKE.match(body.lstrip()):
                    body = " Note: " + body.lstrip()
                out.append("/" * k + body)
                i = j
                continue
            if two == "/*":
                state = "block"
                out.append(two)
                i += 2
                continue
            if c == '"':
                state = "str"
            elif c == "'":
                state = "chr"
            out.append(c)
            i += 1
        elif state == "block":
            if two == "*/":
                state = None
                out.append(two)
                i += 2
            else:
                out.append(c)
                i += 1
        else:
            quote = '"' if state == "str" else "'"
            if c == "\\" and i + 1 < n:
                out.append(text[i:i + 2])
                i += 2
                continue
            if c == quote or c == "\n":
                state = None
            out.append(c)
            i += 1
    return "".join(out)


# step 4

def load_include_dirs(path=None):
    """Returns (dirs, libs): dirs in file order under the project root, libs of each dir."""
    path = Path(path) if path else inputs.DATA / "sdk_lib_include_dirs.txt"
    dirs = []
    for line in path.read_text(encoding="utf-8").splitlines():
        if not line.strip() or line.startswith("#"):
            continue
        d, _, libs = line.partition("\t")
        d = d.lower().rstrip("/")
        if d.startswith(ROOT_PREFIX):
            dirs.append((d[len(ROOT_PREFIX):], set(libs.split(","))))
    return dirs


def load_file_libs(path=None):
    path = Path(path) if path else inputs.DATA / "sdk_lib_paths.txt"
    libs = {}
    for line in path.read_text(encoding="utf-8").splitlines():
        if not line.strip() or line.startswith("#"):
            continue
        f = line.split("\t")
        libs[f[0]] = set(f[2].split(","))
    return libs


class IncludeResolver:
    def __init__(self, dests, lua_names=()):
        self.index = {d.lower(): d for d in dests}
        self.dirs = load_include_dirs()
        self.file_libs = load_file_libs()
        self.lua = set(lua_names)

    def search_dirs(self, includer):
        own = posixpath.dirname(includer).lower()
        libs = self.file_libs.get(includer.lower(), set())
        mine = [d for d, dl in self.dirs if libs & dl]
        rest = [d for d, _ in self.dirs if d not in mine]
        seen, out = set(), []
        for d in [own] + mine + rest:
            if d not in seen:
                seen.add(d)
                out.append(d)
        return out

    def resolve(self, includer, name):
        """Returns the path from the root (original case) or None."""
        name = name.replace("\\", "/")
        for d in self.search_dirs(includer):
            cand = posixpath.normpath(posixpath.join(d, name)).lower()
            if cand in self.index:
                return self.index[cand]
        return None


def rewrite_includes(text, includer, resolver):
    out = []
    for line in text.split("\n"):
        m = _INCLUDE.match(line)
        if m:
            name = m["name"].strip()
            if name in resolver.lua:
                line = f'{m["pre"]}<{name}>{m["post"]}'
            else:
                found = resolver.resolve(includer, name)
                if found:
                    line = f'{m["pre"]}"{found}"{m["post"]}'
        out.append(line)
    return "\n".join(out)


# pipeline

def transform(text, entry, resolver, patterns=None):
    text = unifdef.process(text, patterns if patterns is not None else unifdef.load_patterns(), entry.dest)
    text = rewrite_comments(text)
    text = rewrite_includes(text, entry.dest, resolver)
    if not text.endswith("\n"):
        text += "\n"
    return text


def stage(lekmod_dir, lock=None, entries=None):
    """Steps 1-4 on the git objects: {dest: text}."""
    lock = lock or inputs.load_lock()
    entries = entries or inputs.sdk_entries()
    lua_names = {Path(p).name for p in inputs.lua_files(lekmod_dir, lock)}
    resolver = IncludeResolver([e.dest for e in entries], lua_names)
    patterns = unifdef.load_patterns()
    with ThreadPoolExecutor(8) as ex:  # one `git cat-file` process per blob
        blobs = list(ex.map(lambda e: inputs.read_blob(lekmod_dir, lock.commits[e.ref], e.src), entries))
    files = {}
    for e, blob in zip(entries, blobs):
        files[e.dest] = transform(decode(blob), e, resolver, patterns)
    return files


def build_sdk(lekmod_dir, lock=None, entries=None, patch_path=None):
    """Steps 1-5: {dest: text}, deterministic."""
    files = stage(lekmod_dir, lock, entries)
    p = Path(patch_path) if patch_path else PATCH
    if p.is_file():
        files = patch.apply(files, p.read_text(encoding="utf-8"))
    return dict(sorted(files.items()))


def write_sdk(files, out_dir):
    out_dir = Path(out_dir)
    for dest, text in files.items():
        p = out_dir / dest
        p.parent.mkdir(parents=True, exist_ok=True)
        p.write_bytes(text.encode("utf-8"))
    return out_dir
