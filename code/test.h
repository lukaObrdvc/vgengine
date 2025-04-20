#define CURRENTLY_TESTING mesh_clipping_test
#define TEST_ONLY_ONCE Gamestate->tested_once = true;

// you need to laod a mesh from disk into memory
// then you need to draw a mesh
// you then need to do clipping of a mesh
// store extra geometry in a fixed size storage while doing clipping
// tesselate that geometry and put it in a new mesh (fixed size storage..)
// draw extra geometry

void mesh_clipping_test(void)
{
    // @TODO am I doing the camera transform correctly orwhat, it
    // kind of stretches on the screen.........
    
    
    fill_background();

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
    
    Mesh cube;
    v3* vertices;
    u16* indices;

    v3 s_vertices[8] = {
        V3(-10, -10, -150) , V3(-10, 10, -150) , // 0 1
        V3(10, 10, -150)   , V3(10, -10, -150) , // 2 3
        V3(-10, -10, -170) , V3(-10, 10, -170) , // 4 5
        V3(10, 10, -170)   , V3(10, -10, -170)   // 6 7
    };
    u16 s_indices[36] = {
        3, 1, 0,   2, 1, 3,   4, 5, 7,   7, 5, 6,
        0, 5, 4,   1, 5, 0,   7, 6, 3,   3, 6, 2,
        7, 0, 4,   3, 0, 7,   2, 5, 1,   6, 5, 2
    };

#define RotateVertices2 1
#define ChangeVerticeAngles2 1

    m4 WtoC = M4Unit();
    
    r32 angle = Gamestate->line_angle;
    r32 camera_offs_x = Gamestate->camera_offs_x;
    r32 camera_offs_y = Gamestate->camera_offs_y;
    
    v3 camera_point = M4Mul(V3(0,0,0), M4Tlate(V3(camera_offs_x,
                                                  camera_offs_y,
                                                  0)));
    m4 T = M4Tlate(camera_point);
    WtoC = M4Compose(2, WtoC, T);
    
#if RotateVertices2
    for (int i = 0; i < 8; i++)
        {
            s_vertices[i] = rotate3(s_vertices[i], angle, 0, 0);
        }
#endif

    for (int i = 0; i < 8; i++)
        {
            s_vertices[i] = M4Mul(s_vertices[i], WtoC);
        }
    
    vertices = s_vertices;
    indices = s_indices;
    
    cube.vertices = vertices;
    cube.indices = indices;
    
    *Assets = cube;

    Mesh* mesh = Assets;

    int b = 0;
    int j = -1;
    u32 color = f_color;
    for (int i = 0; i < ArrCount(s_indices)/3; i++)
        {
            if (i % 2 == 0)
                {
                    j++;
                    color = colors[j];
                }
            
            u16 i0 = mesh->indices[b];
            u16 i1 = mesh->indices[b+1];
            u16 i2 = mesh->indices[b+2];
            
            v3 A = mesh->vertices[i0];
            v3 B = mesh->vertices[i1];
            v3 C = mesh->vertices[i2];
            
            triangle t = {A, B, C};
            t = TriangleWorldToRaster(t);
            
            if (Gamestate->reverse_winding)
                {
                    RasterizeTriangle(t.A, t.B, t.C, color, false);
                }
            else
                {
                    RasterizeTriangle(t.C, t.B, t.A, color, false);
                }
            b += 3;
        }

    for (s32 i = 0; i < wnd_height; i++)
        {
            r32* row = (r32*)(zbuffer + i*wnd_pitch);
            for (s32 k = 0; k < wnd_width; k++)
                {
                    *row = Gamestate->cameraParams._far-500;
                    row++;
                }
        }
    
#if ChangeVerticeAngles2
    Gamestate->line_angle += PI / 256;
#endif
    
#undef RotateVertices2
#undef hangeVerticeAngles2
}


void basic_mesh_test(void)
{
    // @TODO ... meshes
    // write file that represents a mesh
    // read file that represents a mesh
    // push contents into the end of temp_mem, and get a handle back
    // now draw a mesh based on handle

    // actually we will skip all the file operations for now, just
    // hardcode it for now

    fill_background();

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
    
    Mesh cube;
    v3* vertices;
    u16* indices;
    
    /*
      front:  back:
      1 2     5 6
      0 3     4 7
      
      left:   right:
      5 1     6 2
      4 0     7 3
      
      down:   up:
      0 3     5 6
      4 7     1 2   */
    
    v3 s_vertices[8] = {
        V3(-10, -10, -150) , V3(-10, 10, -150) , // 0 1
        V3(10, 10, -150)   , V3(10, -10, -150) , // 2 3
        V3(-10, -10, -170) , V3(-10, 10, -170) , // 4 5
        V3(10, 10, -170)   , V3(10, -10, -170)   // 6 7
    };
    u16 s_indices[36] = {
        3, 1, 0,   2, 1, 3,   4, 5, 7,   7, 5, 6,
        0, 5, 4,   1, 5, 0,   7, 6, 3,   3, 6, 2,
        7, 0, 4,   3, 0, 7,   2, 5, 1,   6, 5, 2
    };

#define RotateVertices 1
#define ChangeVerticeAngles 1
#define ORBIT_OFFS 180

    m4 WtoC = M4Unit();
    v3 orbiting_point = V3(0, 0, ORBIT_OFFS);
    
    r32 angle = Gamestate->line_angle;
    r32 camera_angle = Gamestate->camera_angle;
    
    orbiting_point = M4Mul(orbiting_point, M4RotY(camera_angle));
    orbiting_point = M4Mul(orbiting_point, M4Tlate(V3(0, 0, -ORBIT_OFFS)));
    m4 T = M4Tlate(orbiting_point);
    m4 R = M4RotY(camera_angle);
    WtoC = M4Compose(3, WtoC, R, T);
    
#if RotateVertices
    for (int i = 0; i < 8; i++)
        {
            s_vertices[i] = rotate3(s_vertices[i], angle, 0, 0);
        }
#endif

    for (int i = 0; i < 8; i++)
        {
            s_vertices[i] = M4Mul(s_vertices[i], WtoC);
        }

    vertices = s_vertices;
    indices = s_indices;
    
    cube.vertices = vertices;
    cube.indices = indices;
    
    *Assets = cube;

    Mesh* mesh = Assets;

    int b = 0;
    int j = -1;
    u32 color = f_color;
    for (int i = 0; i < ArrCount(s_indices)/3; i++)
        {
            if (i % 2 == 0)
                {
                    j++;
                    color = colors[j];
                }
            
            u16 i0 = mesh->indices[b];
            u16 i1 = mesh->indices[b+1];
            u16 i2 = mesh->indices[b+2];
            
            v3 A = mesh->vertices[i0];
            v3 B = mesh->vertices[i1];
            v3 C = mesh->vertices[i2];
            
            triangle t = {A, B, C};
            t = TriangleWorldToRaster(t);
            
            if (Gamestate->reverse_winding)
                {
                    RasterizeTriangle(t.A, t.B, t.C, color, false);
                }
            else
                {
                    RasterizeTriangle(t.C, t.B, t.A, color, false);
                }
            b += 3;
        }
    
    for (s32 i = 0; i < wnd_height; i++)
        {
            r32* row = (r32*)(zbuffer + i*wnd_pitch);
            for (s32 k = 0; k < wnd_width; k++)
                {
                    *row = Gamestate->cameraParams._far-500;
                    row++;
                }
        }

#if ChangeVerticeAngles
    Gamestate->line_angle += PI / 256;
#endif

#undef RotateVertices
#undef ChangeVerticeAngles
#undef ORBIT_OFFS
}

void orbiting_camera_test(void)
{
    fill_background();
    u32 color = Gamestate->brushes[BRUSH_SCANLINE];

    v3 A = V3(-50, -20, -120); // A->C->B = clockwise winding
    v3 B = V3(60, 0, -150);
    v3 C = V3(-70, 60, -200);

    v3 A2 = V3(-50, -20, -200);
    v3 B2 = V3(60, 0, -150);
    v3 C2 = V3(-70, 60, -120);

#define RotCamera 1
#define RotTriangles 0
#define ChangeAngle 1
#define LogToFile 0
#define ORBIT_OFFS 180
    
    m4 WtoC = M4Unit();
    v3 orbiting_point = V3(0, 0, ORBIT_OFFS); // assuming it's in 0,0,-180 coordsys
    
    r32 angle = Gamestate->line_angle;
    r32 camera_angle = Gamestate->camera_angle;
    
#if RotCamera
    orbiting_point = M4Mul(orbiting_point, M4RotY(camera_angle));
#endif
    orbiting_point = M4Mul(orbiting_point, M4Tlate(V3(0, 0, -ORBIT_OFFS)));
    m4 T = M4Tlate(orbiting_point);
#if RotCamera
    m4 R = M4RotY(camera_angle);
#else
    m4 R = M4RotY(0);
#endif
    WtoC = M4Compose(3, WtoC, R, T);
    
#if RotTriangles
    A = rotate3(A, angle, 0, 0);
    B = rotate3(B, angle, 0, 0);
    C = rotate3(C, angle, 0, 0);

    A2 = rotate3(A2, -(angle+PI/4), 0, 0);
    B2 = rotate3(B2, -(angle+PI/4), 0, 0);
    C2 = rotate3(C2, -(angle+PI/4), 0, 0);
#endif

    /* WtoC.m[3][0] = - WtoC.m[3][0]; */
#if LogToFile    
    char debug_str[1024];

    MatrixToStr(debug_str, WtoC);
    
    b32 err = DBG_WRITE_FILE("W:\\Projects\\vgengine\\data\\logs\\log_camera.txt",
                             debug_str, 256);
#endif    
    A = M4Mul(A, WtoC);
    B = M4Mul(B, WtoC);
    C = M4Mul(C, WtoC);

    A2 = M4Mul(A2, WtoC);
    B2 = M4Mul(B2, WtoC);
    C2 = M4Mul(C2, WtoC);
    
    triangle t = {A, B, C};
    t = TriangleWorldToRaster(t);
    triangle t2 = {A2, B2, C2};
    t2 = TriangleWorldToRaster(t2);

    // when rotating behind the triangles, they are getting culled
    // because winding is reversed, how fix this?
    if (Gamestate->reverse_winding)
        {
            dbg_print("reverse winding\n");
            RasterizeTriangle(t.B, t.C, t.A, color, true);
            RasterizeTriangle(t2.B, t2.C, t2.A, color, false);
        }
    else
        {
            RasterizeTriangle(t.A, t.C, t.B, color, true);
            RasterizeTriangle(t2.A, t2.C, t2.B, color, false);
        }

    for (s32 i = 0; i < wnd_height; i++)
        {
            r32* row = (r32*)(zbuffer + i*wnd_pitch);
            for (s32 j = 0; j < wnd_width; j++)
                {
                    *row = Gamestate->cameraParams._far-500;
                    row++;
                }
        }
#if ChangeAngle
    Gamestate->line_angle += PI / 256;
#endif

    Gamestate->log_to_file_once = false;
    
#undef RotCamera
#undef RotTriangles
#undef ChangeAngle
#undef LogToFile
}

void zb_triangle_test(void)
{
    fill_background();
    u32 color = Gamestate->brushes[BRUSH_SCANLINE];

    v3 A = V3(-50, -10, -160); // A->C->B = clockwise winding
    v3 B = V3(60, 10, -200);
    v3 C = V3(-70, 70, -160);

    v3 A2 = A;
    v3 B2 = B;
    v3 C2 = C;

    A2.z = -200;
    C2.z = -120;

#if 1
    r32 angle = Gamestate->line_angle;
    A = rotate3(A, angle, 0, 0);
    B = rotate3(B, angle, 0, 0);
    C = rotate3(C, angle, 0, 0);

    A2 = rotate3(A2, -(angle+PI/4), 0, 0);
    B2 = rotate3(B2, -(angle+PI/4), 0, 0);
    C2 = rotate3(C2, -(angle+PI/4), 0, 0);
#endif

    // basically just do the camera transform here..
    
    triangle t = {A, B, C};
    t = TriangleWorldToRaster(t);
    triangle t2 = {A2, B2, C2};
    t2 = TriangleWorldToRaster(t2);
        
    RasterizeTriangle(t.A, t.C, t.B, color, true);
    RasterizeTriangle(t2.A, t2.C, t2.B, color, false);

    for (s32 i = 0; i < wnd_height; i++)
        {
            r32* row = (r32*)(zbuffer + i*wnd_pitch);
            for (s32 j = 0; j < wnd_width; j++)
                {
                    *row = Gamestate->cameraParams._far;
                    row++;
                }
        }
#if 1
    Gamestate->line_angle += PI / 256;
#endif
}


void rasterize_triangle_test(void)
{
    // not sure about sign of Z throughout, and stuff..

    fill_background();
    u32 color = Gamestate->brushes[BRUSH_SCANLINE];

    r32 Near = Gamestate->cameraParams._near;
    v3 triangle_base = V3(wnd_width/2.0,
                          wnd_height/2.0,
                          Near-10);

    // what about explicitly figuring out winding?
    v3 A = V3(-50, -10, -160); // A->C->B = clockwise winding
    v3 B = V3(60, 10, -160);
    v3 C = V3(-70, 70, -160);

    r32 angle = Gamestate->line_angle;
    A = rotate3(A, angle, 0, 0);
    B = rotate3(B, angle, 0, 0);
    C = rotate3(C, angle, 0, 0);
    
    /* A = add3(A, triangle_base); */
    /* B = add3(B, triangle_base); */
    /* C = add3(C, triangle_base); */

    r32 invA = Near/A.z;
    r32 invB = Near/B.z;
    r32 invC = Near/C.z;
    v3 A_s = V3(A.x*invA, A.y*invA, A.z);
    v3 B_s = V3(B.x*invB, B.y*invB, B.z);
    v3 C_s = V3(C.x*invC, C.y*invC, C.z);
    
    r32 Fov = Gamestate->cameraParams.fov;
    r32 aspect_ratio = (wnd_width*1.0f)/wnd_height;

    r32 r = -Near*tan(to_rad(Fov/2));
    r32 l = -r;
    r32 t = r/aspect_ratio;
    r32 b = -t;

    v3 A_ndc = V3(A_s.x/r, A_s.y/t, A_s.z);
    v3 B_ndc = V3(B_s.x/r, B_s.y/t, B_s.z);
    v3 C_ndc = V3(C_s.x/r, C_s.y/t, C_s.z);

    /* v3 A_r = V3((A_ndc.x+1)/2*(wnd_width-1), (1-(A_ndc.y+1)/2)*(wnd_height-1), A_ndc.z); */
    /* v3 B_r = V3((B_ndc.x+1)/2*(wnd_width-1), (1-(B_ndc.y+1)/2)*(wnd_height-1), B_ndc.z); */
    /* v3 C_r = V3((C_ndc.x+1)/2*(wnd_width-1), (1-(C_ndc.y+1)/2)*(wnd_height-1), C_ndc.z); */

    v3 A_r = V3((A_ndc.x+1)/2*(wnd_width-1), (A_ndc.y+1)/2*(wnd_height-1), A_ndc.z);
    v3 B_r = V3((B_ndc.x+1)/2*(wnd_width-1), (B_ndc.y+1)/2*(wnd_height-1), B_ndc.z);
    v3 C_r = V3((C_ndc.x+1)/2*(wnd_width-1), (C_ndc.y+1)/2*(wnd_height-1), C_ndc.z);

    RasterizeTriangle(A_r, C_r, B_r, color, true);
    Gamestate->line_angle += PI / 256;

    for (s32 i = 0; i < wnd_height; i++)
        {
            r32* row = (r32*)(zbuffer + i*wnd_pitch);
            for (s32 j = 0; j < wnd_width; j++)
                {
                    *row = Gamestate->cameraParams._far;
                    row++;
                }
        }    
}


// @IMPORTANT we are not nearclipping, so the back of the world is
// projected upside down onto the screen
void clipping_and_camera_test(void)
{
    fill_background();

    u32 color = Gamestate->brushes[BRUSH_SCANLINE];

    v3 screen_center = V3(Gamestate->wnd_center_x,
                          Gamestate->wnd_center_y,
                          0);
    
    v3 A = V3(-50, -10, -20);
    v3 B = V3(60, 10, -20);
    v3 C = V3(-70, 70, -20);

    /* v3 A = V3(-5000, -1000, -50); */
    /* v3 B = V3(6000, 1000, -50); */
    /* v3 C = V3(-7000, 7000, -50); */

    A = add3(A, screen_center);
    B = add3(B, screen_center);
    C = add3(C, screen_center);

    // I believe this is the problem here
    // and I'm processing my input correctly....


    // YOU ARE SIMPLY NOT DOING ROTATION PROPERLY
    // AND MAKING AN INVERSE ROTATION YOU HAVE TO
    // TRANSPOSE INSTEAD.....
    v3 pA = project_new2(A, PERSPECTIVE);
    v3 pB = project_new2(B, PERSPECTIVE);
    v3 pC = project_new2(C, PERSPECTIVE);

    v2 pA2 = V2(pA.x, pA.y);
    v2 pB2 = V2(pB.x, pB.y);
    v2 pC2 = V2(pC.x, pC.y);
        
    line l1 = {.P=pA2, .Q=pB2};
    line l2 = {.P=pB2, .Q=pC2};
    line l3 = {.P=pC2, .Q=pA2};

    if (clip_line(&l1))
        {
            draw_wndline_zbuffered_aa(l1.P, l1.Q, pA.z, color);
        }
    if (clip_line(&l2))
        {
            draw_wndline_zbuffered_aa(l2.P, l2.Q, pA.z, color);
        }
    if (clip_line(&l3))
        {
            draw_wndline_zbuffered_aa(l3.P, l3.Q, pA.z, color);
        }

    
    
    for (s32 i = 0; i < wnd_height; i++)
        {
            r32* row = (r32*)(zbuffer + i*wnd_pitch);
            for (s32 j = 0; j < wnd_width; j++)
                {
                    *row = Gamestate->farclip;
                    row++;
                }
        }

}

void clipping_test(void)
{
    fill_background();

    u32 color = Gamestate->brushes[BRUSH_SCANLINE];

    v3 screen_center = V3(Gamestate->wnd_center_x,
                          Gamestate->wnd_center_y,
                          Gamestate->screen_z);
    
    v3 A = V3(-50, -10, 5); // was -15 on z
    v3 B = V3(60, 10, 5);
    v3 C = V3(-70, 70, 5);

    A = add3(A, screen_center);
    B = add3(B, screen_center);
    C = add3(C, screen_center);
    
    v2 pA = project(A, PERSPECTIVE);
    v2 pB = project(B, PERSPECTIVE);
    v2 pC = project(C, PERSPECTIVE);

    line l1 = {.P=pA, .Q=pB};
    line l2 = {.P=pB, .Q=pC};
    line l3 = {.P=pC, .Q=pA};

    if (clip_line(&l1))
        {
            draw_wndline_aa(l1.P, l1.Q, color);
        }
    if (clip_line(&l2))
        {
            draw_wndline_aa(l2.P, l2.Q, color);
        }
    if (clip_line(&l3))
        {
            draw_wndline_aa(l3.P, l3.Q, color);
        }
    
}

void moving_camera_test(void)
{
    fill_background();
    
    v3 screen_center = V3(Gamestate->wnd_center_x,
                          Gamestate->wnd_center_y,
                          Gamestate->screen_z);

    v3 A = V3(-30,  30, 5);
    v3 B = V3( 30,  30, 5);
    v3 C = V3( 30, -30, 5);
    v3 D = V3(-30, -30, 5);

    A = add3(A, screen_center);
    B = add3(B, screen_center);
    C = add3(C, screen_center);
    D = add3(D, screen_center);

    v2 pA = project(A, PERSPECTIVE);
    v2 pB = project(B, PERSPECTIVE);
    v2 pC = project(C, PERSPECTIVE);
    v2 pD = project(D, PERSPECTIVE);

    /* pA = clamp2(pA); */
    /* pB = clamp2(pB); */
    /* pC = clamp2(pC); */
    /* pD = clamp2(pD); */

    v2 tmp = pA;
    pA = clamp_line(pA, pB);
    pB = clamp_line(pB, pC);
    pC = clamp_line(pC, pD);
    pD = clamp_line(pD, tmp);
    
    u32 color = Gamestate->brushes[BRUSH_SCANLINE];
    draw_wndline_aa(pA, pB, color);
    draw_wndline_aa(pB, pC, color);
    draw_wndline_aa(pC, pD, color);
    draw_wndline_aa(pD, pA, color);
    
}



void zbuffering_triangles_test(void)
{

    fill_background();
    u32 color1 = Gamestate->brushes[BRUSH_SCANLINE];
    u32 color2 = Gamestate->brushes[BRUSH_SCANLINE2];

    v3 screen_center = V3(Gamestate->wnd_center_x,
                          Gamestate->wnd_center_y,
                          Gamestate->screen_z);
    
    v3 A = V3(-50, -10, 7);
    v3 B = V3(60, 10, 50);
    v3 C = V3(-70, 70, 15);

    v3 I = V3(-80, 30, 11);
    v3 J = V3(80, -30, 3);
    v3 K = V3(80, 30, 11);

    A.z += 10;
    B.z += 10;
    C.z += 10;

    I.z += 10;
    J.z += 10;
    K.z += 10;
    
    A = add3(A, screen_center);
    B = add3(B, screen_center);
    C = add3(C, screen_center);

    I = add3(I, screen_center);
    J = add3(J, screen_center);
    K = add3(K, screen_center);

    v2 pA = project(A, PERSPECTIVE);
    v2 pB = project(B, PERSPECTIVE);
    v2 pC = project(C, PERSPECTIVE);

    v2 pI = project(I, PERSPECTIVE);
    v2 pJ = project(J, PERSPECTIVE);
    v2 pK = project(K, PERSPECTIVE);

    r32 kAB = (pA.y - pB.y) / (pA.x - pB.x);
    r32 kBC = (pB.y - pC.y) / (pB.x - pC.x);
    r32 kCA = (pC.y - pA.y) / (pC.x - pA.x);

    r32 kIJ = (pI.y - pJ.y) / (pI.x - pJ.x);
    r32 kJK = (pJ.y - pK.y) / (pJ.x - pK.x);
    r32 kKI = (pK.y - pI.y) / (pK.x - pI.x);

    wndrect outline1 = Wndrect(pA.x, pA.y, pA.x, pA.y);
    wndrect outline2 = Wndrect(pI.x, pI.y, pI.x, pI.y);

    if (pA.x < outline1.left)   outline1.left = pA.x;
    if (pA.y < outline1.bottom) outline1.bottom = pA.y;
    if (pA.x > outline1.right)  outline1.right = pA.x;
    if (pA.y > outline1.top)    outline1.top = pA.y;

    if (pB.x < outline1.left)   outline1.left = pB.x;
    if (pB.y < outline1.bottom) outline1.bottom = pB.y;
    if (pB.x > outline1.right)  outline1.right = pB.x;
    if (pB.y > outline1.top)    outline1.top = pB.y;

    if (pC.x < outline1.left)   outline1.left = pC.x;
    if (pC.y < outline1.bottom) outline1.bottom = pC.y;
    if (pC.x > outline1.right)  outline1.right = pC.x;
    if (pC.y > outline1.top)    outline1.top = pC.y;

    if (pI.x < outline2.left)   outline2.left = pI.x;
    if (pI.y < outline2.bottom) outline2.bottom = pI.y;
    if (pI.x > outline2.right)  outline2.right = pI.x;
    if (pI.y > outline2.top)    outline2.top = pI.y;

    if (pJ.x < outline2.left)   outline2.left = pJ.x;
    if (pJ.y < outline2.bottom) outline2.bottom = pJ.y;
    if (pJ.x > outline2.right)  outline2.right = pJ.x;
    if (pJ.y > outline2.top)    outline2.top = pJ.y;

    if (pK.x < outline2.left)   outline2.left = pK.x;
    if (pK.y < outline2.bottom) outline2.bottom = pK.y;
    if (pK.x > outline2.right)  outline2.right = pK.x;
    if (pK.y > outline2.top)    outline2.top = pK.y;

    b32 two_below1 = true;
    b32 two_below2 = true;
    if (pA.y == outline1.bottom)
        {
            v2 tmp = pA;
            pA = V2(pB.x, pB.y);  // can you do pA = pB ??
            pB = V2(tmp.x, tmp.y);

            r32 tmp2 = kCA;
            kCA = kBC;
            kBC = tmp2;
            
            if (pB.x == outline1.right || pB.x == outline1.left)
                {
                    two_below1 = false;
                }
        }
    else if (pB.y == outline1.bottom)
        {            
            if (pB.x == outline1.right || pB.x == outline1.left)
                {
                    two_below1 = false;
                }
        }
    else if (pC.y == outline1.bottom)
        {
            v2 tmp = pC;
            pC = V2(pB.x, pB.y);
            pB = V2(tmp.x, tmp.y);

            r32 tmp2 = kAB;
            kAB = kCA;
            kCA = tmp2;
            
            if (pB.x == outline1.right || pB.x == outline1.left)
                {
                    two_below1 = false;
                }
        }
    // ------------------------------------------------------------
    if (pI.y == outline2.bottom)
        {
            v2 tmp = pI;
            pI = V2(pJ.x, pJ.y);  // can you do pI = pJ ??
            pJ = V2(tmp.x, tmp.y);

            r32 tmp2 = kCA;
            kCA = kBC;
            kBC = tmp2;
            
            if (pJ.x == outline2.right || pJ.x == outline2.left)
                {
                    two_below2 = false;
                }
        }
    else if (pJ.y == outline2.bottom)
        {            
            if (pJ.x == outline2.right || pJ.x == outline2.left)
                {
                    two_below2 = false;
                }
        }
    else if (pK.y == outline2.bottom)
        {
            v2 tmp = pK;
            pK = V2(pJ.x, pJ.y);
            pJ = V2(tmp.x, tmp.y);

            r32 tmp2 = kAB;
            kAB = kCA;
            kCA = tmp2;
            
            if (pJ.x == outline2.right || pJ.x == outline2.left)
                {
                    two_below2 = false;
                }
        }
    
    s32 offset1 = wnd_pitch*round32(outline1.bottom) +
        round32(outline1.left)*wnd_bytpp;
    s32 height1 = round32(wndrect_height(outline1));
    s32 width1 = round32(wndrect_width(outline1));

    s32 offset2 = wnd_pitch*round32(outline2.bottom) +
        round32(outline2.left)*wnd_bytpp;
    s32 height2 = round32(wndrect_height(outline2));
    s32 width2 = round32(wndrect_width(outline2));

    v3 a1 = V3(A.x, A.y, A.z);
    a1 = sub3(B, a1);
    v3 b1 = V3(A.x, A.y, A.z);
    b1 = sub3(C, b1);

    v3 a2 = V3(I.x, I.y, I.z);
    a2 = sub3(J, a2);
    v3 b2 = V3(I.x, I.y, I.z);
    b2 = sub3(K, b2);

    v3 n1 = V3(a1.y*b1.z - a1.z*b1.y,
               a1.x*b1.z - a1.z*b1.x,
               a1.x*b1.y - a1.y*b1.x);
    v3 n2 = V3(a2.y*b2.z - a2.z*b2.y,
               a2.x*b2.z - a2.z*b2.x,
               a2.x*b2.y - a2.y*b2.x);

    r32 d1 = n1.x*A.x + n1.y*A.y + n1.z*A.z;
    r32 d2 = n2.x*I.x + n2.y*I.y + n2.z*I.z;
    
    // also dividing by 0 when calculating k...
    for (s32 i = 0; i < height1; i++)
        {
            u32* row = (u32*)(wnd_buffer + wnd_pitch*i + offset1);
            for (s32 j = 0; j < width1; j++)
                {
                    b32 inside = true;
                    r32* zbuffer_point = (r32*)(zbuffer + wnd_pitch*i + wnd_bytpp*j + offset1);
                    r32 z = (d1 - n1.x*(outline1.left+j) - n1.y*(outline1.bottom+i)) / n1.z;

                    r32 yAB = kAB*(outline1.left + j - pA.x) + pA.y;
                    r32 yBC = kBC*(outline1.left + j - pB.x) + pB.y;
                    r32 yCA = kCA*(outline1.left + j - pC.x) + pC.y;

                    // the problem is AB, BC, CA, need to be sorted in some way...
                    if (two_below1)
                        {
                            if (outline1.bottom + i < yAB)
                                {
                                    inside = false;
                                }
                            if (outline1.bottom + i < yBC)
                                {
                                    inside = false;
                                }
                            if (outline1.bottom + i > yCA)
                                {
                                    inside = false;
                                }
                        }
                    else
                        {
                            if (outline1.bottom + i < yAB)
                                {
                                    inside = false;
                                }
                            if (outline1.bottom + i > yBC)
                                {
                                    inside = false;
                                }
                            if (outline1.bottom + i > yCA)
                                {
                                    inside = false;
                                }
                        }
                        
                    if (inside)
                        {
                            if (z < *zbuffer_point)
                                {
                                    *row = color1;
                                    *zbuffer_point = z;
                                }
                        }
                    row++;
                }
        }

    // ---------------------------------------------------------------
    for (s32 i = 0; i < height2; i++)
        {
            u32* row = (u32*)(wnd_buffer + wnd_pitch*i + offset2);
            for (s32 j = 0; j < width2; j++)
                {
                    b32 inside = true;
                    r32* zbuffer_point = (r32*)(zbuffer + wnd_pitch*i + wnd_bytpp*j + offset2);
                    r32 z = (d2 - n2.x*(outline2.left+j) - n2.y*(outline2.bottom+i)) / n2.z;
                    
                    r32 yIJ = kIJ*(outline2.left + j - pI.x) + pI.y;
                    r32 yJK = kJK*(outline2.left + j - pJ.x) + pJ.y;
                    r32 yKI = kKI*(outline2.left + j - pK.x) + pK.y;

                    // the problem is AB, BC, CA, need to be sorted in some way...
                    if (two_below2)
                        {
                            if (outline2.bottom + i < yIJ)
                                {
                                    inside = false;
                                }
                            if (outline2.bottom + i < yJK)
                                {
                                    inside = false;
                                }
                            if (outline2.bottom + i > yKI)
                                {
                                    inside = false;
                                }
                        }
                    else
                        {
                            if (outline2.bottom + i < yIJ)
                                {
                                    inside = false;
                                }
                            if (outline2.bottom + i > yJK)
                                {
                                    inside = false;
                                }
                            if (outline2.bottom + i > yKI)
                                {
                                    inside = false;
                                }
                        }

                        
                    if (inside)
                        {
                            if (z < *zbuffer_point)
                                {
                                    *row = color2;
                                    *zbuffer_point = z;
                                }
                        }
                    row++;
                }
        }

    for (s32 i = 0; i < wnd_height; i++)
        {
            r32* row = (r32*)(zbuffer + i*wnd_pitch);
            for (s32 j = 0; j < wnd_width; j++)
                {
                    *row = Gamestate->farclip; // smth else??
                    row++;
                }
        }

    /* draw_wndline_aa(pA, pB, color); */
    /* draw_wndline_aa(pB, pC, color); */
    /* draw_wndline_aa(pC, pA, color); */
    

}


void zbuffering_test(void)
{

    fill_background();
    u32 color = Gamestate->brushes[BRUSH_SCANLINE];

    v3 screen_center = V3(Gamestate->wnd_center_x,
                          Gamestate->wnd_center_y,
                          Gamestate->screen_z);

    v3 rect1A = V3(-30, 30, 5);
    v3 rect1B = V3(30, 30, 5);
    v3 rect1C = V3(30, -30, 5);
    v3 rect1D = V3(-30, -30, 5);

    v3 rect2A = V3(-60, 60, 10);
    v3 rect2B = V3(60, 60, 10);
    v3 rect2C = V3(60, -60, 10);
    v3 rect2D = V3(-60, -60, 10);

    v3 rect3A = V3(-20, 20, 7);
    v3 rect3B = V3(20, 20, 7);
    v3 rect3C = V3(20, -20, 7);
    v3 rect3D = V3(-20, -20, 7);

    rect1A = add3(rect1A, screen_center);
    rect1B = add3(rect1B, screen_center);
    rect1C = add3(rect1C, screen_center);
    rect1D = add3(rect1D, screen_center);

    rect2A = add3(rect2A, screen_center);
    rect2B = add3(rect2B, screen_center);
    rect2C = add3(rect2C, screen_center);
    rect2D = add3(rect2D, screen_center);

    rect3A = add3(rect3A, screen_center);
    rect3B = add3(rect3B, screen_center);
    rect3C = add3(rect3C, screen_center);
    rect3D = add3(rect3D, screen_center);

    PROJECTION p = PERSPECTIVE;
    v2 rect1pA = project(rect1A, p);
    v2 rect1pB = project(rect1B, p);
    v2 rect1pC = project(rect1C, p);
    v2 rect1pD = project(rect1D, p);

    v2 rect2pA = project(rect2A, p);
    v2 rect2pB = project(rect2B, p);
    v2 rect2pC = project(rect2C, p);
    v2 rect2pD = project(rect2D, p);

    v2 rect3pA = project(rect3A, p);
    v2 rect3pB = project(rect3B, p);
    v2 rect3pC = project(rect3C, p);
    v2 rect3pD = project(rect3D, p);

    wndrect rect1 = Wndrect(rect1pD.x, rect1pD.y, rect1pB.x, rect1pB.y);
    wndrect rect2 = Wndrect(rect2pD.x, rect2pD.y, rect2pB.x, rect2pB.y);
    wndrect rect3 = Wndrect(rect3pD.x, rect3pD.y, rect3pB.x, rect3pB.y);

    rect1 = clamp_wndrect(rect1);
    rect2 = clamp_wndrect(rect2);
    rect3 = clamp_wndrect(rect3);
    
    pxl color1 = PXL(0, 255, 0, 255); // zelena plavi
    pxl color2 = PXL(0, 0, 255, 255); // plava crveni
    pxl color3 = PXL(255, 0, 0, 255); // crvena zeleni

    // do z-buffering here:
    //   find equation of a plane for rect1 and rect2
    //   call draw_clamped_zbuffered_wndrect for them...

    // since rect1 and rect2 are parallel to the XY plane,
    // we don't have to calculate a normal to define a
    // plane equation....

    // rect1: z - 35 = 0    -> z = 35
    // rect2: z - 40 = 0    -> z = 40   (obviously this is a simple case...)

    draw_clamped_zbuffered_wndrect(rect1, rect1A.z, color1);
    draw_clamped_zbuffered_wndrect(rect2, rect2A.z, color2);
    draw_clamped_zbuffered_wndrect(rect3, rect3A.z, color3);
    
    /* draw_clamped_wndrect(rect1, color1); */
    /* draw_clamped_wndrect(rect2, color2); */

    for (s32 i = 0; i < wnd_height; i++)
        {
            r32* row = (r32*)(zbuffer + i*wnd_pitch);
            for (s32 j = 0; j < wnd_width; j++)
                {
                    *row = Gamestate->farclip; // smth else??
                    row++;
                }
        }
    
}


void draw_triangle_test(void)
{

    fill_background();
    u32 color = Gamestate->brushes[BRUSH_SCANLINE];

    v3 screen_center = V3(Gamestate->wnd_center_x,
                          Gamestate->wnd_center_y,
                          Gamestate->screen_z);
    
    v3 A = V3(-50, -10, 10);
    v3 B = V3(60, 10, 10);
    //v3 C = V3(0, 70, 10);
    v3 C = V3(-70, 70, 10);

    A = add3(A, screen_center);
    B = add3(B, screen_center);
    C = add3(C, screen_center);

    v2 pA = project(A, PERSPECTIVE);
    v2 pB = project(B, PERSPECTIVE);
    v2 pC = project(C, PERSPECTIVE);

    r32 kAB = (pA.y - pB.y) / (pA.x - pB.x);
    r32 kBC = (pB.y - pC.y) / (pB.x - pC.x);
    r32 kCA = (pC.y - pA.y) / (pC.x - pA.x);

    wndrect outline = Wndrect(pA.x, pA.y, pA.x, pA.y);

    if (pA.x < outline.left)   outline.left = pA.x;
    if (pA.y < outline.bottom) outline.bottom = pA.y;
    if (pA.x > outline.right)  outline.right = pA.x;
    if (pA.y > outline.top)    outline.top = pA.y;

    if (pB.x < outline.left)   outline.left = pB.x;
    if (pB.y < outline.bottom) outline.bottom = pB.y;
    if (pB.x > outline.right)  outline.right = pB.x;
    if (pB.y > outline.top)    outline.top = pB.y;

    if (pC.x < outline.left)   outline.left = pC.x;
    if (pC.y < outline.bottom) outline.bottom = pC.y;
    if (pC.x > outline.right)  outline.right = pC.x;
    if (pC.y > outline.top)    outline.top = pC.y;

    b32 two_below = true;
    if (pA.y == outline.bottom)
        {
            v2 tmp = pA;
            pA = V2(pB.x, pB.y);  // can you do pA = pB ??
            pB = V2(tmp.x, tmp.y);

            r32 tmp2 = kCA;
            kCA = kBC;
            kBC = tmp2;
            
            if (pB.x == outline.right || pB.x == outline.left)
                {
                    two_below = false;
                }
        }
    else if (pB.y == outline.bottom)
        {            
            if (pB.x == outline.right || pB.x == outline.left)
                {
                    two_below = false;
                }
        }
    else if (pC.y == outline.bottom)
        {
            v2 tmp = pC;
            pC = V2(pB.x, pB.y);
            pB = V2(tmp.x, tmp.y);

            r32 tmp2 = kAB;
            kAB = kCA;
            kCA = tmp2;
            
            if (pB.x == outline.right || pB.x == outline.left)
                {
                    two_below = false;
                }
        }
    
    s32 offset = wnd_pitch*round32(outline.bottom) +
        round32(outline.left)*wnd_bytpp;
    s32 height = round32(wndrect_height(outline));
    s32 width = round32(wndrect_width(outline));

    // also dividing by 0 when calculating k...
    for (s32 i = 0; i < height; i++)
        {
            u32* row = (u32*)(wnd_buffer + wnd_pitch*i + offset);
            for (s32 j = 0; j < width; j++)
                {
                    b32 inside = true;

                    r32 yAB = kAB*(outline.left + j - pA.x) + pA.y;
                    r32 yBC = kBC*(outline.left + j - pB.x) + pB.y;
                    r32 yCA = kCA*(outline.left + j - pC.x) + pC.y;

                    // the problem is AB, BC, CA, need to be sorted in some way...
                    if (two_below)
                        {
                            if (outline.bottom + i < yAB)
                                {
                                    inside = false;
                                }
                            if (outline.bottom + i < yBC)
                                {
                                    inside = false;
                                }
                            if (outline.bottom + i > yCA)
                                {
                                    inside = false;
                                }
                        }
                    else
                        {
                            if (outline.bottom + i < yAB)
                                {
                                    inside = false;
                                }
                            if (outline.bottom + i > yBC)
                                {
                                    inside = false;
                                }
                            if (outline.bottom + i > yCA)
                                {
                                    inside = false;
                                }
                        }
                        
                    if (inside)
                        {
                            *row = color;
                        }
                    row++;
                }
        }

    /* draw_wndline_aa(pA, pB, color); */
    /* draw_wndline_aa(pB, pC, color); */
    /* draw_wndline_aa(pC, pA, color); */
    
}



void static_camera_test(void)
{
    fill_background();

    v3 screen_center = V3(Gamestate->wnd_center_x,
                          Gamestate->wnd_center_y,
                          Gamestate->screen_z + 140);

    v3 A = V3(-60, 60, -60);
    v3 B = V3(60, 60, -60);
    v3 C = V3(60, -60, -60);
    v3 D = V3(-60, -60, -60);

    v3 bA = V3(-60, 60, 60);
    v3 bB = V3(60, 60, 60);
    v3 bC = V3(60, -60, 60);
    v3 bD = V3(-60, -60, 60);

    // project is now different
    
    A = add3(A, screen_center);
    v2 pA = project(A, PERSPECTIVE);
            
    B = add3(B, screen_center);
    v2 pB = project(B, PERSPECTIVE);
            
    C = add3(C, screen_center);
    v2 pC = project(C, PERSPECTIVE);

    D = add3(D, screen_center);
    v2 pD = project(D, PERSPECTIVE);


    bA = add3(bA, screen_center);
    v2 pbA = project(bA, PERSPECTIVE);
            
    bB = add3(bB, screen_center);
    v2 pbB = project(bB, PERSPECTIVE);
            
    bC = add3(bC, screen_center);
    v2 pbC = project(bC, PERSPECTIVE);

    bD = add3(bD, screen_center);
    v2 pbD = project(bD, PERSPECTIVE);
            
    v2 pSC = project(screen_center, PERSPECTIVE);

    draw_wndline_aa(pA, pB, Gamestate->brushes[BRUSH_SCANLINE]);
    draw_wndline_aa(pB, pC, Gamestate->brushes[BRUSH_SCANLINE]);
    draw_wndline_aa(pC, pD, Gamestate->brushes[BRUSH_SCANLINE]);
    draw_wndline_aa(pD, pA, Gamestate->brushes[BRUSH_SCANLINE]);

    draw_wndline_aa(pbA, pbB, Gamestate->brushes[BRUSH_SCANLINE]);
    draw_wndline_aa(pbB, pbC, Gamestate->brushes[BRUSH_SCANLINE]);
    draw_wndline_aa(pbC, pbD, Gamestate->brushes[BRUSH_SCANLINE]);
    draw_wndline_aa(pbD, pbA, Gamestate->brushes[BRUSH_SCANLINE]);

    draw_wndline_aa(pA, pbA, Gamestate->brushes[BRUSH_SCANLINE]);
    draw_wndline_aa(pB, pbB, Gamestate->brushes[BRUSH_SCANLINE]);
    draw_wndline_aa(pC, pbC, Gamestate->brushes[BRUSH_SCANLINE]);
    draw_wndline_aa(pD, pbD, Gamestate->brushes[BRUSH_SCANLINE]);
}


void rotate_cube_test(void)
{
    fill_background();
    
    v3 screen_center = V3(Gamestate->wnd_center_x,
                          Gamestate->wnd_center_y,
                          Gamestate->screen_z + 140); // was + 140

    v3 A = V3(-60, 60, -60); // was -60 here and 60 down on z
    v3 B = V3(60, 60, -60);
    v3 C = V3(60, -60, -60);
    v3 D = V3(-60, -60, -60);

    v3 bA = V3(-60, 60, 60);
    v3 bB = V3(60, 60, 60);
    v3 bC = V3(60, -60, 60);
    v3 bD = V3(-60, -60, 60);

    // assume I will offset my_point into screen_center
    // so don't test for it Z
    if (screen_center.z >= wnd_nearclip &&
        screen_center.z <= wnd_farclip)
        {
            A = rotate3(A, Gamestate->line_angle, 0, Gamestate->line_angle);
            A = add3(A, screen_center);
            v2 pA = project(A, PERSPECTIVE);
            
            B = rotate3(B, Gamestate->line_angle, 0, Gamestate->line_angle);
            B = add3(B, screen_center);
            v2 pB = project(B, PERSPECTIVE);
            
            C = rotate3(C, Gamestate->line_angle, 0, Gamestate->line_angle);
            C = add3(C, screen_center);
            v2 pC = project(C, PERSPECTIVE);

            D = rotate3(D, Gamestate->line_angle, 0, Gamestate->line_angle);
            D = add3(D, screen_center);
            v2 pD = project(D, PERSPECTIVE);


            bA = rotate3(bA, Gamestate->line_angle, 0, Gamestate->line_angle);
            bA = add3(bA, screen_center);
            v2 pbA = project(bA, PERSPECTIVE);
            
            bB = rotate3(bB, Gamestate->line_angle, 0, Gamestate->line_angle);
            bB = add3(bB, screen_center);
            v2 pbB = project(bB, PERSPECTIVE);
            
            bC = rotate3(bC, Gamestate->line_angle, 0, Gamestate->line_angle);
            bC = add3(bC, screen_center);
            v2 pbC = project(bC, PERSPECTIVE);

            bD = rotate3(bD, Gamestate->line_angle, 0, Gamestate->line_angle);
            bD = add3(bD, screen_center);
            v2 pbD = project(bD, PERSPECTIVE);

            
            v2 pSC = project(screen_center, PERSPECTIVE);


            // @IMPORTANT this doesn't work bro...
            /* v2 tmp = projected_my_point; */
            /* projected_my_point = clamp_line(projected_my_point, */
            /*                                 projected_screen_center); */
    
            /* projected_screen_center = clamp_line(projected_screen_center, */
            /*                                      tmp); */
    
            draw_wndline_aa(pA, pB, Gamestate->brushes[BRUSH_SCANLINE]);
            draw_wndline_aa(pB, pC, Gamestate->brushes[BRUSH_SCANLINE]);
            draw_wndline_aa(pC, pD, Gamestate->brushes[BRUSH_SCANLINE]);
            draw_wndline_aa(pD, pA, Gamestate->brushes[BRUSH_SCANLINE]);

            draw_wndline_aa(pbA, pbB, Gamestate->brushes[BRUSH_SCANLINE]);
            draw_wndline_aa(pbB, pbC, Gamestate->brushes[BRUSH_SCANLINE]);
            draw_wndline_aa(pbC, pbD, Gamestate->brushes[BRUSH_SCANLINE]);
            draw_wndline_aa(pbD, pbA, Gamestate->brushes[BRUSH_SCANLINE]);

            draw_wndline_aa(pA, pbA, Gamestate->brushes[BRUSH_SCANLINE]);
            draw_wndline_aa(pB, pbB, Gamestate->brushes[BRUSH_SCANLINE]);
            draw_wndline_aa(pC, pbC, Gamestate->brushes[BRUSH_SCANLINE]);
            draw_wndline_aa(pD, pbD, Gamestate->brushes[BRUSH_SCANLINE]);
        }
    Gamestate->line_angle += PI / 256;
    //Gamestate->line_angle += PI / (256*4);
}




void rotate3_test(void)
{
    fill_background();
    
    v3 screen_center = V3(Gamestate->wnd_center_x,
                          Gamestate->wnd_center_y,
                          Gamestate->screen_z + 140);

    v3 my_point = V3(150, 150, 0);

    // assume I will offset my_point into screen_center
    // so don't test for it's Z
    if (screen_center.z >= wnd_nearclip &&
        screen_center.z <= wnd_farclip)
        {
            my_point = rotate3(my_point, Gamestate->line_angle/4, 0, Gamestate->line_angle/4);
            my_point = add3(my_point, screen_center);

            v2 projected_screen_center = project(screen_center, PERSPECTIVE);
            projected_screen_center = clamp2(projected_screen_center); // added
            v2 projected_my_point = project(my_point, PERSPECTIVE);
            projected_my_point = clamp2(projected_my_point); // added

            v2 tmp = projected_my_point;

            // @IMPORTANT this doesn't work bro...
            /* projected_my_point = clamp_line(projected_my_point, */
            /*                                 projected_screen_center); */
    
            /* projected_screen_center = clamp_line(projected_screen_center, */
            /*                                      tmp); */
    
            draw_wndline_aa(projected_screen_center,
                            projected_my_point,
                            Gamestate->brushes[BRUSH_SCANLINE]);
        }
    Gamestate->line_angle += PI / 256;
    //Gamestate->line_angle += PI / (256*4);
}














// @TODO figure out how to rasterize a triangle, then you can draw
// a rotate rectangle better

// @TODO figure out how to abstract a plane in 3D, in particular
// the screen/camera (nearclip, farclip) plane, and then figure
// out how to project the virtual world to this plane (and move
// plane around through user input)

// @TODO after that you can potentially try to figure out
// 3D rotation (quaternions?)



// @TODO simple dynamic arrays, so you don't have to make a #define
// for array sizes...



void line_side_by_side_test(void)
{
    //Gamestate->brushes[BRUSH_NONE] = 0xFFFFFF;
    fill_background();

    u32 color      = Gamestate->brushes[BRUSH_SCANLINE];
    v2 origin      = V2(Gamestate->wnd_center_x,
                        Gamestate->wnd_center_y);
    v2 line        = V2(100, 0);
    
    line = scale2(line, Gamestate->line_scaling_factor);
    line = rotate2(line, -Gamestate->line_angle);
    
    draw_line(origin, line, color);

    v2 origin_aa = V2(Gamestate->wnd_center_x,
                        Gamestate->wnd_center_y + 10);
    v2 line_aa = V2(100, 0);
    line_aa = rotate2(line_aa, -Gamestate->line_angle);
    line_aa = scale2(line_aa, Gamestate->line_scaling_factor);

    line_aa = add2(origin_aa, line_aa);
    //line_aa = transpose2(line_aa, zero2(), origin_aa);
    
    
    draw_wndline_aa(origin_aa, line_aa, color);
    
    Gamestate->line_angle += PI / 256;

    // draw_line_aa
    
    // draw_line
}

// @Note nearclip and farclip should actually be planes
// then you first test a point against those planes
// then you project

// @Note in reality, if you have a large world, you can't
// test every point, so you should probably put things to
// draw in a buffer (sorted by distance between nearclip
// and farclip planes), then you only test for things in
// that buffer, and when it fails, you load new things to
// draw into that buffer

// @Note potentially a different way to do this is with
// defined boundaries of the world and test against those
// instead of against nearclip and farclip planes

// @Note first is essentialy fov bounds check
// second is world chunk bounds check

// @Note actually, because you can move around the camera
// very rapidly, you don't won't to fov check, because
// that would be very slow, you would probably want to
// have a world chunk, and then you have to sort in your
// fov what to draw first (Z buffering, but depends on
// the coordinate system if it's Z axis aligned (XY-plane
// aligned....) )

// @Note for example you can bounds check the camera position
// actually, and if it fails load a chunk around it,
// and sort what to draw first in fov....

void scanlines_concentric_test(void)
{
    v2  origin    = V2(Gamestate->wnd_center_x,
                       Gamestate->wnd_center_y);
    
    r32 spread_x         = Gamestate->concentric_spread_x;
    r32 spread_y         = Gamestate->concentric_spread_y;
    s32 concentric_count = Gamestate->concentric_count;
    
    r32* concentric_z_buffer = Gamestate->concentric_z_values;

    //Gamestate->brushes[0] = 0xFFFFFF;
    fill_background();

    // @TODO maybe don't do i=1 here ?
    for (s32 i=0; i<concentric_count; i++)
        {
            r32 curr_z = concentric_z_buffer[i];
            
            if (curr_z >= wnd_nearclip &&
                curr_z <= wnd_farclip)
                {
                    r32 half_width  = (i+1) * spread_x;
                    r32 half_height = (i+1) * spread_y;
            
                    r32 left   = origin.x - half_width;
                    r32 bottom = origin.y - half_height;
                    r32 right  = origin.x + half_width;
                    r32 top    = origin.y + half_height;

                    v3 A3 = V3(left, top, curr_z);
                    v3 B3 = V3(right, top, curr_z);
                    v3 C3 = V3(right, bottom, curr_z);
                    v3 D3 = V3(left, bottom, curr_z);                    
            
                    v2 A2 = project(A3, PERSPECTIVE);
                    v2 B2 = project(B3, PERSPECTIVE);
                    v2 C2 = project(C3, PERSPECTIVE);
                    v2 D2 = project(D3, PERSPECTIVE);

                    v2 tmp = A2;
                    A2 = clamp_line(A2, B2);
                    B2 = clamp_line(B2, C2);
                    C2 = clamp_line(C2, D2);
                    D2 = clamp_line(D2, tmp);

                    u32 brush = GetBrush(BRUSH_SCANLINE);

                    // first test for edist2 for lines, then draw if true
                    draw_wndline_aa(A2, B2, brush);
                    draw_wndline_aa(B2, C2, brush);
                    draw_wndline_aa(C2, D2, brush);
                    draw_wndline_aa(D2, A2, brush);
                }
        }    
}

void draw_rotated_line_test(void)
{
    // @TODO prevent drawing along left and top wndrect edges when coords are 0...
    fill_background();
    
    pxl color      = PXL(0, 0, 0, 0);
    v2 origin      = V2(Gamestate->wnd_center_x,
                        Gamestate->wnd_center_y);
    v2 offset_line = V2(100, 0);
    v2 line        = V2(50, 0);
    
    line = scale2(line, Gamestate->line_scaling_factor);
    line = rotate2(line, -Gamestate->line_angle);
    offset_line = scale2(offset_line, Gamestate->line_scaling_factor);
    offset_line = rotate2(offset_line, Gamestate->line_angle*1.5);
    // @TODO figure out if this can be done with add2 orwhtv
    v2 start = V2(origin.x+offset_line.x, origin.y-offset_line.y);
    v2 end = V2(origin.x+line.x+offset_line.x, origin.y-line.y-offset_line.y);

    origin = clamp2(origin);
    start  = clamp2(start);
    end    = clamp2(end);
    
    line = transpose2(end, zero2() ,start);
    offset_line = transpose2(start, zero2(), origin);

    //draw_line(start, line, color); because of pxl
    // draw_line(origin, offset_line, color); because of pxl

    Gamestate->line_angle += PI / 256;
}

void perspective_projection_test(void)
{
    Gamestate->concentric_current_z = 0;
    r32* concentric_z_values = Gamestate->concentric_z_values;

    pxl color     = PXL(0, 0, 0, 255);
    r32 thickness = Gamestate->concentric_thickness;    
    v2  origin    = V2(Gamestate->wnd_center_x,
                       Gamestate->wnd_center_y);
    
    r32 spread_x         = Gamestate->concentric_spread_x;
    r32 spread_y         = Gamestate->concentric_spread_y;
    s32 concentric_count = Gamestate->concentric_count;
    
    fill_background();

    for (s32 i=1; i<=concentric_count; i++)
        {
            r32 half_width  = i*(spread_x+thickness);
            r32 half_height = i*(spread_y+thickness);
            
            r32 left   = origin.x - half_width;
            r32 bottom = origin.y - half_height;
            r32 right  = origin.x + half_width;
            r32 top    = origin.y + half_height;
            
            wndrect rect = Wndrect(left, bottom, right, top);

            if (concentric_z_values[i] >= wnd_nearclip &&
                concentric_z_values[i] <= wnd_farclip)
                {
                    draw_wndrect_outline_projected(rect, thickness, color);
                }
            Gamestate->concentric_current_z++;
        }

}

void concentric_test(void)
{
    pxl color     = PXL(0, 0, 0, 255);
    r32 thickness = Gamestate->concentric_thickness;    
    v2  origin    = V2(Gamestate->wnd_center_x,
                       Gamestate->wnd_center_y);
    
    r32 spread_x         = Gamestate->concentric_spread_x;
    r32 spread_y         = Gamestate->concentric_spread_y;
    s32 concentric_count = Gamestate->concentric_count;
    
    fill_background();

    for (s32 i=1; i<=concentric_count; i++)
        {
            r32 half_width  = i*(spread_x+thickness);
            r32 half_height = i*(spread_y+thickness);
            
            r32 left   = origin.x - half_width;
            r32 bottom = origin.y - half_height;
            r32 right  = origin.x + half_width;
            r32 top    = origin.y + half_height;
            
            wndrect rect = Wndrect(left, bottom, right, top);

            draw_wndrect_outline(rect, thickness, color);
        }
}

void draw_rotated_rect_test(void)
{
    fill_background();
    pxl color = PXL(0, 0, 0, 0);
    r32 rect_scaling_factor = Gamestate->rect_scaling_factor;
    v2 origin = V2(Gamestate->wnd_center_x,
                   Gamestate->wnd_center_y);
    v2 A = V2(0, 50);
    v2 B = V2(50, 50);
    //v2 C = V2(50, 0);
    v2 D = V2(0, 0);

    A = scale2(A, rect_scaling_factor);
    B = scale2(B, rect_scaling_factor);
    //C = scale2(C, rect_scaling_factor);
    D = scale2(D, rect_scaling_factor);

    // @Note round instead??
    // @TODO there are holes at certain times, due to numerical inaccuracy...
    s32 width = floor32(edist2(A, B));
    s32 height = floor32(edist2(A, D));

    draw_rotated_rect(width, height, origin, color);

    Gamestate->rect_angle += PI / 256;
}

void draw_sqaure_around_cursor_test(void)
{
    dbg_render(Gamestate->dbg_render_x_offset, Gamestate->dbg_render_y_offset);
    dbg_draw_square_around_cursor(Gamestate->square_length);
}

void file_test(void)
{
    u32 readsize;
    READ_FILE((u8*)"W:\\cardcrafter\\data\\readtest.txt", (u8*)ENGINE_MEMORY->temp_mem, &readsize);
    WRITE_FILE((u8*)"W:\\cardcrafter\\data\\writetest.txt", (u8*)ENGINE_MEMORY->temp_mem, readsize);
    TEST_ONLY_ONCE
}

void none_test(void)
{
    TEST_ONLY_ONCE
}

#define TEST_ CURRENTLY_TESTING();
void test(void)
{
    if (!Gamestate->tested_once)
        {
            TEST_
        }
}
