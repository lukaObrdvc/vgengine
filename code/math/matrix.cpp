// MATRIX FUNCTIONS FOR r32

// @todo figure out how this works with homogeneous coordinates
Vec3f matrix_mul(Matrix4x4f* m, Vec3f v)
{
    Vec3f result;
    result.x = v.x*m->e[0][0] + v.y*m->e[1][0] + v.z*m->e[2][0] + m->e[3][0];
    result.y = v.x*m->e[0][1] + v.y*m->e[1][1] + v.z*m->e[2][1] + m->e[3][1];
    result.z = v.x*m->e[0][2] + v.y*m->e[1][2] + v.z*m->e[2][2] + m->e[3][2];
    return result;
}

void matrix_mul(Matrix4x4f* m1, Matrix4x4f* m2, Matrix4x4f* result)
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

void matrix_transpose(Matrix4x4f* m)
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


// MATRIX FUNCTIONS FOR r64

Vec3d matrix_mul(Matrix4x4d* m, Vec3d v)
{
    Vec3d result;
    result.x = v.x*m->e[0][0] + v.y*m->e[1][0] + v.z*m->e[2][0] + m->e[3][0];
    result.y = v.x*m->e[0][1] + v.y*m->e[1][1] + v.z*m->e[2][1] + m->e[3][1];
    result.z = v.x*m->e[0][2] + v.y*m->e[1][2] + v.z*m->e[2][2] + m->e[3][2];
    return result;
}

void matrix_mul(Matrix4x4d* m1, Matrix4x4d* m2, Matrix4x4d* result)
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

void matrix_transpose(Matrix4x4d* m)
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
