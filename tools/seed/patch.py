"""Unified diff applier without fuzz and without offsets."""
import re

_HUNK = re.compile(r"^@@ -(\d+)(?:,(\d+))? \+(\d+)(?:,(\d+))? @@")


class PatchError(Exception):
    pass


class FilePatch:
    def __init__(self, old, new):
        self.old, self.new = old, new   # paths without a/ b/ ; old is None for a new file
        self.hunks = []                 # (old_start, old_count, new_start, new_count, lines)

    @property
    def path(self):
        return self.new


def _strip(path):
    path = path.split("\t")[0].strip()
    if path == "/dev/null":
        return None
    if path[:2] in ("a/", "b/"):
        path = path[2:]
    return path


def parse(text):
    """Returns a list of FilePatch."""
    lines = text.split("\n")
    if lines and lines[-1] == "":
        lines.pop()
    out, cur, i = [], None, 0
    while i < len(lines):
        line = lines[i]
        if line.startswith("--- ") and i + 1 < len(lines) and lines[i + 1].startswith("+++ "):
            cur = FilePatch(_strip(line[4:]), _strip(lines[i + 1][4:]))
            if cur.new is None:
                raise PatchError("file deletion is not supported")
            out.append(cur)
            i += 2
            continue
        m = _HUNK.match(line)
        if m:
            if cur is None:
                raise PatchError(f"patch line {i + 1}: hunk before file header")
            os_, oc, ns, nc = int(m[1]), m[2], int(m[3]), m[4]
            oc = 1 if oc is None else int(oc)
            nc = 1 if nc is None else int(nc)
            body, i = [], i + 1
            seen_old = seen_new = 0
            while i < len(lines) and (seen_old < oc or seen_new < nc or lines[i].startswith("\\")):
                b = lines[i]
                if b.startswith("\\"):
                    if body:
                        body[-1] = (body[-1][0], body[-1][1], True)
                    i += 1
                    continue
                tag, rest = (b[:1] or " "), b[1:]
                if tag == " " or b == "":
                    seen_old += 1
                    seen_new += 1
                    body.append((" ", rest, False))
                elif tag == "-":
                    seen_old += 1
                    body.append(("-", rest, False))
                elif tag == "+":
                    seen_new += 1
                    body.append(("+", rest, False))
                else:
                    raise PatchError(f"patch line {i + 1}: bad hunk line")
                i += 1
            if seen_old != oc or seen_new != nc:
                raise PatchError(f"patch line {i + 1}: hunk length mismatch")
            cur.hunks.append((os_, oc, ns, nc, body))
            continue
        i += 1
    return out


def apply_text(text, fp):
    """Applies one FilePatch to text; every hunk must match exactly at its stated line."""
    if fp.old is None:
        if text:
            raise PatchError(f"{fp.path}: new file already exists")
        added = []
        for _, _, _, _, body in fp.hunks:
            added += [r for t, r, _ in body if t == "+"]
        return "\n".join(added) + "\n"
    ends_nl = text.endswith("\n")
    src = text.split("\n")
    if ends_nl:
        src.pop()
    out, pos = [], 0
    for os_, oc, _, _, body in fp.hunks:
        start = os_ - 1 if oc else os_
        if start < pos:
            raise PatchError(f"{fp.path}: overlapping hunk at line {os_}")
        out.extend(src[pos:start])
        k = start
        for tag, rest, _ in body:
            if tag in (" ", "-"):
                if k >= len(src) or src[k] != rest:
                    got = src[k] if k < len(src) else "<eof>"
                    raise PatchError(f"{fp.path}:{k + 1}: context mismatch: expected {rest!r}, found {got!r}")
                if tag == " ":
                    out.append(rest)
                k += 1
            else:
                out.append(rest)
        pos = k
    out.extend(src[pos:])
    return "\n".join(out) + ("\n" if ends_nl or not text else "")


def apply(files, patch_text):
    """files: dict path -> text. Returns a new dict with the patched texts."""
    result = dict(files)
    for fp in parse(patch_text):
        if fp.old is not None and fp.path not in result:
            raise PatchError(f"{fp.path}: file not found")
        result[fp.path] = apply_text(result.get(fp.path, ""), fp)
    return result


def stats(patch_text):
    fps = parse(patch_text)
    return {"files": len(fps), "hunks": sum(len(f.hunks) for f in fps)}
