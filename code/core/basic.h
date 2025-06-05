#ifndef BASIC_H
#define BASIC_H

typedef int8_t  s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int64_t s64;

typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef float  r32;
typedef double r64;

typedef s32 b32;
typedef s64 b64;

#define MIN_U    0
#define MAX_U8   255
#define MAX_U16  65535
#define MAX_U32  4294967295
#define MAX_U64  18446744073709551615 
#define MIN_S8  -128
#define MIN_S16 -131072
#define MIN_S32 -2147483648
#define MIN_S64 -9223372036854775808
#define MAX_S8   127
#define MAX_S16  131071
#define MAX_S32  2147483647
#define MAX_S64  9223372036854775807

#define MIN_R32 -3.4028235e+38f
#define MIN_R64 -1.7976931348623157e+308
#define MAX_R32  3.4028235e+38f
#define MAX_R64  1.7976931348623157e+308

#define EPSILON 0.0001f

#define PI  3.14159265359f
#define TAU 6.28318530718f    // 2pi
#define RAD 57.2957795131f    // 360/2pi
#define DEG 0.01745329252f    // 2pi/360

#define KB 1024               // 1024
#define MB 1048576            // 1024*1024
#define GB (u64)1073741824    // 1024*1024*1024
#define TB (u64)1099511627776 // 1024*1024*1024*1024


#define global_variable


#if DEVELOPER
#define ASSERT(expr) if(!((b32)(expr))) {*((s32*)0) = 0;}
#else
#define ASSERT(expr) ((void)0)
#endif

#define ARRAY_COUNT(arr) (sizeof((arr))/sizeof((arr)[0]))


template<typename T>
T Min(T a, T b)
{
    return (a < b ? a : b);
}

template<typename T>
T Max(T a, T b)
{
    return (a > b ? a : b);
}

template<typename T>
T abs(T n)
{
    return (n < 0 ? -n : n);
}

template<typename T>
T sqr(T n)
{
    return n * n;
}

template<typename T>
T clamp(T n, T low, T high)
{
    if      (n < low)  return low;
    else if (n > high) return high;
    else               return n;
}

template<typename T>
void swap(T& a, T& b)
{
    T tmp = a;
    a = b;
    b = tmp;
}

template<typename T>
T signof(T n)
{
    return (T(0) < n) - (n < T(0));
}


inline r32 lerp(r32 A, r32 B, r32 t)
{
    return B * t + A * (1 - t);
}

// @doc rounding, flooring towards zero
inline r32 Floor(r32 n)
{
    return (r32)(s32)(n);
}
inline r32 Ceil(r32 n)
{
    return (r32)(s32)(n+1);
}
inline r32 Round(r32 n)
{
    return (r32)(s32)(n+0.5f);
}
inline r32 decimal(r32 n)
{
    return n-(s32)n;
}

inline s32 floori(r32 n)
{
    return (s32)(n);
}
inline s32 ceili(r32 n)
{
    return (s32)(n+1);
}
inline s32 roundi(r32 n)
{
    return (s32)(n+0.5);
}

inline u32 trunc(u64 n)
{
    ASSERT(n <= MAX_U32);
    return (u32)n;
}
inline s32 trunc(s64 n)
{
    ASSERT(n >= MIN_S32 && n <= MAX_S32);
    return (s32)n;
}

inline s32 to_signed(u32 n)
{
    ASSERT(n <= MAX_S32);
    return (s32)n;
}
inline u32 to_unsigned(s32 n)
{
    ASSERT(n >= 0);
    return (u32)n;
}

inline u32 align_up(u32 n, u32 multiple_of)
{
    return (n + multiple_of - 1) & ( ~(multiple_of - 1));
}
inline u64 align_up(u64 n, u64 multiple_of)
{
    return (n + multiple_of - 1) & ( ~(multiple_of - 1));
}

inline u32 align_down(u32 n, u32 multiple_of)
{
    return n & ~(multiple_of - 1);
}
inline u64 align_down(u64 n, u64 multiple_of)
{
    return n & ~(multiple_of - 1);
}

inline b32 float_compare(r32 a, r32 b)
{
    return abs(a - b) < EPSILON;
}

// @speed write in steps of u64/u32 until you can't no more for speed?
// @doc pass bytesize
inline void memzero(void* ptr, s32 size)
{
    for (s32 i = 0; i < size; i++)
    {
        *((u8*)ptr + i) = 0;
    }
}
inline void memcopy(void* ptr1, s32 size, void* ptr2)
{
    for (s32 i = 0; i < size; i++)
    {
        *((u8*)ptr2 + i) = *((u8*)ptr1 + i);
    }
}


#endif
