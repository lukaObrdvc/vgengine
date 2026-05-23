typedef void* (*Allocate) (void*, uint64, uint64);
typedef void (*Deallocate) (void*, void*);

struct Allocator
{
    Allocate allocate;
    Deallocate deallocate;
    void* instance;
};

struct Arena
{
    uint8* base;
    uint64 size;
#if DEVELOPER
    uint64 highest_size;
    uint64 capacity;
#endif
};

void* arena_push(void* instance, const uint64& size, const uint64& alignment = 1)
{
    assert(is_pow2(alignment));

    Arena* arena = (Arena*)instance;
    
    uint64 unaligned_base = (uint64)(arena->base + arena->size);
    uint64 aligned_base = align_up(unaligned_base, alignment);
    arena->size += size + (aligned_base - unaligned_base);
    assert(arena->size <= arena->capacity);
#if DEVELOPER
    arena->highest_size = _max(arena->highest_size, arena->size);
#endif
return (void*)aligned_base;
}

void arena_pop(void* instance, void* addr) {(void)0;}

Arena frame_arena;

Allocator frame_arena = {
    allocate = arena_push,
    deallocate = arena_pop,
    instance = &frame_arena
};
