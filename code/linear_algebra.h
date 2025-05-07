#ifndef LINEAR_ALGEBRA_H
#define LINEAR_ALGEBRA_H

// @todo rename to vector.h
// @todo make certain functions like vec_add variadic? but it's also
// life if you did operator overloading honestly..

// VECTORS FOR r32
union Vec2f
{
    struct { r32 x, y; };
    r32 e[2];
};

union Vec3f
{
    struct { r32 x, y, z; };
    r32 e[3];
};

union Vec4f
{
    struct { r32 x, y, z, w; };
    r32 e[4];
};


// VECTORS FOR r64
union Vec2d
{
    struct { r64 x, y; };
    r64 e[2];
};

union Vec3d
{
    struct { r64 x, y, z; };
    r64 e[3];
};

union Vec4d
{
    struct { r64 x, y, z, w; };
    r64 e[4];
};


// VECTOR FOR s32
union Vec2i
{
    struct { s32 x, y; };
    s32 e[2];
};

union Vec3i
{
    struct { s32 x, y, z; };
    s32 e[3];
};

union Vec4i
{
    struct { s32 x, y, z, w; };
    s32 e[4];
};


// VECTOR FOR s64
union Vec2l
{
    struct { s64 x, y; };
    s64 e[2];
};

union Vec3l
{
    struct { s64 x, y, z; };
    s64 e[3];
};

union Vec4l
{
    struct { s64 x, y, z, w; };
    s64 e[4];
};


// VECTOR FUNCTIONS FOR r32

inline Vec2f vec_make(r32 x, r32 y)
{
    return {x, y};
}
inline Vec3f vec_make(r32 x, r32 y, r32 z)
{
    return {x, y, z};
}
inline Vec4f vec_make(r32 x, r32 y, r32 z, r32 w)
{
    return {x, y, z, w};
}

inline Vec2f vec_zero2f()
{
    return {0, 0};
}
inline Vec3f vec_zero3f()
{
    return {0, 0, 0};
}
inline Vec4f vec_zero4f()
{
    return {0, 0, 0, 0};
}

inline r32 vec_len(Vec2f v)
{
    return sqrtf(sqr(v.x) + sqr(v.y));
}
inline r32 vec_len(Vec3f v)
{
    return sqrtf(sqr(v.x) + sqr(v.y) + sqr(v.z));
}

// @pot a version without taking sqrt for performance
inline r32 vec_edist(Vec2f v1, Vec2f v2)
{
    return sqrtf(sqr(v1.x - v2.x) +
                 sqr(v1.y - v2.y));
}
inline r32 vec_edist(Vec3f v1, Vec3f v2)
{
    return sqrtf(sqr(v1.x - v2.x) +
                 sqr(v1.y - v2.y) +
                 sqr(v1.z - v2.z));
}

// @doc I think you never want to check for normalized status, just
// normalize instead
inline Vec2f vec_normalize(Vec2f v)
{
    r32 len = vec_len(v);
    return {v.x / len, v.y / len};
}
inline Vec3f vec_normalize(Vec3f v)
{
    r32 len = vec_len(v);
    return {v.x / len, v.y / len, v.z / len};
}

inline Vec2f vec_add(Vec2f v1, Vec2f v2)
{
    return {v1.x + v2.x,
            v1.y + v2.y};
}
inline Vec3f vec_add(Vec3f v1, Vec3f v2)
{
    return {v1.x + v2.x,
            v1.y + v2.y,
            v1.z + v2.z};
}

inline Vec2f vec_sub(Vec2f v1, Vec2f v2)
{
    return {v1.x - v2.x,
            v1.y - v2.y};
}
inline Vec3f vec_sub(Vec3f v1, Vec3f v2)
{
    return {v1.x - v2.x,
            v1.y - v2.y,
            v1.z - v2.z};
}

inline Vec2f vec_dot(Vec2f v1, Vec2f v2)
{
    return {v1.x * v2.x,
            v1.y * v2.y};
}
inline Vec3f vec_dot(Vec3f v1, Vec3f v2)
{
    return {v1.x * v2.x,
            v1.y * v2.y,
            v1.z * v2.z};
}

inline Vec2f vec_dotself(Vec2f v)
{
    return {v.x * v.x,
            v.y * v.y};
}
inline Vec3f vec_dotself(Vec3f v)
{
    return {v.x * v.x,
            v.y * v.y,
            v.z * v.z};
}

// @todo figure out how these actually work mathematically...
inline r32 vec_cross(Vec2f v1, Vec2f v2)
{
    return v1.x * v2.y - v1.y * v2.x;
}
inline Vec3f vec_cross(Vec3f v1, Vec3f v2)
{
    Vec3f result;
    result.x = v1.y * v2.z - v1.z * v2.y;
    result.y = v1.z * v2.x - v1.x * v2.z;
    result.z = v1.x * v2.y - v1.y * v2.x;
    return result;
}

inline Vec2f vec_scale(Vec2f v, r32 s)
{
    return {s * v.x, s * v.y};
}
inline Vec3f vec_scale(Vec3f v, r32 s)
{
    return {s * v.x, s * v.y, s * v.z};
}


// VECTOR FUNCTIONS FOR r64

inline Vec2d vec_make(r64 x, r64 y)
{
    return {x, y};
}
inline Vec3d vec_make(r64 x, r64 y, r64 z)
{
    return {x, y, z};
}
inline Vec4d vec_make(r64 x, r64 y, r64 z, r64 w)
{
    return {x, y, z, w};
}

inline Vec2d vec_zero2d()
{
    return {0, 0};
}
inline Vec3d vec_zero3d()
{
    return {0, 0, 0};
}
inline Vec4d vec_zero4d()
{
    return {0, 0, 0, 0};
}

inline r64 vec_len(Vec2d v)
{
    return sqrt(sqr(v.x) + sqr(v.y));
}
inline r64 vec_len(Vec3d v)
{
    return sqrt(sqr(v.x) + sqr(v.y) + sqr(v.z));
}

inline r64 vec_edist(Vec2d v1, Vec2d v2)
{
    return sqrt(sqr(v1.x - v2.x) +
                sqr(v1.y - v2.y));
}
inline r64 vec_edist(Vec3d v1, Vec3d v2)
{
    return sqrt(sqr(v1.x - v2.x) +
                sqr(v1.y - v2.y) +
                sqr(v1.z - v2.z));
}

inline Vec2d vec_normalize(Vec2d v)
{
    r64 len = vec_len(v);
    return {v.x / len, v.y / len};
}
inline Vec3d vec_normalize(Vec3d v)
{
    r64 len = vec_len(v);
    return {v.x / len, v.y / len, v.z / len};
}

inline Vec2d vec_add(Vec2d v1, Vec2d v2)
{
    return {v1.x + v2.x,
            v1.y + v2.y};
}
inline Vec3d vec_add(Vec3d v1, Vec3d v2)
{
    return {v1.x + v2.x,
            v1.y + v2.y,
            v1.z + v2.z};
}

inline Vec2d vec_sub(Vec2d v1, Vec2d v2)
{
    return {v1.x - v2.x,
            v1.y - v2.y};
}
inline Vec3d vec_sub(Vec3d v1, Vec3d v2)
{
    return {v1.x - v2.x,
            v1.y - v2.y,
            v1.z - v2.z};
}

inline Vec2d vec_dot(Vec2d v1, Vec2d v2)
{
    return {v1.x * v2.x,
            v1.y * v2.y};
}
inline Vec3d vec_dot(Vec3d v1, Vec3d v2)
{
    return {v1.x * v2.x,
            v1.y * v2.y,
            v1.z * v2.z};
}

inline Vec2d vec_dotself(Vec2d v)
{
    return {v.x * v.x,
            v.y * v.y};
}
inline Vec3d vec_dotself(Vec3d v)
{
    return {v.x * v.x,
            v.y * v.y,
            v.z * v.z};
}

inline r64 vec_cross(Vec2d v1,Vec2d v2)
{
    return v1.x * v2.y - v1.y * v2.x;
}
inline Vec3d vec_cross(Vec3d v1, Vec3d v2)
{
    Vec3d result;
    result.x = v1.y * v2.z - v1.z * v2.y;
    result.y = v1.z * v2.x - v1.x * v2.z;
    result.z = v1.x * v2.y - v1.y * v2.x;
    return result;
}

inline Vec2d vec_scale(Vec2d v, r64 s)
{
    return {s * v.x, s * v.y};
}
inline Vec3d vec_scale(Vec3d v, r64 s)
{
    return {s * v.x, s * v.y, s * v.z};
}


// VECTOR FUNCTIONS FOR s32

inline Vec2i vec_make(s32 x, s32 y)
{
    return {x, y};
}
inline Vec3i vec_make(s32 x, s32 y, s32 z)
{
    return {x, y, z};
}
inline Vec4i vec_make(s32 x, s32 y, s32 z, s32 w)
{
    return {x, y, z, w};
}

inline Vec2i vec_zero2i()
{
    return {0, 0};
}
inline Vec3i vec_zero3i()
{
    return {0, 0, 0};
}
inline Vec4i vec_zero4i()
{
    return {0, 0, 0, 0};
}

inline r32 vec_len(Vec2i v)
{
    return sqrtf(sqr(v.x) + sqr(v.y));
}
inline r32 vec_len(Vec3i v)
{
    return sqrtf(sqr(v.x) + sqr(v.y) + sqr(v.z));
}

inline r32 vec_edist(Vec2i v1, Vec2i v2)
{
    return sqrtf(sqr(v1.x - v2.x) +
                 sqr(v1.y - v2.y));
}
inline r32 vec_edist(Vec3i v1, Vec3i v2)
{
    return sqrtf(sqr(v1.x - v2.x) +
                 sqr(v1.y - v2.y) +
                 sqr(v1.z - v2.z));
}

inline Vec2i vec_add(Vec2i v1, Vec2i v2)
{
    return {v1.x + v2.x,
            v1.y + v2.y};
}
inline Vec3i vec_add(Vec3i v1, Vec3i v2)
{
    return {v1.x + v2.x,
            v1.y + v2.y,
            v1.z + v2.z};
}

inline Vec2i vec_sub(Vec2i v1, Vec2i v2)
{
    return {v1.x - v2.x,
            v1.y - v2.y};
}
inline Vec3i vec_sub(Vec3i v1, Vec3i v2)
{
    return {v1.x - v2.x,
            v1.y - v2.y,
            v1.z - v2.z};
}

inline Vec2i vec_dot(Vec2i v1, Vec2i v2)
{
    return {v1.x * v2.x,
            v1.y * v2.y};
}
inline Vec3i vec_dot(Vec3i v1, Vec3i v2)
{
    return {v1.x * v2.x,
            v1.y * v2.y,
            v1.z * v2.z};
}

inline Vec2i vec_dotself(Vec2i v)
{
    return {v.x * v.x,
            v.y * v.y};
}
inline Vec3i vec_dotself(Vec3i v)
{
    return {v.x * v.x,
            v.y * v.y,
            v.z * v.z};
}

inline s32 vec_cross(Vec2i v1, Vec2i v2)
{
    return v1.x * v2.y - v1.y * v2.x;
}
inline Vec3i vec_cross(Vec3i v1, Vec3i v2)
{
    Vec3i result;
    result.x = v1.y * v2.z - v1.z * v2.y;
    result.y = v1.z * v2.x - v1.x * v2.z;
    result.z = v1.x * v2.y - v1.y * v2.x;
    return result;
}

inline Vec2i vec_scale(Vec2i v, s32 s)
{
    return {s * v.x, s * v.y};
}
inline Vec3i vec_scale(Vec3i v, s32 s)
{
    return {s * v.x, s * v.y, s * v.z};
}


// VECTOR FUNCTIONS FOR s64

inline Vec2l vec_make(s64 x, s64 y)
{
    return {x, y};
}
inline Vec3l vec_make(s64 x, s64 y, s64 z)
{
    return {x, y, z};
}
inline Vec4l vec_make(s64 x, s64 y, s64 z, s64 w)
{
    return {x, y, z, w};
}

inline Vec2l vec_zero2l()
{
    return {0, 0};
}
inline Vec3l vec_zero3l()
{
    return {0, 0, 0};
}
inline Vec4l vec_zero4l()
{
    return {0, 0, 0, 0};
}

inline r64 vec_len(Vec2l v)
{
    return sqrt(sqr(v.x) + sqr(v.y));
}
inline r64 vec_len(Vec3l v)
{
    return sqrt(sqr(v.x) + sqr(v.y) + sqr(v.z));
}

inline r64 vec_edist(Vec2l v1, Vec2l v2)
{
    return sqrt(sqr(v1.x - v2.x) +
                sqr(v1.y - v2.y));
}
inline r64 vec_edist(Vec3l v1, Vec3l v2)
{
    return sqrt(sqr(v1.x - v2.x) +
                sqr(v1.y - v2.y) +
                sqr(v1.z - v2.z));
}

inline Vec2l vec_add(Vec2l v1, Vec2l v2)
{
    return {v1.x + v2.x,
            v1.y + v2.y};
}
inline Vec3l vec_add(Vec3l v1, Vec3l v2)
{
    return {v1.x + v2.x,
            v1.y + v2.y,
            v1.z + v2.z};
}

inline Vec2l vec_sub(Vec2l v1, Vec2l v2)
{
    return {v1.x - v2.x,
            v1.y - v2.y};
}
inline Vec3l vec_sub(Vec3l v1, Vec3l v2)
{
    return {v1.x - v2.x,
            v1.y - v2.y,
            v1.z - v2.z};
}

inline Vec2l vec_dot(Vec2l v1, Vec2l v2)
{
    return {v1.x * v2.x,
            v1.y * v2.y};
}
inline Vec3l vec_dot(Vec3l v1, Vec3l v2)
{
    return {v1.x * v2.x,
            v1.y * v2.y,
            v1.z * v2.z};
}

inline Vec2l vec_dotself(Vec2l v)
{
    return {v.x * v.x,
            v.y * v.y};
}
inline Vec3l vec_dotself(Vec3l v)
{
    return {v.x * v.x,
            v.y * v.y,
            v.z * v.z};
}

inline s64 vec_cross(Vec2l v1, Vec2l v2)
{
    return v1.x * v2.y - v1.y * v2.x;
}
inline Vec3l vec_cross(Vec3l v1, Vec3l v2)
{
    Vec3l result;
    result.x = v1.y * v2.z - v1.z * v2.y;
    result.y = v1.z * v2.x - v1.x * v2.z;
    result.z = v1.x * v2.y - v1.y * v2.x;
    return result;
}

inline Vec2l vec_scale(Vec2l v, s64 s)
{
    return {s * v.x, s * v.y};
}
inline Vec3l vec_scale(Vec3l v, s64 s)
{
    return {s * v.x, s * v.y, s * v.z};
}


// @todo perp, transpose, rotate?

#endif
