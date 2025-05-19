#if USE_DLL
extern "C" void platform_init_memory_base(Globals* memoryBase)
{    
    globals = memoryBase;
}
#endif


// @todo you can probably export this stuff and call it once

// @important debug arena stuff THOROUGHLY
void init_memory()
{
    ArenaManager* arenaManager = &ARENA_MANAGER;
    arenaManager->base = (u8*)(globals + 1);
    arenaManager->virtualMemoryUsed = sizeof(Globals);
    ASSERT(arenaManager->virtualMemoryUsed <= TOTAL_RESERVED_MEMORY);
    
    // @pot do I need to align down/up, if sizeof(Globals) makes it
    // unaligned or something?
    u64 firstArenaSize = INITIAL_COMMIT_SIZE_BY_PLATFORM - sizeof(Globals);
    // firstArenaSize = align_up(firstArenaSize, PAGE_SIZE);
    arena_init(&PERM_ARENA, firstArenaSize, firstArenaSize);
    arena_init(&FRAME_ARENA, gigabytes(4));
    
    EngineState* engine_state = ENGINESTATE;
    engine_state = arena_push<EngineState>(&PERM_ARENA);
    Arena frame_buffer_arena;
    Arena zbuffer_arena;
    arena_init(&frame_buffer_arena, MAX_FRAME_BUFFER_SIZE * FRAME_BUFFER_BYTPP, MAX_FRAME_BUFFER_SIZE * FRAME_BUFFER_BYTPP);
    arena_init(&zbuffer_arena, MAX_FRAME_BUFFER_SIZE, MAX_FRAME_BUFFER_SIZE);
    engine_state->frameBuffer.base = frame_buffer_arena.base;
    engine_state->zBuffer = (r32*)zbuffer_arena.base;
    
    // enginestate->frameBuffer.base = arena_push<u8>(&PERM_ARENA, MAX_FRAME_BUFFER_SIZE * FRAME_BUFFER_BYTPP);
    // engineState->zBuffer = arena_push<r32>(&PERM_ARENA, MAX_FRAME_BUFFER_SIZE);
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
