# Notice

## Scope of the licenses

- The MIT license in `LICENSE` covers `tools/` and the repository's own scripts and configuration
  (`tools/bootstrap.py`, `.githooks/`, `civ5re.cfg`, `bins.lock`).
- `Civ5Expansion2/` is a reconstruction made for interoperability research. It contains material derived from the
  Civilization V SDK (copyright Take-Two Interactive Software, Inc. / Firaxis Games, Inc.) and from analysis of the shipped
  game binary. It is **not** covered by the MIT license and carries no license grant from this project. Use of that
  material is subject to the rights of the original copyright holders.
- `third_party/` holds unmodified third-party headers under their own licenses (below).

## Third-party components

| Path | Component | License |
|---|---|---|
| `third_party/libcxx/` | LLVM libc++ headers (LLVM 18) | Apache-2.0 WITH LLVM-exception (`third_party/libcxx/LICENSE.TXT`) |
| `third_party/libc/` | GNU C Library 2.39 headers | LGPL-2.1-or-later (`third_party/libc/COPYING.LIB`); Linux UAPI headers under GPL-2.0 WITH Linux-syscall-note, see `third_party/libc/NOTE.txt` |
| `third_party/lua-5.1.4/` | Lua 5.1.4 headers, Lua.org, PUC-Rio | MIT (`third_party/lua-5.1.4/COPYRIGHT`) |
| `Civ5Expansion2/FirePlace/FireWorks/EventSystem/FastDelegate.h` | FastDelegate, Don Clugston (CodeProject, 2004) | terms not stated in the file header; part of the Civ5Expansion2 material above |

`third_party/compat`, `third_party/fwd`, `third_party/pch.h` and `third_party/include_dirs.txt` are generated stubs
written for this project (MIT).

## Legal note

Unofficial fan project, not affiliated with or endorsed by Firaxis Games, 2K, Aspyr or Take-Two Interactive.
Sid Meier's Civilization V, Civ, Civilization, 2K Games, Firaxis Games, Take-Two Interactive Software and their logos are
trademarks of Take-Two Interactive Software, Inc. This repository contains no game assets; you need your own legally
obtained copy of the game.
