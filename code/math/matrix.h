#ifndef MATRIX_H
#define MATRIX_H


// @doc matrices are row-major
union Matrix4
{
    struct {Vector4 X, Y, Z, W;};
    Vector4 row[4];
    r32 e[4][4];
    r32 i[16];
};

struct Transform;
struct Camera;

Vector3 matrix_mul_vector(Matrix4* m, Vector3 v);
Vector4 matrix_mul_vector4(Matrix4* m, const Vector4& v);
void matrix_mul(Matrix4* m1, Matrix4* m2, Matrix4* result);
void matrix_transpose(Matrix4* m);
void matrix_compose(s32 count, ...);
void model_matrix_for_transform(Matrix4* m, Transform* t);
void view_matrix_for_camera(Matrix4* m, Camera* c);
void perspective_matrix_for_camera(Matrix4* proj, Camera* camera);
void mvp_matrix_for_transform(Matrix4* m, Transform* t, Matrix4* view, Matrix4* proj);

Matrix4* tmatrix_compose(s32 count, ...);


inline void matrix_unit(Matrix4* m)
{
    m->X = {1, 0, 0, 0};
    m->Y = {0, 1, 0, 0};
    m->Z = {0, 0, 1, 0};
    m->W = {0, 0, 0, 1};
}

inline void matrix_make(Matrix4* m, const Vector4& X, const Vector4& Y, const Vector4& Z, const Vector4& W)
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


// tmatrix functions

inline Matrix4* tmatrix_unit()
{
    Matrix4* m = temp_alloc(Matrix4);
    matrix_unit(m);
    return m;
}

inline Matrix4* tmatrix_make(const Vector4& X, const Vector4& Y, const Vector4& Z, const Vector4& W)
{
    Matrix4* m = temp_alloc(Matrix4);
    matrix_make(m, X, Y, Z, W);
    return m;
}

inline Matrix4* tmatrix_mul(Matrix4* m1, Matrix4* m2)
{
    Matrix4* result = temp_alloc(Matrix4);
    matrix_mul(m1, m2, result);
    return result;
}

inline Matrix4* tmatrix_rot_x(r32 angle)
{
    Matrix4* m = temp_alloc(Matrix4);
    matrix_rot_x(m, angle);
    return m;
}

inline Matrix4* tmatrix_rot_y(r32 angle)
{
    Matrix4* m = temp_alloc(Matrix4);
    matrix_rot_y(m, angle);
    return m;
}

inline Matrix4* tmatrix_rot_z(r32 angle)
{
    Matrix4* m = temp_alloc(Matrix4);
    matrix_rot_z(m, angle);
    return m;
}

inline Matrix4* tmatrix_scale(Vector3 v)
{
    Matrix4* m = tmatrix_unit();
    matrix_scale(m, v);
    return m;
}

inline Matrix4* tmatrix_translate(Vector3 v)
{
    Matrix4* m = tmatrix_unit();
    matrix_translate(m, v);
    return m;
}

inline Matrix4* model_tmatrix_from_transform(Transform* t)
{
    Matrix4* m = temp_alloc(Matrix4);
    model_matrix_for_transform(m, t);
    return m;
}

inline Matrix4* mvp_tmatrix_for_transform(Transform* t, Matrix4* view, Matrix4* proj)
{
    Matrix4* m = temp_alloc(Matrix4);
    mvp_matrix_for_transform(m, t, view, proj);
    return m;
}


#endif
