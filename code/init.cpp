void init_memory()
{
    Arena* managing_arena = &MANAGING_ARENA;
    // @todo this should be aligned by alignof(Globals)
    managing_arena->base = (u8*)globals;
    managing_arena->size = sizeof(Globals);
    managing_arena->highest_size = sizeof(Globals);
    managing_arena->capacity = TOTAL_PROGRAM_MEMORY;
    ASSERT(managing_arena->size <= managing_arena->capacity);
    
    arena_make(&PERMANENT_ARENA, megabytes(20));
    arena_make(&TEMPORARY_ARENA, gigabytes(2) - 1); // max for s32?
    arena_make(&SCRATCH_ARENA, SCRATCH_POOL_SIZE * SCRATCH_CAPACITY);
    
    Engine_state* engine_state = arena_push<Engine_state>(&PERMANENT_ARENA);
    engine_state->framebuffer.base = arena_push<u8>(&PERMANENT_ARENA, MAX_FRAMEBUFFER_SIZE * to_unsigned(FRAMEBUFFER_BYTPP));
    engine_state->zbuffer = arena_push<r32>(&PERMANENT_ARENA, MAX_FRAMEBUFFER_SIZE);

    for (int i = 0; i < SCRATCH_POOL_SIZE; i++)
    {
        Scratch scratch;
        scratch.base = (u8*)arena_push_size(&SCRATCH_ARENA, SCRATCH_CAPACITY);
        scratch.size = 0;
        scratch.index = i;
        
        globals->scratch_in_use[i] = false;
        globals->scratch_pool[i] = scratch;
    }
#if DEVELOPER
    globals->scratch_highest_size = 0;
#endif
}

void init_engine_state()
{
    Engine_state* engine_state = ENGINE_STATE;
    
    engine_state->tested_once = 0;

    engine_state->cursor.x = 640.0f;
    engine_state->cursor.y = 360.0f;
    
    engine_state->framebuffer.width = 1280.0f;
    engine_state->framebuffer.height = 720.0f;

    engine_state->framebuffer.base += FRAMEBUFFER_BYTESIZE - FRAMEBUFFER_PITCH;
    engine_state->zbuffer += FRAMEBUFFER_WIDTH * (FRAMEBUFFER_HEIGHT - 1);

    MAIN_CAMERA.position = vec_make(0.0f, 0.0f, 0.0f);
    // MAIN_CAMERA.position = vec_make(640.0f, 360.0f, 0.0f);
    MAIN_CAMERA.orientation = quaternion_identity();
    MAIN_CAMERA.z_near = 5; // 5     500
    MAIN_CAMERA.z_far = 500; // 500  -500
    MAIN_CAMERA.fov = 120;

    engine_state->normalization_counter = 1;
    
    engine_state->camera_angle = 0;
    engine_state->line_angle = 0;
    engine_state->spin_angle = 0;
    
    engine_state->reverse_winding = false;
    
    engine_state->keyflags = 0;
    engine_state->mouseflags = 0;
                    
    // @TODO you should probably have a default for everything but whatever
    // @TODO is this a good way to set a keymap, just setting powers of two.............
    engine_state->keymap[0x25] = 0;
    engine_state->keymap[0x26] = 1; 
    engine_state->keymap[0x27] = 2; 
    engine_state->keymap[0x28] = 3; 

    engine_state->keymap[0x57] = 4;   
    engine_state->keymap[0x53] = 5; 
    engine_state->keymap[0x41] = 6; 
    engine_state->keymap[0x44] = 7; 
    engine_state->keymap[0x51] = 8; 
    engine_state->keymap[0x45] = 9; 

    engine_state->keymap[0x49] = 10;   
    engine_state->keymap[0x4B] = 11;
    engine_state->keymap[0x4A] = 12;
    engine_state->keymap[0x4C] = 13;
    engine_state->keymap[0x55] = 14;
    engine_state->keymap[0x4F] = 15;

    zbuffer_reset();
        

    // ENGINE_STATE->brushes[BRUSH_SCANLINE2] = (((u32)255 << 24) |
    // ((u32)0 << 16) |
    // ((u32)0 << 8) |
    // 255);
}

extern "C" void platform_init_engine()
{
    init_memory();
    init_engine_state();
}

#if USE_DLL
extern "C" void platform_init_memory_base(Globals* memory_base)
{
    globals = memory_base;
}
#endif

