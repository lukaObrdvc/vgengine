#ifndef ARENA_H
#define ARENA_H

// if you wanna reuse space for temporary arena, you cache it's current
// size in scope, and then you set it to that after a number of function
// calls or end of scope

#define MANAGING_ARENA  (&globals->managing_arena)
#define PERMANENT_ARENA (&globals->permanent_arena)
#define TEMPORARY_ARENA (&globals->temporary_arena)

struct Arena
{
    u8* base;
    u64 size;
#if DEVELOPER
    u64 highest_size;
    u64 capacity;
#endif
};

void arena_make(Arena* arena, u64 capacity);
void* arena_push_size(Arena* arena, const u64& size, const u64& alignment);

template<typename T>
T* arena_push(Arena* arena, u64 count = 1)
{
    return (T*) arena_push_size(arena, sizeof(T)*count, alignof(T));
}

inline void arena_reset(Arena* arena)
{
    arena->size = 0;
}

inline void arena_set_size(Arena* arena, u64 size)
{
    ASSERT(size <= arena->size);
    arena->size = size;
}


#define temp_alloc(type, ...) arena_push<type>(TEMPORARY_ARENA, __VA_ARGS__)


#endif
