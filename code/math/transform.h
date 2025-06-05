#ifndef TRANSFORM_H
#define TRANSFORM_H

struct Transform
{
    Vector3 position;
    Quaternion orientation;
    Vector3 scale;
};

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


#endif
