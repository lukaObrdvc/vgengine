#ifndef ARENA_H
#define ARENA_H


#define MAX_ARENAS 20
#define ARENA_DEFAULT_RESERVE_SIZE 4294967296 // 4GB
#define ARENA_DEFAULT_COMMIT_SIZE PAGE_SIZE
#define ARENA_COMMIT_STEP PAGE_SIZE

#define ARENA_MANAGER globals->arenaManager
#define ARENA_COUNT globals->arenaCount
#define PERM_ARENA globals->permArena
#define FRAME_ARENA globals->frameArena
#define ARENAS globals->arenas


struct ArenaManager
{
    u8* base;
    u64 virtualMemoryUsed;
};

struct Arena
{
    u8* base;
    u64 used;
    u64 commited;
#if DEVELOPER
    u64 reserved;
#endif
};


// pop (but this is probably only for scratch based)
// free (with free list stuff)


void arena_init(Arena* arena, u64 reserveSize, u64 commitSize);
Arena* arena_make(u64 reserveSize, u64 commitSize);
void* arena_push_size(Arena* arena, u64 size, u64 alignment);

template<typename T>
T* arena_push(Arena* arena, u32 count = 1)
{
    return (T*) arena_push_size(arena, sizeof(T)*count, alignof(T));
}

inline void zero_memory(void* ptr, u64 size)
{
    u8* bytePtr = (u8*)ptr;
    for (u64 i = 0; i < size; ++i)
        {
            bytePtr[i] = 0;
        }
}

inline void arena_reset(Arena* arena)
{
    arena->used = 0;
}


#endif
