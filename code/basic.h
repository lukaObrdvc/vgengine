#ifndef BASIC_H
#define BASIC_H


#if DEVELOPER
#define ASSERT(expr) if(!((b32)(expr))) {*((s32*)0) = 0;}
#else
#define ASSERT(expr) ((void)0)
#endif


inline constexpr r32 pi  = 3.14159265359f;
inline constexpr r32 tau = 6.28318530718f; // 2*pi


template<typename T>
T signof(T n)
{
    return !signbit((r32)(n)) * 2 - 1;
}

template<typename T>
T signof64(T n)
{
    return !signbit((r64)(n)) * 2 - 1;
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

inline r64 lerp(r64 A, r64 B, r64 t)
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


inline r64 Floor(r64 n)
{
    return (r64)(s32)(n);
}

inline r64 Ceil(r64 n)
{
    return (r64)(s32)(n+1);
}

inline r64 Round(r64 n)
{
    return (r64)(s32)(n+0.5);
}

inline r64 decimal(r64 n)
{
    return n-(s32)n;
}


inline s32 floor_to_int(r32 n)
{
    return (s32)(n);
}

inline s32 ceil_to_int(r32 n)
{
    return (s32)(n+1);
}

inline s32 round_to_int(r32 n)
{
    return (s32)(n+0.5);
}


inline s64 floor_to_int(r64 n)
{
    return (s64)(n);
}

inline s64 ceil_to_int(r64 n)
{
    return (s64)(n+1);
}

inline s64 round_to_int(r64 n)
{
    return (s64)(n+0.5);
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


inline r32 to_rad(r32 angle)
{
    return (angle*360)/tau;
}

inline r64 to_rad(r64 angle)
{
    return (angle*360)/tau;
}


inline r32 to_deg(r32 angle)
{
    return (angle*tau)/360;
}

inline r64 to_deg(r64 angle)
{
    return (angle*tau)/360;
}


#endif
