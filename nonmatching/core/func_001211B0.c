/* NON_MATCHING func_001211B0 -- src/core/001208E8.c
 * Best so far: BYTES 15/736 (98.0% of the bytes match), checked 2026-10-18.
 * Not built into anything: the retail assembly stays in the source file
 * until a candidate is EXACT (docs/NONMATCHING.md). Start from this one.
 * What the last attempts found:
 *   From Lombyte's NON_MATCHING sceCdInit (src/assembly/sdk/library/sceCdInit.c), PAL names.
 *   Only the opening reset block differs: lui register numbering for D_00159850/D_001313FC (15 bytes).
 *   Owner store must be volatile; a plain store at any position is 74+ bytes off.
 *   120 declaration orders and ~3,700 permuter iterations did not move it. Linux 2.9-ee-991111-01: same bytes.
 */

/* libcdvd: bind the CD/DVD init RPC server, send the init mode and read
   back the drive state. */

typedef struct {
    u8 pad0[0x24];
    void *server;
    u8 pad28[0x10];
} CdClient_211B0;

extern s32 D_001313D0_c[]; /* debug level */
extern s32 D_001313E0_c[] __asm__("D_001313E0");
extern s32 D_001313E4_c[] __asm__("D_001313E4");
extern s32 D_001313E8_c[] __asm__("D_001313E8");
extern s32 D_001313EC_c[] __asm__("D_001313EC");
extern s32 D_001313F4_c[] __asm__("D_001313F4");
extern s32 D_001313F8_c[] __asm__("D_001313F8");
extern s32 D_001313FC_c[] __asm__("D_001313FC");
extern s32 D_00131400_c[] __asm__("D_00131400");
extern s32 D_00131404_c[] __asm__("D_00131404");
extern s32 D_00131408_c[] __asm__("D_00131408");
extern s32 D_0013140C_c[] __asm__("D_0013140C");
extern s32 D_00131410_c[] __asm__("D_00131410");
extern u8 D_001325C0_c[] __asm__("D_001325C0");
extern char D_00153050_c[] __asm__("D_00153050");
extern char D_00153070_c[] __asm__("D_00153070");
extern s32 D_00159850_c[] __asm__("D_00159850");
extern CdClient_211B0 D_00159A68_c __asm__("D_00159A68");
extern s32 D_00159AC0_c[] __asm__("D_00159AC0");

extern s32 func_00118BE0_c(void) __asm__("func_00118BE0");
extern s32 func_00120CA0_c(void) __asm__("func_00120CA0");
extern void func_00120BC0_c(void) __asm__("func_00120BC0");
extern void func_00120B28_c(void) __asm__("func_00120B28");
extern s32 func_00120FD0_c(s32 mode) __asm__("func_00120FD0");
extern int func_0011A6C8_c(const char *format, ...) __asm__("func_0011A6C8");
extern s32 func_0011B2F8_c(CdClient_211B0 *cd, u32 request, u32 mode) __asm__("func_0011B2F8");
extern s32 func_0011B4C8_c(CdClient_211B0 *cd, u32 fno, u32 mode, void *send, s32 ssize,
                         void *receive, s32 rsize, void *end_func, void *end_para);
extern void func_0011AE20_c(u32 mode) __asm__("func_0011AE20");
extern void func_0011AD70_c(void *addr, s32 size) __asm__("func_0011AD70");

int func_001211B0(int init_mode) {
    int ret;
    int r;
    int i;
    int stat;
    int a;
    int b;
    int owner;
    int count;
    u8 *rbuf;

    if (func_00120FD0_c(1) != 0) {
        return 0;
    }
    func_0011AE20_c(0);
    owner = func_00118BE0_c();
    count = *(volatile s32 *)D_00131410_c + 1;
    *(volatile s32 *)D_001313E4_c = 1;
    /* Keep the retail reset order while grouping the final bind state. */
    D_001313FC_c[0] = -1;
    D_00131404_c[0] = (D_00131400_c[0] = -1,
                     D_001313F8_c[0] = -1,
                     D_00131408_c[0] = -1);
    D_001313F4_c[0] = 0;
    D_00131410_c[0] = count;
    D_0013140C_c[0] = -1;
    *(volatile s32 *)D_00159850_c = owner;
    while (1) {
        r = func_0011B2F8_c(&D_00159A68_c, 0x80000592, 0);
        if (r < 0) {
            if (D_001313D0_c[0] > 0) {
                func_0011A6C8_c(D_00153050_c, r, D_00131410_c[0]);
            }
            i = 0x100000;
            while (i-- != 0) {
            }
            continue;
        }
        if (D_00159A68_c.server != 0) {
            D_00159AC0_c[0] = init_mode;
            D_0013140C_c[0] = 0;
            func_0011AD70_c(D_00159AC0_c, 4);
            rbuf = D_001325C0_c;
            if (func_0011B4C8_c(&D_00159A68_c, 0, 0, D_00159AC0_c, 4, rbuf, 0x10, 0, 0) < 0) {
                *(volatile s32 *)D_001313E4_c = 0;
                return 0;
            }
            break;
        }
        i = 0x100000;
        while (i-- != 0) {
        }
    }
    /* Read the RPC result through the EE uncached alias. The signed division
       below preserves the retail rounding for negative response components. */
    a = *(s32 *)((u32)(rbuf + 4) | 0x20000000);
    b = *(s32 *)((u32)(rbuf + 8) | 0x20000000);
    stat = *(s32 *)((u32)(rbuf + 0xC) | 0x20000000);
    ret = 1;
    if (stat == 0xFF) {
    } else if (stat == 0xFE) {
        D_001313D0_c[0] = ret;
    } else {
        if (a / 256 < 2 || b / 256 < 2) {
            ret = 2;
        }
    }
    *(volatile s32 *)D_001313E4_c = 0;
    switch (init_mode) {
    case 0:
    case 1:
        break;
    case 5:
        if (D_001313D0_c[0] > 0) {
            func_0011A6C8_c(D_00153070_c);
        }
        func_00120BC0_c();
        *(volatile s32 *)D_001313E8_c = -1;
        *(volatile s32 *)D_001313EC_c = -1;
        *(volatile s32 *)D_001313E0_c = -1;
        goto done;
    }
    func_00120B28_c();
    func_00120CA0_c();
done:
    return ret;
}