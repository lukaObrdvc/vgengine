#ifndef ARENA_H
#define ARENA_H


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


// @doc this is for arenas that are not fixe and you wanna make a lot of
inline Arena* arena_make(u64 reserveSize = ARENA_DEFAULT_RESERVE_SIZE, u64 initialCommit = ARENA_DEFAULT_COMMIT_SIZE)
{
    ASSERT(++ARENA_COUNT <= MAX_ARENAS);
    ASSERT(ARENA_MANAGER.base + ARENA_MANAGER.virtualMemoryUsed + reserveSize <= TOTAL_RESERVED_MEMORY);
    
    Arena newArena;
    newArena.base = ARENA_MANAGER.base + ARENA_MANAGER.virtualMemoryUsed;
    newArena.used = 0;
    newArena.commited = initialCommit;
#if DEVELOPER
    newArena.reserved = reserveSize;
#endif
 
    COMMIT_MEMORY(newArena.base, initialCommit);
    ARENA_MANAGER.virtualMemoryUsed += reserveSize;
    ARENAS[ARENA_COUNT] = newArena;
    return ARENAS + ARENA_COUNT;
}

// @doc this is for arenas you know you want, and you make them once in init somewhere
inline void arena_init(Arena* arena, u64 reserveSize = ARENA_DEFAULT_RESERVE_SIZE, u64 initialCommit = ARENA_DEFAULT_COMMIT_SIZE)
{
    ASSERT(ARENA_MANAGER.base + ARENA_MANAGER.virtualMemoryUsed + reserveSize <= TOTAL_RESERVED_MEMORY);
    
    arena->base = ARENA_MANAGER.base + ARENA_MANAGER.virtualMemoryUsed;
    arena->used = 0;
    arena->commited = initialCommit;
#if DEVELOPER
    arena->reserved = reserveSize;
#endif
 
    COMMIT_MEMORY(arena->base, initialCommit);
    ARENA_MANAGER.virtualMemoryUsed += reserveSize;
}

inline void* arena_push_size(Arena* arena, u64 size, u64 alignment)
{
    u64 unalignedBaseAddr = (u64)(arena->base + arena->used);
    u64 alignedBaseAddr = align_up(unalignedBaseAddr, alignment);
    u64 totalMemoryNeeded = arena->used + (alignedBaseAddr - unalignedBaseAddr) + size;

    if (totalMemoryNeeded > arena->commited)
    {
        u64 commitSize = align_up(totalMemoryNeeded - arena->commited, PAGE_SIZE);
        ASSERT(arena->commited + commitSize <= arena->reserved);
        COMMIT_MEMORY(arena->base + arena->commited, commitSize);
        arena->commited += commitSize;
    }

    arena->used = totalMemoryNeeded;
    return (void*)alignedBaseAddr;
}

template<typename T>
inline T* arena_push(Arena* arena, u32 count = 1)
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
    arena.used = 0;
}

#endif
