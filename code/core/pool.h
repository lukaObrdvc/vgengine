#ifndef POOL_H
#define POOL_H

#define POOL_MAX_ALIGNMENT 16

struct Pool
{
    u8* base;
    s32 num_slots;
    s32 slot_size; // @doc bytesize (always a multiple of POOL_MAX_ALIGNMENT)
    Bit_array flags; // slot occupied or not
    // most recently used, because that's actually more efficient than lru when swapping
    s32 mru;
    s32 first_free; // first_free == -1  => no free
};

inline s32 pool_bytesize(Pool* pool)
{
    return pool->num_slots * pool->slot_size;
}

// @todo what about allocating on scratch (or pool itself?) ?
// welp just do it manually I quess, because you need to deal with this alignment
inline Pool pool_make(Arena* arena, s32 num_slots, s32 slot_size)
{
    s32 new_size = align_up(slot_size, POOL_MAX_ALIGNMENT);
    u8* base = (u8*)arena_push_size(arena, new_size * num_slots, POOL_MAX_ALIGNMENT);
    u64* flags_base = arena_push<u64>(arena, dword_count_for_bit_count(num_slots));
    
    Bit_array flags = bit_array_make(flags_base, num_slots);
    return {base, num_slots, new_size, flags, -1, 0};
}

inline u8* pool_address_of_slot(Pool* pool, s32 i)
{
    ASSERT(i >= 0 && i < pool->num_slots);
    
    return pool->base + pool->slot_size * i;
}

inline void pool_acquire(Pool* pool, s32 i)
{
    ASSERT(i >= 0 && i < pool->num_slots);
    
    bit_array_set(pool->flags, to_unsigned(i));
    if (pool->first_free == i)
    {
        pool->first_free = bit_array_find_first_zero(pool->flags);
    }
}
inline void pool_release(Pool* pool, s32 i)
{
    ASSERT(i >= 0 && i < pool->num_slots);
    
    bit_array_unset(pool->flags, to_unsigned(i));
    if (pool->first_free == -1)
    {
        pool->first_free = i;
    }
}

template<typename T>
T& pool_get(Pool* pool, s32 i)
{
    ASSERT(sizeof(T) <= pool->slot_size);
    ASSERT(alignof(T) <= POOL_MAX_ALIGNMENT);
    pool->mru = i;
   
    return *((T*)(pool_address_of_slot(pool, i)));
}
template<typename T>
void pool_set(Pool* pool, const T& value, s32 i)
{
    ASSERT(sizeof(T) <= pool->slot_size);
    ASSERT(alignof(T) <= POOL_MAX_ALIGNMENT);
    pool->mru = i;
    
    *((T*)(pool_address_of_slot(pool, i))) = value;
}

inline void pool_release_all(Pool* pool)
{
    bit_array_unset_all(pool->flags);
    pool->first_free = 0;
}
inline void pool_acquire_all(Pool* pool)
{
    bit_array_set_all(pool->flags);
    pool->first_free = -1;
}

inline b32 pool_slot_occupied(Pool* pool, s32 i)
{
    ASSERT(i >= 0 && i < pool->num_slots);    
    
    return bit_array_get(pool->flags, to_unsigned(i));
}

inline b32 pool_all_occupied(Pool* pool)
{
    return pool->first_free == -1;
}

// do we return a b32 instead of slot index?
template<typename T>
s32 pool_insert(Pool* pool, const T& value)
{
    ASSERT(sizeof(T) <= pool->slot_size);
    ASSERT(alignof(T) <= POOL_MAX_ALIGNMENT);
    
    s32 i = pool->first_free == -1 ? pool->mru : pool->first_free;
    pool_acquire(pool, i);
    pool_force_set(pool, value, i);
    return i;
}


// pool_toggle?

#endif
