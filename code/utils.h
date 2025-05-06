#ifndef UTILS_H
#define UTILS_H

typedef int8_t s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int64_t s64;

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef float r32;
typedef double r64;

typedef s32 b32;
typedef s64 b64;


#define global_variable
// #define local_persist static           (we do not even use this...)
#define internal static

#define MIN_U 0
#define MAX_U8 255
#define MAX_U16 65535
#define MAX_U32 4294967295
#define MAX_U64 18446744073709551615 
#define MIN_S8 -128
#define MIN_S16 -131072
#define MIN_S32 -2147483648
#define MIN_S64 -9223372036854775808
#define MAX_S8 127
#define MAX_S16 131071
#define MAX_S32 2147483647
#define MAX_S64 9223372036854775807

inline u64 kilobytes(u32 n)
{
    return ((u64)n)*1024;
}

inline u64 megabytes(u32 n)
{
    return ((u64)n)*1024*1024;
}

inline u64 gigabytes(u32 n)
{
    return ((u64)n)*1024*1024*1024;
}

#define ArrCount(arr) (sizeof((arr))/sizeof((arr)[0]))


#endif
