// @doc this is for arenas you know you want, and you make them once in init somewhere
void arena_init(Arena* arena, u64 reserveSize = ARENA_DEFAULT_RESERVE_SIZE, u64 initialCommit = ARENA_DEFAULT_COMMIT_SIZE)
{
    ArenaManager* arenaManager = &ARENA_MANAGER;
    ASSERT(arenaManager->virtualMemoryUsed + reserveSize <= TOTAL_RESERVED_MEMORY);
    
    arena->base = arenaManager->base + arenaManager->virtualMemoryUsed;
    arena->used = 0;
    arena->commited = initialCommit;
#if DEVELOPER
    arena->reserved = reserveSize;
#endif
 
    COMMIT_MEMORY(arena->base, initialCommit);
    arenaManager->virtualMemoryUsed += reserveSize;
}

// @todo maybe caches ARENAS and ARENA_COUNT in scope?
// @doc this is for arenas that are not fixe and you wanna make a lot of
Arena* arena_make(u64 reserveSize = ARENA_DEFAULT_RESERVE_SIZE, u64 initialCommit = ARENA_DEFAULT_COMMIT_SIZE)
{
    ASSERT(++ARENA_COUNT <= MAX_ARENAS);
    ArenaManager* arenaManager = &ARENA_MANAGER;
    ASSERT(arenaManager->virtualMemoryUsed + reserveSize <= TOTAL_RESERVED_MEMORY);
    
    Arena newArena;
    newArena.base = arenaManager->base + arenaManager->virtualMemoryUsed;
    newArena.used = 0;
    newArena.commited = initialCommit;
#if DEVELOPER
    newArena.reserved = reserveSize;
#endif
 
    COMMIT_MEMORY(newArena.base, initialCommit);
    arenaManager->virtualMemoryUsed += reserveSize;
    ARENAS[ARENA_COUNT] = newArena;
    return ARENAS + ARENA_COUNT;
}

void* arena_push_size(Arena* arena, u64 size, u64 alignment)
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

