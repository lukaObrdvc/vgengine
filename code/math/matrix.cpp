// MATRIX FUNCTIONS FOR r32

// @todo figure out how this works with homogeneous coordinates
Vector3 matrix_mul(Matrix4* m, Vector3 v)
{
    Vector3 result;
    result.x = v.x*m->e[0][0] + v.y*m->e[1][0] + v.z*m->e[2][0] + m->e[3][0];
    result.y = v.x*m->e[0][1] + v.y*m->e[1][1] + v.z*m->e[2][1] + m->e[3][1];
    result.z = v.x*m->e[0][2] + v.y*m->e[1][2] + v.z*m->e[2][2] + m->e[3][2];
    return result;
}

void matrix_mul(Matrix4* m1, Matrix4* m2, Matrix4* result)
{
    for (s32 i=0; i < 4; i++)
    {
        for (s32 j=0; j < 4; j++)
        {
            result->e[i][j] = 0; // is this goodio?
            for (s32 k=0; k < 4; k++)
            {
                result->e[i][j] += m1->e[i][k] * m2->e[k][j];
            }
        }
    }
}

void matrix_transpose(Matrix4* m)
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


