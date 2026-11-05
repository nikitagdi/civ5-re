"""One inter-process exclusive lock around analyzeHeadless runs (build/ghidra/headless.lock), reentrant in-process."""

import os
import threading
import time
from contextlib import contextmanager
from pathlib import Path

if os.name == "nt":
    import msvcrt
else:
    import fcntl

LOCK_NAME = "headless.lock"
WAIT = 600.0
STEP = 0.5

_held = {}  # resolved path -> [fd, depth]
_guard = threading.RLock()


class LockTimeout(Exception):
    pass


def _try_lock(fd):
    try:
        if os.name == "nt":
            os.lseek(fd, 0, os.SEEK_SET)
            msvcrt.locking(fd, msvcrt.LK_NBLCK, 1)
        else:
            fcntl.flock(fd, fcntl.LOCK_EX | fcntl.LOCK_NB)
        return True
    except OSError:
        return False


def _unlock(fd):
    if os.name == "nt":
        os.lseek(fd, 0, os.SEEK_SET)
        msvcrt.locking(fd, msvcrt.LK_UNLCK, 1)
    else:
        fcntl.flock(fd, fcntl.LOCK_UN)


def holder(path):
    """The pid recorded in the lock file, None if unreadable."""
    try:
        with open(path, "rb") as f:
            f.seek(1)  # byte 0 is the locked byte on Windows; the pid follows it
            return int(f.read().decode("ascii", "replace").strip() or "x")
    except (OSError, ValueError):
        return None


@contextmanager
def project_lock(path, timeout=WAIT, out=print, step=STEP):
    """Holds the exclusive lock at `path`; waits up to `timeout` s, telling `out` once who holds it."""
    path = Path(path).resolve()
    with _guard:
        entry = _held.get(path)
        if entry:
            entry[1] += 1
    if entry:
        try:
            yield
        finally:
            with _guard:
                entry[1] -= 1
        return
    path.parent.mkdir(parents=True, exist_ok=True)
    fd = os.open(path, os.O_RDWR | os.O_CREAT, 0o644)
    try:
        deadline = time.monotonic() + timeout
        told = False
        while not _try_lock(fd):
            if not told:
                out(f"waiting for project lock (held by pid {holder(path) or '?'})")
                told = True
            if time.monotonic() >= deadline:
                raise LockTimeout(f"{path} held by pid {holder(path) or '?'} for over {timeout:.0f} s")
            time.sleep(step)
        os.lseek(fd, 1, os.SEEK_SET)
        os.ftruncate(fd, 1)
        os.write(fd, f"{os.getpid()}\n".encode("ascii"))
        entry = [fd, 1]
        with _guard:
            _held[path] = entry
        try:
            yield
        finally:
            with _guard:
                del _held[path]
            _unlock(fd)
    finally:
        os.close(fd)
