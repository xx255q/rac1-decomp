Drafts only: nothing here is built into the game or changes the image. These are closer starting points for ten executable functions that aren't exact yet. Each file has a `NON_MATCHING` header in `tools/nonmatching.py`'s format (verdict, date, and what the attempts found), so the next attempt starts from them instead of from scratch.

| Function | What | Verdict | Before |
|---|---|---|---|
| `func_001211B0` | libcdvd `sceCdInit` | BYTES 15/736 | no draft |
| `func_0011BCB0` | libkernl `sceFsInit` | BYTES 18/516 | SIZE 488/516 |
| `func_00121750` | libcdvd `sceCdRead` | SIZE 484/480 | SIZE 460/480 |
| `func_0011CB40` | `sceSifInitIopHeap` | BYTES 9/136 | SIZE 204/136 |
| `func_0012C608` | libmpeg picture-size setup | BYTES 358/680 | no draft |
| `func_0022FDC0` | game/space | SIZE 2004/2012 | SIZE 2040/2012 |
| `func_00121DC8` | libgraph `sceGsSetDefDispEnv` | BYTES 25/884 | functional only |
| `func_0012DB68` | 989snd `snd_StartSoundSystem` | BYTES 17/600 | SIZE 616/600 |
| `func_0022F738` | game/space engine trails | SIZE 1180/1188 | functional only |
| `func_00125F88` | libmpeg `_getRef0` | SIZE 1028/1052 | SIZE 968/1052 |

Notes in the headers, in short:
- **sceCdInit:** from Lombyte's NON_MATCHING draft, PAL names. Only the lui register numbering in the opening reset block differs. 120 declaration orders and about 3,700 permuter iterations didn't move it.
- **sceFsInit:** `memcpy(…, 4)` for the two reply words gives retail's `lwl/lwr/swl/swr`. Only the prologue save order is left.
- **sceCdRead:** from Lombyte's NON_MATCHING draft. It reproduces retail's duplicated `mult`. What's left is where 0x918 lives (`$a3` vs `$v0`) and the semaphore load not landing in the jal slot.
- **sceSifInitIopHeap:** Lombyte's match. 1/136 under `-fno-schedule-insns`, but `00119D88.c` can't take that flag as a whole.
- **func_0012C608:** written from the assembly; the logic is complete, and scheduling and register choice are left.
- **func_0022FDC0:** from Lombyte's draft with PAL symbols; register allocation of the level-render-state base across `func_0012EC30` is left.
- **sceGsSetDefDispEnv:** written from the assembly only (no SDK source). Every instruction is in retail order; only register choice differs in the non-interlaced NTSC/PAL blocks. ~11,700 permuter iterations found nothing better.
- **snd_StartSoundSystem:** from Lombyte, without `#define`. Only the order of six buffer stores before `sceSifInitRpc` differs; all 720 orders were tried.
- **Engine trails:** written from the assembly. Logic and calls match; retail keeps the outer index in a saved register and spills `i+1`/`j+1`.
- **_getRef0:** written from the assembly with the file's `MpegDecoder`/`MpegMbBuffer`/`MpegMcFetch` types. Retail spills `bx` to the stack at once; ours keeps it in a register.

`tools/nonmatching.py stage` only accepts level functions, so these were written with its `header()` directly. The `functional.tsv` rows for files that already had one keep pointing at them. `gen_progress_report.py --check` is unaffected.

🤖 Generated with [Claude Code](https://claude.com/claude-code)

https://claude.ai/code/session_01DPvuaLocudQk662EsHkTtp
