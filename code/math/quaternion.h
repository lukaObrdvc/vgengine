#ifndef QUATERNION_H
#define QUATERNION_H


struct Quaternion
{
    r32 x, y, z, w;
};

Quaternion quaternion_euler(r32 x, r32 y, r32 z);
Vector3 quaternion_rotate(Vector3 v, Quaternion q);
Quaternion quaternion_mul(Quaternion q1, Quaternion q2);
Quaternion quaternion_normalize(Quaternion q);
void quaternion_to_matrix(Quaternion q, Matrix4* m);

inline Quaternion quaternion_identity()
{
    return {0, 0, 0, 1};
}

// @doc requires radians, and normalized axis
inline Quaternion quaternion_from_axis(Vector3 axis, r32 angle)
{
    r32 half_angle = angle * 0.5f;
    r32 s = sinf(half_angle);
    return {axis.x * s,
            axis.y * s,
            axis.z * s,
            cosf(half_angle)};
}

inline r32 quaternion_len(Quaternion q)
{
    return sqrtf(sqr(q.x) + sqr(q.y) + sqr(q.z) + sqr(q.w));
}


#endif
