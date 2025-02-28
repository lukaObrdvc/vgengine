#ifndef UTILS_H
#define UTILS_H

#include "stdint.h"

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

#ifdef __cplusplus
#define literal(t) t
#else
#define literal(t) (t)
#endif

#define true 1
#define false 0

#define global_variable
// #define local_persist static           (we do not even use this...)
#define internal static

#ifdef DEBUG
#define Assert(expr) if(!((b32)(expr))) {*((s32*)0) = 0;}
#else
#define Assert(expr)
#endif

#define InvalidCodePath Assert(0)
#define InvalidDefaultCase Assert(0)

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
// @Fail this is probably false
/* #define MIN_R32 (r32)MIN_S32 */
/* #define MAX_R32 (r32)MAX_S32 */
/* #define MIN_R64 (r64)MIN_S64 */
/* #define MAX_R64 (r64)MAX_S64 */

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

#define arr_count(arr) (sizeof((arr))/sizeof((arr)[0]))
#define clamp(value, min, max) if ((value) < (min)) (value) = (min);   \
    else if ((value) > (max)) (value) = (max)

inline r32 clamp32(r32 value, r32 min, r32 max)
{
    r32 result = value;
    
    if (value < min)
        {
            result = min;
        }
    else if (value > max)
        {
            result = max;
        }

    return result;
}

// @TODO other truncation I might need?
// @TODO this seems useless, figure out if you need it from examples and so on ...
/* inline u32 trunc_u32(u64 n) */
/* { */
/*     Assert(n <= MAX_U32); */
/*     return (u32)n; */
/* } */


#endif
