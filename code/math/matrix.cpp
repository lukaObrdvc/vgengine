Vector3 matrix_mul_vector(Matrix4* m, Vector3 v)
{
    Vector3 result;
    result.x = v.x*m->e[0][0] + v.y*m->e[1][0] + v.z*m->e[2][0] + m->e[3][0];
    result.y = v.x*m->e[0][1] + v.y*m->e[1][1] + v.z*m->e[2][1] + m->e[3][1];
    result.z = v.x*m->e[0][2] + v.y*m->e[1][2] + v.z*m->e[2][2] + m->e[3][2];
    return result;
}

// @doc assume input w is always 1, so you don't have to explicitly multiply by 1
Vector4 matrix_mul_vector4(Matrix4* m, const Vector4& v)
{
    Vector4 result;
    result.x = v.x*m->e[0][0] + v.y*m->e[1][0] + v.z*m->e[2][0] + m->e[3][0];
    result.y = v.x*m->e[0][1] + v.y*m->e[1][1] + v.z*m->e[2][1] + m->e[3][1];
    result.z = v.x*m->e[0][2] + v.y*m->e[1][2] + v.z*m->e[2][2] + m->e[3][2];
    result.w = v.x*m->e[0][3] + v.y*m->e[1][3] + v.z*m->e[2][3] + m->e[3][3];
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
    // need count-2 tmp results, and -1 more because first parameter is result
    Matrix4* result = va_arg(matrices, Matrix4*);
    Matrix4* tmp_results = scratch_push<Matrix4>(scratch, count - 3);

    Matrix4* first = va_arg(matrices, Matrix4*);
    Matrix4* second = va_arg(matrices, Matrix4*);
    
    if (count > 3)
    {
        // you multiply first the first two matrices, put result into scratch
        matrix_mul(first, second, &tmp_results[0]);
    
        // then you multiply the rest of them, there is count-3 left tmp_results
        // but -1 more because the last one we put into result
        for (s32 i = 0; i < count - 4; i++)
        {
            matrix_mul(&tmp_results[i], va_arg(matrices, Matrix4*), &tmp_results[i+1]);
        }

        Matrix4* next = va_arg(matrices, Matrix4*);
        matrix_mul(&tmp_results[count-4], next, result);
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
    Matrix4* tmp_results = scratch_push<Matrix4>(scratch, count - 2);

    Matrix4* first = va_arg(matrices, Matrix4*);
    Matrix4* second = va_arg(matrices, Matrix4*);
    
    if (count > 2)
    {
        matrix_mul(first, second, &tmp_results[0]);
    
        for (s32 i = 0; i < count - 3; i++)
        {
            matrix_mul(&tmp_results[i], va_arg(matrices, Matrix4*), &tmp_results[i+1]);
        }

        matrix_mul(&tmp_results[count-3], va_arg(matrices, Matrix4*), result);
    }
    else
    {
        matrix_mul(first, second, result);
    }
    release_scratch(scratch);
    va_end(matrices);

    return result;
}

void model_matrix_for_transform(Matrix4* m, Transform* t)
{
    Scratch* scratch = get_scratch();
    Matrix4* R = scratch_push<Matrix4>(scratch);
    Matrix4* T = scratch_push<Matrix4>(scratch);
    Matrix4* S = scratch_push<Matrix4>(scratch);

    quaternion_to_matrix(t->orientation, R);
    matrix_unit(T);
    matrix_translate(T, t->position);
    matrix_unit(S);
    matrix_scale(S, t->scale);
    
    matrix_compose(4, m, S, R, T);
    release_scratch(scratch);
}

void view_matrix_for_camera(Matrix4* m, Camera* c = &MAIN_CAMERA)
{
    Scratch* scratch = get_scratch();
    Matrix4* R = scratch_push<Matrix4>(scratch);
    Matrix4* T = scratch_push<Matrix4>(scratch);
    
    quaternion_to_matrix(quaternion_conjugate(c->orientation), R);
    matrix_unit(T);
    matrix_translate(T, vec_negate(c->position));
    matrix_mul(T, R, m);
    
    release_scratch(scratch);
}

void perspective_matrix_for_camera(Matrix4* proj, Camera* c = &MAIN_CAMERA)
{
    r32 n = c->z_near;
    r32 f = c->z_far;
    r32 t = tan(radians(c->fov / 2)) * n;
    r32 r = t * ASPECT_RATIO;
    
    proj->X = {n/r, 0,   0,             0};
    proj->Y = {0,   n/t, 0,             0};
    proj->Z = {0,   0,  -(f+n)/(f-n),  -1};
    proj->W = {0,   0,  -(2*f*n)/(f-n), 0};
}

void mvp_matrix_for_transform(Matrix4* m, Transform* t, Matrix4* view, Matrix4* proj)
{
    Scratch* scratch = get_scratch();
    Matrix4* model = scratch_push<Matrix4>(scratch);

    model_matrix_for_transform(model, t);

    matrix_compose(4, m, model, view, proj);
    
    release_scratch(scratch);
}

inline Matrix4* view_tmatrix_for_camera(Camera* c = &MAIN_CAMERA)
{
    Matrix4* m = temp_alloc(Matrix4);
    view_matrix_for_camera(m, c);
    return m;
}

inline Matrix4* perspective_tmatrix_for_camera(Camera* camera = &MAIN_CAMERA)
{
    Matrix4* m = temp_alloc(Matrix4);
    perspective_matrix_for_camera(m, camera);
    return m;
}
