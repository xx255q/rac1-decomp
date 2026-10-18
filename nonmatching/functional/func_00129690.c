/* NON_MATCHING func_00129690 -- src/core/00125630.c
 * Best so far: BYTES 206/692 (70.2% of the bytes match), checked 2026-10-18.
 * Not built into anything: the retail assembly stays in the source file
 * until a candidate is EXACT (docs/NONMATCHING.md). Start from this one.
 * What the last attempts found:
 *   Same size now (was SIZE 680/692). Retail stores through rec even when mode is not 1-3 (rec stays NULL).
 *   Fixed: with a1 != 0 retail tests rec 0x1D0 (0x1E0 when mode is 1), not 0x1C8; the slot swap is three pairwise
 *   swaps; the kind != 3 tail sets ret = 1 per test. Left: mode/kind and n/2/3 constants sit in other temporaries,
 *   and retail builds ret = (x == 1) with a movn from a register holding 1 where ours uses sltiu.
 */
int func_00129690(char *p, int a1) {
    int kind;
    int mode;
    int n;
    int ret;
    char *rec;

    ret = 0;
    rec = 0;
    mode = *(int *)(p + 0x174);
    kind = *(int *)(p + 0x150);
    n = (mode == 3) ? 2 : 4;
    if (kind == 3) {
        int a = *(int *)(p + 0x1C4);
        int b = *(int *)(p + 0x1D4);
        int c = *(int *)(p + 0x1E4);
        int less = (*(int *)(p + 0xA0) + *(int *)(p + 0xA4)) < n;
        *(int *)(p + 0x1C0) = a;
        *(int *)(p + 0x1D0) = b;
        *(int *)(p + 0x1E0) = c;
        if (!less) {
            *(int *)(p + 0xE8) = 0;
            *(int *)(p + 0x1A8) = 0;
            *(int *)(p + 0x1A4) = 0;
        }
        if (*(int *)(p + 0xE8) != 0 || *(int *)(p + 0x1A8) != 0) {
            if (*(int *)(p + 0x1A4) == 0) {
                *(int *)(*(char **)(p + 0x1B8) + 0x28) = 0;
                *(int *)(*(char **)(p + 0x1C8) + 0x28) = 0;
                *(int *)(*(char **)(p + 0x1D8) + 0x28) = 0;
                mode = *(int *)(p + 0x174);
            }
        }
        *(int *)(p + 0xE8) = 0;
        *(int *)(p + 0x1A8) = 0;
        if (mode == 3) {
            if (*(int *)(*(char **)(p + 0x1B8) + 0x28) == 1 || *(int *)(p + 0x1A4) != 0) {
                ret = (*(int *)(*(char **)(p + 0x1BC) + 0x28) == 1);
            }
        } else {
            if ((*(int *)(*(char **)(p + 0x1C8) + 0x28) == 1 && *(int *)(*(char **)(p + 0x1D8) + 0x28) == 1) ||
                *(int *)(p + 0x1A4) != 0) {
                if (*(int *)(*(char **)(p + 0x1CC) + 0x28) == 1) {
                    ret = (*(int *)(*(char **)(p + 0x1DC) + 0x28) == 1);
                }
            }
        }
    } else {
        if (a1 == 0) {
            int t;
            t = *(int *)(p + 0x1B8);
            *(int *)(p + 0x1B8) = *(int *)(p + 0x1BC);
            *(int *)(p + 0x1BC) = t;
            t = *(int *)(p + 0x1C8);
            *(int *)(p + 0x1C8) = *(int *)(p + 0x1CC);
            *(int *)(p + 0x1CC) = t;
            t = *(int *)(p + 0x1D8);
            *(int *)(p + 0x1D8) = *(int *)(p + 0x1DC);
            *(int *)(p + 0x1DC) = t;
        }
        *(int *)(p + 0x1C0) = *(int *)(p + 0x1BC);
        *(int *)(p + 0x1D0) = *(int *)(p + 0x1CC);
        *(int *)(p + 0x1E0) = *(int *)(p + 0x1DC);
        if (mode == 3) {
            if (kind != 2) {
                ret = 1;
            } else if (*(int *)(*(char **)(p + 0x1B8) + 0x28) == 1) {
                ret = 1;
            }
        } else {
            char *sel = (mode != 1) ? *(char **)(p + 0x1D0) : *(char **)(p + 0x1E0);
            if (kind != 2) {
                ret = 1;
            } else if (a1 != 0 && *(int *)(sel + 0x28) == 1) {
                ret = 1;
            } else if (*(int *)(*(char **)(p + 0x1C8) + 0x28) == 1 && *(int *)(*(char **)(p + 0x1D8) + 0x28) == 1) {
                ret = 1;
            }
        }
    }
    if (mode == 2)
        rec = *(char **)(p + 0x1E0);
    else if (mode == 1)
        rec = *(char **)(p + 0x1D0);
    else if (mode == 3)
        rec = *(char **)(p + 0x1C0);
    *(int *)(rec + 0x28) = 0;
    *(long long *)(rec + 0x18) = *(long long *)(p + 0x828);
    *(int *)(rec + 0x2C) = *(int *)(p + 0x150);
    *(long long *)(rec + 0x20) = *(long long *)(p + 0x830);
    *(int *)(rec + 0x30) = *(int *)(p + 0x174);
    *(int *)(rec + 0x34) = *(int *)(p + 0x13C);
    *(int *)(rec + 0x38) = *(int *)(p + 0x188);
    *(int *)(rec + 0x3C) = *(int *)(p + 0x178);
    *(int *)(rec + 0x40) = *(int *)(p + 0x184);
    *(int *)(rec + 0x44) = *(int *)(p + 0x18C);
    *(int *)(rec + 0x48) = *(int *)(p + 0x190);
    *(int *)(rec + 0x4C) = *(int *)(p + 0x194);
    *(int *)(rec + 0x50) = *(int *)(p + 0x198);
    *(int *)(rec + 0x54) = *(int *)(p + 0x19C);
    *(int *)(rec + 0x58) = *(int *)(p + 0x1A0);
    *(int *)(rec + 0x5C) = *(int *)(p + 0x148);
    *(int *)(rec + 0x60) = *(int *)(p + 0x14C);
    return ret;
}
