#ifndef ALIAS_H
#define ALIAS_H


#define PLATFORM_API globals->platformAPI
#define BYTPP globals->platformAPI.platformDisplay.bytesPerPixel
#define TOTAL_RESERVED_MEMORY PLATFORM_API.platformMemory.reservedMemory
#define PAGE_SIZE PLATFORM_API.platformMemory.pageSize
#define ARENA_MANAGER globals->arenaManager
#define ARENA_COUNT globals->arenaCount
#define ARENAS globals->arenas
#define PERM_ARENA &globals->permArena
#define FRAME_ARENA &globals->frameArena

#define INITIAL_COMMIT_SIZE_BY_PLATFORM 8388608 // 8MB
#define MAX_ARENAS 20
#define ARENA_DEFAULT_RESERVE_SIZE 4294967296 // 4GB
#define ARENA_DEFAULT_COMMIT_SIZE PAGE_SIZE
#define ARENA_COMMIT_STEP PAGE_SIZE

#define MAX_FRAMEBUFFER_SIZE 2073600 // 1920*1080



#endif
