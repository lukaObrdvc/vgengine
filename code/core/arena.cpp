void arena_make(Arena* arena, u32 capacity)
{
    Arena* managing_arena = &MANAGING_ARENA;
    u64 unaligned_base = (u64)(managing_arena->base + managing_arena->size);
    u64 aligned_base = align_up(unaligned_base, 1); // is this good?
    
    arena->base = (u8*)aligned_base;
    arena->size = 0;
#if DEVELOPER
    arena->highest_size = 0;
    arena->capacity = to_signed(capacity);
#endif

    managing_arena->size += capacity + (aligned_base - unaligned_base);
    ASSERT(managing_arena->size <= managing_arena->capacity);
    managing_arena->highest_size = Max(managing_arena->highest_size, managing_arena->size);
}

void* arena_push_size(Arena* arena, const u64& size, const u64& alignment)
{
    u64 unaligned_base = (u64)(arena->base + arena->size);
    u64 aligned_base = align_up(unaligned_base, alignment);
    arena->size += size + (aligned_base - unaligned_base);
    ASSERT(arena->size <= arena->capacity);
    arena->highest_size = Max(arena->highest_size, arena->size);
    return (void*)aligned_base;
}

#define temp_alloc(type, ...) arena_push<type>(&TEMPORARY_ARENA, __VA_ARGS__)
