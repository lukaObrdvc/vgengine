#define CURRENTLY_TESTING final_giga_test
#define TEST_ONLY_ONCE ENGINESTATE->tested_once = true

void final_giga_test()
{
    u32 f_color = color_make(0.0f, 0.0f, 0.0f, 1.0f);
    u32 b_color = color_make(1.0f, 1.0f, 1.0f, 1.0f);
    u32 l_color = color_make(1.0f, 0.0f, 0.0f, 1.0f);
    u32 r_color = color_make(0.0f, 1.0f, 0.0f, 1.0f);
    u32 d_color = color_make(0.0f, 0.0f, 1.0f, 1.0f);
    u32 u_color = color_make(0.5f, 0.5f, 0.0f, 1.0f);

    u32 colors[6] = {
        f_color, b_color,
        l_color, r_color,
        d_color, u_color
    };
    
    Vec3f s_vertices[8] = {
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
    Matrix4x4f* cameraMatrix = arena_push<Matrix4x4f>(&FRAME_ARENA);
    matrix_unit(cameraMatrix);
    matrix_translate(cameraMatrix, vec_negate(camera->position));
    Matrix4x4f* tmp = arena_push<Matrix4x4f>(&FRAME_ARENA);
    Matrix4x4f* tmp2 = arena_push<Matrix4x4f>(&FRAME_ARENA);
    quaternion_to_matrix(camera->orientation, tmp2);
    matrix_mul(cameraMatrix, tmp2, tmp);
    swap(cameraMatrix, tmp);
    
    for (s32 i = 0; i < 8; i++)
        {
            s_vertices[i] = matrix_mul(cameraMatrix, s_vertices[i]);
        }

    Mesh cube;
    cube.vertices = s_vertices;
    cube.indices = s_indices;

    s32 b = 0;
    s32 j = -1;
    u32 color = f_color;
    for (s32 i = 0; i < ArrCount(s_indices)/3; i++)
        {
            if (i % 2 == 0)
                {
                    j++;
                    color = colors[j];
                }
            
            u16 i0 = cube.indices[b];
            u16 i1 = cube.indices[b+1];
            u16 i2 = cube.indices[b+2];
            
            Vec3f A = cube.vertices[i0];
            Vec3f B = cube.vertices[i1];
            Vec3f C = cube.vertices[i2];

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
            
                    if (ENGINESTATE->reverse_winding)
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

    Vec3f A = vec_make(-50.0f, -20.0f, -120.0f); // A->C->B = clockwise winding
    Vec3f B = vec_make(60.0f, 0.0f, -150.0f);
    Vec3f C = vec_make(-70.0f, 60.0f, -200.0f);

    Vec3f A2 = vec_make(-50.0f, -20.0f, -200.0f);
    Vec3f B2 = vec_make(60.0f, 0.0f, -150.0f);
    Vec3f C2 = vec_make(-70.0f, 60.0f, -120.0f);

#define RotCamera 1
#define RotTriangles 0
#define ChangeAngle 1
#define ORBIT_OFFS 180
    
    Matrix4x4f* WtoC = arena_push<Matrix4x4f>(&FRAME_ARENA);
    matrix_unit(WtoC);
    Vec3f orbiting_point = vec_make(0.0f, 0.0f, (r32)ORBIT_OFFS); // assuming it's in 0,0,-180 coordsys
    
    r32 angle = ENGINESTATE->line_angle;
    r32 camera_angle = ENGINESTATE->camera_angle;

    Matrix4x4f* tempRotY = arena_push<Matrix4x4f>(&FRAME_ARENA);
    matrix_rotation_y(tempRotY, camera_angle);
    Matrix4x4f* tempTrans = arena_push<Matrix4x4f>(&FRAME_ARENA);
    matrix_translate(tempTrans, vec_make(0.0f, 0.0f, (r32)-ORBIT_OFFS));
#if RotCamera
    orbiting_point = matrix_mul(tempRotY, orbiting_point);
#endif
    orbiting_point = matrix_mul(tempTrans, orbiting_point);

    Matrix4x4f* tempRotY2 = arena_push<Matrix4x4f>(&FRAME_ARENA);
    Matrix4x4f* tempTrans2 = arena_push<Matrix4x4f>(&FRAME_ARENA);
    matrix_translate(tempTrans2, orbiting_point);
#if RotCamera
    matrix_rotation_y(tempRotY2, camera_angle);
#else
    matrix_rotation_y(tempRotY2, 0);
#endif
    Matrix4x4f* tempResult = arena_push<Matrix4x4f>(&FRAME_ARENA);
    matrix_mul(WtoC, tempRotY2, tempResult);
    matrix_mul(tempResult, tempTrans2, WtoC);
    
#if RotTriangles
    Matrix4x4f* rot1 = arena_push<Matrix4x4f>(&FRAME_ARENA);
    Matrix4x4f* rot2 = arena_push<Matrix4x4f>(&FRAME_ARENA);
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
    if (ENGINESTATE->reverse_winding)
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
    ENGINESTATE->line_angle += pi / 256;
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
    if (!ENGINESTATE->tested_once)
        {
            _TEST;
        }
}
