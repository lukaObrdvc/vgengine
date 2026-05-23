typedef int8_t  int8;
typedef int16_t int16;
typedef int32_t int32;
typedef int64_t int64;

typedef uint8_t  uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;

typedef float  float32;
typedef double float64;

typedef int32 bool32;
typedef int64 bool64;

typedef int32  int;
typedef bool32 bool;

constexpr uint8  min_uint   = 0;
constexpr uint8  max_uint8  = 255U;
constexpr uint16 max_uint16 = 65535U;
constexpr uint32 max_uint32 = 4294967295U;
constexpr uint64 max_uint64 = 18446744073709551615ULL;

constexpr int8  min_int8  = -128;
constexpr int16 min_int16 = -32768;
constexpr int32 min_int32 = -2147483648;
constexpr int64 min_int64 = -9223372036854775808LL;
constexpr int8  max_int8  =  127;
constexpr int16 max_int16 =  32767;
constexpr int32 max_int32 =  2147483647;
constexpr int64 max_int64 =  9223372036854775807LL;

constexpr float32 min_float32 = -3.4028235e+38f;
constexpr float64 min_float64 = -1.7976931348623157e+308;
constexpr float32 max_float32 =  3.4028235e+38f;
constexpr float64 max_float64 =  1.7976931348623157e+308;

constexpr float32 epsilon = 0.0001f;
constexpr float32 pi  = 3.14159265359f;

constexpr uint16 kb = 1024U;            // 1024
constexpr uint32 mb = 1048576U;         // 1024*1024
constexpr uint32 gb = 1073741824U;      // 1024*1024*1024
constexpr uint64 tb = 1099511627776ULL; // 1024*1024*1024*1024

inline void assert(bool expr)
{
    #if DEVELOPER
    if (!expr) *((int*)0) = 0;
    #endif
}

template<typename T>
T _min(T a, T b)
{
    return (a < b ? a : b);
}

template<typename T>
T _max(T a, T b)
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

template<typename T>
T pow2(T n)
{
    return 1 << n;
}

template<typename T>
uint64 pow10(T n)
{
    assert(n <= 17); // avoid overflow
    
    uint64 result = 1;
    
    for (int i = 0; i < n; i++)
        result *= 10;

    return result;
}

template<typename T>
bool is_pow2(T n)
{
    return ~(n & (n - 1));
}

template <typename T>
T fast_mod(T n, T m)
{
    assert(is_pow2(m));
    return n & (m - 1);
}

template<typename T>
T align_up(T n, T m)
{
    assert(is_pow2(m));
    return (n + m - 1) & ( ~(m - 1));
}

template<typename T>
T align_down(T n, T m)
{
    assert(is_pow2(m));
    return n & ~(m - 1);
}

inline float lerp(float A, float B, float t)
{
    return B * t + A * (1 - t);
}

// rounding, flooring is towards zero
inline float _floor(float n)
{
    return (float)(int)(n);
}
inline float _ceil(float n)
{
    return (float)(int)(n + 1);
}
inline float _round(float n)
{
    return (float)(int)(n + 0.5f);
}
inline float decimal(float n)
{
    return n - (int)n;
}

inline int floor_to_int(float n)
{
    return (int)n;
}
inline int ceil_to_int(float n)
{
    return (int)(n + 1);
}
inline int round_to_int(float n)
{
    return (int)(n + 0.5);
}

inline uint32 trunc(uint64 n)
{
    assert(n <= max_uint32);
    return (uint32)n;
}
inline int32 trunc(int64 n)
{
    assert(n >= min_int32 && n <= max_int32);
    return (int32)n;
}

inline int to_signed(uint n)
{
    assert(n <= max_int32);
    return (int)n;
}
inline uint to_unsigned(int n)
{
    assert(n >= 0);
    return (uint)n;
}

inline float to_deg(float rad)
{
    return rad * 57.2957795131f; // 360 / 2pi
}
inline float to_rad(float deg)
{
    return deg * 0.01745329252f; // 2pi / 360
}

inline bool float_compare(float a, float b)
{
    return abs(a - b) < epsilon;
}

// @speed use intrinsics
inline void memset(void* base, uint32 size, uint8 value = 0)
{
    for (uint32 i = 0; i < size; i++)
    {
        *((uint8*)base + i) = value;
    }
}
inline void memcpy(void* dest, void* src, uint32 size)
{
    for (uint32 i = 0; i < size; i++)
    {
        *((uint8*)dest + i) = *((uint8*)src + i);
    }
}
inline bool memcmp(void* base1, void* base2, uint32 size)
{
    for (uint32 i = 0; i < size; i++)
    {
        if (*((uint8*)base1 + i) != *((uint8*)base2 + i))
        {
            return false;
        }
    }
    return true;
}

// template<typename T>
// void insertion_sort(T* arr, int count)
// {
//     for (int i = 1; i < count; i++)
//     {
//         int value = arr[i];
//         int j = i;
//         while (j > 0 && arr[j - 1] > value)
//         {
//             arr[j] = arr[j - 1];
//             j--;
//         }
//         arr[j] = value;
//     }
// }

// fmod
// find first zero/one
// how to represent angles? rad/deg/normalized??
// moduo angle, normalize angle?
// a bunch of char/string functions...
