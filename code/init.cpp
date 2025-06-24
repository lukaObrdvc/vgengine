void init_memory()
{
    Arena* managing_arena = MANAGING_ARENA;
    // @todo this should be aligned by alignof(Globals)
    managing_arena->base = (u8*)globals;
    managing_arena->size = sizeof(Globals);
#if DEVELOPER
    managing_arena->highest_size = sizeof(Globals);
    managing_arena->capacity = TOTAL_PROGRAM_MEMORY;
#endif
    ASSERT(managing_arena->size <= managing_arena->capacity);
    
    arena_make(PERMANENT_ARENA, 20 * MB);
    arena_make(TEMPORARY_ARENA, 2 * GB);
    arena_make(SCRATCH_ARENA, SCRATCH_POOL_SIZE * SCRATCH_CAPACITY);
    
    for (int i = 0; i < SCRATCH_POOL_SIZE; i++)
    {
        Scratch scratch;
        scratch.base = (u8*)arena_push_size(SCRATCH_ARENA, SCRATCH_CAPACITY);
        scratch.size = 0;
        scratch.index = i;
        
        globals->scratch_in_use[i] = false;
        globals->scratch_pool[i] = scratch;
    }
#if DEVELOPER
    globals->scratch_highest_size = 0;
    globals->scratch_highest_index = 0;
#endif
}

void init_engine_state()
{
    Engine_state* engine_state = arena_push<Engine_state>(PERMANENT_ARENA); // this used to be in init_memory ...
    Camera* camera = MAIN_CAMERA;
    Input* input = INPUT;
    Arena* permanent_arena = PERMANENT_ARENA;

    engine_state->running = true;
    
    u64* keys_base = arena_push<u64>(permanent_arena, dword_count_for_bit_count(NUM_KEYS));
    u64* keys_pressed_base = arena_push<u64>(permanent_arena, dword_count_for_bit_count(NUM_KEYS));
    u64* keys_released_base = arena_push<u64>(permanent_arena, dword_count_for_bit_count(NUM_KEYS));
    input->keys = bit_array_make(keys_base, NUM_KEYS);
    input->keys_pressed = bit_array_make(keys_pressed_base, NUM_KEYS);
    input->keys_released = bit_array_make(keys_released_base, NUM_KEYS);
    
    input->mkeys = unset_all_flags();
    input->mkeys_pressed = unset_all_flags();
    input->mkeys_released = unset_all_flags();
    input->moved_mouse = false;
    input->cursor_x = 0;
    input->cursor_y = 0;
    
    engine_state->tested_once = 0;

    engine_state->framebuffer_width = 1280;
    engine_state->framebuffer_height = 720;

    camera->position = vec_make(0.0f, 0.0f, 0.0f);
    camera->orientation = quaternion_identity();
    // @todo figure out proper z values and what they actually mean and so on...
    camera->z_near = 5;
    camera->z_far = 500;
    camera->fov = 120;

    engine_state->prng_seed = RANDOM_DEFAULT_SEED;
    engine_state->normalization_counter = 1;
    engine_state->aspect_ratio = FRAMEBUFFER_WIDTH / (r32)FRAMEBUFFER_HEIGHT;

    engine_state->camera_angle = 0;
    engine_state->line_angle = 0;
    engine_state->spin_angle = 0;
    engine_state->cube_scaling_factor = 1.0f;
    engine_state->cube_scale_up = true;
}

extern "C" void platform_init_engine(Platform_init_result* out)
{
    init_memory();
    init_engine_state();
    
    load_font_permanently(str("Consolas8"), MYFONT_CONSOLAS8);
    load_font_permanently(str("Consolas16"), MYFONT_CONSOLAS16);
    load_font_permanently(str("Consolas32"), MYFONT_CONSOLAS32);
    load_font_permanently(str("Consolas64"), MYFONT_CONSOLAS64);

    out->input_address = INPUT;
    out->window_width = 1280; // + 16
    out->window_height = 720; // +39
    out->window_offs_x = 50;
    out->window_offs_y = 50;
}

#if USE_DLL
extern "C" void platform_init_memory_base(Globals* memory_base)
{
    globals = memory_base;
}
#endif

