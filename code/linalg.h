#ifndef LINALG_H
#define LINALG_H


typedef struct V2
{
    r32 x;
    r32 y;
} v2;

typedef struct V3
{
    r32 x;
    r32 y;
    r32 z;
} v3;

typedef struct V4
{
    r32 x;
    r32 y;
    r32 z;
    r32 a;
} v4;


// @TODO spherical and cylindtrical coordsys ??

// @Note right handed coordinate system:
// Z towards me
// X towards right
// Y towards up

// @Note do I want these
/* typedef v2 origin2; */
/* typedef v3 origin3; */

// @TODO do I want a difference between a real and an int coordsys?

// @Note do I want these
/* typedef struct Coordsys2 */
/* { */
/*     origin2 origin; */
/*     rad offset; */
/* } coordsys2; */

/* // @TODO extend later when you know how to do 3d rotation */
/* typedef struct Coordsys3 */
/* { */
/*     origin3 origin; */
/* } coordsys3; */


// @Note this is essentially for a spherical coordinate system,
// if you add an angle as well
/*
struct vdirectional2
{
    r32 len;
    uv2 dir;
};

struct vdirectional3
{
    r32 len;
    uv3 dir;
};

// @Note how can I know what format exactly is in memory, otherwise I
// cannot use this can I ?

struct vector2
{
    union
    {
        struct
        {
            v2 comps; //components
            basis2 basis;
        };
        struct
        {
            r32 len;
            uv2 dir;
        };
    };
};

struct vector3
{
    union
    {
        struct
        {
            v3 comps; //components
            basis3 basis;
        };
        struct
        {
            r32 len;
            uv3 dir;
        };
    };
};

@Note you actually do not want these because by design every vector's pov
is a coordinate system with origin (0,0,0) and axis aligned basis vectors.
If you want to see how that vector looks in a different coordinate system,
rotate it by angle offset of these two coordinate systems and then
transpose.

struct vector2
{
    v2 comps;
    basis2 basis;
};

struct vector3
{
    v3 comps;
    basis3 basis;
};
  
*/

// @Note Descartes' coordinate system

// @Note If we establish that every coordsys will be a Descartes one,
// do I need to care about the basis, since the only thing that can be
// different is the +/- direction of axes. Then we establish how that
// will look (will it change, do I need/care) and I don't need to keep
// information about the basis
/*

struct uv2
{
    r32 x;
    r32 y;

    inline r32 operator*(uv2);
    
    inline void set(r32, r32);
    inline void set(v2);
};

struct uv3
{
    r32 x;
    r32 y;
    r32 z;
    
    inline r32 operator*(uv3);

    inline void set(r32, r32, r32);
    inline void set(v3);
};
  
struct basis2
{
    uv2 i;
    uv2 j;

    inline void set(uv2, uv2);
};

struct basis3
{
    uv3 i;
    uv3 j;
    uv3 k;

    inline void set(uv3, uv3, uv3);
};

struct coordsys2
{
    origin2 origin;
    basis2 basis;
};

struct coordsys3
{
    origin3 origin;
    basis3 basis;
};
*/

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
// @Note these are maybe not that useful you can
// make them manually in higher level code

/* inline v2 icomponent(v2 vec) */
/* { */
/*     v2 result; */
/*     result.x = vec.x; */
/*     result.y = 0; */
/*     return result; */
/* } */

/* inline v2 jcomponent(v2 vec) */
/* { */
/*     v2 result; */
/*     result.x = 0; */
/*     result.y = vec.y; */
/*     return result; */
/* } */

/* inline v3 icomponent3(v3 vec) */
/* { */
/*     v3 result; */
/*     result.x = vec.x; */
/*     result.y = 0; */
/*     result.z = 0; */
/*     return result; */
/* } */

/* inline v3 jcomponent3(v3 vec) */
/* { */
/*     v3 result; */
/*     result.x = 0; */
/*     result.y = vec.y; */
/*     result.z = 0; */
/*     return result; */
/* } */

/* inline v3 kcomponent3(v3 vec) */
/* { */
/*     v3 result; */
/*     result.x = 0; */
/*     result.y = 0; */
/*     result.z = vec.z; */
/*     return result; */
/* } */

// @TODO perp for 3d ??
inline v2 perp2(v2 vec)
{
    v2 result;
    result.x = vec.y;
    result.y = -vec.x;
    return result;
}

inline v2 scale2(v2 v, r32 s)
{
    v2 result;
    result = v;
    result.x = result.x * s;
    result.y = result.y * s;
    return result;
}

inline v3 scale3(v3 v, r32 s)
{
    v3 result;
    result = v;
    result.x = result.x * s;
    result.y = result.y * s;
    result.z = result.z * s;
    return result;
}

// @Note for 3d you have 3 anglees of offset of coordsys ?
// this is actually hard to do, try it later............

// @Note in the way I'm doing rotation you have to subtract
// the given angle by the angle the vector is already offset
// from positive X axis area. I believe there exists a better
// more formal way to do this, so maybe change to that eventually...?

/* inline v2 rotate_d(v2 vec, degree angle) */
/* { */
/*     v2 result; */
/*     if (angle != 0 ) */
/*         { */
/*             if (elenv(vec) != 0) angle += to_degree((rad)acos(vec.x/elenv(vec)))*signofr32(vec.y); */
/*             result.x = elenv(vec)*(r32)cos(to_rad(angle)); */
/*             result.y = elenv(vec)*(r32)sin(to_rad(angle)); */
/*             return result; */
/*         } */
/*     return vec; */
/* } */

/* inline v2 rotate(v2 vec, rad angle) */
/* { */
/*     v2 result; */
/*     if (angle != 0 ) */
/*         { */
/*             if (elenv(vec) != 0) angle += (rad)acos(vec.x/elenv(vec))*signofr32(vec.y); */
/*             result.x = elenv(vec)*(r32)cos(angle); */
/*             result.y = elenv(vec)*(r32)sin(angle); */
/*             return result; */
/*         } */
/*     return vec; */
/* } */
/* /\* /\\* *\/ */
/* inline void transpose(v2* vec, coordsys2 from, coordsys2 to) */
/* { */
/*     //\*vec = rotate(*vec, to.offset - from.offset); */
/*     //\*vec += from.origin - to.origin; */
/* } */

/* // @TODO apply rotate first when you know how to do rotation in 3d */
/* inline void transpose(v3* vec, coordsys3 from, coordsys3 to) */
/* { */
/*     //\*vec += from.origin - to.origin; */
/* } */
/* *\/ */
/* /\* */


/* inline v2 icomponent(v2 vec) */
/* { */
/*     v2 result; */
/*     basis2 basis; */
/*     basis.i.set(1, 0); */
/*     basis.j.set(0, 1); */
/*     v2 comps; */
/*     comps.x = vec.comps.x; */
/*     comps.y = 0; */
/*     result.basis = basis; */
/*     result.comps = comps; */
/*     return result; */
/* } */

/* inline v2 jcomponent(v2 vec) */
/* { */
/*     v2 result; */
/*     basis2 basis; */
/*     basis.i.set(1, 0); */
/*     basis.j.set(0, 1); */
/*     v2 comps; */
/*     comps.x = 0; */
/*     comps.y = vec.comps.y; */
/*     result.basis = basis; */
/*     result.comps = comps; */
/*     return result; */
/* } */

/* inline v3 icomponent(v3 vec) */
/* { */
/*     v3 result; */
/*     basis3 basis; */
/*     basis.i.set(1, 0, 0); */
/*     basis.j.set(0, 1, 0); */
/*     basis.k.set(0, 0, 1); */
/*     v3 comps; */
/*     comps.x = vec.comps.x; */
/*     comps.y = 0; */
/*     comps.z = 0; */
/*     result.basis = basis; */
/*     result.comps = comps; */
/*     return result; */
/* } */

/* inline v3 jcomponent(v3 vec) */
/* { */
/*     v3 result; */
/*     basis3 basis; */
/*     basis.i.set(1, 0, 0); */
/*     basis.j.set(0, 1, 0); */
/*     basis.k.set(0, 0, 1); */
/*     v3 comps; */
/*     comps.x = 0; */
/*     comps.y = vec.comps.y; */
/*     comps.z = 0; */
/*     result.basis = basis; */
/*     result.comps = comps; */
/*     return result; */
/* } */

/* inline v3 zcomponent(v3 vec) */
/* { */
/*     v3 result; */
/*     basis3 basis; */
/*     basis.i.set(1, 0, 0); */
/*     basis.j.set(0, 1, 0); */
/*     basis.k.set(0, 0, 1); */
/*     v3 comps; */
/*     comps.x = 0; */
/*     comps.y = 0; */
/*     comps.z = vec.comps.z; */
/*     result.basis = basis; */
/*     result.comps = comps; */
/*     return result; */
/* } */
/* *\/ */


#endif
