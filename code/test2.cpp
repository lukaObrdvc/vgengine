#define CURRENTLY_TESTING model_matrix_test
#define TEST_ONLY_ONCE ENGINE_STATE->tested_once = true



void model_matrix_test(Matrix4* view, Matrix4* proj)
{
    u32 f_color = color_make(0.0f, 0.0f, 0.0f, 1.0f);
    u32 b_color = color_make(1, 1.0f, 1.0f, 1.0f);
    u32 l_color = color_make(1.0f, 0.0f, 0.0f, 1.0f);
    u32 r_color = color_make(0.0f, 1.0f, 0.0f, 1.0f);
    u32 d_color = color_make(0.0f, 0.0f, 1.0f, 1.0f);
    u32 u_color = color_make(0.5f, 0.5f, 0.0f, 1.0f);

    u32 colors[12] = {
        f_color, f_color,
        b_color, b_color,
        l_color, l_color,
        r_color, r_color,
        d_color, d_color,
        u_color, u_color
    };

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

    // Transform* cube_transform = &ENGINE_STATE->cube_transform;
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
    
    
    render_mesh(cube_mesh, mvp, colors, 8, 36);
    

    ENGINE_STATE->spin_angle += PI / KB;

    if (ENGINE_STATE->cube_scaling_factor >= 1.8f)
    {
        ENGINE_STATE->cube_scale_up = false;
    }
    if (ENGINE_STATE->cube_scaling_factor <= 0.2f)
    {
        ENGINE_STATE->cube_scale_up = true;
    }

    if (ENGINE_STATE->cube_scale_up)
    {
        ENGINE_STATE->cube_scaling_factor += 0.01f;
    }
    else
    {
        ENGINE_STATE->cube_scaling_factor -= 0.01f;
    }
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

void none_test(void)
{
    TEST_ONLY_ONCE;
}

#define _TEST CURRENTLY_TESTING()
void test(void)
{
    if (!ENGINE_STATE->tested_once)
    {
        // _TEST;
    }
}
