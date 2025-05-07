#ifndef MATRIX_H
#define MATRIX_H


// @todo do the row/column major thingy, with macro toggle
// @todo make some of these noinline?

// @doc matrices are row-major


union Matrix4x4f
{
    struct {Vec4f X, Y, Z, W;};
    Vec4f row[4];
    r32 e[4][4];
    r32 i[16];
};

union Matrix4x4d
{
    struct {Vec4d X, Y, Z, W;};
    Vec4d row[4];
    r64 e[4][4];
    r64 i[16];
};


// MATRIX FUNCTIONS FOR r32

inline void matrix_unit(Matrix4x4f* m)
{
    m->X = {1, 0, 0, 0};
    m->Y = {0, 1, 0, 0};
    m->Z = {0, 0, 1, 0};
    m->W = {0, 0, 0, 1};
}

inline void matrix_make(Matrix4x4f* m, Vec4f X, Vec4f Y, Vec4f Z, Vec4f W)
{
    m->X = X;
    m->Y = Y;
    m->Z = Z;
    m->W = W;
}

// @todo figure out how this works with homogeneous coordinates
inline Vec3f matrix_mul(Matrix4x4f* m, Vec3f v)
{
    Vec3f result;
    result.x = v.x*m->e[0][0] + v.y*m->e[1][0] + v.z*m->e[2][0] + m->e[3][0];
    result.y = v.x*m->e[0][1] + v.y*m->e[1][1] + v.z*m->e[2][1] + m->e[3][1];
    result.z = v.x*m->e[0][2] + v.y*m->e[1][2] + v.z*m->e[2][2] + m->e[3][2];
    return result;
}

inline void matrix_mul(Matrix4x4f* m1, Matrix4x4f* m2, Matrix4x4f* result)
{
    for (s32 i=0; i < 4; i++)
        {
            for (s32 j=0; j < 4; j++)
                {
                    for (s32 k=0; k < 4; k++)
                        {
                            result->e[i][j] += m1->e[i][k] * m2->e[k][j];
                        }
                }
        }
}

inline void matrix_transpose(Matrix4x4f* m)
{
    swap(m->e[0][1], m->e[1][0]);
    swap(m->e[0][2], m->e[2][0]);
    swap(m->e[0][3], m->e[3][0]);

    swap(m->e[1][0], m->e[0][1]);
    swap(m->e[1][2], m->e[2][1]);
    swap(m->e[1][3], m->e[3][1]);

    swap(m->e[2][0], m->e[0][2]);
    swap(m->e[2][1], m->e[1][2]);
    swap(m->e[2][3], m->e[3][2]);

    swap(m->e[3][0], m->e[0][3]);
    swap(m->e[3][1], m->e[1][3]);
    swap(m->e[3][2], m->e[2][3]);
}

inline void matrix_rotation_x(Matrix4x4f* m, r32 angle)
{
    r32 sinA = sinf(angle);
    r32 cosA = cosf(angle);

    m->X = {1,    0,     0, 0};
    m->Y = {0,  cosA, sinA, 0};
    m->Z = {0, -sinA, cosA, 0};
    m->W = {0,     0,    0, 1};
}

inline void matrix_rotation_y(Matrix4x4f* m, r32 angle)
{
    r32 sinA = sinf(angle);
    r32 cosA = cosf(angle);

    m->X = {cosA, 0, -sinA, 0};
    m->Y = {0,    1,     0, 0};
    m->Z = {sinA, 0,  cosA, 0};
    m->W = {0,    0,     0, 1};
}

inline void matrix_rotation_z(Matrix4x4f* m, r32 angle)
{
    r32 sinA = sinf(angle);
    r32 cosA = cosf(angle);

    m->X = { cosA, sinA, 0, 0};
    m->Y = {-sinA, cosA, 0, 0};
    m->Z = {0,        0, 1, 0};
    m->W = {0,        0, 0, 1};
}

inline void matrix_scale(Matrix4x4f* m, Vec3f v)
{
    m->e[0][0] *= v.x;
    m->e[1][1] *= v.y;
    m->e[2][2] *= v.z;
}

inline void matrix_translate(Matrix4x4f* m, Vec3f v)
{
    m->e[3][0] += v.x;
    m->e[3][1] += v.y;
    m->e[3][2] += v.z;
}


// MATRIX FUNCTIONS FOR r64

inline void matrix_unit(Matrix4x4d* m)
{
    m->X = {1, 0, 0, 0};
    m->Y = {0, 1, 0, 0};
    m->Z = {0, 0, 1, 0};
    m->W = {0, 0, 0, 1};
}

inline void matrix_make(Matrix4x4d* m, Vec4d X, Vec4d Y, Vec4d Z, Vec4d W)
{
    m->X = X;
    m->Y = Y;
    m->Z = Z;
    m->W = W;
}

inline Vec3d matrix_mul(Matrix4x4d* m, Vec3d v)
{
    Vec3d result;
    result.x = v.x*m->e[0][0] + v.y*m->e[1][0] + v.z*m->e[2][0] + m->e[3][0];
    result.y = v.x*m->e[0][1] + v.y*m->e[1][1] + v.z*m->e[2][1] + m->e[3][1];
    result.z = v.x*m->e[0][2] + v.y*m->e[1][2] + v.z*m->e[2][2] + m->e[3][2];
    return result;
}

inline void matrix_mul(Matrix4x4d* m1, Matrix4x4d* m2, Matrix4x4d* result)
{
    for (s32 i=0; i < 4; i++)
        {
            for (s32 j=0; j < 4; j++)
                {
                    for (s32 k=0; k < 4; k++)
                        {
                            result->e[i][j] += m1->e[i][k] * m2->e[k][j];
                        }
                }
        }
}

inline void matrix_transpose(Matrix4x4d* m)
{
    swap(m->e[0][1], m->e[1][0]);
    swap(m->e[0][2], m->e[2][0]);
    swap(m->e[0][3], m->e[3][0]);

    swap(m->e[1][0], m->e[0][1]);
    swap(m->e[1][2], m->e[2][1]);
    swap(m->e[1][3], m->e[3][1]);

    swap(m->e[2][0], m->e[0][2]);
    swap(m->e[2][1], m->e[1][2]);
    swap(m->e[2][3], m->e[3][2]);

    swap(m->e[3][0], m->e[0][3]);
    swap(m->e[3][1], m->e[1][3]);
    swap(m->e[3][2], m->e[2][3]);
}

inline void matrix_rotation_x(Matrix4x4d* m, r64 angle)
{
    r64 sinA = sin(angle);
    r64 cosA = cos(angle);

    m->X = {1,    0,     0, 0};
    m->Y = {0,  cosA, sinA, 0};
    m->Z = {0, -sinA, cosA, 0};
    m->W = {0,     0,    0, 1};
}

inline void matrix_rotation_y(Matrix4x4d* m, r64 angle)
{
    r64 sinA = sin(angle);
    r64 cosA = cos(angle);

    m->X = {cosA, 0, -sinA, 0};
    m->Y = {0,    1,     0, 0};
    m->Z = {sinA, 0,  cosA, 0};
    m->W = {0,    0,     0, 1};
}

inline void matrix_rotation_z(Matrix4x4d* m, r64 angle)
{
    r64 sinA = sin(angle);
    r64 cosA = cos(angle);

    m->X = { cosA, sinA, 0, 0};
    m->Y = {-sinA, cosA, 0, 0};
    m->Z = {0,        0, 1, 0};
    m->W = {0,        0, 0, 1};
}

inline void matrix_scale(Matrix4x4d* m, Vec3d v)
{
    m->e[0][0] *= v.x;
    m->e[1][1] *= v.y;
    m->e[2][2] *= v.z;
}

inline void matrix_translate(Matrix4x4d* m, Vec3d v)
{
    m->e[3][0] += v.x;
    m->e[3][1] += v.y;
    m->e[3][2] += v.z;
}

// @todo matrix_compose_transforms variadic function (or template?)

#endif
