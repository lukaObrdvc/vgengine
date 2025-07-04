void arena_make(Arena* arena, u64 capacity)
{
    Arena* managing_arena = MANAGING_ARENA;
    
    arena->base = managing_arena->base + managing_arena->size;
    arena->size = 0;
#if DEVELOPER
    arena->highest_size = 0;
    arena->capacity = capacity;
#endif

    managing_arena->size += capacity;
    ASSERT(managing_arena->size <= managing_arena->capacity);
#if DEVELOPER
    managing_arena->highest_size = Max(managing_arena->highest_size, managing_arena->size);
#endif
}

void* arena_push_size(Arena* arena, const u64& size, const u64& alignment = 1)
{
    ASSERT(is_pow2(alignment));
    
    u64 unaligned_base = (u64)(arena->base + arena->size);
    u64 aligned_base = align_up(unaligned_base, alignment);
    arena->size += size + (aligned_base - unaligned_base);
    ASSERT(arena->size <= arena->capacity);
#if DEVELOPER
    arena->highest_size = Max(arena->highest_size, arena->size);
#endif
    return (void*)aligned_base;
}

inline void temp_reset()
{
    arena_reset(TEMPORARY_ARENA);
}

inline void temp_set_size(u64 size)
{
    arena_set_size(TEMPORARY_ARENA, size);
}

