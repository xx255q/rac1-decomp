/* NON_MATCHING func_0022FDC0 -- src/game/space.c
 * Best so far: SIZE ours 2004 / retail 2012, checked 2026-10-18.
 * Not built into anything: the retail assembly stays in the source file
 * until a candidate is EXACT (docs/NONMATCHING.md). Start from this one.
 * What the last attempts found:
 *   From Lombyte's NON_MATCHING update_level_gameplay_frame, renamed to PAL through config/overlays/us_map.tsv (al
 *   PAL loads sbh/cli/D_0016100C via MACRO_ADDR and does not take Lombyte's volatile-union stack round trip for th
 *   D_0015EF4A: retail lh, then `lui $at; sh` after the branch; ours puts the store in a branch-likely slot (volat
 *   Left: lrs base computed after func_0012EC30 into $s7 (ours keeps it in $s0 across the call), the archive loop'
 */
/* Per-frame update of the level's resident render sequence (Lombyte:
   update_level_gameplay_frame): steps the fade and the sequence clock,
   starts the next scene when the current one ends, and moves each object
   of the sequence between its two animation frames, recording its
   transform history.
   Adapted from Lombyte (MIT): src/assembly/textbin/gameplay/state/update_level_gameplay_frame.c
   (NON_MATCHING there), renamed to PAL symbols. */
extern int D_0015EF4A_fx __asm__("D_0015EF4A") MACRO_ADDR;
extern s32 cli_fx __asm__("D_0015EE84") MACRO_ADDR;
extern u8 lavail_fx[] __asm__("D_0013DE48") NOT_SDA;
extern u8 D_00160460_fx[16] __asm__("D_00160560");

typedef u32 u128_2FDC0 __attribute__((mode(TI), aligned(16)));
typedef union {
    u128_2FDC0 quadword;
    f32 components[4];
} Vector4_fx;
typedef struct {
    Vector4_fx rows[4];
} Matrix4x4_fx;

typedef struct {
    u8 pad00[0xC];
    u8 count; /* 0x0C */
    u8 pad0D[0x24 - 0x0D];
    f32 base_scale; /* 0x24 */
    u8 pad28[0x48 - 0x28];
    s32 frames[1]; /* 0x48 */
} RenderModel_fx;

typedef struct {
    u8 pad00[0x10];
    Vector4_fx position;   /* 0x10 */
    RenderModel_fx *model; /* 0x20 */
} RenderObjectHeader_fx;

typedef struct {
    u8 pad00[0x10];
    u8 position[0x14];  /* 0x10 */
    RenderModel_fx *model; /* 0x24 */
    u8 pad28[4];
    f32 fade; /* 0x2C */
    u8 pad30[2];
    s16 selected_index; /* 0x32 */
    u16 flags;          /* 0x34 */
    u8 pad36[0x50 - 0x36];
    u8 state_a; /* 0x50 */
    u8 state_b;
    u8 selected_a;
    u8 selected_b;
    f32 blend; /* 0x54 */
    u8 pad58[0x68 - 0x58];
    u8 *frame_data_a; /* 0x68 */
    u8 *frame_data_b; /* 0x6C */
    u8 pad70;
    u8 cached_selector;
    u8 opacity;
    u8 pad73[5];
    u8 *animation_positions; /* 0x78 */
} LevelRenderObject_fx;

typedef struct {
    u8 pad00[0x34];
    s32 time;           /* 0x34 */
    s32 frame;          /* 0x38 */
    s32 sequence_frame; /* 0x3C */
    s16 end;            /* 0x40 */
    u8 pad42[2];
    s16 count; /* 0x44 */
    u8 pad46[0x58 - 0x46];
    s32 source_begin;              /* 0x58 */
    s32 source_end;                /* 0x5C */
    s32 prepared_frames[0x46];     /* 0x60 */
    LevelRenderObject_fx *objects[1]; /* 0x178 */
} LevelGameplayState_fx;

typedef struct {
    u8 pad00[0x50];
    s32 history_index; /* 0x50 */
    s32 history_count; /* 0x54 */
    s32 mode;          /* 0x58 */
    s32 state;         /* 0x5C */
    u8 pad60[0xC0 - 0x60];
    Vector4_fx primary_history_positions[32];   /* 0xC0 */
    Vector4_fx secondary_history_positions[32]; /* 0x2C0 */
} LevelRenderState_fx;

typedef struct {
    u8 pad00[0xB0];
    f32 projection_scale;
} LevelProjectionState_fx;

typedef struct {
    s32 offset;
    s32 available;
} RenderArchiveEntry_fx;

typedef struct {
    u8 pad00[4];
    s32 data_offset; /* 0x04 */
    u8 pad08[0x50 - 0x08];
    s32 scene_offsets[5]; /* 0x50 */
} RenderArchiveTable_fx;

typedef struct {
    u8 pad00[4];
    s32 source_begin_offset;
    s32 source_end_offset;
    u8 pad0C[8];
    RenderArchiveTable_fx *archive_table; /* 0x14 */
} LevelRenderArchive_fx;

typedef struct {
    u32 w0;
    u32 w1;
} ColorPair_fx;

extern LevelRenderState_fx lrs_fx __asm__("D_0013E130");
extern u8 vvr_fx[] __asm__("D_0013E6C0");
extern s32 sbh_fx __asm__("D_0015EE5C") MACRO_ADDR;
extern s16 D_0015EF48_fx SDATA(D_0015EF48);
extern f32 seqm_fx __asm__("D_0015F53C") MACRO_ADDR;
extern s32 slip_fx __asm__("D_0015F6FC") MACRO_ADDR;
extern f32 D_00160504_fx __asm__("D_00160504") MACRO_ADDR;
extern s32 D_001605E0_fx SDATA(D_001605E0);
extern s32 D_001605F0_fx SDATA(D_001605F0);
extern s32 D_00160600_fx SDATA(D_00160600);
extern s32 cvu_fx __asm__("D_0016100C") MACRO_ADDR;
extern LevelGameplayState_fx rseq_fx __asm__("D_0018CC20");
extern LevelGameplayState_fx D_0018CB20_b[] __asm__("D_0018CC20");
extern LevelGameplayState_fx D_0018CB20_c[] __asm__("D_0018CC20");
extern LevelRenderObject_fx *D_0018CC98_fx[] __asm__("D_0018CD98");
extern LevelProjectionState_fx vctx_fx __asm__("D_0018CE00");
extern LevelRenderArchive_fx D_001940C0_fx __asm__("D_001941C0");
extern ColorPair_fx D_001D9A30_fx[] __asm__("D_001D9DC0");
extern u128_2FDC0 D_001D9AE0_fx[] __asm__("D_001D9E70");
extern f32 D_001D9B30_fx[] __asm__("D_001D9EC0");
extern f32 cmfov_fx[] __asm__("D_001D9ED8");

extern void fill_transfer_words(void *, s32, s32) __asm__("func_001F99B0");
extern void read_global_table_entry(void) __asm__("func_0012EC30");
extern void snd_flush_sound_commands(void) __asm__("func_0012DDC0");
extern void snd_play_sound_vol_pan_pmpb(s32, s32, s32, s32, s32, s32, s32,
                                        void *) __asm__("func_0012E448");
extern void snd_reset_state_and_flush_commands(void) __asm__("func_0012EC40");
extern void noop_callback_h(void) __asm__("func_001E97A8");
extern void noop_callback_i(void) __asm__("func_001E97B0");
extern void update_view_context(void) __asm__("func_001F3140");
extern s32 scale_ticks(s32) __asm__("func_001F98C0");
extern u32 fast_mem_copy(void *, void *, s32) __asm__("func_001F9A98");
extern void add_vectors(void *, void *, void *) __asm__("func_001F9BD8");
extern void scale_vector(void *, void *, f32) __asm__("func_001F9C30");
extern f32 convert_integer_to_float(s32) __asm__("func_001FA888");
extern void parse_space_scene_chunk(s32) __asm__("func_00205220");
extern void mark_moby_for_removal(LevelRenderObject_fx *) __asm__("func_0020D678");
extern void update_moby_animation_state(LevelRenderObject_fx *) __asm__("func_0020D6D0");
extern void calculate_object_transform(LevelRenderObject_fx *, s32, void *) __asm__("func_0020DAF8");
extern void refresh_moby_spatial_bounds(LevelRenderObject_fx *) __asm__("func_0020ED48");
extern void build_object_rotation_matrix(void) __asm__("func_0022F128");
extern s32 rand_fx(void) __asm__("func_001160D8");



void func_0022FDC0(void) {
    union {
        Vector4_fx position;
        volatile s32 sound_bank;
    } first_position;
    Vector4_fx second_position;
    Matrix4x4_fx second_transform;
    Matrix4x4_fx first_transform;
    LevelGameplayState_fx *gameplay_state;
    LevelRenderState_fx *render_state;
    u16 transform_count;
    LevelRenderObject_fx *object;
    LevelRenderObject_fx *expired_object;
    s32 mode_advance;
    s32 sound_bank;
    RenderModel_fx *model;
    RenderArchiveTable_fx *archive_table;
    u8 *archive_data;
    s32 *scene_offset;
    RenderArchiveEntry_fx *archive_entry;
    s32 object_index;
    s32 expired_object_index;
    s32 prepared_frame_index;
    s32 payload_offset;
    u8 *frame_offsets;
    s32 *current_frame_pointer;
    s32 *next_frame_pointer;
    f32 unused_fraction;
    s32 subframe;
    s32 unused_index;
    s32 color_index;
    s32 history_slot;
    s32 next_frame_payload;
    s32 current_frame_payload;
    s32 frame;
    u8 *animation_positions;
    s32 current_frame;
    s32 next_frame;
    f32 blend;
    f32 scale;

    noop_callback_i();
    seqm_fx -= 0.25f;
    rseq_fx.frame++;
    rseq_fx.time++;
    if (seqm_fx < 0.0f) {
        seqm_fx = 0.0f;
    }
    if (rseq_fx.time == 1 && cli_fx != 0 &&
        (cli_fx != 1 || lavail_fx[3] != 0)) {
        read_global_table_entry();
        snd_play_sound_vol_pan_pmpb(sbh_fx, lrs_fx.mode, 0x400, 0, 0, 0, 0, vvr_fx);
        snd_reset_state_and_flush_commands();
        snd_flush_sound_commands();
    }
    if (rseq_fx.time >= rseq_fx.end) {
        for (expired_object_index = 0; expired_object_index < rseq_fx.count;
             expired_object_index++) {
            expired_object = rseq_fx.objects[expired_object_index];
            if (expired_object != 0) {
                expired_object->model->count--;
                expired_object->model->frames[expired_object->model->count] = 0;
                mark_moby_for_removal(expired_object);
            }
        }
        if (cli_fx != 0 && (cli_fx != 1 || lavail_fx[3] != 0) &&
            D_0015EF48_fx < 3) {
            lrs_fx.state = 0;
        }
        if (lrs_fx.state < 2) {
            if (lrs_fx.state == 0) {
                mode_advance = (rand_fx() >> 16) % 3 + 1;
                lrs_fx.mode = (lrs_fx.mode + mode_advance) & 3;
            } else {
                lrs_fx.mode = 4;
            }
            lrs_fx.history_index = 0;
            lrs_fx.history_count = 0;
            lrs_fx.state++;
            qcopy(&D_001605F0_fx, &D_001605E0_fx);
            fill_transfer_words(&rseq_fx, 0, 0x1C0);
            rseq_fx.source_begin = D_001940C0_fx.source_begin_offset + cvu_fx;
            rseq_fx.source_end = D_001940C0_fx.source_end_offset + cvu_fx;
            archive_table = D_001940C0_fx.archive_table;
            archive_data = (u8 *)archive_table + archive_table->data_offset;
            scene_offset = &archive_table->scene_offsets[0];
            scene_offset += lrs_fx.mode;
            archive_entry = (RenderArchiveEntry_fx *)(archive_data + *scene_offset);
            for (prepared_frame_index = 0;
                 prepared_frame_index < 0x46 && archive_entry->available != 0;
                 prepared_frame_index++, archive_entry++) {
                payload_offset = 0x800;
                rseq_fx.prepared_frames[prepared_frame_index] =
                    (s32)(*scene_offset + archive_data) + (archive_entry->offset + payload_offset);
            }
            parse_space_scene_chunk(0);
        } else {
            if (*(volatile short *)&D_0015EF4A_fx != 0) {
                *(volatile short *)&D_0015EF4A_fx = 0;
            }
            slip_fx = 1;
            return;
        }
    } else if (rseq_fx.frame >= 0x60) {
        parse_space_scene_chunk(++rseq_fx.sequence_frame);
    }
    build_object_rotation_matrix();
    if (vctx_fx.projection_scale < cmfov_fx[lrs_fx.mode]) {
        vctx_fx.projection_scale = cmfov_fx[lrs_fx.mode];
    }
    update_view_context();
    scale = 1.0f;
    if (lrs_fx.mode == 4) {
        scale = (f32)(rseq_fx.end - rseq_fx.time) / (f32)rseq_fx.end;
        scale_vector(&first_position, &D_00160600_fx, scale);
        add_vectors(&D_001605F0_fx, &D_001605F0_fx, &first_position);
    }
    scale_vector(D_00160460_fx, &D_001D9AE0_fx[lrs_fx.mode],
                 (f32)(rseq_fx.time - scale_ticks(0x78)) * 20.0f * scale);
    D_00160504_fx = D_001D9B30_fx[lrs_fx.mode];
    for (object_index = 0; object_index < rseq_fx.count; object_index++) {
        object = D_0018CC98_fx[object_index];
        for (subframe = 0; subframe < 2; subframe++) {
            frame = rseq_fx.frame;
            current_frame = frame >> 1;
            next_frame = current_frame + 1;
            model = object->model;
            frame_offsets = (u8 *)model->frames[model->count - 1] + 0x1C;
            current_frame_pointer = (s32 *)(frame_offsets + current_frame * 4);
            next_frame_pointer = (s32 *)(frame_offsets + next_frame * 4);
            current_frame_payload = *current_frame_pointer + 0x10;
            next_frame_payload = *next_frame_pointer + 0x10;
            blend = convert_integer_to_float(frame & 1) * 0.5f + (f32)subframe * 0.25f;
            object->blend = blend;
            animation_positions = object->animation_positions;
            scale_vector(&first_position, animation_positions + current_frame * 16, 1.0f - blend);
            scale_vector(&second_position, animation_positions + next_frame * 16, object->blend);
            transform_count = 2;
            add_vectors(object->position, &first_position, &second_position);
            object->selected_a = 2;
            object->selected_b = transform_count;
            object->state_a = 0;
            object->state_b = 0;
            update_moby_animation_state(object);
            fast_mem_copy(object->frame_data_a + 0x10, (void *)current_frame_payload, 0x20);
            fast_mem_copy(object->frame_data_b + 0x10, (void *)next_frame_payload, 0x20);
            *(volatile u8 *)&object->opacity = 0xFF;
            *(volatile u8 *)&object->cached_selector = 0xFF;
            object->selected_index = 0x1FF;
            refresh_moby_spatial_bounds(object);
            object->selected_a = object->model->count - 1;
            object->selected_b = object->model->count - 1;
            calculate_object_transform(object, 1, &first_transform);
            calculate_object_transform(object, 2, &second_transform);
            lrs_fx.history_index = (lrs_fx.history_index + 1) & 0x1F;
            if (lrs_fx.history_count < 0x20) {
                lrs_fx.history_count++;
            }
            history_slot = lrs_fx.history_index;
            qcopy(&lrs_fx.primary_history_positions[history_slot],
                  &first_transform.rows[3]);
            qcopy(&lrs_fx.secondary_history_positions[history_slot],
                  &second_transform.rows[3]);
            if (lrs_fx.mode == 4) {
                if (cli_fx == 0 || (cli_fx == 1 && lavail_fx[3] == 0)) {
                    object->flags |= 1;
                    lrs_fx.history_count = 0;
                }
                if (rseq_fx.time > rseq_fx.end - 0x38) {
                    object->fade =
                        object->model->base_scale *
                        ((f32)(rseq_fx.end - rseq_fx.time) * 0.017857144f);
                    for (color_index = 0; color_index < 3; color_index++) {
                        D_001D9A30_fx[color_index].w0 =
                            (D_001D9A30_fx[color_index].w0 & 0xFFFFFF) |
                            ((D_0018CB20_b[0].end - D_0018CB20_c[0].time) << 24);
                    }
                } else {
                    for (color_index = 0; color_index < 3; color_index++) {
                        D_001D9A30_fx[color_index].w0 =
                            (D_001D9A30_fx[color_index].w0 & 0xFFFFFF) | 0x38000000;
                    }
                }
            }
        }
    }
    noop_callback_h();
}
