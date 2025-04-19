#ifndef MATH_H
#define MATH_H


// @TODO <math.h> has r32 sqrt(r32) function which I'm using for now,
// same for sin, cos
// implement intrinsics later
// I'm using signbit as well for signof, try dissasembly and implement
// yourself later (Intrinsics...)

// @TODO check if work for negative, if desired like that
// @IMPORTANT needs to work for negative numbers
inline s32 floor32(r32 n)
{
    return (s32)(n);
}

inline s64 floor64(r64 n)
{
    return (s64)(n);
}

// @Note ceils towards zero for negative numbers
inline s32 ceil32(r32 n)
{
    return (s32)(n+1);
}

inline s64 ceil64(r64 n)
{
    return (s64)(n+1);
}

// @Note rounds towards zero for negative numbers 
inline s32 round32(r32 n)
{
    return (s32)(n+0.5);
}

inline s64 round64(r64 n)
{
    return (s64)(n+0.5);
}

inline r32 decimal32(r32 n)
{
    return n-(s32)n;
}

inline r64 decimal64(r64 n)
{
    return n-(s64)n;
}

#define abs(n) (((n) >= 0) ? (n) : -(n))

#define signof32(n) (s32)(!signbit((r32)(n))*2-1)
#define signof64(n) (s64)(!signbit((r64)(n))*2-1)

// @Note potentially same name used from <math.h> (or in winapi?)
#define sqr(n) ((n)*(n))

// @Note this is probably a bad idea
/* #define normalize(n, max) ((n)*1.0f/(max)*1.0f) */
/* #define renormalize(n, max) ((n)*(max)) */

#define PI ((r32) 3.14159)
#define TAU ((r32) 2*3.14159)

inline r32 to_degree(r32 angle)
{
    return (angle*360)/TAU;
}

inline r32 to_rad(r32 angle)
{
    return (angle*TAU)/360;
}

// @TODO figure out what you actually want from these from real examples
// once you have some, until then don't use these
/* inline degree moduo_degree(degree angle) */
/* { */
/*     return (r32)(((s32)angle)%DEGREE_MAX) + angle-(s32)angle; */
/* } */

/* inline rad moduo_rad(rad angle) */
/* { */
/*     return (r32)(((s32)(angle/PI))%((s32)(RAD_MAX/PI))) + angle-(s32)angle; */
/* } */

inline r32 lerp(r32 low, r32 high, r32 step)
{
    return high*step + low*(1-step);
    // @Note low+step*(high-low);
}

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

#endif
