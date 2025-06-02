#ifndef VECTOR_H
#define VECTOR_H

// @todo make certain functions like vec_add variadic? but it's also
// like if you did operator overloading honestly..

// @todo implement vec_negate for everything..

// VECTORS FOR r32
union Vector2
{
    struct { r32 x, y; };
    r32 e[2];
};

union Vector3
{
    struct { r32 x, y, z; };
    r32 e[3];
};

union Vector4
{
    struct { r32 x, y, z, w; };
    r32 e[4];
};


// VECTOR FOR s32
union Vector2i
{
    struct { s32 x, y; };
    s32 e[2];
};

union Vector3i
{
    struct { s32 x, y, z; };
    s32 e[3];
};

union Vector4i
{
    struct { s32 x, y, z, w; };
    s32 e[4];
};


// VECTOR FUNCTIONS FOR r32

inline Vector2 vec_make(r32 x, r32 y)
{
    return {x, y};
}
inline Vector3 vec_make(r32 x, r32 y, r32 z)
{
    return {x, y, z};
}
inline Vector4 vec_make(r32 x, r32 y, r32 z, r32 w)
{
    return {x, y, z, w};
}

inline Vector2 vec_zero2()
{
    return {0, 0};
}
inline Vector3 vec_zero3()
{
    return {0, 0, 0};
}
inline Vector4 vec_zero4()
{
    return {0, 0, 0, 0};
}

inline r32 vec_len(Vector2 v)
{
    return sqrtf(sqr(v.x) + sqr(v.y));
}
inline r32 vec_len(Vector3 v)
{
    return sqrtf(sqr(v.x) + sqr(v.y) + sqr(v.z));
}

inline r32 vec_len_squared(Vector2 v)
{
    return sqr(v.x) + sqr(v.y);
}
inline r32 vec_len_squared(Vector3 v)
{
    return sqr(v.x) + sqr(v.y) + sqr(v.z);
}

inline r32 vec_edist(Vector2 v1, Vector2 v2)
{
    return sqrtf(sqr(v1.x - v2.x) + sqr(v1.y - v2.y));
}
inline r32 vec_edist(const Vector3& v1, const Vector3& v2)
{
    return sqrtf(sqr(v1.x - v2.x) + sqr(v1.y - v2.y) + sqr(v1.z - v2.z));
}

inline r32 vec_edist_squared(Vector2 v1, Vector2 v2)
{
    return sqr(v1.x - v2.x) + sqr(v1.y - v2.y);
}
inline r32 vec_edist_squared(const Vector3& v1, const Vector3& v2)
{
    return sqr(v1.x - v2.x) + sqr(v1.y - v2.y) + sqr(v1.z - v2.z);
}

// @doc I think you never want to check for normalized status, just
// normalize instead
inline Vector2 vec_normalize(Vector2 v)
{
    r32 len = vec_len(v);
    return {v.x / len, v.y / len};
}
inline Vector3 vec_normalize(Vector3 v)
{
    r32 len = vec_len(v);
    return {v.x / len, v.y / len, v.z / len};
}

inline Vector2 vec_add(Vector2 v1, Vector2 v2)
{
    return {v1.x + v2.x,
            v1.y + v2.y};
}
inline Vector3 vec_add(const Vector3& v1, const Vector3& v2)
{
    return {v1.x + v2.x,
            v1.y + v2.y,
            v1.z + v2.z};
}

inline Vector2 vec_sub(Vector2 v1, Vector2 v2)
{
    return {v1.x - v2.x,
            v1.y - v2.y};
}
inline Vector3 vec_sub(const Vector3& v1, const Vector3& v2)
{
    return {v1.x - v2.x,
            v1.y - v2.y,
            v1.z - v2.z};
}

inline Vector2 vec_dot(Vector2 v1, Vector2 v2)
{
    return {v1.x * v2.x,
            v1.y * v2.y};
}
inline Vector3 vec_dot(const Vector3& v1, const Vector3& v2)
{
    return {v1.x * v2.x,
            v1.y * v2.y,
            v1.z * v2.z};
}

inline Vector2 vec_dotself(Vector2 v)
{
    return {v.x * v.x,
            v.y * v.y};
}
inline Vector3 vec_dotself(Vector3 v)
{
    return {v.x * v.x,
            v.y * v.y,
            v.z * v.z};
}

// @todo figure out how these actually work mathematically...
inline r32 vec_cross(Vector2 v1, Vector2 v2)
{
    return v1.x * v2.y - v1.y * v2.x;
}
inline Vector3 vec_cross(const Vector3& v1, const Vector3& v2)
{
    Vector3 result;
    result.x = v1.y * v2.z - v1.z * v2.y;
    result.y = v1.z * v2.x - v1.x * v2.z;
    result.z = v1.x * v2.y - v1.y * v2.x;
    return result;
}

inline Vector2 vec_scale(Vector2 v, r32 s)
{
    return {s * v.x, s * v.y};
}
inline Vector3 vec_scale(Vector3 v, r32 s)
{
    return {s * v.x, s * v.y, s * v.z};
}

inline Vector2 vec_negate(Vector2 v)
{
    return {-1 * v.x, -1 * v.y};
}
inline Vector3 vec_negate(Vector3 v)
{
    return {-1 * v.x, -1 * v.y, -1 * v.z};
}


// VECTOR FUNCTIONS FOR s32

inline Vector2i vec_make(s32 x, s32 y)
{
    return {x, y};
}
inline Vector3i vec_make(s32 x, s32 y, s32 z)
{
    return {x, y, z};
}
inline Vector4i vec_make(s32 x, s32 y, s32 z, s32 w)
{
    return {x, y, z, w};
}

inline Vector2i vec_zero2i()
{
    return {0, 0};
}
inline Vector3i vec_zero3i()
{
    return {0, 0, 0};
}
inline Vector4i vec_zero4i()
{
    return {0, 0, 0, 0};
}

inline r32 vec_len(Vector2i v)
{
    return sqrtf(sqr(v.x) + sqr(v.y));
}
inline r32 vec_len(Vector3i v)
{
    return sqrtf(sqr(v.x) + sqr(v.y) + sqr(v.z));
}

inline r32 vec_edist(Vector2i v1, Vector2i v2)
{
    return sqrtf(sqr(v1.x - v2.x) +
                 sqr(v1.y - v2.y));
}
inline r32 vec_edist(const Vector3i& v1, const Vector3i& v2)
{
    return sqrtf(sqr(v1.x - v2.x) +
                 sqr(v1.y - v2.y) +
                 sqr(v1.z - v2.z));
}

inline Vector2i vec_add(Vector2i v1, Vector2i v2)
{
    return {v1.x + v2.x,
            v1.y + v2.y};
}
inline Vector3i vec_add(const Vector3i& v1, const Vector3i& v2)
{
    return {v1.x + v2.x,
            v1.y + v2.y,
            v1.z + v2.z};
}

inline Vector2i vec_sub(Vector2i v1, Vector2i v2)
{
    return {v1.x - v2.x,
            v1.y - v2.y};
}
inline Vector3i vec_sub(const Vector3i& v1, const Vector3i& v2)
{
    return {v1.x - v2.x,
            v1.y - v2.y,
            v1.z - v2.z};
}

inline Vector2i vec_dot(Vector2i v1, Vector2i v2)
{
    return {v1.x * v2.x,
            v1.y * v2.y};
}
inline Vector3i vec_dot(const Vector3i& v1, const Vector3i& v2)
{
    return {v1.x * v2.x,
            v1.y * v2.y,
            v1.z * v2.z};
}

inline Vector2i vec_dotself(Vector2i v)
{
    return {v.x * v.x,
            v.y * v.y};
}
inline Vector3i vec_dotself(Vector3i v)
{
    return {v.x * v.x,
            v.y * v.y,
            v.z * v.z};
}

inline s32 vec_cross(Vector2i v1, Vector2i v2)
{
    return v1.x * v2.y - v1.y * v2.x;
}
inline Vector3i vec_cross(const Vector3i& v1, const Vector3i& v2)
{
    Vector3i result;
    result.x = v1.y * v2.z - v1.z * v2.y;
    result.y = v1.z * v2.x - v1.x * v2.z;
    result.z = v1.x * v2.y - v1.y * v2.x;
    return result;
}

inline Vector2i vec_scale(Vector2i v, s32 s)
{
    return {s * v.x, s * v.y};
}
inline Vector3i vec_scale(Vector3i v, s32 s)
{
    return {s * v.x, s * v.y, s * v.z};
}


// @todo perp, transpose, rotate?

#endif
