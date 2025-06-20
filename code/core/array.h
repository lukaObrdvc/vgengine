#ifndef ARRAY_H
#define ARRAY_H

// basically just pick a right tradeoff between giving arrays too large of a capacity
// so that you waste too much memory, and giving arrays too small of a capacity so you
// fire too many assertions which causes friction during development...

// @todo put platform.h depends into array.cpp

#define ARRAY_DEFAULT_CAPACITY 2048 // 2 KB


template<typename T>
struct Array
{
    T* base;
    s32 size;
    s32 capacity;
#if DEVELOPER
    s32 highest_size;
#endif

    // set
    inline T& operator[](s32 index)
    {
        ASSERT(index >= 0 && index < size);
        return base[index];
    }
    // get
    inline const T& operator[](s32 index) const
    {
        ASSERT(index >= 0 && index < size);
        return base[index];
    }
};

inline s32 arr_cap(s32 n = 0, s32 cap = ARRAY_DEFAULT_CAPACITY)
{
    return mul2(cap, n);
}

template<typename T>
Array<T> arr_make(T* base, s32 capacity)
{
#if DEVELOPER
    return {base, 0, capacity, 0};
#else
    return {base, 0, capacity};
#endif
}

template<typename T>
Array<T> tarr_make(s32 n = 0, s32 cap = ARRAY_DEFAULT_CAPACITY)
{
    s32 capacity = mul2(cap, n);
    T* base = temp_alloc(T, capacity);
#if DEVELOPER
    return {base, 0, capacity, 0};
#else
    return {base, 0, capacity};
#endif
}

template<typename T>
void arr_push(Array<T>* arr, const T& value)
{
    ASSERT(arr->size < arr->capacity);

    arr->base[arr->size++] = value;
#if DEVELOPER
    arr->highest_size = Max(arr->size, arr->highest_size);
#endif
}

template<typename T>
T& arr_pop(Array<T>* arr)
{
    ASSERT(arr->size > 0);
    
    return arr->base[--arr->size];
}

template<typename T>
void arr_reset(Array<T>* arr)
{
    arr->size = 0;
}

template<typename T>
void arr_reset_to(Array<T>* arr, s32 size)
{
    ASSERT(size >= 0 && size <= arr->size);
    
    arr->size = size;
}

template<typename T>
T& arr_first(Array<T>* arr)
{
    ASSERT(arr->size > 0);
    
    return arr->base[0];
}

template<typename T>
T& arr_last(Array<T>* arr)
{
    ASSERT(arr->size > 0);
    
    return arr->base[arr->size - 1];
}

template<typename T>
s32 arr_bytesize(Array<T>* arr)
{
    s32 result = sizeof(T) * arr->size;
    ASSERT(result >= 0); // avoid overflow
    return result;
}

template<typename T>
s32 arr_bytesize_total(Array<T>* arr)
{
    s32 result = sizeof(T) * arr->capacity;
    ASSERT(result >= 0); // avoid overflow
    return result;
}

template<typename T>
void tarr_free(Array<T>* arr)
{
    u64 new_arena_size = arr->base - TEMPORARY_ARENA->base;
    // array was the last thing pushed onto arena
    ASSERT(TEMPORARY_ARENA->size - arr_bytesize_total(arr) == new_arena_size);
    arena_set_size(TEMPORARY_ARENA, new_arena_size);
}

template<typename T>
void arr_insert(Array<T>* arr, s32 index, const T& value)
{
    ASSERT(arr->size < arr->capacity);
    ASSERT(index >= 0 && index <= arr->size);

    for (s32 i = arr->size; i > index; i--)
    {
        arr->base[i] = arr->base[i - 1];
    }

    arr->base[index] = value;
    arr->size += 1;
#if DEVELOPER
    arr->highest_size = Max(arr->size, arr->highest_size);
#endif
}

template<typename T>
void arr_remove_at(Array<T>* arr, s32 index)
{
    ASSERT(index >= 0 && index < arr->size);
    
    for (s32 i = index; i < arr->size - 1; i++)
    {
        arr->base[i] = arr->base[i + 1];
    }
    
    arr->size -= 1;
}

template<typename T>
void arr_remove_at_cheaply(Array<T>* arr, s32 index)
{
    ASSERT(index >= 0 && index < arr->size);
    
    arr->base[index] = arr->base[arr->size - 1];
    arr->size -= 1;
}

template<typename T>
Slice<T> arr_slice(Array<T>* arr, s32 index, s32 count)
{
    ASSERT(count >= 0);
    ASSERT(index >= 0);
    
    if (index + count > arr->size)
    {
        count = arr->size - index;
    }
    
    return {arr->base + index, count};
}

template<typename T>
Slice<T> arr_slice(Array<T>* arr, s32 index)
{
    return arr_slice(arr, index, arr->size - index);
}

// copying will do shallow copy of pointers
template<typename T>
Array<T> arr_copy(Array<T>* arr, T* dst)
{
    for (s32 i = 0; i < arr->size; i++)
    {
        dst[i] = arr->base[i];
    }
    
#if DEVELOPER
    return {dst, arr->size, arr->capacity, arr->highest_size};
#else
    return {dst, arr->size, arr->capacity};
#endif
}

template<typename T>
Array<T> tarr_copy(Array<T>* arr)
{
    s32 capacity = mul2(arr->capacity, 1);
    T* base = temp_alloc(T, capacity);
    for (s32 i = 0; i < arr->size; i++)
    {
        base[i] = arr->base[i];
    }
    
#if DEVELOPER
    return {base, arr->size, capacity, arr->highest_size};
#else
    return {base, arr->size, capacity};
#endif
}

template<typename T>
b32 arr_compare(Array<T>* arr1, Array<T>* arr2)
{
    if (arr1->size != arr2->size) return false;
    
    return memcmp(arr1->base, arr2->base, arr_bytesize(arr1));
}

// need == overloaded for these; c++20 gives a default operator
// arr_contains, arr_push_if_unique

#endif
