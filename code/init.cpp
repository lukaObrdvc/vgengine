#if USE_DLL
void platform_init_memory_base(Globals* memoryBase)
{    
    globals = memoryBase;
}
#endif


// @todo you can probably export this stuff and call it once

void init_memory()
{
    ArenaManager* arenaManager = &ARENA_MANAGER;
    arenaManager->base = (u8*)(globals + 1);
    arenaManager->virtualMemoryUsed = sizeof(Globals);
    // @todo assert onto arenaManager here?

    // @pot do I need to align down/up, if sizeof(Globals) makes it
    // unaligned or something?
    u64 firstArenaSize = INITIAL_COMMIT_SIZE_BY_PLATFORM - sizeof(Globals);
    arena_init(&PERM_ARENA, firstArenaSize, firstArenaSize);
    arena_init(&FRAME_ARENA, gigabytes(4));
    
    EngineState* engineState = ENGINESTATE;
    engineState->frameBuffer.base = arena_push<u8>(&PERM_ARENA, MAX_FRAME_BUFFER_SIZE * FRAME_BUFFER_BYTPP);
    engineState->zBuffer = (u8*)arena_push<r32>(&PERM_ARENA, MAX_FRAME_BUFFER_SIZE);
}

void init_engine_state()
{
    s32 init_FRAME_BUFFER_WIDTH    = 1280;
    s32 init_FRAME_BUFFER_HEIGHT   = 720;
    s32 init_wnd_center_x = 640;
    s32 init_wnd_center_y = 360;
    
    
    ENGINESTATE->tested_once = 0;

    ENGINESTATE->cursor.x = init_wnd_center_x;
    ENGINESTATE->cursor.y = init_wnd_center_y;
    
    FRAME_BUFFER_WIDTH = init_FRAME_BUFFER_WIDTH;
    FRAME_BUFFER_HEIGHT = init_FRAME_BUFFER_HEIGHT;

    MAIN_CAMERA.position = vec_make(640.0f, 360.0f, 0.0f);
    MAIN_CAMERA.orientation = quaternion_identity();
    MAIN_CAMERA.zNear = 5;
    MAIN_CAMERA.zFar = 500;
    MAIN_CAMERA.fov = 120;
                    
    ENGINESTATE->camera_angle = 0;
    ENGINESTATE->log_to_file_once = false;
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
