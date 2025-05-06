#ifndef VECTOR_H
#define VECTOR_H

// @Note do I want floor2, floor3 and similar...?

// @TODO do the union trick on these

#define sqr(n) ((n)*(n))

typedef struct tagV2
{
    r32 x;
    r32 y;
} v2;

typedef struct tagV3
{
    r32 x;
    r32 y;
    r32 z;
} v3;

typedef struct tagV4
{
    r32 x;
    r32 y;
    r32 z;
    r32 w;
} v4;

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

inline v4 V4(r32 x, r32 y, r32 z, r32 w)
{
    v4 result;

    result.x = x;
    result.y = y;
    result.z = z;
    result.w = w;
    
    return result;
}

// @TODO probably turn these zeroes into macros so you don't have to
// add () everywhere
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

inline v2 sub2(v2 vec1, v2 vec2)
{
    v2 result;

    result.x = vec1.x - vec2.x;
    result.y = vec1.y - vec2.y;
    
    return result;
}

inline v3 sub3(v3 vec1, v3 vec2)
{
    v3 result;

    result.x = vec1.x - vec2.x;
    result.y = vec1.y - vec2.y;
    result.z = vec1.z - vec2.z;
    
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

// @matrix

typedef union tagM4
{
    struct
    {
        v4 X;
        v4 Y;
        v4 Z;
        v4 W;
    };
    v4 row[4];
    r32 m[4][4];
} m4;

inline m4 M4v(v4 XX, v4 YY, v4 ZZ, v4 WW)
{
    m4 result;

    result.X = XX;
    result.Y = YY;
    result.Z = ZZ;
    result.W = WW;
    
    return result;
}

inline m4 M4(r32 m00, r32 m01, r32 m02, r32 m03,
             r32 m10, r32 m11, r32 m12, r32 m13,
             r32 m20, r32 m21, r32 m22, r32 m23,
             r32 m30, r32 m31, r32 m32, r32 m33)
{
    m4 result;

    result.m[0][0] = m00;
    result.m[0][1] = m01;
    result.m[0][2] = m02;
    result.m[0][3] = m03;

    result.m[1][0] = m10;
    result.m[1][1] = m11;
    result.m[1][2] = m12;
    result.m[1][3] = m13;

    result.m[2][0] = m20;
    result.m[2][1] = m21;
    result.m[2][2] = m22;
    result.m[2][3] = m23;

    result.m[3][0] = m30;
    result.m[3][1] = m31;
    result.m[3][2] = m32;
    result.m[3][3] = m33;
    
    return result;
}

inline v3 M4Mul(v3 v, m4 M)
{
    v3 result;

    result.x = v.x*M.m[0][0] + v.y*M.m[1][0] + v.z*M.m[2][0] + M.m[3][0];
    result.y = v.x*M.m[0][1] + v.y*M.m[1][1] + v.z*M.m[2][1] + M.m[3][1];
    result.z = v.x*M.m[0][2] + v.y*M.m[1][2] + v.z*M.m[2][2] + M.m[3][2];
    /* result.w = v.x*M.m[0][3] + v.y*M.m[1][3] + v.z*M.m[2][3] + v.w*M.m[3][3]; */

    return result;
}

// @TODO optimization when one matrix is M4Unit
inline m4 M4MulM4p(m4* m1, m4* m2)
{
    m4 result = {0}; // @TODO make an M4Zero();

    for (s32 i=0; i < 4; i++) // by row
        {
            for (s32 j=0; j < 4; j++) // by col
                {
                    for (s32 k=0; k < 4; k++)
                        {
                            result.m[i][j] += m1->m[i][k] * m2->m[k][j];
                        }
                }
        }
    
    return result;
}

inline m4 M4MulM4(m4 m1, m4 m2)
{
    m4 result;

    for (s32 i=0; i < 4; i++)
        {
            for (s32 j=0; j < 4; j++)
                {
                    for (s32 k=0; k < 4; k++)
                        {
                            result.m[i][j] += m1.m[i][k] * m2.m[k][j];
                        }
                }
        }
    
    return result;
}

inline m4 M4Trans(m4 M)
{
    m4 result;
    // @TODO @IMPORTANT make a swap utility function and use that instead.?
    result.m[0][0] = M.m[0][0];
    result.m[0][1] = M.m[1][0];
    result.m[0][2] = M.m[2][0];
    result.m[0][3] = M.m[3][0];

    result.m[1][0] = M.m[0][1];
    result.m[1][1] = M.m[1][1];
    result.m[1][2] = M.m[2][1];
    result.m[1][3] = M.m[3][1];

    result.m[2][0] = M.m[0][2];
    result.m[2][1] = M.m[1][2];
    result.m[2][2] = M.m[2][2];
    result.m[2][3] = M.m[3][2];

    result.m[3][0] = M.m[0][3];
    result.m[3][1] = M.m[1][3];
    result.m[3][2] = M.m[2][3];
    result.m[3][3] = M.m[3][3];
    
    return result;
}

inline m4 M4RotX(r32 a)
{
    m4 result;

    result.m[0][0] = 1;
    result.m[0][1] = 0;
    result.m[0][2] = 0;
    result.m[0][3] = 0;

    result.m[1][0] = 0;
    result.m[1][1] = cos(a);
    result.m[1][2] = sin(a);
    result.m[1][3] = 0;

    result.m[2][0] = 0;
    result.m[2][1] = -sin(a);
    result.m[2][2] = cos(a);
    result.m[2][3] = 0;

    result.m[3][0] = 0;
    result.m[3][1] = 0;
    result.m[3][2] = 0;
    result.m[3][3] = 1;
    
    return result;
}

inline m4 M4RotY(r32 a)
{
    m4 result;

    result.m[0][0] = cos(a);
    result.m[0][1] = 0;
    result.m[0][2] = -sin(a);
    result.m[0][3] = 0;

    result.m[1][0] = 0;
    result.m[1][1] = 1;
    result.m[1][2] = 0;
    result.m[1][3] = 0;

    result.m[2][0] = sin(a);
    result.m[2][1] = 0;
    result.m[2][2] = cos(a);
    result.m[2][3] = 0;

    result.m[3][0] = 0;
    result.m[3][1] = 0;
    result.m[3][2] = 0;
    result.m[3][3] = 1;
    
    return result;
}

inline m4 M4RotZ(r32 a)
{
    m4 result;
    
    result.m[0][0] = cos(a);
    result.m[0][1] = sin(a);
    result.m[0][2] = 0;
    result.m[0][3] = 0;

    result.m[1][0] = -sin(a);
    result.m[1][1] = cos(a);
    result.m[1][2] = 0;
    result.m[1][3] = 0;

    result.m[2][0] = 0;
    result.m[2][1] = 0;
    result.m[2][2] = 1;
    result.m[2][3] = 0;

    result.m[3][0] = 0;
    result.m[3][1] = 0;
    result.m[3][2] = 0;
    result.m[3][3] = 1;
    
    return result;
}

inline m4 M4Scale(v3 v)
{
    m4 result = {0};
    result.m[0][0] = v.x;
    result.m[1][1] = v.y;
    result.m[2][2] = v.z;
    result.m[3][3] = 1;
    return result;
}

inline m4 M4Unit()
{
    m4 result = {0};
    result.m[0][0] = 1;
    result.m[1][1] = 1;
    result.m[2][2] = 1;
    result.m[3][3] = 1;
    return result;
}

inline m4 M4Tlate(v3 v) // maybe one that adds upon previous?
{
    m4 result = M4Unit();

    result.m[3][0]=v.x;
    result.m[3][1]=v.y;
    result.m[3][2]=v.z;
    
    return result;
}

// these are maybe useless
inline m4 M4SetX(m4 m, v3 v)
{
    m4 result;

    result.m[0][0]=v.x;
    result.m[0][1]=v.y;
    result.m[0][2]=v.z;
    
    return result;
}

inline m4 M4SetY(m4 m, v3 v)
{
    m4 result;

    result.m[1][0]=v.x;
    result.m[1][1]=v.y;
    result.m[1][2]=v.z;
    
    return result;
}

inline m4 M4SetZ(m4 m, v3 v)
{
    m4 result;

    result.m[2][0]=v.x;
    result.m[2][1]=v.y;
    result.m[2][2]=v.z;
    
    return result;
}

inline m4 M4Compose(s32 count, ...)
{
    m4 result = M4Unit();

    va_list matrices;
    va_start(matrices, count);

    for (s32 i = 0; i < count; i++)
        {
            result = M4MulM4p(&result, va_arg(matrices, m4*));
        }

    va_end(matrices);
    
    return result;
}

#endif
