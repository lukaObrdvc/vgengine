void init_memory()
{
    Arena* managing_arena = MANAGING_ARENA;
    // @todo this should be aligned by alignof(Globals)
    managing_arena->base = (u8*)globals;
    managing_arena->size = sizeof(Globals);
    managing_arena->highest_size = sizeof(Globals);
    managing_arena->capacity = TOTAL_PROGRAM_MEMORY;
    ASSERT(managing_arena->size <= managing_arena->capacity);
    
    arena_make(PERMANENT_ARENA, 20 * MB);
    arena_make(TEMPORARY_ARENA, 2 * GB);
    arena_make(SCRATCH_ARENA, SCRATCH_POOL_SIZE * SCRATCH_CAPACITY);
    
    Engine_state* engine_state = arena_push<Engine_state>(PERMANENT_ARENA);
    // @todo push this once once per frame instead, that way you don't waste space as well
    engine_state->framebuffer.base = arena_push<u8>(PERMANENT_ARENA, MAX_FRAMEBUFFER_SIZE * to_unsigned(BYTPP));
    engine_state->zbuffer = arena_push<r32>(PERMANENT_ARENA, MAX_FRAMEBUFFER_SIZE);
    
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
    Engine_state* engine_state = ENGINE_STATE;
    Camera* camera = MAIN_CAMERA;

    // @todo do I place these pushes elsewhere?
    u64* keys_base = arena_push<u64>(PERMANENT_ARENA, dword_count_for_bit_count(NUM_KEYS));
    u64* keys_pressed_base = arena_push<u64>(PERMANENT_ARENA, dword_count_for_bit_count(NUM_KEYS));
    u64* keys_released_base = arena_push<u64>(PERMANENT_ARENA, dword_count_for_bit_count(NUM_KEYS));
    INPUT->keys = bit_array_make(keys_base, NUM_KEYS);
    INPUT->keys_pressed = bit_array_make(keys_pressed_base, NUM_KEYS);
    INPUT->keys_released = bit_array_make(keys_released_base, NUM_KEYS);
    
    INPUT->mkeys = unset_all_flags();
    INPUT->mkeys_pressed = unset_all_flags();
    INPUT->mkeys_released = unset_all_flags();
    INPUT->moved_mouse = false;
    INPUT->cursor_x = 0;
    INPUT->cursor_y = 0;
    
    engine_state->tested_once = 0;

    engine_state->framebuffer.width = 1280.0f;
    engine_state->framebuffer.height = 720.0f;

    // @doc this guarantees Y is up when accessing these buffers
    engine_state->framebuffer.base += FRAMEBUFFER_BYTESIZE - FRAMEBUFFER_PITCH;
    engine_state->zbuffer += FRAMEBUFFER_WIDTH * (FRAMEBUFFER_HEIGHT - 1);

    camera->position = vec_make(0.0f, 0.0f, 0.0f);
    camera->orientation = quaternion_identity();
    camera->z_near = 5; // @todo are these goodio?
    camera->z_far = 500;
    camera->fov = 120;

    engine_state->prng_seed = RANDOM_DEFAULT_SEED;
    engine_state->normalization_counter = 1;
    engine_state->aspect_ratio = FRAMEBUFFER_WIDTH / (r32)FRAMEBUFFER_HEIGHT;

    // @todo rename to better
    engine_state->camera_angle = 0;
    engine_state->line_angle = 0;
    engine_state->spin_angle = 0;
    engine_state->cube_scaling_factor = 1.0f;
    engine_state->cube_scale_up = true;
    
    zbuffer_reset();
}

extern "C" void platform_init_engine(Platform_init_out* out)
{
    init_memory();
    init_engine_state();

    out->input_address = INPUT;
}

#if USE_DLL
extern "C" void platform_init_memory_base(Globals* memory_base)
{
    globals = memory_base;
}
#endif

