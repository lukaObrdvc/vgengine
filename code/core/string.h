#ifndef STRING_H
#define STRING_H

// string builder stuff?

// because at least the platform (WinAPI) requires a C string, you
// cannot pass your own string, you have to convert to a C string,
// which is easy if you just cast the base pointer, but you also
// need to null terminate it, which means it's a good idea, to leave
// an extra slot at the end of every string, and null terminate it
// by default so you don't even have to think about this

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
