#ifndef FLAGS_H
#define FLAGS_H

// this is like Bit_array but only for flags that
// are u8, u16, u32, u64

// @doc assume flag is already shifted to left
// (for example 1 << 5 is 0b10000 = 16), so you don't have to shift
// here every time


// @doc the second parameter doesn't have to be just one flag, so
// you can actually pass multiple flags (you can obviously combine
// with | operator) (for the next 4 functions)

// FUNCTIONS FOR U8

inline u8 set_flags(u8 flags, u8 flags2)
{
    return flags | flags2;
}
inline u8 unset_flags(u8 flags, u8 flags2)
{
    return flags & (~ flags2);
}
inline u8 toggle_flags(u8 flags, u8 flags2)
{
    return flags ^ flags2;
}
inline u8 get_flags(u8 flags, u8 flags2)
{
    return flags & flags2;
}
inline u8 diff_flags(u8 f1, u8 f2)
{
    return f1 ^ f2; // same as toggle_flags but whatever this is more explicit
}
// @doc these two are not commutative (f1=new f2=old)
inline u8 diff_flags_in_1(u8 f1, u8 f2)
{
    return (f1 ^ f2) & f2;
}
inline u8 diff_flags_in_0(u8 f1, u8 f2)
{
    return (f1 ^ f2) & ( ~f2);
}
inline b32 flags_all_are_set(u8 f)
{
    return f == MAX_U8;
}
inline b32 flags_all_are_unset(u8 f)
{
    return f == 0;
}
inline u8 set_all_flags8()
{
    return MAX_U8;
}
// @doc let's just have only one and let it be promoted by language
inline u8 unset_all_flags()
{
    return 0;
}
inline b32 flags_are_equal(u8 f1, u8 f2)
{
    return f1 == f2;
}

// FUNCTIONS FOR U16

inline u16 set_flags(u16 flags, u16 flags2)
{
    return flags | flags2;
}
inline u16 unset_flags(u16 flags, u16 flags2)
{
    return flags & (~ flags2);
}
inline u16 toggle_flags(u16 flags, u16 flags2)
{
    return flags ^ flags2;
}
inline u16 get_flags(u16 flags, u16 flags2)
{
    return flags & flags2;
}
inline u16 diff_flags(u16 f1, u16 f2)
{
    return f1 ^ f2;
}
inline u16 diff_flags_in_1(u16 f1, u16 f2)
{
    return (f1 ^ f2) & f2;
}
inline u16 diff_flags_in_0(u16 f1, u16 f2)
{
    return (f1 ^ f2) & ( ~f2);
}
inline b32 flags_all_are_set(u16 f)
{
    return f == MAX_U16;
}
inline b32 flags_all_are_unset(u16 f)
{
    return f == 0;
}
inline u16 set_all_flags16()
{
    return MAX_U16;
}
inline b32 flags_are_equal(u16 f1, u16 f2)
{
    return f1 == f2;
}

// FUNCTIONS FOR U32

inline u32 set_flags(u32 flags, u32 flags2)
{
    return flags | flags2;
}
inline u32 unset_flags(u32 flags, u32 flags2)
{
    return flags & (~ flags2);
}
inline u32 toggle_flags(u32 flags, u32 flags2)
{
    return flags ^ flags2;
}
inline u32 get_flags(u32 flags, u32 flags2)
{
    return flags & flags2;
}
inline u32 diff_flags(u32 f1, u32 f2)
{
    return f1 ^ f2;
}
inline u32 diff_flags_in_1(u32 f1, u32 f2)
{
    return (f1 ^ f2) & f2;
}
inline u32 diff_flags_in_0(u32 f1, u32 f2)
{
    return (f1 ^ f2) & ( ~f2);
}
inline b32 flags_all_are_set(u32 f)
{
    return f == MAX_U32;
}
inline b32 flags_all_are_unset(u32 f)
{
    return f == 0;
}
inline u32 set_all_flags32()
{
    return MAX_U32;
}
inline b32 flags_are_equal(u32 f1, u32 f2)
{
    return f1 == f2;
}

// FUNCTIONS FOR U64

inline u64 set_flags(u64 flags, u64 flags2)
{
    return flags | flags2;
}
inline u64 unset_flags(u64 flags, u64 flags2)
{
    return flags & (~ flags2);
}
inline u64 toggle_flags(u64 flags, u64 flags2)
{
    return flags ^ flags2;
}
inline u64 get_flags(u64 flags, u64 flags2)
{
    return flags & flags2;
}
inline u64 diff_flags(u64 f1, u64 f2)
{
    return f1 ^ f2;
}
inline u64 diff_flags_in_1(u64 f1, u64 f2)
{
    return (f1 ^ f2) & f2;
}
inline u64 diff_flags_in_0(u64 f1, u64 f2)
{
    return (f1 ^ f2) & ( ~f2);
}
inline b32 flags_all_are_set(u64 f)
{
    return f == MAX_U64;
}
inline b32 flags_all_are_unset(u64 f)
{
    return f == 0;
}
inline u64 set_all_flags64()
{
    return MAX_U64;
}
inline b32 flags_are_equal(u64 f1, u64 f2)
{
    return f1 == f2;
}


#endif
