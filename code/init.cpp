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
    // @todo assert onto arenaManager here?

    // @pot do I need to align down/up, if sizeof(Globals) makes it
    // unaligned or something?
    u64 firstArenaSize = INITIAL_COMMIT_SIZE_BY_PLATFORM - sizeof(Globals);
    firstArenaSize = align_up(firstArenaSize, PAGE_SIZE);
    arena_init(&PERM_ARENA, firstArenaSize, firstArenaSize);

    MEMORY_BASIC_INFORMATION mbi3;
    VirtualQuery((&PERM_ARENA)->base + (&PERM_ARENA)->commited + 100, &mbi3, sizeof(mbi3));
    
    arena_init(&FRAME_ARENA, gigabytes(4));

    MEMORY_BASIC_INFORMATION mbi4;
    VirtualQuery((&FRAME_ARENA)->base + (&FRAME_ARENA)->commited, &mbi4, sizeof(mbi4));

    MEMORY_BASIC_INFORMATION mbi2;
    VirtualQuery(ARENA_MANAGER.base + ARENA_MANAGER.virtualMemoryUsed, &mbi2, sizeof(mbi2));
    
    EngineState* engineState = ENGINESTATE;
    engineState = arena_push<EngineState>(&PERM_ARENA);
    engineState->frameBuffer.base = arena_push<u8>(&PERM_ARENA, MAX_FRAME_BUFFER_SIZE * FRAME_BUFFER_BYTPP);
    // engineState->frameBuffer.base = (u8*)arena_push<u32>(&PERM_ARENA, MAX_FRAME_BUFFER_SIZE);
    engineState->zBuffer = arena_push<r32>(&PERM_ARENA, MAX_FRAME_BUFFER_SIZE);
    // commit_memory((u8*)ENGINESTATE->zBuffer, MAX_FRAME_BUFFER_SIZE * sizeof(r32));
    ASSERT(ENGINESTATE->zBuffer != NULL);
    MEMORY_BASIC_INFORMATION mbi;
    VirtualQuery(ENGINESTATE->zBuffer, &mbi, sizeof(mbi));
    int i = 0;
    i++;

    u8* zStart = (u8*)ENGINESTATE->zBuffer;
    u8* zEnd   = zStart + (MAX_FRAME_BUFFER_SIZE * sizeof(r32));
    u8* commitStart = ARENA_MANAGER.base;
    u8* commitEnd   = ARENA_MANAGER.base + INITIAL_COMMIT_SIZE_BY_PLATFORM;

    ASSERT(zStart >= commitStart && zEnd <= commitEnd);
}

// 4194304
// 20971520

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


    ASSERT(ENGINESTATE->zBuffer != NULL);
    MEMORY_BASIC_INFORMATION mbi;
    VirtualQuery(ENGINESTATE->zBuffer, &mbi, sizeof(mbi));
    
    *(Z_BUFFER) = Z_BUFFER_RESET_VALUE;
    
    r32* zb = ENGINESTATE->zBuffer;
    r32* zb_end = zb + MAX_FRAME_BUFFER_SIZE;
    
    r32* zbuffer_base = Z_BUFFER;
    
    r32* access_00 = zbuffer_access(0, 0);
    r32* access_00_end = access_00 + 1;
    
    ASSERT(zbuffer_base >= zb && zbuffer_base < zb_end);
    ASSERT(access_00 >= zb && access_00_end <= zb_end);
    
    r32* raw_start = (r32*)ENGINESTATE->zBuffer;
    r32* raw_end   = raw_start + FRAME_BUFFER_WIDTH * FRAME_BUFFER_HEIGHT;
    
    r32* test_ptr = zbuffer_access(0, 0);
    ASSERT(test_ptr >= raw_start && test_ptr < raw_end);
    *test_ptr = Z_BUFFER_RESET_VALUE;
    // *zbuffer_access(0, 0) = Z_BUFFER_RESET_VALUE;
    
    zbuffer_reset();
        

    // ENGINESTATE->brushes[BRUSH_SCANLINE2] = (((u32)255 << 24) |
                                             // ((u32)0 << 16) |
                                             // ((u32)0 << 8) |
                                             // 255);
}
