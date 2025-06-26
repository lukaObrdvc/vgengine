#ifndef STRING_H
#define STRING_H

// string literals live in read only static memory (which is just
// added to the binary file size), so you can just take set base
// pointer to that, they also have '\0' at the end by default, and
// changing values at those addresses is UB; without using these you
// would have to fill your memory with string data from files or input,
// in order to reuse memory, but if you use string literals then they
// live forever (but they use up binary size, not stack or heap)

// @doc basically assume when using strings that length refers to the
// readable, useful part, and then when dealing with memory you +1
// when needed, for '\0' char (so you can convert to c string when required)

// @doc you have to copy_str from a string literal if you want to
// modify it, otherwise it's like it's const

// @todo maybe make a special arena for strings which is the default?

// @todo integer to string, string to integer

struct String
{
    u8* base;
    s32 length;
};

String to_string(s32 n, Arena* arena);
String to_string(u64 n, Arena* arena);
String to_string(r32 n, s32 decimals, Arena* arena);
String to_string(r64 n, s32 decimals, Arena* arena);

inline s32 len_of_c_string(const char* s)
{
    s32 len = 0;
    while (s[len] != '\0')
    {
        len++;
        ASSERT(len < MAX_S32); // assume string wasn't null terminated
    }
    return len;
}

inline String str(const char* s)
{
    return {(u8*)s, len_of_c_string(s)};
}

inline const u8* cstr(String s)
{
    return s.base;
}

inline String copy_str(const char* s, Arena* arena = TEMPORARY_ARENA)
{
    String result;
    result.length = len_of_c_string(s);
    result.base = arena_push<u8>(arena, result.length + 1); // +1 for null termination
    memcpy(result.base, (u8*)s, result.length + 1);
    return result;
}

inline String copy_str(String s, Arena* arena = TEMPORARY_ARENA)
{
    String result;
    result.length = s.length;
    result.base = arena_push<u8>(arena, result.length + 1); // +1 for null termination
    memcpy(result.base, s.base, result.length + 1);
    return result;
}

inline b32 string_compare(String a, String b)
{
    if (a.length != b.length) return false;

    return memcmp(a.base, b.base, a.length);
}

inline b32 string_compare(const char* a, const char* b)
{
    s32 a_len = len_of_c_string(a);
    s32 b_len = len_of_c_string(b);
    
    if (a_len != b_len) return false;

    return memcmp((u8*)a, (u8*)b, a_len);
}

// @todo make these variadic
inline String concat(const String& s1, const String& s2, Arena* arena = TEMPORARY_ARENA)
{
    String result;
    result.length = s1.length + s2.length;
    result.base = arena_push<u8>(arena, result.length + 1); // +1 for null termination
    memcpy(result.base, s1.base, s1.length);
    memcpy(result.base + s1.length, s2.base, s2.length + 1);
    return result;
}

inline String concat(const char* s1, const char* s2, Arena* arena = TEMPORARY_ARENA)
{
    s32 s1_len = len_of_c_string(s1);
    s32 s2_len = len_of_c_string(s2);
    String result;
    result.length = s1_len + s2_len;
    result.base = arena_push<u8>(arena, result.length + 1); // +1 for null termination
    memcpy(result.base, (u8*)s1, s1_len);
    memcpy(result.base + s1_len, (u8*)s2, s2_len + 1);
    return result;
}

#endif
