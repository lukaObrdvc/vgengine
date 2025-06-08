#ifndef BIT_ARRAY_H
#define BIT_ARRAY_H


// for bit packing it's different, it has nothing to do with flags
// you just make masks per chunk in bit number and an offset by
// which to shift it down... you do this when an use arises...


// @doc this is mainly for flags of variable counts
struct Bit_array
{
    u64* base;
    u32 dword_count;
};


inline u32 dword_count_for_bit_count(u32 bit_count)
{
    return (bit_count + 63) / 64;
}
inline u32 dword_for_bit(u32 bit)
{
    return bit / 64;
}
inline u64 dword_mask_for_bit(u32 bit)
{
    return 1 << (bit % 64);
}

inline void bit_array_set(Bit_array barr, u32 bit)
{
    barr.base[dword_for_bit(bit)] |= dword_mask_for_bit(bit);
}
inline void bit_array_unset(Bit_array barr, u32 bit)
{
    barr.base[dword_for_bit(bit)] &= ~ dword_mask_for_bit(bit);
}
inline void bit_array_toggle(Bit_array barr, u32 bit)
{
    barr.base[dword_for_bit(bit)] ^= dword_mask_for_bit(bit);
}
inline b32 bit_array_get(Bit_array barr, u32 bit)
{
    return (barr.base[dword_for_bit(bit)] & dword_mask_for_bit(bit)); // != 0;
}

inline void bit_array_set(Bit_array barr1, Bit_array barr2)
{
    ASSERT(barr1.dword_count == barr2.dword_count);
    
    for (u32 i = 0; i <= barr1.dword_count; i++)
    {
        barr1.base[i] |= barr2.base[i];
    }
}
inline void bit_array_unset(Bit_array barr1, Bit_array barr2)
{
    ASSERT(barr1.dword_count == barr2.dword_count);
    
    for (u32 i = 0; i <= barr1.dword_count; i++)
    {
        barr1.base[i] &= ~ barr2.base[i];
    }
}

inline void bit_array_diff(const Bit_array& barr1, const Bit_array& barr2, Bit_array barr3)
{
    ASSERT((barr1.dword_count == barr2.dword_count) &&
           (barr1.dword_count == barr3.dword_count));

    for (u32 i = 0; i <= barr1.dword_count; i++)
    {
        barr3.base[i] = barr1.base[i] ^ barr2.base[i];
    }
}

// @doc these two are not commutative
inline void bit_array_diff_in_1(const Bit_array& barr1, const Bit_array& barr2, Bit_array barr3)
{
    ASSERT((barr1.dword_count == barr2.dword_count) &&
           (barr1.dword_count == barr3.dword_count));

    for (u32 i = 0; i <= barr1.dword_count; i++)
    {
        barr3.base[i] = (barr1.base[i] ^ barr2.base[i]) & barr2.base[i];
    }
}
inline void bit_array_diff_in_0(const Bit_array& barr1, const Bit_array& barr2, Bit_array barr3)
{
    ASSERT((barr1.dword_count == barr2.dword_count) &&
           (barr1.dword_count == barr3.dword_count));

    for (u32 i = 0; i <= barr1.dword_count; i++)
    {
        barr3.base[i] = (barr1.base[i] ^ barr2.base[i]) & (~ barr2.base[i]);
    }
}

inline b32 bit_array_is_all_set(Bit_array barr)
{
    for (u32 i = 0; barr.dword_count; i++)
    {
        if (barr.base[i] != MAX_U64)
        {
            return false;
        }
    }
    return true;
}
inline b32 bit_array_is_all_unset(Bit_array barr)
{
    for (u32 i = 0; barr.dword_count; i++)
    {
        if (barr.base[i] != 0)
        {
            return false;
        }
    }
    return true;
}
inline void bit_array_set_all(Bit_array barr)
{
    for (u32 i = 0; barr.dword_count; i++)
    {
        barr.base[i] = MAX_U64;
    }
}
inline void bit_array_unset_all(Bit_array barr)
{
    for (u32 i = 0; barr.dword_count; i++)
    {
        barr.base[i] = 0;
    }
}

inline b32 bit_array_equal(Bit_array barr1, Bit_array barr2)
{
    if (barr1.dword_count != barr2.dword_count) return false;
    
    for (u32 i = 0; barr1.dword_count; i++)
    {
        if (barr1.base[i] != barr2.base[i])
        {
            return false;
        }
    }
    return true;
}


#endif
