/* NON_MATCHING func_0022F738 -- src/game/space.c
 * Best so far: SIZE ours 1180 / retail 1188, checked 2026-10-18.
 * Not built into anything: the retail assembly stays in the source file
 * until a candidate is EXACT (docs/NONMATCHING.md). Start from this one.
 * What the last attempts found:
 *   Engine trails, written from the assembly. Structure and calls match; register allocation does not:
 *   retail keeps the outer index in $s5 and spills i+1 (0x130) and j+1 (0x138); ours spills i itself.
 *   Needed: u64 (not unsigned long long) words, one call per texture branch, the trails as one
 *   D_0013E1F0[2][32][4] array, D_001605D0 through $gp (extern short).
 *   ~1,700 permuter iterations: no clean improvement.
 */

typedef struct {
    float pos[4][4];
    unsigned int col[4];
    float st[4][2];
    u64 clamp;
    u64 tex0;
    u64 tex1;
    u64 alpha;
} TrailQuad_2F738 __attribute__((aligned(16)));

extern int D_0015F6E8_2F738 __asm__("D_0015F6E8") MACRO_ADDR;
extern char D_0013E130_2F738[] __asm__("D_0013E130");
extern float D_0013E1F0_2F738[2][32][4] __asm__("D_0013E1F0");
extern float D_001D9DA0_2F738[4][2] __asm__("D_001D9DA0");
extern unsigned int D_001D9DC0_2F738[][2] __asm__("D_001D9DC0");
extern short D_001605D0_2F738 __asm__("D_001605D0");
extern u64 func_001F4868_2F738(int) __asm__("func_001F4868");
extern float func_001FA888_2F738(int) __asm__("func_001FA888");
extern unsigned int func_001FA8A8_2F738(unsigned int, unsigned int, float) __asm__("func_001FA8A8");
extern void func_001F9BD8_2F738(void *, void *, void *) __asm__("func_001F9BD8");
extern void func_001F9BF0_2F738(void *, void *, void *) __asm__("func_001F9BF0");
extern void func_001F9CA0_2F738(void *, void *, void *) __asm__("func_001F9CA0");
extern void func_001F9DC0_2F738(void *, void *, float) __asm__("func_001F9DC0");
extern void func_001F7EF8_2F738(void *, int, int) __asm__("func_001F7EF8");

/* The ship's two engine trails in the space scenes: each trail is the ring
   of 32 positions at D_0013E1F0 + 0x200 * trail (the newest at D_0013E130
   +0x50, the count drawn +0x54), drawn as a ribbon of two quads a segment.
   One quad spans the cross of the trail's direction with the line between
   the two engines, the other that line itself. Each corner sits on one of
   two consecutive positions, pushed out by (1 - t^2) * width, and the first
   quad is tinted from the ship's colour pair by t = age / 32. */
void func_0022F738(s32 unused) {
    TrailQuad_2F738 q;
    float across[4][4];
    float side[4][4];
    int i;
    int j;
    int m;
    int n;
    int k;
    int tex;

    if (D_0015F6E8_2F738 == 6 && *(int *)(D_0013E130_2F738 + 0x20) == 4) {
        q.tex0 = func_001F4868_2F738(0);
    } else {
        q.tex0 = func_001F4868_2F738(0x13);
    }
    q.tex1 = 0xFF9000000260;
    q.alpha = 0x8000000048;
    q.clamp = 0;
    for (n = 0; n < 4; n++) {
        q.st[n][0] = D_001D9DA0_2F738[n][0];
        q.st[n][1] = D_001D9DA0_2F738[n][1];
    }
    for (i = 0; i < *(int *)(D_0013E130_2F738 + 0x54) - 1; i++) {
        int k1;
        int age = i + 1;
        int k2;

        k = (*(int *)(D_0013E130_2F738 + 0x50) - i + 0x1F) & 0x1F;
        k1 = (k + 1) & 0x1F;
        k2 = (k + 2) & 0x1F;
        for (j = 0; j < 2; j++) {
            func_001F9BF0_2F738(across[0], D_0013E1F0_2F738[1][k], D_0013E1F0_2F738[0][k]);
            func_001F9BF0_2F738(across[1], D_0013E1F0_2F738[0][k], D_0013E1F0_2F738[1][k]);
            func_001F9BF0_2F738(across[2], D_0013E1F0_2F738[1][k1], D_0013E1F0_2F738[0][k1]);
            func_001F9BF0_2F738(across[3], D_0013E1F0_2F738[0][k1], D_0013E1F0_2F738[1][k1]);
            func_001F9BF0_2F738(side[1], D_0013E1F0_2F738[j][k1], D_0013E1F0_2F738[j][k]);
            func_001F9BF0_2F738(side[3], D_0013E1F0_2F738[j][k2], D_0013E1F0_2F738[j][k1]);
            if (i == 0) {
                qcopy(side[3], side[1]);
            }
            func_001F9CA0_2F738(side[0], across[0], side[1]);
            func_001F9CA0_2F738(side[1], across[1], side[1]);
            func_001F9CA0_2F738(side[2], across[2], side[3]);
            func_001F9CA0_2F738(side[3], across[3], side[3]);
            for (m = 0; m < 4; m++) {
                float t = func_001FA888_2F738(age - (m >> 1)) * 0.03125f;
                int ship = *(short *)(D_0013E130_2F738 + 0x26);

                q.col[m] = func_001FA8A8_2F738(D_001D9DC0_2F738[ship][0], D_001D9DC0_2F738[ship][1], t);
                func_001F9DC0_2F738(q.pos[m], side[m], (1.0f - t * t) * ((float *)&D_001605D0_2F738)[*(short *)(D_0013E130_2F738 + 0x26)]);
                func_001F9BD8_2F738(q.pos[m], q.pos[m], D_0013E1F0_2F738[j][((k + (m >> 1)) & 0x1F)]);
            }
            func_001F7EF8_2F738(&q, 0, 0);
            for (m = 0; m < 4; m++) {
                float t = func_001FA888_2F738(age - (m >> 1)) * 0.03125f;

                func_001F9DC0_2F738(q.pos[m], across[m], (1.0f - t * t) * ((float *)&D_001605D0_2F738)[*(short *)(D_0013E130_2F738 + 0x26)]);
                func_001F9BD8_2F738(q.pos[m], q.pos[m], D_0013E1F0_2F738[j][((k + (m >> 1)) & 0x1F)]);
            }
            func_001F7EF8_2F738(&q, 0, 0);
        }
    }
}
