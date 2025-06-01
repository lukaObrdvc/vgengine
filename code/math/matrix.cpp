// MATRIX FUNCTIONS FOR r32

// @todo figure out how this works with homogeneous coordinates
Vector3 matrix_mul_vector(Matrix4* m, Vector3 v)
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
            result->e[i][j] = 0;
            for (s32 k=0; k < 4; k++)
            {
                result->e[i][j] += m1->e[i][k] * m2->e[k][j];
            }
        }
    }
}

Matrix4* tmatrix_mul(Matrix4* m1, Matrix4* m2)
{
    Matrix4* result = temp_alloc(Matrix4);
    matrix_mul(m1, m2, result);
    return result;
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

// @doc we assume multiplications go from left to right, and the first
// matrix is the the result of composition
void matrix_compose(s32 count, ...)
{
    ASSERT(count >= 3);
    
    va_list matrices;
    va_start(matrices, count);
    Scratch* scratch = get_scratch();

    // you need count-1 matrices for results, but then the last
    // result you want to write into the first argument, so you only
    // need count-2 tmp results
    Matrix4* result = va_arg(matrices, Matrix4*);
    Matrix4* tmp_results = scratch_push<Matrix4>(scratch, count - 2);

    Matrix4* first = va_arg(matrices, Matrix4*);
    Matrix4* second = va_arg(matrices, Matrix4*);
    
    if (count > 3)
    {
        // you multiply first the first two matrices, put result into scratch
        matrix_mul(first, second, &tmp_results[0]);
    
        // then you multiply the rest of them, there is count-3 left tmp_results
        for (s32 i = 0; i < count - 3; i++)
        {
            matrix_mul(&tmp_results[i], va_arg(matrices, Matrix4*), &tmp_results[i+1]);
        }
    
        matrix_mul(&tmp_results[count-2], va_arg(matrices, Matrix4*), result);
    }
    else
    {
        matrix_mul(first, second, result);
    }
    release_scratch(scratch);
    va_end(matrices);
}

Matrix4* tmatrix_compose(s32 count, ...)
{
    ASSERT(count >= 2);
    
    va_list matrices;
    va_start(matrices, count);
    Scratch* scratch = get_scratch();

    Matrix4* result = temp_alloc(Matrix4);
    Matrix4* tmp_results = scratch_push<Matrix4>(scratch, count - 1);

    Matrix4* first = va_arg(matrices, Matrix4*);
    Matrix4* second = va_arg(matrices, Matrix4*);
    
    if (count > 2)
    {
        matrix_mul(first, second, &tmp_results[0]);
    
        for (s32 i = 0; i < count - 2; i++)
        {
            matrix_mul(&tmp_results[i], va_arg(matrices, Matrix4*), &tmp_results[i+1]);
        }
    
        matrix_mul(&tmp_results[count-1], va_arg(matrices, Matrix4*), result);
    }
    else
    {
        matrix_mul(first, second, result);
    }
    release_scratch(scratch);
    va_end(matrices);

    return result;
}
