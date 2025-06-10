#ifndef STRING_H
#define STRING_H


struct String
{
    u8* base;
    s32 length;
};

inline b32 string_compare(String a, String b)
{
    if (a.length != b.length) return false;

    return memcmp(a.base, b.base, a.length);
}


#endif
