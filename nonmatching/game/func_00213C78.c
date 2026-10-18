/* NON_MATCHING func_00213C78 -- src/game/mobyutil.c
 * Best so far: BYTES 11/148 (92.6% of the bytes match), checked 2026-10-18.
 * Not built into anything: the retail assembly stays in the source file
 * until a candidate is EXACT (docs/NONMATCHING.md). Start from this one.
 * What the last attempts found:
 *   Right size now (was SIZE 144/148): D_00160024 is $gp-relative (SDATA) and takes the call's result,
 *   m = D_00160024 = func_0020E6B8(); the flags halfword at +0x34 is read volatile, which leaves retail's
 *   empty beqz slot. Left: retail loads the +0x74 callback once into $v1 and leaves the jalr slot empty
 *   (daddu $a0 before it); here the call reloads it through a volatile pointer into $v0. A plain or local
 *   callback fills the jalr slot (SIZE 144). Lombyte's pending draft (update_visible_resident_objects) is the same.
 *   Ruled out 2026-10-18: SN v1.36 and v2.74 compilers, and -fno-schedule-insns(2)/-fno-sched-interblock/-fno-force-mem/
 *   -fno-expensive-optimizations/-fno-cse-follow-jumps/-fno-regmove/-mno-split-addresses: none gets closer.
 */
/* Moby update loop. Fetches the head of the moby list from func_0020E6B8_X and keeps it in
   D_00160024_X. For each moby whose state byte is non-negative (signed): calls func_0020E3D0_X
   unless flags 0x40 is set, calls its update callback at 0x74 if non-null, then calls
   func_0020ED48_X unless flags 0x4 is set. Walks the list through next at 0x28. */
extern void *func_0020E6B8_X(void) __asm__("func_0020E6B8");
extern void func_0020E3D0_X(void *) __asm__("func_0020E3D0");
extern void func_0020ED48_X(void *) __asm__("func_0020ED48");
extern void *D_00160024_X SDATA(D_00160024);

void func_00213C78(void) {
    char *m;

    m = D_00160024_X = func_0020E6B8_X();
    if (m != 0) {
        do {
            if ((signed char)m[0x20] >= 0) {
                if (!(*(volatile unsigned short *)(m + 0x34) & 0x40)) {
                    func_0020E3D0_X(m);
                }
                if (*(void (**)(void *))(m + 0x74) != 0) {
                    (*(void (* volatile *)(void *))(m + 0x74))(m);
                }
                if (!(*(volatile unsigned short *)(m + 0x34) & 0x4)) {
                    func_0020ED48_X(m);
                }
            }
            m = *(char **)(m + 0x28);
        } while (m != 0);
    }
}
