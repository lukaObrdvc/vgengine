#ifndef MATRIX_H
#define MATRIX_H


// @todo do the row/column major thingy, with macro toggle


// @doc matrices are row-major
union Matrix4
{
    struct {Vector4 X, Y, Z, W;};
    Vector4 row[4];
    r32 e[4][4];
    r32 i[16];
};

Vector3 matrix_mul_vector(Matrix4* m, Vector3 v);
void matrix_mul(Matrix4* m1, Matrix4* m2, Matrix4* result);
void matrix_transpose(Matrix4* m);


// MATRIX FUNCTIONS FOR r32

inline void matrix_unit(Matrix4* m)
{
    m->X = {1, 0, 0, 0};
    m->Y = {0, 1, 0, 0};
    m->Z = {0, 0, 1, 0};
    m->W = {0, 0, 0, 1};
}

inline void matrix_make(Matrix4* m, Vector4 X, Vector4 Y, Vector4 Z, Vector4 W)
{
    m->X = X;
    m->Y = Y;
    m->Z = Z;
    m->W = W;
}

inline void matrix_rot_x(Matrix4* m, r32 angle)
{
    r32 sinA = sinf(angle);
    r32 cosA = cosf(angle);

    m->X = {1,    0,     0, 0};
    m->Y = {0,  cosA, sinA, 0};
    m->Z = {0, -sinA, cosA, 0};
    m->W = {0,     0,    0, 1};
}

inline void matrix_rot_y(Matrix4* m, r32 angle)
{
    r32 sinA = sinf(angle);
    r32 cosA = cosf(angle);

    m->X = {cosA, 0, -sinA, 0};
    m->Y = {0,    1,     0, 0};
    m->Z = {sinA, 0,  cosA, 0};
    m->W = {0,    0,     0, 1};
}

inline void matrix_rot_z(Matrix4* m, r32 angle)
{
    r32 sinA = sinf(angle);
    r32 cosA = cosf(angle);

    m->X = { cosA, sinA, 0, 0};
    m->Y = {-sinA, cosA, 0, 0};
    m->Z = {0,        0, 1, 0};
    m->W = {0,        0, 0, 1};
}

// @todo make versions which already have unit matrix values
// elsewhere?

inline void matrix_scale(Matrix4* m, Vector3 v)
{
    m->e[0][0] *= v.x;
    m->e[1][1] *= v.y;
    m->e[2][2] *= v.z;
}

inline void matrix_translate(Matrix4* m, Vector3 v)
{
    m->e[3][0] += v.x;
    m->e[3][1] += v.y;
    m->e[3][2] += v.z;
}


#endif
