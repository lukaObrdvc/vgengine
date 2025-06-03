// @todo figure out if clocwise vs anti-clockwise?
// @doc this requires radians
Quaternion quaternion_from_euler(r32 x, r32 y, r32 z)
{
    r32 hax = x * 0.5f;
    r32 hay = y * 0.5f;
    r32 haz = z * 0.5f;
    
    r32 cx = cosf(hax);
    r32 sx = sinf(hax);
    r32 cy = cosf(hay);
    r32 sy = sinf(hay);
    r32 cz = cosf(haz);
    r32 sz = sinf(haz);

    Quaternion q;
    q.w = cx * cy * cz + sx * sy * sz;
    q.x = sx * cy * cz - cx * sy * sz;
    q.y = cx * sy * cz + sx * cy * sz;
    q.z = cx * cy * sz - sx * sy * cz;
    return q;
}

// @doc when multiplying multipple vertices by a quaternion, it's
// cheaper to convert to matrix instead and do matrix multiplication
Vector3 quaternion_rot_vector(const Vector3& v, const Quaternion& q)
{
    Vector3 qv = { q.x, q.y, q.z };
    Vector3 temp = vec_scale(vec_cross(qv, v), 2.0f);
    return vec_add(vec_add(v, vec_scale(temp, q.w)),
                   vec_cross(qv, temp));
}

// @todo variadic function for this?
// @doc applies q1 first, then q2, so not commutative
// @doc think of it like q2 is the previous rotation, and q1 is building
// upon it
Quaternion quaternion_chain(const Quaternion& q1, const Quaternion& q2)
{
    Quaternion result;
    result.w = q2.w * q1.w - q2.x * q1.x - q2.y * q1.y - q2.z * q1.z;
    result.x = q2.w * q1.x + q2.x * q1.w + q2.y * q1.z - q2.z * q1.y;
    result.y = q2.w * q1.y - q2.x * q1.z + q2.y * q1.w + q2.z * q1.x;
    result.z = q2.w * q1.z + q2.x * q1.y - q2.y * q1.x + q2.z * q1.w;
    return result;
}

Quaternion quaternion_normalize(Quaternion q)
{
    r32 len = quaternion_len(q);
    if (len == 0.0f) return {0, 0, 0, 1}; // avoid divide-by-zero
    
    r32 inv_len = 1.0f / len;
    return {q.x * inv_len,
            q.y * inv_len,
            q.z * inv_len,
            q.w * inv_len};
}

void quaternion_to_matrix(const Quaternion& q, Matrix4* m)
{
    r32 xx = q.x * q.x;
    r32 yy = q.y * q.y;
    r32 zz = q.z * q.z;
    r32 xy = q.x * q.y;
    r32 xz = q.x * q.z;
    r32 yz = q.y * q.z;
    r32 wx = q.w * q.x;
    r32 wy = q.w * q.y;
    r32 wz = q.w * q.z;

    m->e[0][0] = 1.0f - 2.0f * (yy + zz);
    m->e[0][1] = 2.0f * (xy + wz);
    m->e[0][2] = 2.0f * (xz - wy);
    m->e[0][3] = 0.0f;

    m->e[1][0] = 2.0f * (xy - wz);
    m->e[1][1] = 1.0f - 2.0f * (xx + zz);
    m->e[1][2] = 2.0f * (yz + wx);
    m->e[1][3] = 0.0f;

    m->e[2][0] = 2.0f * (xz + wy);
    m->e[2][1] = 2.0f * (yz - wx);
    m->e[2][2] = 1.0f - 2.0f * (xx + yy);
    m->e[2][3] = 0.0f;

    m->W = {0.0f, 0.0f, 0.0f, 1.0f};
}

Matrix4* quaternion_to_tmatrix(Quaternion q)
{
    Matrix4* m = temp_alloc(Matrix4);
    quaternion_to_matrix(q, m);
    return m;
}

