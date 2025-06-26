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

#define MIN_U    0U
#define MAX_U8   255U
#define MAX_U16  65535U
#define MAX_U32  4294967295U
#define MAX_U64  18446744073709551615ULL
#define MIN_S8  -128
#define MIN_S16 -32768
#define MIN_S32 -2147483648
#define MIN_S64 -9223372036854775808LL
#define MAX_S8   127
#define MAX_S16  32767
#define MAX_S32  2147483647
#define MAX_S64  9223372036854775807LL

#define MIN_R32 -3.4028235e+38f
#define MIN_R64 -1.7976931348623157e+308
#define MAX_R32  3.4028235e+38f
#define MAX_R64  1.7976931348623157e+308

#define EPSILON 0.0001f

#define PI  3.14159265359f
#define TAU 6.28318530718f  // 2pi
#define RAD 57.2957795131f  // 360/2pi
#define DEG 0.01745329252f  // 2pi/360

#define KB 1024U            // 1024
#define MB 1048576U         // 1024*1024
#define GB 1073741824ULL    // 1024*1024*1024
#define TB 1099511627776ULL // 1024*1024*1024*1024


#define global_variable


#if DEVELOPER
#define ASSERT(expr) if(!((b32)(expr))) {*((s32*)0) = 0;}
#else
#define ASSERT(expr) ((void)0)
#endif

// only works when arr is defined globally, in scope, or in a struct,
// not when passed to functions
#define C_ARRAY_COUNT(arr) (sizeof((arr))/sizeof((arr)[0]))


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
T clamp01(T n)
{
    if      (n < 0) return 0;
    else if (n > 1) return 1;
    else            return n;
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

template<typename T>
T pow2(T n)
{
    return 1 << n;
}

template<typename T>
u64 pow10(T n)
{
    ASSERT(n <= 17); // otherwise it overflows
    
    u64 result = 1;
    
    for (s32 i = 0; i < n; i++)
    {
        result *= 10;
    }

    return result;
}

template<typename T>
b32 is_pow2(T n)
{
    return ~(n & (n - 1)); // ! instead of ! ?
}

template <typename T>
T mul2(T n, T exponent)
{
    return n << exponent;
}

template <typename T>
T div2(T n, T exponent)
{
    return n >> exponent;
}

template<typename T>
T align_up(T n, T multiple_of)
{
    ASSERT(is_pow2(multiple_of));
    return (n + multiple_of - 1) & ( ~(multiple_of - 1));
}

template<typename T>
T align_down(T n, T multiple_of)
{
    ASSERT(is_pow2(multiple_of));
    return n & ~(multiple_of - 1);
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

inline b32 float_compare(r32 a, r32 b)
{
    return abs(a - b) < EPSILON;
}

// @speed write in steps of u64/u32/u16/u8
// @doc pass bytesize
inline void memset(void* base, u32 size, u8 value = 0)
{
    for (u32 i = 0; i < size; i++)
    {
        *((u8*)base + i) = value;
    }
}
inline void memcpy(void* dest, void* src, u32 size)
{
    for (u32 i = 0; i < size; i++)
    {
        *((u8*)dest + i) = *((u8*)src + i);
    }
}
inline b32 memcmp(void* base1, void* base2, u32 size)
{
    for (u32 i = 0; i < size; i++)
    {
        if (*((u8*)base1 + i) != *((u8*)base2 + i))
        {
            return false;
        }
    }
    return true;
}

inline b32 toggle_bool(b32 b)
{
    return !b;
}

inline r32 fmod(r32 n, r32 mod)
{
    // @todo this is naive, do better
    return (r32)(floori(n) % floori(mod));
}

inline b32 is_alpha(u8 c)
{
    return c >= '0' && c <= '9';
}

inline s32 to_alpha(u8 c)
{
    ASSERT(is_alpha(c));
    
    return c - '0';
}

inline s32 num_digits(s32 n)
{
    n = abs(n);
    s32 result = 0;

    if (n == 0) return 1;
    
    while (n > 0)
    {
        n /= 10;
        result++;
    }
    
    return result;
}

inline s32 num_digits(s64 n)
{
    n = abs(n);
    s32 result = 0;

    if (n == 0) return 1;
    
    while (n > 0)
    {
        n /= 10;
        result++;
    }
    
    return result;
}

inline s32 num_digits(u64 n)
{
    s32 result = 0;

    if (n == 0) return 1;
    
    while (n > 0)
    {
        n /= 10;
        result++;
    }
    
    return result;
}

inline s32 num_digits(r32 n)
{
    s32 result = 0;

    if (float_compare(n, 0.0f)) return 1;
    
    n = abs(n);
    s32 ni = floori(n);
    
    while (ni > 0)
    {
        ni /= 10;
        result++;
    }
    
    return result;
}

inline s32 num_digits(r64 n)
{
    s32 result = 0;

    if (float_compare(n, 0.0)) return 1;
    
    n = abs(n);
    s64 ni = floori(n);
    
    while (ni > 0)
    {
        ni /= 10;
        result++;
    }
    
    return result;
}

s32 num_decimal_digits(r32 f)
{
    u8 buffer[64];
    snprintf((char*)buffer, sizeof(buffer), "%.20f", f);
    
    s32 first_decimal = -1;
    for (s32 i = 0; buffer[i] != '\0'; i++)
    {
        if (buffer[i] == '.')
        {
            first_decimal = i + 1;
            break;
        }
    }

    if (first_decimal == -1 || buffer[first_decimal] == '\0') return 0;
    
    s32 last_non_zero = -1;

    for (s32 i = first_decimal; buffer[i] != '\0'; i++)
    {
        if (buffer[i] != '0') last_non_zero = i;
    }
    
    if (last_non_zero == -1) return 0;
    return (last_non_zero >= first_decimal) ? (last_non_zero - first_decimal + 1) : 0;    
}

s32 num_decimal_digits(r64 f)
{
    u8 buffer[128];
    snprintf((char*)buffer, sizeof(buffer), "%.20f", f);
    
    s32 first_decimal = -1;
    for (s32 i = 0; buffer[i] != '\0'; i++)
    {
        if (buffer[i] == '.')
        {
            first_decimal = i + 1;
            break;
        }
    }

    if (first_decimal == -1 || buffer[first_decimal] == '\0') return 0;
    
    s32 last_non_zero = -1;

    for (s32 i = first_decimal; buffer[i] != '\0'; i++)
    {
        if (buffer[i] != '0') last_non_zero = i;
    }
    
    if (last_non_zero == -1) return 0;
    return (last_non_zero >= first_decimal) ? (last_non_zero - first_decimal + 1) : 0;    
}

inline r32 round_decimals(r32 n, s32 decimals)
{
    if (decimals > 17) decimals = 17; // avoid overflow on pow10

    u64 pow = pow10(decimals);
    n *= pow;
    n = Round(n);
    n /= pow;

    return n;
}

inline r64 round_decimals(r64 n, s32 decimals)
{
    if (decimals > 17) decimals = 17; // avoid overflow on pow10

    u64 pow = pow10(decimals);
    n *= pow;
    n = Round(n);
    n /= pow;

    return n;
}



// @todo find first zero/one ??
// @todo fast mod

// @todo figure out how best to represent angles: degrees/radians/normalized??
// @todo moduo(cardinal angle...)/normalize degree and rad..?

// @todo log10 (I think it exists in math.h)

#endif
