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

    for (s32 i = 0; i < a.length; i++)
    {
        if (a.base[i] == b.base[i]) return false;
    }

    return true;
}


#endif
