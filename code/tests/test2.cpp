#define CURRENTLY_TESTING model_matrix_test
#define TEST_ONLY_ONCE ENGINE_STATE->tested_once = true


struct Job_context
{
    s32* chunk;
    s32 chunk_size;
    s32 begin_index;
};
struct Job_context2
{
    s32* chunk;
    s32 chunk_size;
    s32* sum;
};

JOB(job_init_arr)
{
    Job_context* d = (Job_context*) data;
    
    for (s32 i = 0; i < d->chunk_size; i++)
    {
        d->chunk[i] = i + d->begin_index;
    }
}

JOB(job_sum_arr)
{
    Job_context2* d = (Job_context2*) data;

    s32 result = 0;
    for (s32 i = 0; i < d->chunk_size; i++)
    {
        result += d->chunk[i];
    }
    *(d->sum) = result;
}

void threading_test()
{
#define ARR_SIZE 10000

    BEGIN_PROFILING(str("without_threading"));
    s32 arr[ARR_SIZE];
    
    for (s32 i = 0; i < ARR_SIZE; i++)
    {
        arr[i] = i;
    }

    s32 sum = 0;

    for (s32 i = 0; i < ARR_SIZE; i++)
    {
        sum += arr[i];
    }
    END_PROFILING();

    BEGIN_PROFILING(str("with_threading"));
    s32 arr2[ARR_SIZE];
    s32* sums = temp_alloc(s32, NUM_WORKERS);
    // for (s32 i = 0; i < NUM_WORKERS; i++) sums[i] = 0;
    memset(sums, NUM_WORKERS * sizeof(s32));

    s32 chunk_size = ARR_SIZE / NUM_WORKERS;
    s32 last_chunk_size = chunk_size + ARR_SIZE % NUM_WORKERS;

    for (s32 i = 0; i < NUM_WORKERS; i++)
    {
        Job_context* data = temp_alloc(Job_context);
        data->chunk = arr2 + i * chunk_size;
        data->chunk_size = i == NUM_WORKERS - 1 ? last_chunk_size : chunk_size;
        data->begin_index = i * chunk_size;

        Job job;
        job.proc = job_init_arr;
        job.data = data;
        submit_job(job);
    }
    
    wait_for_all_jobs();
    
    ASSERT(memcmp(arr, arr2, (u32)(ARR_SIZE * sizeof(s32))));
    
    for (s32 i = 0; i < NUM_WORKERS; i++)
    {
        Job_context2* data = temp_alloc(Job_context2);
        data->chunk = arr2 + i * chunk_size;
        data->chunk_size = i == NUM_WORKERS - 1 ? last_chunk_size : chunk_size;
        data->sum = &sums[i];
        
        Job job;
        job.proc = job_sum_arr;
        job.data = data;
        submit_job(job);
    }
    
    wait_for_all_jobs();

    // remove this part later  ---------------
#define SUM(n) (((n))*((n)+1)/2)
    
    s32 dbg_table[7];
    dbg_table[0] = SUM(1427);
    dbg_table[1] = SUM(2855) - SUM(1427);
    dbg_table[2] = SUM(4283) - SUM(2855);
    dbg_table[3] = SUM(5711) - SUM(4283);
    dbg_table[4] = SUM(7139) - SUM(5711);
    dbg_table[5] = SUM(8567) - SUM(7139);
    dbg_table[6] = SUM(9999) - SUM(8567);
#undef SUM
    
    s32 sum2 = 0;
    for (s32 i = 0; i < NUM_WORKERS; i++)
    {
        ASSERT(sums[i] == dbg_table[i]);
        sum2 += sums[i];
    }
    END_PROFILING();

#if DEVELOPER
    Profiler* p = 0;

    for (s32 i = 0; i < NUM_PROFILERS; i++)
    {
        Profiler* prof = &PROFILERS[i];
        
        if (string_compare(str("without_threading"), prof->name))
        {
            p = prof;
            break;
        }
    }

    // should sum to 50005000-1 (does not overflow on s32)
    
    String string = concat(4, to_string(sum), str("\n"), to_string(profiler_avg_time(p), 2), str("ms"));
    Font* font = get_font(MYFONT_CONSOLAS16);
    Vector2 offset = vec_make(0.0f, 200.0f);
    draw_string(string, font, offset, -4.0f);

    Profiler* p2 = 0;

    for (s32 i = 0; i < NUM_PROFILERS; i++)
    {
        Profiler* prof = &PROFILERS[i];
        
        if (string_compare(str("with_threading"), prof->name))
        {
            p2 = prof;
            break;
        }
    }
    
    String string2 = concat(4, to_string(sum2), str("\n"), to_string(profiler_avg_time(p2), 2), str("ms"));
    Font* font2 = get_font(MYFONT_CONSOLAS16);
    Vector2 offset2 = vec_make(100.0f, 200.0f);
    draw_string(string2, font2, offset2, -4.0f);
#endif

#undef ARR_SIZE
}


void model_matrix_test(Matrix4* view, Matrix4* proj)
{
    Color f_color = color_make(0.5f, 0.0f, 0.5f, 1.0f);
    Color b_color = color_make(1.0f, 1.0f, 1.0f, 1.0f);
    Color l_color = color_make(1.0f, 0.0f, 0.0f, 1.0f);
    Color r_color = color_make(0.0f, 1.0f, 0.0f, 1.0f);
    Color d_color = color_make(0.0f, 0.0f, 1.0f, 1.0f);
    Color u_color = color_make(0.5f, 0.5f, 0.0f, 1.0f);
    
    Color colors[12] = {
        f_color, f_color,
        b_color, b_color,
        l_color, l_color,
        r_color, r_color,
        d_color, d_color,
        u_color, u_color};

    r32 a = 10.0f; // half of cube dimension
    
    Vector3 s_vertices[8] = {
        vec_make(-a, -a, -a) , vec_make(-a, a, -a) ,
        vec_make(a, a, -a)   , vec_make(a, -a, -a) ,
        vec_make(-a, -a, a)  , vec_make(-a, a, a)  ,
        vec_make(a, a, a)    , vec_make(a, -a, a)
    };
    u16 s_indices[36] = {
        3, 1, 0,   2, 1, 3,   4, 5, 7,   7, 5, 6,
        0, 5, 4,   1, 5, 0,   7, 6, 3,   3, 6, 2,
        7, 0, 4,   3, 0, 7,   2, 5, 1,   6, 5, 2
    };

    Mesh cube_mesh;
    cube_mesh.vertices = s_vertices;
    cube_mesh.indices = s_indices;
    cube_mesh.num_vertices = 8;
    cube_mesh.num_indices = 36;

    Transform* cube_transform = temp_alloc(Transform);
    
    r32 orbit_angle = ENGINE_STATE->spin_angle;
    Vector3 orbit = vec_make(0.0f, 0.0f, -180.0f);
    Quaternion orbit_rot = quaternion_from_axis(vec_up(), orbit_angle);
    orbit = quaternion_rot_vector(orbit, orbit_rot);
    cube_transform->position = orbit;
    
    Quaternion spin_rot = quaternion_from_axis(vec_up(), -orbit_angle);
    cube_transform->orientation = spin_rot;
    
    r32 scaling_factor = ENGINE_STATE->cube_scaling_factor;
    cube_transform->scale = vec_make(scaling_factor, scaling_factor, scaling_factor);

    Matrix4* mvp = mvp_tmatrix_for_transform(cube_transform, view, proj);
    
    render_mesh(cube_mesh, mvp, colors);

    cube_transform->position.x += 25;
    mvp = mvp_tmatrix_for_transform(cube_transform, view, proj);
    render_mesh(cube_mesh, mvp, colors);

    cube_transform->position.x += 25;
    mvp = mvp_tmatrix_for_transform(cube_transform, view, proj);
    render_mesh(cube_mesh, mvp, colors);
    
    cube_transform->position.x += 25;
    mvp = mvp_tmatrix_for_transform(cube_transform, view, proj);
    render_mesh(cube_mesh, mvp, colors);

    cube_transform->position.z += 25;
    mvp = mvp_tmatrix_for_transform(cube_transform, view, proj);    
    render_mesh(cube_mesh, mvp, colors);
    
    cube_transform->position.x -= 25;
    mvp = mvp_tmatrix_for_transform(cube_transform, view, proj);
    render_mesh(cube_mesh, mvp, colors);

    cube_transform->position.x -= 25;
    mvp = mvp_tmatrix_for_transform(cube_transform, view, proj);
    render_mesh(cube_mesh, mvp, colors);

    cube_transform->position.x -= 25;
    mvp = mvp_tmatrix_for_transform(cube_transform, view, proj);
    render_mesh(cube_mesh, mvp, colors);

    cube_transform->position.z += 25;
    mvp = mvp_tmatrix_for_transform(cube_transform, view, proj);
    render_mesh(cube_mesh, mvp, colors);

    cube_transform->position.x += 25;
    mvp = mvp_tmatrix_for_transform(cube_transform, view, proj);
    render_mesh(cube_mesh, mvp, colors);

    cube_transform->position.x += 25;
    mvp = mvp_tmatrix_for_transform(cube_transform, view, proj);
    render_mesh(cube_mesh, mvp, colors);

    cube_transform->position.x += 25;
    mvp = mvp_tmatrix_for_transform(cube_transform, view, proj);
    render_mesh(cube_mesh, mvp, colors);

    cube_transform->position.z += 25;
    mvp = mvp_tmatrix_for_transform(cube_transform, view, proj);
    render_mesh(cube_mesh, mvp, colors);

    cube_transform->position.x -= 25;
    mvp = mvp_tmatrix_for_transform(cube_transform, view, proj);
    render_mesh(cube_mesh, mvp, colors);
    
    cube_transform->position.x -= 25;
    mvp = mvp_tmatrix_for_transform(cube_transform, view, proj);
    render_mesh(cube_mesh, mvp, colors);
    
    cube_transform->position.x -= 25;
    mvp = mvp_tmatrix_for_transform(cube_transform, view, proj);
    render_mesh(cube_mesh, mvp, colors);
    

    // ENGINE_STATE->spin_angle += PI / KB;

    // if (ENGINE_STATE->cube_scaling_factor >= 1.8f)
    // {
    //     ENGINE_STATE->cube_scale_up = false;
    // }
    // if (ENGINE_STATE->cube_scaling_factor <= 0.2f)
    // {
    //     ENGINE_STATE->cube_scale_up = true;
    // }

    // if (ENGINE_STATE->cube_scale_up)
    // {
    //     ENGINE_STATE->cube_scaling_factor += 0.01f;
    // }
    // else
    // {
    //     ENGINE_STATE->cube_scaling_factor -= 0.01f;
    // }
}

// void final_giga_test()
// {
//     u32 f_color = color_make(0.0f, 0.0f, 0.0f, 1.0f);
//     u32 b_color = color_make(1, 1.0f, 1.0f, 1.0f);
//     u32 l_color = color_make(1.0f, 0.0f, 0.0f, 1.0f);
//     u32 r_color = color_make(0.0f, 1.0f, 0.0f, 1.0f);
//     u32 d_color = color_make(0.0f, 0.0f, 1.0f, 1.0f);
//     u32 u_color = color_make(0.5f, 0.5f, 0.0f, 1.0f);
    
//     u32 colors[6] = {
//         f_color, b_color,
//         l_color, r_color,
//         d_color, u_color
//     };
    
//     // 168 B with mesh
//     // 432 B without mesh
//     Vector3 s_vertices[8] = {
//         vec_make(-10.0f, -10.0f, -150.0f) , vec_make(-10.0f, 10.0f, -150.0f) ,
//         vec_make(10.0f, 10.0f, -150.0f)   , vec_make(10.0f, -10.0f, -150.0f) ,
//         vec_make(-10.0f, -10.0f, -170.0f) , vec_make(-10.0f, 10.0f, -170.0f) ,
//         vec_make(10.0f, 10.0f, -170.0f)   , vec_make(10.0f, -10.0f, -170.0f)
//     };
//     u16 s_indices[36] = {
//         3, 1, 0,   2, 1, 3,   4, 5, 7,   7, 5, 6,
//         0, 5, 4,   1, 5, 0,   7, 6, 3,   3, 6, 2,
//         7, 0, 4,   3, 0, 7,   2, 5, 1,   6, 5, 2
//     };
    
//     Camera* camera = &MAIN_CAMERA;
//     Matrix4* R = quaternion_to_tmatrix(quaternion_conjugate(camera->orientation));
//     Matrix4* T = tmatrix_translate(vec_negate(camera->position));
//     Matrix4* WtoC = tmatrix_mul(T, R);
    
//     for (s32 i = 0; i < 8; i++)
//     {
//         s_vertices[i] = matrix_mul_vector(WtoC, s_vertices[i]);
//     }

//     Mesh cube;
//     cube.vertices = s_vertices;
//     cube.indices = s_indices;

//     s32 b = 0;
//     s32 j = -1;
//     u32 color = f_color;
//     for (s32 i = 0; i < array_count(s_indices)/3; i++)
//     {
//         if (i % 2 == 0)
//         {
//             j++;
//             color = colors[j];
//         }
        
//         u16 i0 = cube.indices[b];
//         u16 i1 = cube.indices[b+1];
//         u16 i2 = cube.indices[b+2];
            
//         Vector3 A = cube.vertices[i0];
//         Vector3 B = cube.vertices[i1];
//         Vector3 C = cube.vertices[i2];

//         Triangle t = {A, B, C};
//         TriangleHom extraGeometry[4];
//         s32 count = TriangleWorldToRasterPROJ(&t, extraGeometry);

//         for (s32 k = 0; k < count; k++)
//         {
//             TriangleHom tHom = extraGeometry[k];
//             Triangle tri;
//             tri.A = vec_make(tHom.A.x, tHom.A.y, tHom.A.z);
//             tri.B = vec_make(tHom.B.x, tHom.B.y, tHom.B.z);
//             tri.C = vec_make(tHom.C.x, tHom.C.y, tHom.C.z);
            
//             if (ENGINE_STATE->reverse_winding)
//             {
//                 RasterizeTriangle(tri, color, false);
//             }
//             else
//             {
//                 RasterizeTriangle(tri, color, false);
//             }
//         }
//         b += 3;
//     }
// }

// void composite_rotations_test()
// {
//     u32 color = ((u32)255 << 24) | ((u32)0 << 16) | ((u32)0 << 8) | ((u32)0);
    
//     Vector3 A = vec_make(-50.0f, -20.0f, -30.0f); // A->C->B = clockwise winding
//     Vector3 B = vec_make(60.0f, 0.0f, 0.0f);
//     Vector3 C = vec_make(-70.0f, 60.0f, 50.0f);

//     Vector3 A2 = vec_make(-50.0f, -20.0f, 50.0f);
//     Vector3 B2 = vec_make(60.0f, 0.0f, 0.0f);
//     Vector3 C2 = vec_make(-70.0f, 60.0f, -30.0f);

//     Vector3 orbiting_point = vec_make(0.0f, 0.0f, 360.0f);
    
//     r32 angle = ENGINE_STATE->line_angle;
//     r32 spin_angle = ENGINE_STATE->spin_angle;
//     r32 camera_angle = ENGINE_STATE->camera_angle;

//     Quaternion camera_rot_axis = quaternion_from_axis(vec_up(), camera_angle);

//     orbiting_point = quaternion_rot_vector(orbiting_point, camera_rot_axis);
    
//     Matrix4* T = tmatrix_translate(orbiting_point);
//     camera_rot_axis = quaternion_inverse_angle(camera_rot_axis);
//     Matrix4* R = quaternion_to_tmatrix(camera_rot_axis);
    
//     Matrix4* WtoC = tmatrix_mul(T, R);

// #define RotTriangles 1
// #define OrbitTriangles 1
// #define ChangeAngle 1
    
// #if RotTriangles
//     Vector3 world_y = vec_up();
//     Quaternion tilt_rot = quaternion_from_axis(vec_right(), PI / 8);
//     Vector3 tilt_axis = quaternion_rot_vector(world_y, tilt_rot);

//     Quaternion spin_rot1 = quaternion_from_axis(tilt_axis, spin_angle);
//     Quaternion spin_rot2 = quaternion_inverse_angle(spin_rot1);
    
// #if OrbitTriangles
//     Quaternion orbit_rot = quaternion_from_axis(world_y, angle);
//     Quaternion final_rot1 = quaternion_chain(spin_rot1, orbit_rot);
//     Quaternion final_rot2 = quaternion_chain(spin_rot2, orbit_rot);
// #else
//     Quaternion final_rot1 = spin_rot1;
//     Quaternion final_rot2 = spin_rot2;
// #endif
//     Matrix4* rot1 = quaternion_to_tmatrix(final_rot1);
//     Matrix4* rot2 = quaternion_to_tmatrix(final_rot2);

//     A = matrix_mul_vector(rot1, A);
//     B = matrix_mul_vector(rot1, B);
//     C = matrix_mul_vector(rot1, C);

//     A2 = matrix_mul_vector(rot2, A2);
//     B2 = matrix_mul_vector(rot2, B2);
//     C2 = matrix_mul_vector(rot2, C2);
// #endif

//     A = matrix_mul_vector(WtoC, A);
//     B = matrix_mul_vector(WtoC, B);
//     C = matrix_mul_vector(WtoC, C);

//     A2 = matrix_mul_vector(WtoC, A2);
//     B2 = matrix_mul_vector(WtoC, B2);
//     C2 = matrix_mul_vector(WtoC, C2);

//     // vertices are now in camera space
    
//     Triangle t = {A, B, C};
//     t = TriangleWorldToRaster(t);
//     Triangle t2 = {A2, B2, C2};
//     t2 = TriangleWorldToRaster(t2);

//     // vertices are now in raster space
    
//     // when rotating behind the triangles, they are getting culled
//     // because winding is reversed, how fix this?

//     // this doesn't work like before because bro you're not
//     // passing vertices anymore, you're passing a Triangle struct
//     // which will always have the same frickin winding
//     if (ENGINE_STATE->reverse_winding)
//     {
//         RasterizeTriangle(t, color, true);
//         RasterizeTriangle(t2, color, false);
//     }
//     else
//     {
//         RasterizeTriangle(t, color, true);
//         RasterizeTriangle(t2, color, false);
//     }
// #if ChangeAngle
//     ENGINE_STATE->spin_angle += PI / 516;
//     ENGINE_STATE->line_angle += PI / kilobytes(2);
//     // ENGINE_STATE->line_angle += PI / kilobytes(2);
// #endif

// #undef RotTriangles
// #undef ChangeAngle
// }

// void quaternion_test()
// {
//     u32 color = ((u32)255 << 24) | ((u32)0 << 16) | ((u32)0 << 8) | ((u32)0);
    
//     Vector3 A = vec_make(-50.0f, -20.0f, -30.0f); // A->C->B = clockwise winding
//     Vector3 B = vec_make(60.0f, 0.0f, 0.0f);
//     Vector3 C = vec_make(-70.0f, 60.0f, 50.0f);

//     Vector3 A2 = vec_make(-50.0f, -20.0f, 50.0f);
//     Vector3 B2 = vec_make(60.0f, 0.0f, 0.0f);
//     Vector3 C2 = vec_make(-70.0f, 60.0f, -30.0f);

//     Vector3 orbiting_point = vec_make(0.0f, 0.0f, 360.0f);
    
//     r32 angle = ENGINE_STATE->line_angle;
//     r32 camera_angle = ENGINE_STATE->camera_angle;
    
//     Quaternion camera_rot_axis = quaternion_from_axis(vec_make(0.0f, 1.0f, 0.0f), camera_angle);

//     orbiting_point = quaternion_rot_vector(orbiting_point, camera_rot_axis);
    
//     Matrix4* T = tmatrix_translate(orbiting_point);
//     camera_rot_axis.w *= -1; 
//     Matrix4* R = quaternion_to_tmatrix(camera_rot_axis);
    
//     Matrix4* WtoC = tmatrix_mul(T, R);

// #define RotTriangles 1
// #define ChangeAngle 0
    
// #if RotTriangles
//     Matrix4* rot1 = tmatrix_rot_y(angle);
//     Matrix4* rot2 = tmatrix_rot_y(angle + PI/4);

//     A = matrix_mul_vector(rot1, A);
//     B = matrix_mul_vector(rot1, B);
//     C = matrix_mul_vector(rot1, C);

//     A2 = matrix_mul_vector(rot2, A2);
//     B2 = matrix_mul_vector(rot2, B2);
//     C2 = matrix_mul_vector(rot2, C2);
// #endif

//     A = matrix_mul_vector(WtoC, A);
//     B = matrix_mul_vector(WtoC, B);
//     C = matrix_mul_vector(WtoC, C);

//     A2 = matrix_mul_vector(WtoC, A2);
//     B2 = matrix_mul_vector(WtoC, B2);
//     C2 = matrix_mul_vector(WtoC, C2);

//     // vertices are now in camera space
    
//     Triangle t = {A, B, C};
//     t = TriangleWorldToRaster(t);
//     Triangle t2 = {A2, B2, C2};
//     t2 = TriangleWorldToRaster(t2);

//     // vertices are now in raster space
    
//     // when rotating behind the triangles, they are getting culled
//     // because winding is reversed, how fix this?

//     // this doesn't work like before because bro you're not
//     // passing vertices anymore, you're passing a Triangle struct
//     // which will always have the same frickin winding
//     if (ENGINE_STATE->reverse_winding)
//     {
//         RasterizeTriangle(t, color, true);
//         RasterizeTriangle(t2, color, false);
//     }
//     else
//     {
//         RasterizeTriangle(t, color, true);
//         RasterizeTriangle(t2, color, false);
//     }
// #if ChangeAngle
//     ENGINE_STATE->line_angle += PI / 256;
//     // ENGINE_STATE->line_angle += PI / kilobytes(2);
// #endif

// #undef RotTriangles
// #undef ChangeAngle
// }

// void orbiting_camera_test()
// {
//     u32 color = ((u32)255 << 24) | ((u32)0 << 16) | ((u32)0 << 8) | ((u32)0);
    
//     Vector3 A = vec_make(-50.0f, -20.0f, -30.0f); // A->C->B = clockwise winding
//     Vector3 B = vec_make(60.0f, 0.0f, 0.0f);
//     Vector3 C = vec_make(-70.0f, 60.0f, 50.0f);

//     Vector3 A2 = vec_make(-50.0f, -20.0f, 50.0f);
//     Vector3 B2 = vec_make(60.0f, 0.0f, 0.0f);
//     Vector3 C2 = vec_make(-70.0f, 60.0f, -30.0f);
    
//     Vector3 orbiting_point = vec_make(0.0f, 0.0f, 360.0f);
    
//     r32 angle = ENGINE_STATE->line_angle;
//     r32 camera_angle = ENGINE_STATE->camera_angle;
    
//     Matrix4* R = tmatrix_rot_y(camera_angle);
//     Matrix4* R2 = tmatrix_rot_y(-camera_angle);

//     orbiting_point = matrix_mul_vector(R, orbiting_point);
    
//     Matrix4* T = tmatrix_translate(orbiting_point);
    
//     // does order here make sense?
//     // Matrix4* WtoC = tmatrix_compose(2, T, R2);
//     Matrix4* WtoC = tmatrix_mul(T, R2);

// #define RotTriangles 1
// #define ChangeAngle 0
    
// #if RotTriangles
//     Matrix4* rot1 = tmatrix_rot_y(angle);
//     Matrix4* rot2 = tmatrix_rot_y(angle + PI/4);

//     A = matrix_mul_vector(rot1, A);
//     B = matrix_mul_vector(rot1, B);
//     C = matrix_mul_vector(rot1, C);

//     A2 = matrix_mul_vector(rot2, A2);
//     B2 = matrix_mul_vector(rot2, B2);
//     C2 = matrix_mul_vector(rot2, C2);
// #endif

//     A = matrix_mul_vector(WtoC, A);
//     B = matrix_mul_vector(WtoC, B);
//     C = matrix_mul_vector(WtoC, C);

//     A2 = matrix_mul_vector(WtoC, A2);
//     B2 = matrix_mul_vector(WtoC, B2);
//     C2 = matrix_mul_vector(WtoC, C2);

//     // vertices are now in camera space
    
//     Triangle t = {A, B, C};
//     t = TriangleWorldToRaster(t);
//     Triangle t2 = {A2, B2, C2};
//     t2 = TriangleWorldToRaster(t2);

//     // vertices are now in raster space
    
//     // when rotating behind the triangles, they are getting culled
//     // because winding is reversed, how fix this?

//     // this doesn't work like before because bro you're not
//     // passing vertices anymore, you're passing a Triangle struct
//     // which will always have the same frickin winding
//     if (ENGINE_STATE->reverse_winding)
//     {
//         RasterizeTriangle(t, color, true);
//         RasterizeTriangle(t2, color, false);
//     }
//     else
//     {
//         RasterizeTriangle(t, color, true);
//         RasterizeTriangle(t2, color, false);
//     }
// #if ChangeAngle
//     ENGINE_STATE->line_angle += PI / 256;
//     // ENGINE_STATE->line_angle += PI / kilobytes(2);
// #endif

// #undef RotTriangles
// #undef ChangeAngle
// }

void none_test()
{
    TEST_ONLY_ONCE;
}

#define _TEST CURRENTLY_TESTING()
void test()
{
    if (!ENGINE_STATE->tested_once)
    {
        // _TEST;
    }
}
