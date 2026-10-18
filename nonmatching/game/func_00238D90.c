/* NON_MATCHING func_00238D90 -- src/game/update.c
 * Best so far: BYTES 123/516 (76.2% of the bytes match), checked 2026-10-18.
 * Not built into anything: the retail assembly stays in the source file
 * until a candidate is EXACT (docs/NONMATCHING.md). Start from this one.
 * What the last attempts found:
 *   Right size now (was SIZE 524/516): rebuilt on func_00238F98's draft (the float twin), with retail's
 *   16-byte struct copies, the pointer to the second point, and per-output func_001FA898 calls.
 *   Left: ours loads and converts D_0013E600 half_width right after the first projection; retail only after
 *   the scale step's stores to the second point. A volatile D_0013E600 gives lw/mtc1 instead of lwc1/cvt
 *   (SIZE 532); a volatile store of the scaled y changes nothing.
 */
/* functional: medium. The NTSC decomp's pending C for the same instructions (a draft it has not matched
   yet), carried to PAL's names by OpenRAC's tools/port.py. Never compared with retail's bytes here. */
typedef u32 u128_38D90 __attribute__((mode(TI), aligned(16)));
extern u8 D_00187180_D90[] __asm__("D_00187180");
extern volatile f32 D_0018CE00_D90[] __asm__("D_0018CE00");
struct Screen_38D90 {
    s32 width; /* 0x0: display width (D_00151780 +0x150) */
    s32 height; /* 0x4: display height (D_00151780 +0x152) */
    s32 half_width; /* 0x8: width >> 1 */
    s32 half_height; /* 0xC: height >> 1 */
    s32 left; /* 0x10: (0x800 - half_width) << 4 */
    s32 top; /* 0x14: (0x800 - half_height) << 4 */
    s32 right; /* 0x18: (0x800 + half_width) << 4 */
    s32 bottom; /* 0x1C: (0x800 + half_height) << 4 */
};
extern struct Screen_38D90 D_0013E600_D90 __asm__("D_0013E600");
extern void func_001F9BF0_D90(void *, void *, void *) __asm__("func_001F9BF0");
extern void func_001F9C30_D90(void *, void *, f32) __asm__("func_001F9C30");
extern void func_001F9EE8_D90(void *, void *, void *) __asm__("func_001F9EE8");

extern int func_001FA898_D90(float) __asm__("func_001FA898");

/* Projects two corner points through the view matrix to screen space and
   returns the rectangle they span, rounded to integers: x/y from the first
   point, width/height from the first to the second.
   Same body as func_00238F98, which returns the floats. */
void func_00238D90(u128_38D90 *first, u128_38D90 *opposite, int *width, int *height, int *x, int *y) {
    f32 first_projected[4] __attribute__((aligned(16)));
    f32 opposite_projected[4] __attribute__((aligned(16)));
    f32 *opposite_pointer;
    f32 scale_x;
    f32 scale_y;
    f32 projected_coordinate;

    opposite_pointer = opposite_projected;
    *(u128_38D90 *)first_projected = *first;
    *(u128_38D90 *)opposite_pointer = *opposite;
    func_001F9BF0_D90(first_projected, first_projected, D_00187180_D90);
    func_001F9BF0_D90(opposite_pointer, opposite_pointer, D_00187180_D90);
    func_001F9C30_D90(first_projected, first_projected, 1024.0f);
    func_001F9C30_D90(opposite_pointer, opposite_pointer, 1024.0f);
    opposite_pointer[3] = 1.0f;
    first_projected[3] = 1.0f;
    func_001F9EE8_D90(first_projected, first_projected, D_00187180_D90 - 0x40);
    func_001F9EE8_D90(opposite_pointer, opposite_pointer, D_00187180_D90 - 0x40);
    first_projected[0] *= 1.0f / first_projected[3];
    first_projected[1] *= 1.0f / first_projected[3];
    opposite_pointer[0] *= 1.0f / opposite_pointer[3];
    *(volatile f32 *)&opposite_pointer[1] *= 1.0f / opposite_pointer[3];
    scale_y = D_0018CE00_D90[101];
    scale_x = D_0018CE00_D90[100];
    first_projected[0] *= scale_x;
    first_projected[1] *= scale_y;
    opposite_pointer[0] *= scale_x;
    projected_coordinate = opposite_pointer[1] * scale_y;
    opposite_pointer[1] = projected_coordinate;
    *x = func_001FA898_D90(first_projected[0] * 0.25f + (f32)D_0013E600_D90.half_width);
    *y = func_001FA898_D90(first_projected[1] * 0.25f + (f32)D_0013E600_D90.half_height);
    *width = func_001FA898_D90((opposite_pointer[0] - first_projected[0]) * 0.25f);
    *height = func_001FA898_D90((opposite_pointer[1] - first_projected[1]) * 0.25f);
}
