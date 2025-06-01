#ifndef BASIC_H
#define BASIC_H


#define global_variable

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

// @todo replace this with constexpr
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
// @todo for r32, r64?

#if DEVELOPER
#define ASSERT(expr) if(!((b32)(expr))) {*((s32*)0) = 0;}
#else
#define ASSERT(expr) ((void)0)
#endif

// THIS IS TEMPORARILY HERE ---------------------------
#define ARR_COUNT(arr) (sizeof((arr))/sizeof((arr)[0]))
// ---------------------------------------------------

#define PI  3.14159265359f
#define TAU 6.28318530718f // 2*PI


template<typename T>
T signof(T n)
{
    return !signbit((r32)(n)) * 2 - 1;
}

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

// @todo replace 360/tau and tau/360 with precomputed values
inline r32 radians(r32 angle)
{
    return (angle*360)/TAU;
}


inline r32 degrees(r32 angle)
{
    return (angle*TAU)/360;
}


inline u64 kilobytes(u64 n)
{
    return n*1024;
}

inline u64 megabytes(u64 n)
{
    return n*1024*1024;
}

inline u64 gigabytes(u64 n)
{
    return n*1024*1024*1024;
}

inline u64 terabytes(u64 n)
{
    return n*1024*1024*1024*1024;
}

inline u32 align_up(u32 n, u32 multipleOf)
{
    return (n + multipleOf - 1) & ( ~(multipleOf - 1));
}
inline u64 align_up(u64 n, u64 multipleOf)
{
    return (n + multipleOf - 1) & ( ~(multipleOf - 1));
}

inline u32 align_down(u32 n, u32 multipleOf)
{
    return n & ~(multipleOf - 1);
}
inline u64 align_down(u64 n, u64 multipleOf)
{
    return n & ~(multipleOf - 1);
}


#endif
