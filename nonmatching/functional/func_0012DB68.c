/* NON_MATCHING func_0012DB68 -- src/core/989snd.c
 * Best so far: BYTES 17/600 (97.2% of the bytes match), checked 2026-10-18.
 * Not built into anything: the retail assembly stays in the source file
 * until a candidate is EXACT (docs/NONMATCHING.md). Start from this one.
 * What the last attempts found:
 *   snd_StartSoundSystem from Lombyte (MIT) src/textbin/audio/snd_start_sound_system.c, PAL names.
 *   Only the six buffer stores before sceSifInitRpc are scheduled differently (retail keeps
 *   addiu $s1,-1 in the jal slot). Element-1 stores need separate MACRO_ADDR names.
 *   All 720 orders of the six stores tried: best is this one (17 bytes). volatile: SIZE 608.
 *   -mno-split-addresses for 989snd.c changes everything else; not the answer.
 */

struct SifClientDataStartSound_DB68 {
    u8 pad_0[0x24];
    void *volatile server;
};
struct StartSoundWork_DB68 {
    volatile s32 read_active;
    u8 pad_4[0xC];
    volatile s32 read_error;
};

extern u8 snd_command_buffer1_ss[] __asm__("D_00133380");
extern u8 snd_command_buffer2_ss[] __asm__("D_00134380");
extern u8 snd_stream_buffer1_ss[] __asm__("D_00135380");
extern u8 snd_stream_buffer2_ss[] __asm__("D_00136380");
extern u8 snd_return_buffer1_ss[] __asm__("D_00137380");
extern u8 snd_return_buffer2_ss[] __asm__("D_001377C0");
extern u8 snd_sif_bind_rpc_error_string_ss[] __asm__("D_00153D50");
extern u8 snd_989_snd_source_file_ss[] __asm__("D_00153D78");
extern struct SifClientDataStartSound_DB68 sound_command_client_ss __asm__("D_0015ECC0");
extern struct SifClientDataStartSound_DB68 sound_stream_client_ss __asm__("D_0015ECE8");
extern struct StartSoundWork_DB68 sound_read_work_ss __asm__("D_00137C00");
extern u8 *snd_batch_command_buffers_ss[2] SDATA(D_0015EDA0);
extern s32 snd_batch_free_bytes_ss SDATA(D_0015EDA8);
extern s32 D_0015EDAC_ss __asm__("D_0015EDAC") MACRO_ADDR;
extern u8 *snd_stream_buffers_ss[2] SDATA(D_0015EDB0);
extern u8 *snd_batch_return_buffers_ss[2] SDATA(D_0015EDB8);
extern s32 snd_cd_callback_pending_ss SDATA(D_0015EDC8);
extern s32 snd_cd_callback_fn_ss SDATA(D_0015EDD0);
extern s64 snd_cd_callback_data_ss __asm__("D_0015EDD8") MACRO_ADDR;
extern u32 snd_cd_callback_arg_ss __asm__("D_0015EE00") MACRO_ADDR;
extern u8 *snd_batch_command_buffer1_ss __asm__("D_0015EDA4") MACRO_ADDR;
extern u8 *snd_stream_buffer1p_ss __asm__("D_0015EDB4") MACRO_ADDR;
extern u8 *snd_batch_return_buffer1_ss __asm__("D_0015EDBC") MACRO_ADDR;

extern void sceSifInitRpc_ss(u32) __asm__("func_0011AE20");
extern s32 sceSifBindRpc_ss(struct SifClientDataStartSound_DB68 *, u32, s32) __asm__("func_0011B2F8");
extern s32 printf_ss(const char *, ...) __asm__("func_00116078");
extern s32 snd_send_iop_command_and_wait_ss(s32, s32, void *) __asm__("func_0012E688");

/* snd_StartSoundSystem: point the double-buffered command, return and
   stream buffers at their static areas, bind the sound command and
   stream RPC servers (spinning between attempts, halting with an error
   if a bind fails), clear the CD callback state and both batches, and
   send the IOP the address of the read-status word (command 4).
   Adapted from Lombyte (MIT): src/textbin/audio/snd_start_sound_system.c, snd_start_sound_system. */
s32 func_0012DB68(void) {
    s32 bind_result;
    s32 command_arg;
    u8 *const command_buffer1 = snd_command_buffer2_ss;
    u8 *const command_buffer0 = snd_command_buffer1_ss;

    snd_batch_command_buffers_ss[0] = command_buffer0;
    snd_batch_command_buffer1_ss = command_buffer1;
    snd_batch_return_buffer1_ss = snd_return_buffer2_ss;
    snd_stream_buffers_ss[0] = snd_stream_buffer1_ss;
    snd_stream_buffer1p_ss = snd_stream_buffer2_ss;
    snd_batch_return_buffers_ss[0] = snd_return_buffer1_ss;
    sceSifInitRpc_ss(0);

    for (;;) {
        bind_result = sceSifBindRpc_ss(&sound_command_client_ss, 0x123456, 0);
        if (bind_result < 0) {
            printf_ss((const char *)snd_sif_bind_rpc_error_string_ss, snd_989_snd_source_file_ss, 0x73);
            for (;;) {
            }
        }
        command_arg = 10000;
        for (command_arg--; command_arg != -1; command_arg--) {
        }
        if (sound_command_client_ss.server != 0) {
            break;
        }
    }

    snd_cd_callback_pending_ss = 0;
    snd_cd_callback_fn_ss = 0;
    snd_cd_callback_data_ss = 0;
    snd_cd_callback_arg_ss = 0;

    do {
        bind_result = sceSifBindRpc_ss(&sound_stream_client_ss, 0x123457, 0);
        if (bind_result < 0) {
            printf_ss((const char *)snd_sif_bind_rpc_error_string_ss, snd_989_snd_source_file_ss, 0x88);
            for (;;) {
            }
        }
        command_arg = 10000;
        for (command_arg--; command_arg != -1; command_arg--) {
        }
    } while (sound_stream_client_ss.server == 0);

    *(s32 *)snd_command_buffer1_ss = 0;
    sound_read_work_ss.read_active = 0;
    *(s32 *)snd_command_buffer2_ss = 0;
    sound_read_work_ss.read_error = 0;
    snd_batch_free_bytes_ss = 0xFFC;
    D_0015EDAC_ss = 0xFFC;
    command_arg = (s32)(u32)&sound_read_work_ss.read_active;
    return snd_send_iop_command_and_wait_ss(0, 4, &command_arg);
}
