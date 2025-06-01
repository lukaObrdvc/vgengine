#ifndef ARENA_H
#define ARENA_H

// arena allocator is an allocator that allocates on a pre-defined
// region of memory, so objects can be efficiently deallocated all at once

// bump allocator is an arena allocator that doesn't use
// a free list (just bumps pointer forward when allocating)

// objects grouped by common size:
// block allocator is an array, where every element is a block
// and the stuff that you actually put in blocks can be lower
// size than the size of the block (internal fragmentation)
// but asserts if it's bigger

// objects grouped by common lifetimes:
// stack allocator is just an arena but using arena_set_size
// scratch allocator is just an arena but using arena_reset




// @todo use & ? instead of placing & everywhere frickin else

#define MANAGING_ARENA globals->managing_arena
#define PERMANENT_ARENA globals->permanent_arena
#define TEMPORARY_ARENA globals->temporary_arena



// @todo alignas?

// if you wanna reuse space for temporary arena, you cache it's current
// size in scope, and then you set it to that after a number of function
// calls or end of scope

// @todo array stuff
// @pot free list version, which can reuse space

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

inline void arena_set_size(Arena* arena, u64 size)
{
    ASSERT(size <= arena->size);
    arena->size = size;
}


#define temp_alloc(type, ...) arena_push<type>(&TEMPORARY_ARENA, __VA_ARGS__)


#endif
