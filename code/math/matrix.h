#ifndef MATRIX_H
#define MATRIX_H


// @todo do the row/column major thingy, with macro toggle


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

Vec3f matrix_mul(Matrix4x4f* m, Vec3f v);
void matrix_mul(Matrix4x4f* m1, Matrix4x4f* m2, Matrix4x4f* result);
void matrix_transpose(Matrix4x4f* m);

Vec3d matrix_mul(Matrix4x4d* m, Vec3d v);
void matrix_mul(Matrix4x4d* m1, Matrix4x4d* m2, Matrix4x4d* result);
void matrix_transpose(Matrix4x4d* m);


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

// inline void MatrixToStr(u8* buff, m4 M)
// {
//     s32 pos = 0;
//     for (s32 i = 0; i < 4; i++)
//         {
//             for (s32 j = 0; j < 4; j++)
//                 {
//                     pos += sprintf((char* const)(buff+pos), "%8.2f", M.m[i][j]);
//                 }
//             pos += sprintf((char* const)(buff+pos), "\n");
//         }
// }


#endif
