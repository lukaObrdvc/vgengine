void model_matrix_from_transform(Matrix4* m, Transform* t)
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

