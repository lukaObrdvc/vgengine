// @doc this is for arenas you know you want, and you make them once in init somewhere
void arena_make(Arena* arena, s32 capacity)
{
    Arena* managing_arena = &MANAGING_ARENA;
    u8* unaligned_base = managing_arena->base + managing_arena->size;
    u8* aligned_base = align_up(unaligned_base, 2); // is this good?
    
    arena->base = aligned_base;
    arena->size = 0;
#if DEVELOPER
    arena->highest_size = 0;
    arena->capacity = capacity;
#endif

    managing_arena->size += capacity + (aligned_base - unaligned_base);
    ASSERT(managing_arena->size <= managing_arena->capacity);
    managing_arena->highest_size = Max(managing_arena->highest_size, managing_arena->size);
}

void* arena_push_size(Arena* arena, u32 size, u32 alignment)
{
    u32 unaligned_base = (u32)(arena->base + arena->size);
    u32 aligned_base = align_up(unaligned_base, alignment);
    arena->size += size + (aligned_base - unaligned_base);
    ASSERT(arena->size <= arena->capacity);
    arena->highest_size = Max(arena->highest_size, arena->size);
    return (void*)aligned_base;
}

