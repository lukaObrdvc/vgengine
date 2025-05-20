#if USE_DLL
extern "C" void platform_init_memory_base(Globals* memory_base)
{    
    globals = memory_base;
}
#endif


// @todo you can probably export this stuff and call it once

void init_memory()
{
    Arena* managing_arena = &MANAGING_ARENA;
    managing_arena->base = (u8*)globals; // assume this address is aligned
    managing_arena->size = sizeof(Globals);
    managing_arena->highest_size = sizeof(Globals);
    managing_arena->capacity = TOTAL_PROGRAM_MEMORY;
    ASSERT(managing_arena->size <= managing_arena->capacity);
    
    arena_make(&PERMANENT_ARENA, megabytes(20));
    arena_make(&TEMPORARY_ARENA, gigabytes(2) - 1);
    
    Engine_state* engine_state = arena_push<Engine_state>(&PERMANENT_ARENA);
    engine_state->framebuffer.base = arena_push<u8>(&PERMANENT_ARENA, MAX_FRAMEBUFFER_SIZE * to_unsigned(FRAMEBUFFER_BYTPP));
    engine_state->zbuffer = arena_push<r32>(&PERMANENT_ARENA, MAX_FRAMEBUFFER_SIZE);
}

void init_engine_state()
{
    ENGINE_STATE->tested_once = 0;

    ENGINE_STATE->cursor.x = 640.0f;
    ENGINE_STATE->cursor.y = 360.0f;
    
    ENGINE_STATE->framebuffer.width = 1280.0f;
    ENGINE_STATE->framebuffer.height = 720.0f;

    ENGINE_STATE->framebuffer.base += FRAMEBUFFER_BYTESIZE - FRAMEBUFFER_PITCH;
    ENGINE_STATE->zbuffer += FRAMEBUFFER_WIDTH * (FRAMEBUFFER_HEIGHT - 1);

    MAIN_CAMERA.position = vec_make(640.0f, 360.0f, 0.0f);
    MAIN_CAMERA.orientation = quaternion_identity();
    MAIN_CAMERA.z_near = 5;
    MAIN_CAMERA.z_far = 500;
    MAIN_CAMERA.fov = 120;
                    
    ENGINE_STATE->camera_angle = 0;
    ENGINE_STATE->reverse_winding = false;
    
    ENGINE_STATE->keyflags = 0;
    ENGINE_STATE->mouseflags = 0;
                    
                    
    ENGINE_STATE->line_angle = 0;
    ENGINE_STATE->rect_angle = 0;
            
    ENGINE_STATE->camera_offs_x = 0;
    ENGINE_STATE->camera_offs_y = 0;

    ENGINE_STATE->do_da_thing = false;
    
    // @TODO you should probably have a default for everything but whatever
    // @TODO is this a good way to set a keymap, just setting powers of two.............
    ENGINE_STATE->keymap[0x25] = 0;
    ENGINE_STATE->keymap[0x26] = 1; 
    ENGINE_STATE->keymap[0x27] = 2; 
    ENGINE_STATE->keymap[0x28] = 3; 

    ENGINE_STATE->keymap[0x57] = 4;   
    ENGINE_STATE->keymap[0x53] = 5; 
    ENGINE_STATE->keymap[0x41] = 6; 
    ENGINE_STATE->keymap[0x44] = 7; 
    ENGINE_STATE->keymap[0x51] = 8; 
    ENGINE_STATE->keymap[0x45] = 9; 

    ENGINE_STATE->keymap[0x49] = 10;   
    ENGINE_STATE->keymap[0x4B] = 11;
    ENGINE_STATE->keymap[0x4A] = 12;
    ENGINE_STATE->keymap[0x4C] = 13;
    ENGINE_STATE->keymap[0x55] = 14;
    ENGINE_STATE->keymap[0x4F] = 15;

    zbuffer_reset();
        

    // ENGINE_STATE->brushes[BRUSH_SCANLINE2] = (((u32)255 << 24) |
                                             // ((u32)0 << 16) |
                                             // ((u32)0 << 8) |
                                             // 255);
}
