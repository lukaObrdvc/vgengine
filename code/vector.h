#ifndef VECTOR_H
#define VECTOR_H

typedef struct
{
    r32 x;
    r32 y;
} v2;

typedef struct
{
    r32 x;
    r32 y;
    r32 z;
} v3;

inline r32 edist2(v2 vec1, v2 vec2)
{
    return (r32)sqrt(sqr(vec1.x-vec2.x)+
                     sqr(vec1.y-vec2.y));
}

inline r32 edist3(v3 vec1, v3 vec2)
{
    return (r32)sqrt(sqr(vec1.x-vec2.x)+
                     sqr(vec1.y-vec2.y)+
                     sqr(vec1.z-vec2.z));
}

inline v2 V2(r32 x, r32 y)
{
    v2 result;

    result.x = x;
    result.y = y;
    
    return result;
}

inline v3 V3(r32 x, r32 y, r32 z)
{
    v3 result;

    result.x = x;
    result.y = y;
    result.z = z;
    
    return result;
}

inline v2 zero2(void)
{
    v2 result;

    result.x = 0;
    result.y = 0;
    
    return result;
}

inline v3 zero3(void)
{
    v3 result;

    result.x = 0;
    result.y = 0;
    result.z = 0;
    
    return result;
}

inline v2 add2(v2 vec1, v2 vec2)
{
    v2 result;

    result.x = vec1.x + vec2.x;
    result.y = vec1.y + vec2.y;
    
    return result;
}

inline v3 add3(v3 vec1, v3 vec2)
{
    v3 result;

    result.x = vec1.x + vec2.x;
    result.y = vec1.y + vec2.y;
    result.z = vec1.z + vec2.z;
    
    return result;
}

inline v2 dot2(v2 vec1, v2 vec2)
{
    v2 result;

    result.x = vec1.x * vec2.x;
    result.y = vec1.y * vec2.y;
    
    return result;
}

inline v3 dot3(v3 vec1, v3 vec2)
{
    v3 result;

    result.x = vec1.x * vec2.x;
    result.y = vec1.y * vec2.y;
    result.z = vec1.z * vec2.z;
    
    return result;
}

inline v2 scale2(v2 vec, r32 scalar)
{
    v2 result;

    result.x = vec.x * scalar;
    result.y = vec.y * scalar;
    
    return result;
}

inline v3 scale3(v3 vec, r32 scalar)
{
    v3 result;
    
    result.x = vec.x * scalar;
    result.y = vec.y * scalar;
    result.z = vec.z * scalar;
    
    return result;
}

/* v2 result = { .x = point.x , .y = point.y }; */

/* if (point.x < 0) */
/*     { */
/*         result.x = 0; */
/*     } */
/* else if (point.x > wnd_width) */
/*     { */
/*         result.x = wnd_width; */
/*     } */
    
/* if (point.y < 0) */
/*     { */
/*         result.y = 0; */
/*     } */
/* else if (point.y > wnd_height) */
/*     { */
/*         result.y = wnd_height; */
/*     } */
    
/* return result; */

inline v2 clamp2(v2 vec)
{
    v2 result;

    clamp(vec.x, 0, wnd_width);
    clamp(vec.y, 0, wnd_height);
    
    return result;
}

inline v3 clamp3(v3 vec)
{
    v3 result;

    clamp(vec.x, 0, wnd_width);
    clamp(vec.y, 0, wnd_height);
    clamp(vec.z, wnd_nearclip, wnd_farclip); // @TODO is this correct?
    
    return result;
}

inline v2 unit2(v2 vec)
{
    v2 result;

    r32 len = edist2(vec, zero2());
    vec.x = vec.x / len;
    vec.y = vec.y / len;
    
    return result;
}

inline v3 unit3(v3 vec)
{
    v3 result;

    r32 len = edist3(vec, zero3());
    vec.x = vec.x / len;
    vec.y = vec.y / len;
    vec.z = vec.z / len;
    
    return result;
}

inline v2 transpose2(v2 vec, v2 from, v2 to)
{
    v2 result;

    result.x = vec.x - (to.x - from.x);
    result.y = vec.y - (to.y - from.y);
    
    return result;
}

inline v3 transpose3(v3 vec, v3 from, v3 to)
{
    v3 result;

    result.x = vec.x - (to.x - from.x);
    result.y = vec.y - (to.y - from.y);
    result.z = vec.z - (to.z - from.z);
    
    return result;
}

// @TODO add v3 versions of these

inline v2 perp2(v2 vec)
{
    v2 result;
    
    result.x = vec.y;
    result.y = -vec.x;
    
    return result;
}

inline v2 rotate2(v2 vec, r32 angle)
{
    v2 result;

    result.x = cos(angle)*vec.x - sin(angle)*vec.y;
    result.y = sin(angle)*vec.x + cos(angle)*vec.y;
    
    return result;
}

#endif
