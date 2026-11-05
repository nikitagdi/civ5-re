import configparser
import os
import re
from dataclasses import dataclass
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
SHARED_FILE = "civ5re.cfg"
LOCAL_FILE = "civ5re.local.cfg"
SOURCES = ("default", SHARED_FILE, LOCAL_FILE, "env")

SHARED = "shared"
LOCAL = "local"
_FLAT = "_flat"


class ConfigError(Exception):
    pass


@dataclass(frozen=True)
class Key:
    section: str
    name: str
    file: str
    default: str = ""
    required: bool = False
    kind: str = "str"  # str | int | dir | dirs (";"-separated)
    choices: tuple = ()
    needed_by: str = ""  # commands that fail without it; empty = silently optional
    doc: str = ""

    @property
    def env(self):
        return env_name(self.section, self.name)

    @property
    def label(self):
        return f"[{self.section}] {self.name}" if self.section else self.name


def env_name(section, name):
    parts = ["CIV5RE"] + ([section] if section else []) + [name]
    return "_".join(parts).upper().replace("-", "_")


KEYS = (
    Key("", "llvm_dir", LOCAL, kind="dir", needed_by="binfacts, typedb, checks, seed",
        doc="LLVM 22 install (clang++, ld.lld, llvm-cxxfilt). Read by binfacts, typedb, checks, seed."),
    Key("", "ghidra_dir", LOCAL, kind="dir", needed_by="ghidra *",
        doc="Ghidra 12.1.2 install directory. Read by ghidra-sync."),
    Key("", "ghidra_project_dir", LOCAL, required=True, kind="dir",
        doc="Directory of the Ghidra project (not in git). Read by ghidra-sync."),
    Key("", "bin_dir", LOCAL, kind="dir", needed_by="binfacts (rebuild), ghidra *",
        doc="Root of the binaries listed in bins.lock. Read by bootstrap, binfacts, ghidra-sync."),
    Key("", "game_dir", LOCAL, kind="dir", needed_by="lua-api",
        doc="Installed game with Lua/XML. Read by lua-api."),
    Key("ghidra", "heap", SHARED, "16G",
        doc="Heap size of headless Ghidra (GHIDRA_HEADLESS_MAXMEM). Read by ghidra-sync."),
    Key("ghidra", "mcp_port", SHARED, "8089", kind="int",
        doc="First port searched for the ghidra-mcp plugin. Read by ghidra-sync."),
    Key("seed", "lekmod_dir", LOCAL, kind="dir", needed_by="seed",
        doc="Lekmod checkout (SDK headers, Lua, PDB/lib evidence). Read by seed."),
    Key("seed", "libcxx_src", LOCAL, kind="dir", needed_by="seed",
        doc="libc++ headers copied into third_party. Read by seed."),
    Key("seed", "libc_src", LOCAL, kind="dir", needed_by="seed",
        doc="glibc headers copied into third_party. Read by seed."),
    Key("lua-api", "out", LOCAL, kind="dir",
        doc="Directory that receives a copy of the lua-api output (build/ always gets it). Read by lua-api."),
    Key("checks", "jobs", SHARED, "0", kind="int",
        doc="Worker count for checks, 0 = all cores. Read by checks."),
    Key("checks", "export_coverage", SHARED, "report", choices=("report", "required"),
        doc="Rule A10: report or required. seed sets required. Read by checks."),
)


def check_collisions(keys=KEYS):
    seen = {}
    for k in keys:
        other = seen.get(k.env)
        if other is not None:
            raise ConfigError(f"env name {k.env} is produced by both {other.label} and {k.label}")
        seen[k.env] = k


def _lookup(keys, section, name):
    for k in keys:
        if k.section == section and k.name == name:
            return k
    return None


class Config:
    def __init__(self, root, values, origins, keys=KEYS):
        self.root = Path(root)
        self._values = values
        self._origins = origins
        self._keys = keys

    def _key(self, args):
        if len(args) == 1:
            section, name = "", args[0]
        elif len(args) == 2:
            section, name = args
        else:
            raise TypeError("expected ([section,] key)")
        k = _lookup(self._keys, section, name)
        if k is None:
            raise ConfigError(f"unknown config key {(section + '.' if section else '') + name}")
        return k

    def get(self, *args):
        k = self._key(args)
        return self._values[(k.section, k.name)]

    def source(self, *args):
        k = self._key(args)
        return self._origins[(k.section, k.name)]

    def path(self, *args):
        value = self.get(*args)
        if not value:
            return None
        p = Path(value)
        return p if p.is_absolute() else self.root / p

    def tool(self, name):
        llvm = self.path("llvm_dir")
        if llvm is None:
            raise ConfigError("llvm_dir is not set: put it in civ5re.local.cfg or env CIV5RE_LLVM_DIR (see civ5re.local.cfg.example)")
        return llvm / "bin" / (name + (".exe" if os.name == "nt" else ""))

    def missing_required(self):
        return [k for k in self._keys if k.required and not self._values[(k.section, k.name)]]


def _read_ini(path, keys, allowed):
    text = path.read_text(encoding="utf-8-sig")
    parser = configparser.RawConfigParser(interpolation=None, strict=True, delimiters=("=",))
    parser.optionxform = str
    try:
        parser.read_string(f"[{_FLAT}]\n{text}", source=path.name)
    except configparser.Error as e:
        raise ConfigError(f"{path.name}: {str(e).splitlines()[0]}") from None
    out = {}
    for sec in parser.sections():
        section = "" if sec == _FLAT else sec
        for name, value in parser.items(sec):
            k = _lookup(keys, section, name)
            label = f"[{section}] {name}" if section else name
            if k is None:
                raise ConfigError(f"{path.name}: unknown key {label}")
            if k.file not in allowed:
                raise ConfigError(
                    f"{path.name}: local key {label} is not allowed here "
                    f"(set it in {LOCAL_FILE} or env {k.env})")
            out[(section, name)] = value.strip()
    return out


def _validate(k, value, origin):
    if value and k.kind == "int" and not re.fullmatch(r"\d+", value):
        raise ConfigError(f"{k.label} = {value!r} ({origin}) must be a non-negative integer (env {k.env})")
    if value and k.choices and value not in k.choices:
        raise ConfigError(
            f"{k.label} = {value!r} ({origin}) must be one of {', '.join(k.choices)} (env {k.env})")


def load(root=None, environ=None, keys=KEYS):
    check_collisions(keys)
    root = Path(root) if root is not None else ROOT
    environ = os.environ if environ is None else environ
    values = {(k.section, k.name): k.default for k in keys}
    origins = {(k.section, k.name): "default" for k in keys}

    layers = ((SHARED_FILE, (SHARED,)), (LOCAL_FILE, (SHARED, LOCAL)))
    for fname, allowed in layers:
        p = root / fname
        if p.is_file():
            for ident, value in _read_ini(p, keys, allowed).items():
                values[ident] = value
                origins[ident] = fname
    for k in keys:
        if k.env in environ:
            values[(k.section, k.name)] = environ[k.env].strip()
            origins[(k.section, k.name)] = "env"
    for k in keys:
        ident = (k.section, k.name)
        _validate(k, values[ident], origins[ident])
    return Config(root, values, origins, keys)


_HEADER = re.compile(r"^\s*\[([^\]]+)\]\s*$")


def set_key(file, section, key, value, keys=KEYS):
    k = _lookup(keys, section, key)
    if k is None:
        raise ConfigError(f"unknown config key {(section + '.' if section else '') + key}")
    if "\n" in value or "\r" in value:
        raise ConfigError("value must be a single line")
    path = Path(file)
    text = path.read_bytes().decode("utf-8") if path.exists() else ""
    nl = "\r\n" if "\r\n" in text else "\n"
    lines = text.splitlines(keepends=True)
    if lines and not lines[-1].endswith(("\n", "\r")):
        lines[-1] += nl
    new_line = f"{key} = {value}{nl}"
    key_re = re.compile(rf"^\s*{re.escape(key)}\s*=")

    cur = ""
    header_idx = None if section else -1
    last_key_idx = None
    first_header = len(lines)
    for i, line in enumerate(lines):
        m = _HEADER.match(line)
        if m:
            if cur == "" and first_header == len(lines):
                first_header = i
            cur = m.group(1)
            if cur == section:
                header_idx = i
            continue
        if cur == section and line.strip() and not line.lstrip().startswith(("#", ";")):
            last_key_idx = i
            if key_re.match(line):
                lines[i] = new_line
                path.write_text("".join(lines), encoding="utf-8", newline="")
                return
    if header_idx is None:
        if lines and lines[-1].strip():
            lines.append(nl)
        lines += [f"[{section}]{nl}", new_line]
    else:
        if last_key_idx is not None:
            pos = last_key_idx + 1
        elif section:
            pos = header_idx + 1
        else:
            pos = first_header
        lines.insert(pos, new_line)
    path.write_text("".join(lines), encoding="utf-8", newline="")


def local_template(keys=KEYS):
    out = [
        "# Machine-specific settings, not in git. Never overwritten by bootstrap.",
        "# Precedence: civ5re.cfg < this file < env CIV5RE_<KEY> / CIV5RE_<SECTION>_<KEY>.",
        "",
    ]
    sections = []
    for k in keys:
        if k.file == LOCAL and k.section not in sections:
            sections.append(k.section)
    sections.sort(key=lambda s: s != "")
    for section in sections:
        if section:
            out.append(f"[{section}]")
        for k in keys:
            if k.file == LOCAL and k.section == section:
                req = " Required." if k.required else ""
                out += [f"# {k.doc}{req} Env: {k.env}", f"{k.name} = {k.default}", ""]
    return "\n".join(out)
