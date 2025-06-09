#ifndef SLICE_H
#define SLICE_H

template<typename T>
struct Slice
{
    T* base;
    s32 size;
    
    inline T& operator[](s32 i)
    {
        ASSERT(i >= 0 && i < size);
        return base[i];
    }
    inline const T& operator[](s32 i) const
    {
        ASSERT(i >= 0 && i < size);
        return base[i];
    }
};

template<typename T>
T& slice_first(Slice<T> s)
{
    ASSERT(s.size > 0);
    return s.base[0];
}
template<typename T>
T& slice_last(Slice<T> s)
{
    ASSERT(s.size > 0);
    return s.base[s.size - 1];
}

template<typename T>
s32 slice_bytesize(Slice<T> s)
{
    s32 result = sizeof(T) * s.size;
    ASSERT(result >= 0); // avoid overflow
    return result;
}

template<typename T>
b32 slice_compare(Slice<T> s1, Slice<T> s2)
{
    if(s1.size != s2.size) return false;
    
    return memcmp(s1.base, s2.base, slice_bytesize(s1));
}

template<typename T>
void slice_copy(Slice<T> s, T* dst)
{
    for (s32 i = 0; i < s.size; i++)
    {
        dst[i] = s.base[i];
    }
}

template<typename T>
Slice<T> slice_slice(Slice<T> s, s32 index, s32 count)
{
    ASSERT(count >= 0);
    ASSERT(index >= 0);
    
    if (index + count > s.size)
    {
        count = s.size - index;
    }
    
    return {s.base + index, count};
}

template<typename T>
Slice<T> slice_slice(Slice<T> s, s32 index)
{
    return slice_slice(s, index, s.size - index);
}

#endif
