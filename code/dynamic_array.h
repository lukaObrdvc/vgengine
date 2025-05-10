#ifndef DYNAMIC_ARRAY_H
#define DYNAMIC_ARRAY_H


// @doc size and capacity in bytes
template<typename T>
struct DynArr
{
    T* base;
    u32 size;
    u32 capacity;
};

template<typename T>
struct DynArr64
{
    T* base;
    u64 size;
    u64 capacity;
};

template<typename T>
struct Slice
{
    T* base;
    u32 size;
};

// regular static array: base + size

// pushable static array: base + curr_size (and then you just assume
//   you never exceed max_size, and this size is allocated in the
//   arena (or whatever else, but you know when you're pusing that
//   you're not overwriting useful stuff, and memory is valid and so on))

// non-resizable dynamic array: base + size + capacity (like the previous
//   but you keep track of capacity for error checking basically...)

// resizable dynamic array: base + size + capacity (when size exceeds
//   capacity, then you allocate a new capacity (probably 2*old) onto
//   arena (or wherever) and you copy elements there, and you don't
//   reuse old memory..)




#endif
