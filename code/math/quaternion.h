#ifndef QUATERNION_H
#define QUATERNION_H


struct Quaternion
{
    r32 x, y, z, w;
};

Quaternion quaternion_euler(r32 x, r32 y, r32 z);
Vec3f quaternion_rotate(Vec3f v, Quaternion q);
Quaternion quaternion_mul(Quaternion q1, Quaternion q2);
Quaternion quaternion_normalize(Quaternion q);
void quaternion_to_matrix(Quaternion q, Matrix4x4f* m);

inline Quaternion quaternion_identity()
{
    return {0, 0, 0, 1};
}

// @doc requires radians, and normalized axis
inline Quaternion quaternion_from_axis(Vec3f axis, r32 angle)
{
    r32 halfAngle = angle * 0.5f;
    r32 s = sinf(halfAngle);
    return {axis.x * s,
            axis.y * s,
            axis.z * s,
            cosf(halfAngle)};
}

inline r32 quaternion_len(Quaternion q)
{
    return sqrtf(sqr(q.x) + sqr(q.y) + sqr(q.z) + sqr(q.w));
}


#endif
