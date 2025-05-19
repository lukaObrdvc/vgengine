#if USE_DLL
extern "C" void platform_init_memory_base(Globals* memoryBase)
{    
    globals = memoryBase;
}
#endif


// @todo you can probably export this stuff and call it once

void init_memory()
{
    Arena* managing_arena = &MANAGING_ARENA;
    managing_arena.base = globals; // assume this address is aligned
    managing_arena.size = sizeof(Globals);
    managing_arena.highest_size = sizeof(Globals);
    managing_arena.capacity = TOTAL_PROGRAM_MEMORY;
    ASSERT(managing_arena.size <= managing_arena.capacity);
    
    arena_init(&PERMANENT_ARENA, megabytes(2));
    arena_init(&TEMPORARY_ARENA, gigabytes(2));
    
    Engine_state* engine_state = arena_push<EngineState>(&PERMANENT_ARENA);
    engine_state->frame_buffer.base = arena_push<u8>(&PERMANENT_ARENA, MAX_FRAME_BUFFER_SIZE * FRAME_BUFFER_BYTPP);
    engine_state->zbuffer = arena_push<r32>(&PERMANENT_ARENA, MAX_FRAME_BUFFER_SIZE);
}

void init_engine_state()
{
    ENGINESTATE->tested_once = 0;

    ENGINESTATE->cursor.x = 640.0f;
    ENGINESTATE->cursor.y = 360.0f;
    
    ENGINESTATE->frameBuffer.width = 1280.0f;
    ENGINESTATE->frameBuffer.height = 720.0f;

    MAIN_CAMERA.position = vec_make(640.0f, 360.0f, 0.0f);
    MAIN_CAMERA.orientation = quaternion_identity();
    MAIN_CAMERA.zNear = 5;
    MAIN_CAMERA.zFar = 500;
    MAIN_CAMERA.fov = 120;
                    
    ENGINESTATE->camera_angle = 0;
    ENGINESTATE->reverse_winding = false;
    
    ENGINESTATE->keyflags = 0;
    ENGINESTATE->mouseflags = 0;
                    
                    
    ENGINESTATE->line_angle = 0;
    ENGINESTATE->rect_angle = 0;
            
    ENGINESTATE->camera_offs_x = 0;
    ENGINESTATE->camera_offs_y = 0;

    
    // @TODO you should probably have a default for everything but whatever
    // @TODO is this a good way to set a keymap, just setting powers of two.............
    ENGINESTATE->keymap[0x25] = 0;
    ENGINESTATE->keymap[0x26] = 1; 
    ENGINESTATE->keymap[0x27] = 2; 
    ENGINESTATE->keymap[0x28] = 3; 

    ENGINESTATE->keymap[0x57] = 4;   
    ENGINESTATE->keymap[0x53] = 5; 
    ENGINESTATE->keymap[0x41] = 6; 
    ENGINESTATE->keymap[0x44] = 7; 
    ENGINESTATE->keymap[0x51] = 8; 
    ENGINESTATE->keymap[0x45] = 9; 

    ENGINESTATE->keymap[0x49] = 10;   
    ENGINESTATE->keymap[0x4B] = 11;
    ENGINESTATE->keymap[0x4A] = 12;
    ENGINESTATE->keymap[0x4C] = 13;
    ENGINESTATE->keymap[0x55] = 14;
    ENGINESTATE->keymap[0x4F] = 15;

    zbuffer_reset();
        

    // ENGINESTATE->brushes[BRUSH_SCANLINE2] = (((u32)255 << 24) |
                                             // ((u32)0 << 16) |
                                             // ((u32)0 << 8) |
                                             // 255);
}
