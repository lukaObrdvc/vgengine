#ifndef ARENA_H
#define ARENA_H


#define DYNARR_ARENA_DEFAULT_CAPACITY 4096 // 4 KB

#define MANAGING_ARENA globals->managing_arena
#define PERMANENT_ARENA globals->permanent_arena
#define TEMPORARY_ARENA globals->temporary_arena
#define DYNARR_ARENA globals->dynarr_arena


// if you wanna reuse space for temporary arena, you cache it's current
// size in scope, and then you set it to that after a number of function
// calls or end of scope

// @todo array stuff
// @pot free list version, which can reuse space

struct Arena
{
    u8* base;
    s32 size;
#if DEVELOPER
    s32 highest_size; // max(size, highest_size) when pushing
    s32 capacity; // assert(size+sizetopush <= capacity) when pushing
#endif
};


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
