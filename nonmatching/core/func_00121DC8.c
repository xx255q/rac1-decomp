/* NON_MATCHING func_00121DC8 -- src/core/00121D18.c
 * Best so far: BYTES 25/884 (97.2% of the bytes match), checked 2026-10-18.
 * Not built into anything: the retail assembly stays in the source file
 * until a candidate is EXACT (docs/NONMATCHING.md). Start from this one.
 * What the last attempts found:
 *   sceGsSetDefDispEnv, written from the assembly only (libgraph; no SDK source used).
 *   Every instruction is in retail order; only registers differ in the non-interlaced NTSC/PAL
 *   blocks and their shared tail (scale factor in $a0 not $v0; DH/product swap $a2/$a3).
 *   Needed: nested ifs for SMODE2, block-local magh/ox with ox = (long)off[0] + const (64-bit),
 *   scale written inline in the non-interlaced blocks, 480p dx*2+232 in its own long variable.
 *   Field order WXHYM (non-interlaced) and MHXYW (interlaced) were the best of 120 each.
 *   ~11,700 permuter iterations from this version found nothing better.
 */

typedef struct {
    short interlace;
    short omode;
    short ffmode;
} GsGParam_DC8;

typedef struct {
    u64 pmode;
    u64 smode2;
    u64 dispfb;
    u64 display;
    u64 bgcolor;
} GsDispEnv_DC8;

extern GsGParam_DC8 *func_00121D08_dc8(void) __asm__("func_00121D08");
extern void func_00121DB8_dc8(int, int *, int *, int *, int *) __asm__("func_00121DB8");
extern char D_00153140[];

/* sceGsSetDefDispEnv: fill a display environment for a psm frame buffer of
   w x h shown at (dx, dy): PMODE, SMODE2 from the interlace and field
   settings, DISPFB, DISPLAY placed for NTSC (2), PAL (3) or 480p (0x50)
   with the console's own offsets (asked of func_00121DB8 when
   func_00121D18 allows, zero otherwise), and a black background.
   Written from the retail routine. */
void func_00121DC8(GsDispEnv_DC8 *disp, short psm, short w, short h, short dx, short dy) {
    GsGParam_DC8 *gp = func_00121D08_dc8();
    int off[4];

    if (gp->omode != 2 && gp->omode != 3 && func_00121D18()) {
        func_00121DB8_dc8(gp->omode, &off[0], &off[1], &off[2], &off[3]);
    } else {
        off[3] = 0;
        off[2] = 0;
        off[1] = 0;
        off[0] = 0;
    }
    disp->pmode = 0x66;
    if (gp->interlace) {
        if (gp->ffmode == 0) {
            disp->smode2 = 1;
        } else {
            disp->smode2 = 3;
        }
    } else {
        disp->smode2 = 2;
    }
    disp->dispfb = ((u64)(psm & 0xF) << 15) | ((u64)(((w + 63) >> 6) & 0x3F) << 9);
    if (gp->omode == 2) {
        if (gp->interlace == 1) {
            int magh = (w + 2559) / w;
            long ox = (long)off[0] + 636;
            disp->display = ((u64)(magh - 1) << 23) | ((u64)(gp->ffmode ? h * 2 - 1 : h - 1) << 44) | ((dx * magh + ox) & 0xFFF) | ((u64)((dy + off[1] + 50) & 0xFFF) << 12) | ((u64)(magh * w - 1) << 32);
        } else {
            long ox = (long)off[0] + 636;
            disp->display = ((u64)(((w + 2559) / w) * w - 1) << 32) | ((dx * ((w + 2559) / w) + ox) & 0xFFF) | ((u64)(h - 1) << 44) | ((u64)((dy + off[1] + 25) & 0xFFF) << 12) | ((u64)(((w + 2559) / w) - 1) << 23);
        }
    } else if (gp->omode == 3) {
        if (gp->interlace == 1) {
            int magh = (w + 2559) / w;
            long ox = (long)off[0] + 656;
            disp->display = ((u64)(magh - 1) << 23) | ((u64)(gp->ffmode ? h * 2 - 1 : h - 1) << 44) | ((dx * magh + ox) & 0xFFF) | ((u64)((dy + off[1] + 72) & 0xFFF) << 12) | ((u64)(magh * w - 1) << 32);
        } else {
            long ox = (long)off[0] + 656;
            disp->display = ((u64)(((w + 2559) / w) * w - 1) << 32) | ((dx * ((w + 2559) / w) + ox) & 0xFFF) | ((u64)(h - 1) << 44) | ((u64)((dy + off[1] + 36) & 0xFFF) << 12) | ((u64)(((w + 2559) / w) - 1) << 23);
        }
    } else if (gp->omode == 0x50) {
        long dx2 = (long)(dx * 2) + 232;
        disp->display = ((u64)(1) << 23) | ((u64)(h - 1) << 44) | (((720 - w) / 2 * 2 + (long)off[0] + dx2) & 0xFFF) | ((u64)((dy + off[1] + 35) & 0xFFF) << 12) | ((u64)(w * 2 - 1) << 32);
    } else {
        func_0011A6C8(D_00153140);
    }
    disp->bgcolor = 0;
}
