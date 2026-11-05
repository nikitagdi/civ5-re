import hashlib
import tomllib
from dataclasses import dataclass
from pathlib import Path

REQUIRED = ("path", "role", "sha256", "size", "origin")


class BinlockError(Exception):
    pass


@dataclass(frozen=True)
class Entry:
    path: str
    role: str
    sha256: str
    size: int
    origin: str


def load(path):
    try:
        with open(path, "rb") as f:
            data = tomllib.load(f)
    except tomllib.TOMLDecodeError as e:
        raise BinlockError(f"{Path(path).name}: {e}") from None
    entries = []
    for i, raw in enumerate(data.get("file", [])):
        missing = [k for k in REQUIRED if k not in raw]
        if missing:
            raise BinlockError(f"{Path(path).name}: file #{i + 1} lacks {', '.join(missing)}")
        entries.append(Entry(raw["path"], raw["role"], raw["sha256"].lower(), int(raw["size"]), raw["origin"]))
    return entries


def target(entries):
    found = [e for e in entries if e.role == "target"]
    if len(found) != 1:
        raise BinlockError(f"bins.lock must have exactly one role = \"target\" entry, found {len(found)}")
    return found[0]


def sha256_of(path):
    h = hashlib.sha256()
    with open(path, "rb") as f:
        for chunk in iter(lambda: f.read(1 << 20), b""):
            h.update(chunk)
    return h.hexdigest()


def verify(entry, bin_dir):
    """Returns (status, message); status is missing | ok | mismatch."""
    p = Path(bin_dir) / entry.path
    if not p.is_file():
        return "missing", f"{entry.path} not found under {bin_dir}"
    size = p.stat().st_size
    if size != entry.size:
        return "mismatch", f"{entry.path}: size {size} != {entry.size}"
    digest = sha256_of(p)
    if digest != entry.sha256:
        return "mismatch", f"{entry.path}: sha256 {digest} != {entry.sha256}"
    return "ok", f"{entry.path}: sha256 and size verified"
