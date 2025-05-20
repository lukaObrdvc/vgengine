#define CURRENTLY_TESTING final_giga_test
#define TEST_ONLY_ONCE ENGINE_STATE->tested_once = true

void final_giga_test()
{
    u32 f_color = color_make(0.0f, 0.0f, 0.0f, 1.0f);
    u32 b_color = color_make(1, 1.0f, 1.0f, 1.0f);
    u32 l_color = color_make(1.0f, 0.0f, 0.0f, 1.0f);
    u32 r_color = color_make(0.0f, 1.0f, 0.0f, 1.0f);
    u32 d_color = color_make(0.0f, 0.0f, 1.0f, 1.0f);
    u32 u_color = color_make(0.5f, 0.5f, 0.0f, 1.0f);

    u32 colors[6] = {
        f_color, b_color,
        l_color, r_color,
        d_color, u_color
    };
    
    Vector3 s_vertices[8] = {
        vec_make(-10.0f, -10.0f, -150.0f) , vec_make(-10.0f, 10.0f, -150.0f) ,
        vec_make(10.0f, 10.0f, -150.0f)   , vec_make(10.0f, -10.0f, -150.0f) ,
        vec_make(-10.0f, -10.0f, -170.0f) , vec_make(-10.0f, 10.0f, -170.0f) ,
        vec_make(10.0f, 10.0f, -170.0f)   , vec_make(10.0f, -10.0f, -170.0f)
    };
    u16 s_indices[36] = {
        3, 1, 0,   2, 1, 3,   4, 5, 7,   7, 5, 6,
        0, 5, 4,   1, 5, 0,   7, 6, 3,   3, 6, 2,
        7, 0, 4,   3, 0, 7,   2, 5, 1,   6, 5, 2
    };

    Camera* camera = &MAIN_CAMERA;
    Matrix4* camera_matrix = arena_push<Matrix4>(&TEMPORARY_ARENA);
    matrix_unit(camera_matrix);
    matrix_translate(camera_matrix, vec_negate(camera->position));
    Matrix4* tmp = arena_push<Matrix4>(&TEMPORARY_ARENA);
    Matrix4* tmp2 = arena_push<Matrix4>(&TEMPORARY_ARENA);
    quaternion_to_matrix(camera->orientation, tmp2);
    matrix_mul(camera_matrix, tmp2, tmp);
    swap(camera_matrix, tmp);
    
    for (s32 i = 0; i < 8; i++)
        {
            s_vertices[i] = matrix_mul(camera_matrix, s_vertices[i]);
        }

    Mesh cube;
    cube.vertices = s_vertices;
    cube.indices = s_indices;

    s32 b = 0;
    s32 j = -1;
    u32 color = f_color;
    for (s32 i = 0; i < ARR_COUNT(s_indices)/3; i++)
        {
            if (i % 2 == 0)
                {
                    j++;
                    color = colors[j];
                }
            
            u16 i0 = cube.indices[b];
            u16 i1 = cube.indices[b+1];
            u16 i2 = cube.indices[b+2];
            
            Vector3 A = cube.vertices[i0];
            Vector3 B = cube.vertices[i1];
            Vector3 C = cube.vertices[i2];

            Triangle t = {A, B, C};
            TriangleHom extraGeometry[4];
            s32 count = TriangleWorldToRasterPROJ(&t, extraGeometry);

            for (s32 k = 0; k < count; k++)
                {
                    TriangleHom tHom = extraGeometry[k];
                    Triangle tri;
                    tri.A = vec_make(tHom.A.x, tHom.A.y, tHom.A.z);
                    tri.B = vec_make(tHom.B.x, tHom.B.y, tHom.B.z);
                    tri.C = vec_make(tHom.C.x, tHom.C.y, tHom.C.z);
            
                    if (ENGINE_STATE->reverse_winding)
                        {
                            RasterizeTriangle(tri, color, false);
                        }
                    else
                        {
                            RasterizeTriangle(tri, color, false);
                        }
                }
            b += 3;
        }
}

void orbiting_camera_test()
{
    u32 color = ((u32)255 << 24) | ((u32)0 << 16) | ((u32)0 << 8) | ((u32)0);

    Vector3 A = vec_make(-50.0f, -20.0f, -120.0f); // A->C->B = clockwise winding
    Vector3 B = vec_make(60.0f, 0.0f, -150.0f);
    Vector3 C = vec_make(-70.0f, 60.0f, -200.0f);

    Vector3 A2 = vec_make(-50.0f, -20.0f, -200.0f);
    Vector3 B2 = vec_make(60.0f, 0.0f, -150.0f);
    Vector3 C2 = vec_make(-70.0f, 60.0f, -120.0f);

#define RotCamera 1
#define RotTriangles 0
#define ChangeAngle 1
#define ORBIT_OFFS 180
    
    Matrix4* WtoC = arena_push<Matrix4>(&TEMPORARY_ARENA);
    matrix_unit(WtoC);
    Vector3 orbiting_point = vec_make(0.0f, 0.0f, (r32)ORBIT_OFFS); // assuming it's in 0,0,-180 coordsys
    
    r32 angle = ENGINE_STATE->line_angle;
    r32 camera_angle = ENGINE_STATE->camera_angle;

    Matrix4* tempRotY = arena_push<Matrix4>(&TEMPORARY_ARENA);
    matrix_rotation_y(tempRotY, camera_angle);
    Matrix4* tempTrans = arena_push<Matrix4>(&TEMPORARY_ARENA);
    matrix_translate(tempTrans, vec_make(0.0f, 0.0f, (r32)-ORBIT_OFFS));
#if RotCamera
    orbiting_point = matrix_mul(tempRotY, orbiting_point);
#endif
    orbiting_point = matrix_mul(tempTrans, orbiting_point);

    Matrix4* tempRotY2 = arena_push<Matrix4>(&TEMPORARY_ARENA);
    Matrix4* tempTrans2 = arena_push<Matrix4>(&TEMPORARY_ARENA);
    matrix_translate(tempTrans2, orbiting_point);
#if RotCamera
    matrix_rotation_y(tempRotY2, camera_angle);
#else
    matrix_rotation_y(tempRotY2, 0);
#endif
    Matrix4* tempResult = arena_push<Matrix4>(&TEMPORARY_ARENA);
    matrix_mul(WtoC, tempRotY2, tempResult);
    matrix_mul(tempResult, tempTrans2, WtoC);
    
#if RotTriangles
    Matrix4* rot1 = arena_push<Matrix4>(&TEMPORARY_ARENA);
    Matrix4* rot2 = arena_push<Matrix4>(&TEMPORARY_ARENA);
    matrix_rotation_x(rot1, angle);
    matrix_rotation_x(rot2, -(angle+pi/4));

    A = matrix_mul(rot1, A);
    B = matrix_mul(rot1, B);
    C = matrix_mul(rot1, C);

    A2 = matrix_mul(rot2, A2);
    B2 = matrix_mul(rot2, B2);
    C2 = matrix_mul(rot2, C2);
#endif

    A = matrix_mul(WtoC, A);
    B = matrix_mul(WtoC, B);
    C = matrix_mul(WtoC, C);

    A2 = matrix_mul(WtoC, A2);
    B2 = matrix_mul(WtoC, B2);
    C2 = matrix_mul(WtoC, C2);

    // vertices are now in camera space
    
    Triangle t = {A, B, C};
    t = TriangleWorldToRaster(t);
    Triangle t2 = {A2, B2, C2};
    t2 = TriangleWorldToRaster(t2);

    // vertices are now in raster space
    
    // when rotating behind the triangles, they are getting culled
    // because winding is reversed, how fix this?
    if (ENGINE_STATE->reverse_winding)
        {
            RasterizeTriangle(t, color, true);
            RasterizeTriangle(t2, color, false);
        }
    else
        {
            RasterizeTriangle(t, color, true);
            RasterizeTriangle(t2, color, false);
        }
#if ChangeAngle
    ENGINE_STATE->line_angle += PI / 256;
#endif

#undef RotCamera
#undef RotTriangles
#undef ChangeAngle
#undef LogToFile
}

void none_test(void)
{
    TEST_ONLY_ONCE;
}

#define _TEST CURRENTLY_TESTING()
void test(void)
{
    if (!ENGINE_STATE->tested_once)
        {
            _TEST;
        }
}
