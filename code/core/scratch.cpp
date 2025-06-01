inline Scratch* get_scratch()
{
    for (int i = 0; i < SCRATCH_POOL_SIZE; i++)
    {
        if (!globals->scratch_in_use[i])
        {
            globals->scratch_in_use[i] = true;
            return &globals->scratch_pool[i];
        }
    }
    // trying to get a scratch when there isn't a free one
    ASSERT(false);
    return {0};
}

inline void release_scratch(Scratch* scratch)
{
    scratch->size = 0;
    globals->scratch_in_use[scratch->index] = false;
}

inline void* scratch_push_size(Scratch* scratch, s32 size, s32 alignment)
{
    u64 unaligned_base = (u64)(scratch->base + scratch->size);
    u64 aligned_base = align_up(unaligned_base, (u64)alignment);
    scratch->size += size + (aligned_base - unaligned_base);
    ASSERT(scratch->size <= SCRATCH_CAPACITY);
#if DEVELOPER
    globals->scratch_highest_size = Max(scratch->size, globals->scratch_highest_size);
#endif
    return (void*)aligned_base;
}

