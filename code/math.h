#ifndef MATH_H
#define MATH_H

#include <math.h>

// @Note <math.h> has r32 sqrt(r32) function which I'm using for now,
// same for sin, cos
// implement intrinsics later
// I'm using signbit as well for signof, try dissasembly and implement
// yourself later

// @TODO intrinsics for these?

// @TODO check if work for negative, if desired like that
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

#define PI 3.14159
#define RAD_MAX 2*3.14159
#define DEGREE_MAX 360

// @Note do I want this?
typedef r32 degree;
typedef r32 rad;

// @Note do I want to have a 32/64 version, cast to r first,
// then cast to s (what do I really want?)
#define signof32(n) (s32)(!signbit((r32)(n))*2-1)
#define signof64(n) (s64)(!signbit((r64)(n))*2-1)

#define sqr(n) ((n)*(n))

// @Note cannot use % with real numbers ??

// @Note this is probably a bad idea
/* #define normalize(n, max) ((n)*1.0f/(max)*1.0f) */
/* #define renormalize(n, max) ((n)*(max)) */

// @TODO are these two correct
inline degree moduo_degree(degree angle)
{
    return (r32)(((s32)angle)%DEGREE_MAX) + angle-(s32)angle;
}

inline rad moduo_rad(rad angle)
{
    return (r32)(((s32)(angle/PI))%((s32)(RAD_MAX/PI))) + angle-(s32)angle;
}

// @Note assume for these two they are already moduo below degree/rad_max
inline degree to_degree(rad angle)
{
    return (angle*DEGREE_MAX)/RAD_MAX;
}

inline rad to_rad(degree angle)
{
    return (angle*RAD_MAX)/DEGREE_MAX;
}

// @Note assume already this is done elsewhere Assert(step >= 0.0 && step <= 1.0);
inline r32 lerp(r32 low, r32 high, r32 step)
{
    return high*step + low*(1-step);
    // @Note low+step*(high-low);
}


#endif
