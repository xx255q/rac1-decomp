/* NON_MATCHING func_00214778 -- src/game/mobyutil.c
 * Best so far: BYTES 522/1408 (62.9% of the bytes match), checked 2026-10-18.
 * Not built into anything: the retail assembly stays in the source file
 * until a candidate is EXACT (docs/NONMATCHING.md). Start from this one.
 * What the last attempts found:
 *   Rewritten from the assembly (was SIZE 1396/1408). Fixes: an->0x16 = -1 is stored even when an is NULL;
 *   the B == 0 path keeps retail's an != 0 test; the 16-byte copy is a plain long long copy (lq/sq at offsets);
 *   D_001600C8 is reached through $gp (declared extern short); func_00215078 is called in each branch.
 *   Left: retail branches where ours emits movn/movz (case 1's c == 2 test, case 7's D_0013E633 choice),
 *   the dx/dy subtraction operand registers, and saved-register numbering.
 */
/* functional: medium. Picks the next animation (returned) for an actor from its mode/state bytes (B+0x28/0x29/0x2A) and the buttons, via a state table D_001CC400; sets *outA (animation status), *outI (state id), *outF (B+0x2C speed), advances/reloads the timer at an+0x1C. Static check only; the 0x47 block (chase-toward-D_0013F450 kick) is the least certain part. */
extern short D_001600C8_4770 __asm__("D_001600C8");
extern u8 D_001CC400_4770[] __asm__("D_001CC400");
extern u8 D_001CC448_4770[] __asm__("D_001CC448");
extern u8 D_0013E633_4770[] __asm__("D_0013E633");
extern u8 D_0013F450_4770[] __asm__("D_0013F450");
extern s32 func_00215048_4770(void *) __asm__("func_00215048");
extern u8 *func_00215078_4770(void *) __asm__("func_00215078");
extern s32 func_001F9938_4770(void *) __asm__("func_001F9938");
extern s32 func_001F98C0_4770(s32) __asm__("func_001F98C0");
extern void func_001F9C30_4770(void *, void *, f32) __asm__("func_001F9C30");
extern void func_001F9DC0_4770(void *, void *, f32) __asm__("func_001F9DC0");
extern void func_001F9BD8_4770(void *, void *, void *) __asm__("func_001F9BD8");
extern void func_001F9E10_4770(void *, void *, f32) __asm__("func_001F9E10");
extern f32 func_001F9CB8_4770(void *) __asm__("func_001F9CB8");
extern f32 func_001FA058_4770(f32, f32) __asm__("func_001FA058");
extern f32 func_001F9F90_4770(f32) __asm__("func_001F9F90");
extern f32 func_001F9FA8_4770(f32) __asm__("func_001F9FA8");

s32 func_00214778(u8 *a, u8 *B, u8 *an, u32 btn, s32 *outA, f32 *outF, s32 *outI, s32 k) {
    f32 v[4] __attribute__((aligned(16)));
    f32 w[4] __attribute__((aligned(16)));
    s32 state;
    s32 a22;
    s32 anim;
    s32 frames;
    u32 c;
    u8 *p;

    *outA = 0;
    if (outI != 0) {
        *outI = -1;
    }
    if (B != 0 && *(u8 **)(B + 0x20) != 0) {
        s32 cls = *(s16 *)(*(u8 **)(B + 0x20) + 0xA6);
        if ((cls == 0x131 || cls == 0xA8) && an != 0) {
            *(f32 *)(B + 0x2C) = ((f32 *)&D_001600C8_4770)[an[8]];
        }
    }
    if (a == 0) {
        if (an != 0) {
            *(s16 *)(an + 0x1C) = 0;
            *(s16 *)(an + 0x14) = -1;
        }
        *(s16 *)(an + 0x16) = -1;
        *outA = 0;
        return 0;
    }
    if (an == 0) {
        an = (u8 *)func_00215048_4770(a);
        if (an == 0) {
            *outA = 0;
            return 0;
        }
    }
    if (k == 4) {
        k = an[8];
    }
    if (B == 0) {
        if (an != 0) {
            func_001F9938_4770(an + 0x1C);
            *(s16 *)(an + 0x16) = -1;
        }
        *outA = 1;
        if (outF != 0) {
            *outF = 0;
        }
        return 0xB;
    }

    if (*(f32 *)(B + 0x1C) == 5627.9248f) {
        func_001F9C30_4770(B + 0x10, B + 0x10, *(f32 *)(D_001CC448_4770 + B[0x28] * 4 + k * 32));
        a22 = *(u16 *)(B + 0x2A);
        *(f32 *)(B + 0x1C) = 5627.9248f;
        if (a22 == 0x47) {
            f32 dx = *(f32 *)(a + 0x10) - *(f32 *)(D_0013F450_4770 + 0x80);
            f32 dy = *(f32 *)(a + 0x14) - *(f32 *)(D_0013F450_4770 + 0x84);
            v[0] = func_001F9F90_4770(func_001FA058_4770(dx, dy));
            dx = *(f32 *)(a + 0x10) - *(f32 *)(D_0013F450_4770 + 0x80);
            dy = *(f32 *)(a + 0x14) - *(f32 *)(D_0013F450_4770 + 0x84);
            v[1] = func_001F9FA8_4770(func_001FA058_4770(dx, dy));
            v[2] = 1.0f;
            func_001F9DC0_4770(v, v, func_001F9CB8_4770(B + 0x10) * 0.7f);
            func_001F9C30_4770(B + 0x10, B + 0x10, 0.3f);
            func_001F9BD8_4770(w, B + 0x10, v);
            *(long long *)(B + 0x10) = *(long long *)w;
            func_001F9E10_4770(B + 0x10, B + 0x10, 1.0f);
        }
    } else {
        if (*(u8 **)(B + 0x20) == 0) {
            B[0x28] = 8;
            a22 = 0x47;
        } else {
            a22 = *(s16 *)(*(u8 **)(B + 0x20) + 0xA6);
            B[0x28] = 8;
        }
    }

    switch (B[0x28]) {
    case 0:
        if ((u16)(*(u16 *)(B + 0x2A) - 1) < 2) {
            *outA = 2;
            state = 0;
            break;
        }
        goto other;
    case 1:
        c = B[0x29];
        if (c < 2) {
            *outA = 2;
            state = 0xB;
        } else if (c == 2) {
            *outA = 2;
            state = 5;
        } else {
            *outA = 2;
            state = 9;
        }
        break;
    case 2:
        if (B[0x29] < 2) {
            *outA = 2;
            state = 8;
        } else {
            *outA = 2;
            state = 0xD;
        }
        break;
    case 3:
        c = B[0x29];
        if (c < 2) {
            *outA = 2;
            state = 0xA;
        } else if (c < 3) {
            *outA = 2;
            state = 0xF;
        } else {
            *outA = 2;
            state = 0x10;
            *(s16 *)(an + 0x14) = -1;
            *(s16 *)(an + 0x1C) = 0;
        }
        break;
    case 4:
        *outA = 2;
        state = 0xE;
        break;
    case 5:
        if (B[0x29] < 2) {
            *outA = 3;
            state = 4;
        } else {
            *outA = 4;
            state = 7;
        }
        break;
    case 7:
        if (B[0x29] < 2) {
            *outA = 2;
            state = 0xC;
        } else {
            *outA = 2;
            state = 0x11;
        }
        break;
    default:
    other:
        *outA = 2;
        state = 1;
        break;
    }
    if (outI != 0) {
        *outI = a22;
    }

    anim = D_001CC400_4770[an[8] + state * 4];
    if (*(s16 *)(an + 0x14) != a22) {
        *(s16 *)(an + 0x1C) = 0;
    }
    if (func_001F9938_4770(an + 0x1C) != 0) {
        switch (state) {
        case 0:
            frames = 0x25;
            break;
        case 1:
            frames = 0xF;
            break;
        case 4:
            frames = 0x3C;
            break;
        case 7:
            if (D_0013E633_4770[0] != 0) {
                frames = 0x2D;
            } else {
                frames = 0x3C;
            }
            break;
        case 11:
            frames = 0x3C;
            break;
        default:
            frames = 0x1E;
            break;
        }
        *(s16 *)(an + 0x1C) = func_001F98C0_4770(frames);
    } else {
        switch (state) {
        case 0:
        case 1:
            if (!(btn & 0x10)) {
                anim = 0xB;
            }
            if (!(btn & 0x20)) {
                *outA = 1;
            }
            *(f32 *)(B + 0x2C) = 0;
            break;
        case 4:
            if (!(btn & 1)) {
                anim = 0xB;
            }
            if (!(btn & 2)) {
                *outA = 1;
            }
            *(f32 *)(B + 0x2C) = 0;
            break;
        case 7:
            if (!(btn & 4)) {
                anim = 0xB;
            }
            if (!(btn & 8)) {
                *outA = 1;
            }
            *(f32 *)(B + 0x2C) = 0;
            break;
        case 11:
            anim = 0xC;
            break;
        default:
            *(f32 *)(B + 0x2C) = 0;
            anim = 0xD;
            break;
        }
    }
    *(s16 *)(an + 0x16) = a22;
    *(s16 *)(an + 0x14) = a22;
    if (outF != 0) {
        *outF = *(f32 *)(B + 0x2C);
    }
    if (state == 4) {
        p = func_00215078_4770(a);
        if (p != 0) {
            p[0x2E] = 1;
        }
    } else {
        p = func_00215078_4770(a);
        if (p != 0) {
            p[0x2E] = 0;
        }
    }
    return anim;
}
