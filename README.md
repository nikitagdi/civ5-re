# civ5-re

Partial reverse engineering of Sid Meier's Civilization V (Expansion 2 / Brave New World, Linux build 1.0.3.279).

- `Civ5Expansion2/` is a reconstruction of the engine and game-core headers (and some sources) that mirrors the original
  layout. Every declaration carries a `///` marker that says where it comes from (`origin binary` or `origin guess`)
  and ties it to the shipped binary (addresses, sizes, vtables).
- `tools/` holds the Python and Java tooling that builds facts from the binary, applies them to a Ghidra project and
  checks the reconstruction against them.
- Binaries are not stored. `bins.lock` lists the expected files with sha256 and size.

Unofficial fan project, not affiliated with or endorsed by Firaxis Games, 2K, Aspyr or Take-Two Interactive.
See [NOTICE.md](NOTICE.md) for licensing and trademarks. **Civ5Expansion2/ is not MIT-licensed.**

## Requirements

- Python 3.11 or newer
- Ghidra 12.1.x (only for the `ghidra` commands)
- LLVM/clang 22 (`clang++`, `ld.lld`, `llvm-cxxfilt`)
- Your own copy of Civilization V on Steam. The target is the Aspyr Linux build; fetch it with SteamCMD:
  `download_depot 8930 282301` (app 8930, depot 282301, Linux 1.0.3.279), and put it under
  `<bin_dir>/linux_1.0.3.279/`. `bins.lock` has the expected sha256 of every file. Entries with role `windows` are
  optional (Steam Windows depot files, Lekmod builds).

## Setup

```
python tools/bootstrap.py
```

creates `.venv`, installs the `.githooks` pre-commit check (`core.hooksPath`), writes an empty `civ5re.local.cfg`
and verifies `bins.lock` against `bin_dir`. Machine-specific paths live in `civ5re.local.cfg` (git-ignored) or in
environment variables (`CIV5RE_<KEY>`); see `civ5re.local.cfg.example`:

| Key | Needed by |
|---|---|
| `llvm_dir` | binfacts, typedb, check, seed |
| `ghidra_dir` | ghidra * |
| `ghidra_project_dir` (required) | ghidra * |
| `bin_dir` | binfacts (rebuild), ghidra * |
| `game_dir` | lua-api |
| `[seed] lekmod_dir`, `libcxx_src`, `libc_src` | seed |
| `[lua-api] out` | optional |

Unset keys give an error that names the key and the environment variable.

## Commands

Run from the repository root with the venv Python: `.venv/Scripts/python -m civ5re <command>` (Windows) or
`.venv/bin/python -m civ5re <command>`.

| Command | Purpose |
|---|---|
| `binfacts` (`report`, `find`, `bytes`, `vtable`) | Civ5XP -> `build/binfacts.json`; query symbols, bytes, vtables |
| `typedb` | header tree -> `build/typedb.json` (clang AST) |
| `ghidra` (`rebuild`, `apply`, `verify`, `functions`, `decomp`, `paramid`) | sync names/types into a Ghidra project, headless decompile |
| `check` | run the rule catalog over the tree (`--staged` for the pre-commit hook) |
| `selftest` | run the rule and plugin fixtures |
| `seed` (`env`, `tree`) | regenerate `third_party/` and the SDK copies, scaffold exe classes (needs a Lekmod checkout) |
| `lua-api` | Lua API contract -> `build/lua-api.json`, LuaLS stubs, coverage |

Tests: `python -m unittest discover -s tools` (tests that need the game, the binary or a Lekmod checkout skip
themselves when it is missing).

## Layout

```
Civ5Expansion2/   reconstructed source tree (see NOTICE.md)
third_party/      headers needed to run clang over the tree on Linux/glibc/libc++ (see third_party/MANIFEST.txt)
tools/            civ5re (CLI, config, bins.lock), binfacts, typedb, ghidra, checks, luaapi, seed
bins.lock         expected binaries (sha256, size, role)
civ5re.cfg        shared settings
.githooks/        pre-commit (rule check), post-commit (Ghidra apply)
```
