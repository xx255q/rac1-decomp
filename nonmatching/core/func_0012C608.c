/* NON_MATCHING func_0012C608 -- src/core/0012AC80.c
 * Best so far: BYTES 358/680 (47.4% of the bytes match), checked 2026-10-18.
 * Not built into anything: the retail assembly stays in the source file
 * until a candidate is EXACT (docs/NONMATCHING.md). Start from this one.
 * What the last attempts found:
 *   Written from the assembly (libmpeg picture-size setup); same size, logic complete.
 *   Left: the six default stores' order, `addiu $v0,$fp,0x528` hoisted into both compare delay slots,
 *   and mult order (retail h*0x180 first, then *w). Hoisting d+0x528 into a local helps the slots but costs elsewh
 */
extern void func_0012BD50_sz(void *) __asm__("func_0012BD50");
extern unsigned int func_0012BD60_sz(void *, char *, unsigned int, unsigned int) __asm__("func_0012BD60");
extern int func_0012C4C0_sz(void *, int, int) __asm__("func_0012C4C0");
extern void func_0012C8B0_sz(void *, void *, void *, void *, void *, void *, void *, void *,
                             void *, unsigned int, unsigned int, unsigned int, int, int) __asm__("func_0012C8B0");

/* Picture-size setup (libmpeg): from the decoder's pixel width and height
   (+0x124, +0x128) derive the size in macroblocks (+0x12C, +0x130; frame
   pictures round the height to 32 lines when the sequence is not
   progressive), and when the size in pixels changed, record it in the
   caller's object, reallocate the three reference frames and lay out the
   picture planes for the new size. An object with no sequence header yet
   (+0x848 == 0) first gets MPEG-1 defaults. */
void func_0012C608(void *argp) {
    int *arg0 = (int *)argp;
    char *d = *(char **)((char *)arg0 + 0x40);
    int seq = *(int *)(d + 0x848);
    int mbh;
    int w;
    int h;
    unsigned int size;

    if (seq == 0) {
        *(int *)(d + 0x188) = 1;
        *(int *)(d + 0x140) = 1;
        *(int *)(d + 0x13C) = 1;
        *(int *)(d + 0x144) = 5;
        *(int *)(d + 0x17C) = 1;
        *(int *)(d + 0x174) = 3;
    }
    *(int *)(d + 0x12C) = (*(int *)(d + 0x124) + 15) >> 4;
    if (seq != 0 && *(int *)(d + 0x13C) == 0) {
        mbh = ((*(int *)(d + 0x128) + 31) >> 5) * 2;
    } else {
        mbh = (*(int *)(d + 0x128) + 15) >> 4;
    }
    *(int *)(d + 0x130) = mbh;
    h = mbh * 16;
    w = *(int *)(d + 0x12C) * 16;
    if (w != arg0[0] || h != arg0[1]) {
        arg0[1] = h;
        arg0[0] = w;
        size = (unsigned int)(w * (h * 0x180)) >> 8;
        func_0012BD50_sz(d + 0x108);
        *(unsigned int *)(d + 0xFC) = func_0012BD60_sz(d, d + 0x108, size, 0x40);
        *(unsigned int *)(d + 0x100) = func_0012BD60_sz(d, d + 0x108, size, 0x40);
        *(unsigned int *)(d + 0x104) = func_0012BD60_sz(d, d + 0x108, size, 0x40);
        func_0012C8B0_sz(d + 0x1E8, d + 0x250, d + 0x2B8, d + 0x320, d + 0x388, d + 0x3F0,
                         d + 0x458, d + 0x4C0, d + 0x528,
                         *(unsigned int *)(d + 0xFC), *(unsigned int *)(d + 0x100),
                         *(unsigned int *)(d + 0x104), w, h);
        func_0012C4C0_sz(d + 0x1E8, w, h);
        func_0012C4C0_sz(d + 0x250, w, h);
        func_0012C4C0_sz(d + 0x2B8, w, h);
        func_0012C4C0_sz(d + 0x320, w, h >> 1);
        func_0012C4C0_sz(d + 0x388, w, h >> 1);
        func_0012C4C0_sz(d + 0x3F0, w, h >> 1);
        func_0012C4C0_sz(d + 0x458, w, h >> 1);
        func_0012C4C0_sz(d + 0x4C0, w, h >> 1);
        func_0012C4C0_sz(d + 0x528, w, h >> 1);
    }
}
