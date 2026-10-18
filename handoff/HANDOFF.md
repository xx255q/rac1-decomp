# rac1-decomp: where we are (handoff, 2026-10-18)

Jacob (GitHub **xx255q**) contributes to **OpenRAC/rac1-decomp**, the matching
decompilation of Ratchet & Clank (PS2, PAL v2.00 `SCES_509.16`). Claude does
the decompiling; Jacob opens PRs and the maintainers (Kryštof Malinda) merge.
Paste or attach this file at the start of a new chat to pick up from here.

## Status

| What | State |
|---|---|
| PR #90 (7 matches, 9 remnant splits) | **merged** |
| PR #91 (`_type2id`, `_lastFrame`, `_printf`, `snd_SendCurrentBatch`, `GetEffectTex`) | **merged** 2026-10-18 (merge `fc9e1e5b`) |
| Branch `near-miss-drafts` on xx255q's fork: 10 NON_MATCHING drafts | pushed and rebased on main (`75378ebb`); **no PR opened yet**. PR text: `DRAFTS_PR_BODY.md` |
| Executable after #91 | 1199/1199 decompiled functions exact; report 4746/5118 |

12 executable functions matched in total (PR #90: 7, PR #91: 5).

Open the drafts PR at:
https://github.com/OpenRAC/rac1-decomp/compare/main...xx255q:rac1-decomp:near-miss-drafts

## Near-miss drafts (on `near-miss-drafts`)

Each file has a header saying what was tried. Best results:

| Function | What | Best | What's left |
|---|---|---|---|
| func_00121DC8 | sceGsSetDefDispEnv (libgraph) | BYTES 25/884 | only register choice in the non-interlaced blocks |
| func_0012DB68 | snd_StartSoundSystem (989snd) | BYTES 17/600 | order of 6 buffer stores (all 720 orders tried) |
| func_0011CB40 | sceSifInitIopHeap | BYTES 9/136 | needs `-fno-schedule-insns`, which `00119D88.c` can't take whole |
| func_001211B0 | sceCdInit | BYTES 15/736 | lui register numbering in the reset block |
| func_0011BCB0 | sceFsInit | BYTES 18/516 | prologue save order |
| func_00121750 | sceCdRead | SIZE 484/480 | where 0x918 lives; semaphore load slot |
| func_0012C608 | libmpeg picture-size setup | BYTES 358/680 | scheduling, registers |
| func_0022FDC0 | game/space | SIZE 2004/2012 | register for the level-render-state base |
| func_0022F738 | engine trails | SIZE 1180/1188 | retail keeps i in $s5 and spills i+1/j+1 |
| func_00125F88 | _getRef0 (libmpeg) | SIZE 1028/1052 | retail spills bx to 0x4($sp) at once |

Known walls (documented): `func_0011B868` `_sceFs_Rcv_Intr` is EXACT but its
jump table belongs to a different retail object than kprintf.o (needs a split
of `00119D88.c` or per-table sections; left to the maintainers).
`func_0012AAA8` and `func_00119EA8` need the removed `fix_trunc_slot` step.
`func_0012DA28` is a bare tail jump 2.95.3 can't emit.

Next candidates: `python3 tools/rank_candidates.py`, and
`python3 tools/lombyte.py todo` for functions Lombyte matched that we haven't.
The movie functions in that list (`0023B…`–`0023D…`) must NOT be ported from
Lombyte (see rules).

## Rules (project and Jacob's)

- Commit as **`Jacob <xx255q@users.noreply.github.com>`** (not his Gmail), with
  the `Co-Authored-By` / `Claude-Session` trailers.
- Never commit `baserom/`, `toolchain/`, `build-sn/`, `asm/`, `tools/ext/` or
  any retail bytes.
- Never use Sony SDK source, samples or leaked material. SDK functions (libcdvd,
  libgraph, libmpeg, libkernl, 989snd) are written from the assembly, or ported
  from Lombyte's matched C.
- Movie code (`src/game/movie/`, ezmpeg) is written from the assembly only: no
  Lombyte ports there.
- Lombyte (MIT, github.com/mateuszklysz/Lombyte) ports are fine but must say
  `Adapted from Lombyte (MIT): <path>, <name>.` in the comment above the function.
- `tools/integrate.py` refuses `#define`, inline asm and barriers
  (`do {} while (0)`, empty asm). Write those out in plain C.
- A definition whose C name differs from its symbol uses the `func_X_r` name
  (the audit regex only counts `func_X(` or `func_X_r(`).
- Run only ONE compile-heavy job at a time: two permuters at once deadlocked
  Wine once.

## Setting up a new session (Linux cloud container)

Start the session with **OpenRAC/rac1-decomp** as its repository (then Claude
can open and edit PRs itself; a session started from the fork can't attach
OpenRAC's repo because both are named `rac1-decomp`). Add the fork as a remote:
`git remote add fork https://github.com/xx255q/rac1-decomp`.

1. **Base ROM:** Jacob must upload `SCES_509.16` again (sha1
   `79956931bd62fafd8d20fa2eae796dbaf2e15e83`, 1,388,100 bytes; the other copy,
   1,381,764 bytes, is the wrong version) → `baserom/SCES_509.16`.
2. **Toolchains** (README step 3):
   `git clone https://github.com/AngheloAlf/SN-Systems-ProDG_for_PS2_3.01 toolchain/sn-prodg-3.01`
   `git clone https://github.com/AngheloAlf/sce_ps2_sdk_24 toolchain/sn-prodg-24`
   plus decompme/compilers' **`ee-gcc2.9-991111-01.tar.xz`** unpacked to
   `toolchain/ee-gcc-2.9-991111-01` (libgcc and libmpeg's object need it).
3. **Wine** (no Docker in the cloud): `dpkg --add-architecture i386 && apt-get update &&
   apt-get install wine wine32:i386` (Wine 9.0 worked),
   `export WINEDEBUG=-all WINEPREFIX=/opt/wineprefix` for every build command.
4. **Python:** `pip install -r requirements.txt rabbitizer pyelftools` (a venv at
   `/opt/venv` was used), then `bash tools/setup_asm.sh`.
5. **Permuter:** `apt-get install binutils-mips-linux-gnu`, and rename
   `tools/ext/mips-binutils/usr/bin` to `bin.vendored` (the vendored objdump lacks
   libopcodes here).
6. **Lombyte:** `git clone https://github.com/mateuszklysz/Lombyte ~/Projects/Lombyte`
   and `git -C ~/Projects/Lombyte fetch origin progress:progress` (tools/lombyte.py
   reads it).
7. **Check:** `bash tools/build_sn.sh` must say every audited function is exact.

The level dumps (`baserom/overlays/`) need the full ISO, which hasn't been
uploaded (it's >500 MB; split it with 7-Zip into 450 MB parts if needed). Without
them, level-code results in `progress/report.json` are carried over from main.

## Workflow

```bash
python tools/try_func.py func_X cand.c [--arm=NAME] [--diff]   # EXACT / BYTES n/size / SIZE ours/retail
echo "func_X cand.c" > m.txt && python tools/integrate.py m.txt --apply   # land an EXACT candidate
bash tools/build_sn.sh                                         # full build + audit (must be all exact)
python3 tools/permuter_setup.py func_X cand.c && bash tools/permuter_run.sh build-sn/permuter/func_X -j2 --stop-on-zero
```

Progress report on Linux: the stock `tools/gen_progress_report.py` assumes
Windows `make.exe`/`gen_ld.py` and the level dumps. Apply
`gen_progress_report_linux.patch` to a copy, put main's committed
`progress/report.json` at `build-sn/report_main.json`, then run the copy with
`PYTHONPATH=tools python <copy> --no-build` after a full build, and `--check`.

Helper scripts in `tools/` here (need `rabbitizer`, `pyelftools`):
- `align.py FUNC obj.o` aligns our compiled function with retail by opcode and shows differing runs.
- `odis.py FUNC obj.o [start end]` disassembles our compiled function.
- `score.sh FUNC cand.c …` runs try_func on many candidates and prints an opcode-diff score (lower is better). Good for sweeping generated variants.

Matching tricks that worked in this codebase:
- `long long` is 128-bit on EE gcc: use `u64` for 64-bit GS words.
- `MACRO_ADDR` for retail's one-register `lui`/`lw`; `SDATA(sym)` or `extern short` for `$gp`.
  The assembler goes by the *last* `.extern` size for a symbol in the file.
- A call in each branch (`if (c) x = f(0); else x = f(0x13);`) gives retail's branchy
  layout where a single call with a variable gives `movz`.
- Nested `if`s rather than `?:` when retail branches instead of using `movn`/`movz`.
- Writing a value inline vs. in a variable changes register choice and tail sharing
  (sceGsSetDefDispEnv went from 44 to 25 bytes that way).
- 2.9-ee folds `(x + c1) + y` constants outward; a separate `long` variable keeps
  retail's grouping.
