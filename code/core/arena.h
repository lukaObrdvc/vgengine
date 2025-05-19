#ifndef ARENA_H
#define ARENA_H


#define MANAGING_ARENA globals->managing_arena
#define PERMANENT_ARENA globals->permanent_arena
#define TEMPORARY_ARENA globals->temporary_arena


// if you wanna reuse space for temporary arena, you cache it's current
// size in scope, and then you set it to that after a number of function
// calls or end of scope

// @todo array stuff
// @pot free list version, which can reuse space

// @todo maybe I need u32 or something
struct Arena
{
    u8* base;
    s32 size;
#if DEVELOPER
    s32 highest_size;
    s32 capacity;
#endif
};


void arena_init(Arena* arena, s32 capacity);
void* arena_push_size(Arena* arena, u32 size, u32 alignment);

template<typename T>
T* arena_push(Arena* arena, u32 count = 1)
{
    return (T*) arena_push_size(arena, sizeof(T)*count, alignof(T));
}

inline void zero_memory(void* ptr, u64 size)
{
    u8* byte_ptr = (u8*)ptr;
    for (u64 i = 0; i < size; i++)
    {
        byte_ptr[i] = 0;
    }
}

inline void arena_reset(Arena* arena)
{
    arena->size = 0;
}

inline void arena_set_size(Arena* arena, u32 size)
{
    ASSERT(size <= arena->size);
    arena->size = size;
}


#endif
