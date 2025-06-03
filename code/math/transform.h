#ifndef TRANSFORM_H
#define TRANSFORM_H

struct Transform
{
    Vector3 position;
    Quaternion orientation;
    Vector3 scale;
};

void model_matrix_from_transform(Matrix4* m, Transform* t);

inline void transform_identity(Transform* t)
{
    t->position = vec_zero3();
    t->orientation = quaternion_identity();
    t->scale = vec_make(1.0f, 1.0f, 1.0f);
}
inline Transform* ttransform_identity()
{
    Transform* t = temp_alloc(Transform);
    transform_identity(t);
    return t;
}

inline Matrix4* model_tmatrix_from_transform(Transform* t)
{
    Matrix4* m = temp_alloc(Matrix4);
    model_matrix_from_transform(m, t);
    return m;
}


#endif
