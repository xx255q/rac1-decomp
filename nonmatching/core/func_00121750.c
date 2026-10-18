/* NON_MATCHING func_00121750 -- src/core/00121750.c
 * Best so far: SIZE ours 484 / retail 480, checked 2026-10-18.
 * Not built into anything: the retail assembly stays in the source file
 * until a candidate is EXACT (docs/NONMATCHING.md). Start from this one.
 * What the last attempts found:
 *   From Lombyte's NON_MATCHING sceCdRead (src/assembly/sdk/library/sceCdRead.c), PAL names.
 *   `count * (dataPattern = 0x918)` reproduces retail's two `mult` (the shared mult copied into the b's slot).
 *   Left: 0x918 lands in $a3 (dataPattern's register) where retail has $v0.
 *   Left: the D_001313E8 load goes before the D_001313F0 store, not in the jal slot (+1 nop); non-volatile alone d
 *   A temp for 0x918 gives $v0 but merges the two mults (476/480). Linux 2.9-ee-991111-01 gives the same bytes.
 */
typedef struct {
    u8 trycount;
    u8 spindlctrl;
    u8 datapattern;
    u8 pad;
} CdRMode_121750;
typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    u8 unkC;
    u8 unkD;
    u8 unkE;
    u8 pad_F[0x1];
    s32 unk10;
    s32 unk14;
} CdDriveState_121750;

extern u8 D_00120788_cr[] __asm__("func_00120A88");
extern s32 D_001312D0_cr[] __asm__("D_001313D0");
extern u32 D_001312E8_cr[] __asm__("D_001313E8");
extern volatile u32 D_001312F0_cr[] __asm__("D_001313F0");
extern volatile u32 D_001312F4_cr[] __asm__("D_001313F4");
extern volatile u32 D_00131314_cr[] __asm__("D_00131414");
extern CdDriveState_121750 D_001313C0_cr __asm__("D_001314C0");
extern u8 D_001323C0_cr[] __asm__("D_001324C0");
extern u8 D_00132480_cr[] __asm__("D_00132580");
extern u8 D_00132490_cr[] __asm__("D_00132590");
extern u8 D_00152FC8_cr[] __asm__("D_001530C8");
extern u8 D_00152FE0_cr[] __asm__("D_001530E0");
extern s32 SignalSema_cr(u32 semaphore) __asm__("func_00118C90");
extern s32 cd_check_ncmd_cr() __asm__("func_00120D28");
extern s32 sceCdNcmdDiskReady_cr() __asm__("func_00120E98");
extern s32 scePrintf_cr() __asm__("func_0011A6C8");
extern s32 sceSifCallRpc_cr() __asm__("func_0011B4C8");
extern void sceSifWriteBackDCache_cr(void *ptr, s32 size) __asm__("func_0011AD70");
int func_00121750(int dwSector, int dwSectorCount, void *pDestination, unsigned char *pModeRaw) {
    CdRMode_121750 *pMode = (CdRMode_121750 *)pModeRaw;
    s32 byteCount;
    s32 dataPattern;
    CdDriveState_121750 *st = &D_001313C0_cr;

    if (D_001312F4_cr[0] & 1) {
        goto block_2;
    }
    if (sceCdNcmdDiskReady_cr() == 6) {
        return 0;
    }
block_2:
    if (cd_check_ncmd_cr(4) == 0) {
        return 0;
    }
    st->unk0 = dwSector;
    st->unk4 = dwSectorCount;
    st->unk8 = pDestination;
    st->unkC = (u8)pMode->trycount;
    st->unkD = (u8)pMode->spindlctrl;
    st->unkE = (u8)pMode->datapattern;
    st->unk10 = D_001323C0_cr;
    st->unk14 = D_00132480_cr;
    dataPattern = pMode->datapattern;
    if (dataPattern == 1) {
        goto block_7;
    }
    byteCount = dwSectorCount << 0xB;
    if (dataPattern < 2) {
        goto block_9;
    }
    if (dataPattern == 2) {
        goto block_8;
    }
    goto block_9;
block_7:
    byteCount = dwSectorCount * (dataPattern = 0x918);
    goto block_9;
block_8:
    byteCount = dwSectorCount * 0x924;
block_9:
    *(s32 *)D_00132480_cr = 0;
    if (D_001312F4_cr[0] & 2) {
        goto block_11;
    }
    sceSifWriteBackDCache_cr(pDestination, byteCount);
block_11:
    sceSifWriteBackDCache_cr(D_001323C0_cr, 0x90);
    sceSifWriteBackDCache_cr(st, 0x18);
    sceSifWriteBackDCache_cr(D_00132480_cr, 4);
    if (D_001312D0_cr[0] <= 0) {
        goto block_13;
    }
    scePrintf_cr(D_00152FC8_cr);
block_13:
    D_00131314_cr[0] = 1;
    D_001312F0_cr[0] = 1;
    if (sceSifCallRpc_cr(D_00132490_cr, 1, 1, st, 0x18, 0, 0, D_00120788_cr, D_001323C0_cr) < 0) {
        goto block_15;
    }
    goto block_18;
block_15:
    D_00131314_cr[0] = 0;
    D_001312F0_cr[0] = 0;
    SignalSema_cr(D_001312E8_cr[0]);
    return 0;
block_18:
    if (D_001312D0_cr[0] <= 0) {
        return 1;
    }
    scePrintf_cr(D_00152FE0_cr);
    return 1;
}