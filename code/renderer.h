#ifndef RENDERER_H
#define RENDERER_H

// @Note essentialy a rectangular portion of the window that can have a Z

// @Note you can associate a Z with this,
// so that you can layer multiple of these on each other
// and project them

// @Note must use y-is-up and x-is-right coordinate system,
// do not do preemptive abs on dims or figuring out min/max coords

// @Note also use z-is-towards-me (establishes right-handed coordsys)
typedef union
{
    struct
    {
        r32 left;    // x_min
        r32 bottom;  // y_min
        r32 right;   // x_max
        r32 top;     // y_max
    };
    struct
    {
        v2 ll;     // lower left  corner
        v2 ur;     // upper right corner
    };
} wndrect;

inline wndrect Wndrect(r32 l, r32 b, r32 r, r32 t)
{
    wndrect result;

    result.left = l;
    result.bottom = b;
    result.right = r;
    result.top = t;
    
    return result;
}

inline r32 wndrect_width(wndrect rect)
{
    r32 result;
    
    result = rect.right - rect.left;
    
    return result;
}

inline r32 wndrect_height(wndrect rect)
{
    r32 result;
    
    result = rect.top - rect.bottom;
    
    return result;
}


#endif
