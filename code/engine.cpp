inline r32 slope2(v2 vec1, v2 vec2)
{
    r32 result;

    result = (vec1.y - vec2.y) / (vec1.x / vec2.x);
    
    return result;
}

inline r32 EdgeFunction(v3 c, v3 b, v3 a)
{
    return (c.x-a.x)*(b.y-a.y)-(c.y-a.y)*(b.x-a.x);
}

inline wndrect ObtainTriangleBBox(v3 p0, v3 p1, v3 p2)
{
    wndrect result;

    result.left   = Min(Min(p0.x, p1.x), p2.x);
    result.right  = Max(Max(p0.x, p1.x), p2.x);
    result.bottom = Min(Min(p0.y, p1.y), p2.y);
    result.top    = Max(Max(p0.y, p1.y), p2.y);

    return result;
}
        
void RasterizeTriangle(v3 p0, v3 p1, v3 p2, u32 color, b32 inv)
{
    // perspective-correct interpolation
    // top-left rule
    // anti-aliasing
    wndrect bbox = ObtainTriangleBBox(p0, p1, p2);
    r32 area = EdgeFunction(p2, p1, p0);

    for (s32 j = Floor(bbox.bottom); j < Floor(bbox.top); j++)
        {
            
            for (s32 i = Floor(bbox.left); i < Floor(bbox.right); i++)
                {
                    v3 p = V3(i+0.5f, j+0.5f, 0);
                    r32 w0 = EdgeFunction(p, p2, p1);
                    r32 w1 = EdgeFunction(p, p0, p2);
                    r32 w2 = EdgeFunction(p, p1, p0);

                    if (w0 >= 0 && w1 >= 0 && w2 >= 0) // inside test
                        {
                            w0 /= area; // these are barycentric coords
                            w1 /= area;
                            w2 /= area;

                            r32 z = 1/(w0/p0.z+w1/p1.z+w2/p2.z);
                            r32* zbuffer_point = (r32*)(Z_BUFFER+FRAME_BUFFER_PITCH*j+BYTPP*i);

                            u32 r = (u32)(w0*255);
                            u32 g = (u32)(w1*255);
                            u32 b = (u32)(w2*255);

                            u32 intColor = (((u32)255 << 24) | (r << 16) | (g << 8) | b);
                            
                            // if (z >= *zbuffer_point)
                            if (z < *zbuffer_point)
                                {
                                    *((u32*)(FRAME_BUFFER+j*FRAME_BUFFER_PITCH+i*BYTPP)) = (inv ? intColor : color);
                                    *zbuffer_point = z;
                                }
                        }
                }
        }
}

typedef union tagTriangle
{
    struct
    {
        v3 A;
        v3 B;
        v3 C;
    };
    v3 v[3];
} triangle;

triangle TriangleWorldToRaster(triangle tri)
{
    // @TODO refactor to use projection matrix instead
    triangle result;

    // camera space
    
    v3 A = tri.A;
    v3 B = tri.B;
    v3 C = tri.C;
    
    r32 Near = Z_NEAR;

    r32 invA = Near/A.z;
    r32 invB = Near/B.z;
    r32 invC = Near/C.z;
    v3 A_s = V3(A.x*invA, A.y*invA, A.z);
    v3 B_s = V3(B.x*invB, B.y*invB, B.z);
    v3 C_s = V3(C.x*invC, C.y*invC, C.z);
    
    r32 Fov = FOV;
    r32 aspect_ratio = (FRAME_BUFFER_WIDTH*1.0f)/FRAME_BUFFER_HEIGHT;

    r32 r = -Near*tan(radians(Fov/2));
    r32 l = -r;
    r32 t = r/aspect_ratio; // is it the opposite here actually, first t then r?
    r32 b = -t;
    
    // clip space (before converting from homogeneous to cartesian, which
    // does the perspective divide essentialy)

    // ndc space
    
    v3 A_ndc = V3(A_s.x/r, A_s.y/t, A_s.z);
    v3 B_ndc = V3(B_s.x/r, B_s.y/t, B_s.z);
    v3 C_ndc = V3(C_s.x/r, C_s.y/t, C_s.z);

    /* v3 A_r = V3((A_ndc.x+1)/2*(FRAME_BUFFER_WIDTH-1), (1-(A_ndc.y+1)/2)*(FRAME_BUFFER_HEIGHT-1), A_ndc.z); */
    /* v3 B_r = V3((B_ndc.x+1)/2*(FRAME_BUFFER_WIDTH-1), (1-(B_ndc.y+1)/2)*(FRAME_BUFFER_HEIGHT-1), B_ndc.z); */
    /* v3 C_r = V3((C_ndc.x+1)/2*(FRAME_BUFFER_WIDTH-1), (1-(C_ndc.y+1)/2)*(FRAME_BUFFER_HEIGHT-1), C_ndc.z); */

    // raster space
    
    v3 A_r = V3((A_ndc.x+1)/2*(FRAME_BUFFER_WIDTH-1), (A_ndc.y+1)/2*(FRAME_BUFFER_HEIGHT-1), A_ndc.z);
    v3 B_r = V3((B_ndc.x+1)/2*(FRAME_BUFFER_WIDTH-1), (B_ndc.y+1)/2*(FRAME_BUFFER_HEIGHT-1), B_ndc.z);
    v3 C_r = V3((C_ndc.x+1)/2*(FRAME_BUFFER_WIDTH-1), (C_ndc.y+1)/2*(FRAME_BUFFER_HEIGHT-1), C_ndc.z);

    result.A = A_r;
    result.B = B_r;
    result.C = C_r;
    
    return result;
}

typedef struct TagTriangleHom
{
    v4 A;
    v4 B;
    v4 C;
} TriangleHom;

#include "clip.h"

inline m4 M4Proj()
{
    m4 result = M4Unit();

    r32 aspectRatio = (FRAME_BUFFER_WIDTH*1.0f) / FRAME_BUFFER_HEIGHT;
    r32 fov = FOV;
    
    r32 n = Z_NEAR;
    r32 f = Z_FAR;
    r32 t = tan(radians(fov / 2)) * n;
    r32 r = t * aspectRatio;

    result.m[0][0] = n / r;
    result.m[1][1] = n / t;
    result.m[2][2] = - (f + n) / (f - n);
    result.m[3][3] = 0;
    result.m[2][3] = - 1;
    result.m[3][2] = - (2 * f * n) / (f - n);

    return result;
}

int TriangleWorldToRasterPROJ(triangle tri, TriangleHom* out_triangles)
{
    m4 PROJ = M4Proj();

    v3 A_clip3 = M4Mul(tri.A, PROJ);
    v3 B_clip3 = M4Mul(tri.B, PROJ);
    v3 C_clip3 = M4Mul(tri.C, PROJ);

    v4 A_clip = V4(A_clip3.x, A_clip3.y, A_clip3.z, -tri.A.z);
    v4 B_clip = V4(B_clip3.x, B_clip3.y, B_clip3.z, -tri.B.z);
    v4 C_clip = V4(C_clip3.x, C_clip3.y, C_clip3.z, -tri.C.z);
    
    int count = ClipTriangle(A_clip, B_clip, C_clip, out_triangles);
    for (int i = 0; i < count; i++)
        {
            TriangleHom t = out_triangles[i];
            
            v4 A_ndc = V4(t.A.x/t.A.w, t.A.y/t.A.w, t.A.z/t.A.w, 1);
            v4 B_ndc = V4(t.B.x/t.B.w, t.B.y/t.B.w, t.B.z/t.B.w, 1);
            v4 C_ndc = V4(t.C.x/t.C.w, t.C.y/t.C.w, t.C.z/t.C.w, 1);

            v4 A_r = V4((A_ndc.x+1)/2*(FRAME_BUFFER_WIDTH-1), (A_ndc.y+1)/2*(FRAME_BUFFER_HEIGHT-1), A_ndc.z, 1);
            v4 B_r = V4((B_ndc.x+1)/2*(FRAME_BUFFER_WIDTH-1), (B_ndc.y+1)/2*(FRAME_BUFFER_HEIGHT-1), B_ndc.z, 1);
            v4 C_r = V4((C_ndc.x+1)/2*(FRAME_BUFFER_WIDTH-1), (C_ndc.y+1)/2*(FRAME_BUFFER_HEIGHT-1), C_ndc.z, 1);

            t.A = A_r;
            t.B = B_r;
            t.C = C_r;
            out_triangles[i] = t;
        }
    
    return count;
}




// z layers for rect
// draw rect outline (should be done with lines)
// clamp rect to viewport..
// constant access to frame and z buffers through x,y (i,j), and index
// rasterize axis-aligned quad
// 2d rotation? transpose/translation .?
// color functionality..

// draw line, anti-aliasing, clipping, projection, z-buffering

// slope between vectors

b32 process_input(u64 curr_keyflags_to_set,
                  u64 curr_keyflags_to_unset,
                  u8 curr_mouseflags_to_set,
                  u8 curr_mouseflags_to_unset,
                  v2 curr_cursor)
{
    // dude am I doing zbuffering correctly, maybe that's the problem??
    b32 result = true;

    // need to make WASD work in the direction camera is facing
    // moving by Y can be the same because we don't want to ever yaw
    // distance between prev and curr cursor gives you roll and pitch
    // by x component and y component;
    // but if you wanted to yaw (do it through keys)
    // then moving by Y is not the same
    
    // @TODO fix moving faster diagonally if you care...
    
    u64 prev_kflags = ENGINESTATE->keyflags;
    ENGINESTATE->keyflags |= curr_keyflags_to_set;
    ENGINESTATE->keyflags &= ~curr_keyflags_to_unset;
    u64 kflags = ENGINESTATE->keyflags;
    u64 kflags_trans = kflags ^ prev_kflags;
    u64 kflags_trans_to_up = kflags_trans & prev_kflags;
    u64 kflags_trans_to_down = kflags_trans & (~prev_kflags);
    
    u8 prev_mflags = ENGINESTATE->mouseflags;
    ENGINESTATE->mouseflags |= curr_mouseflags_to_set;
    ENGINESTATE->mouseflags &= ~curr_mouseflags_to_unset;
    u8 mflags = ENGINESTATE->mouseflags;
    u8 mflags_trans = mflags ^ prev_mflags;
    u8 mflags_trans_to_up = mflags_trans & prev_mflags;
    u8 mflags_trans_to_down = mflags_trans & (~prev_mflags);

    Camera camera = MAIN_CAMERA;
    curr_cursor.y = to_yisup(curr_cursor.y);
    v2 cursor_difference;
    cursor_difference.x = curr_cursor.x - FRAME_BUFFER_WIDTH/2;
    cursor_difference.y = curr_cursor.y - FRAME_BUFFER_HEIGHT/2;
    // camera.roll += cursor_difference.y / kilobytes(1);
    // camera.pitch -= cursor_difference.x / kilobytes(1);
    ENGINESTATE->cursor = curr_cursor;

    if (ExtractKey(kflags_trans_to_up, KEY_U))
        {
            if (ENGINESTATE->reverse_winding)
                {
                    ENGINESTATE->reverse_winding = false;
                }
            else
                {
                    ENGINESTATE->reverse_winding = true;
                }
        }
    
    if (ExtractKey(kflags, KEY_D))
        {
            // ENGINESTATE->camera_angle += pi/256;
            ENGINESTATE->log_to_file_once = true;
        }

    if (ExtractKey(kflags, KEY_A))
        {            
            // ENGINESTATE->camera_angle -= pi/256;
            ENGINESTATE->log_to_file_once = true;
        }
    
    if (ExtractKey(kflags, KEY_UP))
        {
            ENGINESTATE->camera_offs_y += 5.0f;
            ENGINESTATE->wnd_center_y+= 5;
            ENGINESTATE->dbg_render_y_offset+= 10;
        }
    if (ExtractKey(kflags, KEY_DOWN))
        {
            ENGINESTATE->camera_offs_y -= 5.0f;            
            ENGINESTATE->wnd_center_y-= 5;
            ENGINESTATE->dbg_render_y_offset-= 10;
        }
    if (ExtractKey(kflags, KEY_LEFT))
        {
            ENGINESTATE->camera_offs_x -= 5.0f;
            ENGINESTATE->wnd_center_x-= 5;
            ENGINESTATE->dbg_render_x_offset-= 10;
        }
    if (ExtractKey(kflags, KEY_RIGHT))
        {
            ENGINESTATE->camera_offs_x += 5.0f;
            ENGINESTATE->wnd_center_x+= 5;
            ENGINESTATE->dbg_render_x_offset+= 10;
        }
    
    v3 camera_movement = zero3();
    if (ExtractKey(kflags, KEY_W))
        {
            camera_movement.z -= 2.0f;
        }
    if (ExtractKey(kflags, KEY_S))
        {
            camera_movement.z += 2.0f;
        }
    if (ExtractKey(kflags, KEY_A))
        {
            camera_movement.x -= 2.0f;
        }
    if (ExtractKey(kflags, KEY_D))
        {
            camera_movement.x += 2.0f;
        }
    if (ExtractKey(kflags, KEY_Q))
        {
            camera_movement.y -= 2.0f;
        }
    if (ExtractKey(kflags, KEY_E))
        {
            camera_movement.y += 2.0f;
        }    

    // m4 rotationMatrix = M4Compose(2,
                                  // M4RotX(camera.roll),
                                  // M4RotY(camera.pitch)
                                  // );
    // camera_movement = M4Mul(camera_movement, rotationMatrix);
    // camera.fpoint = add3(camera.fpoint, camera_movement);
    
    if (result)
        {
            MAIN_CAMERA = camera;
        }
    
    if (ExtractKey(mflags_trans_to_up, MOUSE_M1))
        {
            ENGINESTATE->square_length += 50;
            ENGINESTATE->concentric_spread_x+=10;
            ENGINESTATE->concentric_spread_y+=4;
            ENGINESTATE->line_scaling_factor += 0.1f;
            ENGINESTATE->rect_scaling_factor += 0.1f;
            for (s32 i = 0; i < ENGINESTATE->concentric_count; i++)
                {
                    ENGINESTATE->concentric_z_values[i]++;
                }
        }
    if (ExtractKey(mflags_trans_to_up, MOUSE_M2))
        {
            ENGINESTATE->square_length -= 50;
            ENGINESTATE->concentric_spread_x-=10;
            ENGINESTATE->concentric_spread_y-=4;
            ENGINESTATE->line_scaling_factor -= 0.1f;
            ENGINESTATE->rect_scaling_factor -= 0.1f;
            for (s32 i = 0; i < ENGINESTATE->concentric_count; i++)
                {
                    ENGINESTATE->concentric_z_values[i]--;
                }
        }

    return result;
}

#if USE_DLL
void platform_init_memory_base(Globals* memoryBase)
{    
    globals = memoryBase;
}
#endif

void init_memory()
{
    ArenaManager arenaManager = ARENA_MANAGER;
    arenaManager.base = (u8*)(globals + 1);
    arenaManager.virtualMemoryUsed = sizeof(Globals);

    // @pot do I need to align down/up, if sizeof(Globals) makes it
    // unaligned or something?
    u64 firstArenaSize = INITIAL_COMMIT_SIZE_BY_PLATFORM - sizeof(Globals);
    arena_init(PERM_ARENA, firstArenaSize, firstArenaSize);
    arena_init(FRAME_ARENA, gigabytes(4));
            
    FRAME_BUFFER = arena_push<u8>(PERM_ARENA, MAX_FRAMEBUFFER_SIZE * BYTPP);
    Z_BUFFER = arena_push<r32>(PERM_ARENA, MAX_FRAMEBUFFER_SIZE);
}

// @todo you can probably export this and call it once
void init_engine_state(void)
{
    s32 init_FRAME_BUFFER_WIDTH    = 1280;
    s32 init_FRAME_BUFFER_HEIGHT   = 720;
    s32 init_wnd_center_x = 640;
    s32 init_wnd_center_y = 360;
    
    
    ENGINESTATE->tested_once = 0;

    ENGINESTATE->cursor.x = init_wnd_center_x;
    ENGINESTATE->cursor.y = init_wnd_center_y;
    
    FRAME_BUFFER_WIDTH = init_FRAME_BUFFER_WIDTH;
    FRAME_BUFFER_HEIGHT = init_FRAME_BUFFER_HEIGHT;

    MAIN_CAMERA.position = vec_make(640.0f, 360.0f, 0.0f);
    MAIN_CAMERA.orientation = quaternion_identity();
    MAIN_CAMERA.zNear = 5;
    MAIN_CAMERA.zFar = 500;
    MAIN_CAMERA.fov = 120;
                    
    ENGINESTATE->camera_angle = 0;
    ENGINESTATE->log_to_file_once = false;
    ENGINESTATE->reverse_winding = false;

    ENGINESTATE->keyflags = 0;
    ENGINESTATE->mouseflags = 0;
                    
    ENGINESTATE->wnd_center_x = init_wnd_center_x;
    ENGINESTATE->wnd_center_y = init_wnd_center_y;
                    
    ENGINESTATE->dbg_render_x_offset = 0; 
    ENGINESTATE->dbg_render_y_offset = 0; 
    ENGINESTATE->square_length = 10;       
    ENGINESTATE->concentric_thickness = 5;
    ENGINESTATE->concentric_count = 10;
    ENGINESTATE->concentric_spread_x = 50; 
    ENGINESTATE->concentric_spread_y = 50;
    ENGINESTATE->concentric_current_z = 0;
    ENGINESTATE->line_angle = 0;
    ENGINESTATE->line_scaling_factor = 1;
    ENGINESTATE->rect_angle = 0;
    ENGINESTATE->rect_scaling_factor = 1;
            
    ENGINESTATE->camera_offs_x = 0;
    ENGINESTATE->camera_offs_y = 0;


    // @TODO you should probably have a default for everything but whatever
    // @TODO is this a good way to set a keymap, just setting powers of two.............
    ENGINESTATE->keymap[0x25] = 0;
    ENGINESTATE->keymap[0x26] = 1; 
    ENGINESTATE->keymap[0x27] = 2; 
    ENGINESTATE->keymap[0x28] = 3; 

    ENGINESTATE->keymap[0x57] = 4;   
    ENGINESTATE->keymap[0x53] = 5; 
    ENGINESTATE->keymap[0x41] = 6; 
    ENGINESTATE->keymap[0x44] = 7; 
    ENGINESTATE->keymap[0x51] = 8; 
    ENGINESTATE->keymap[0x45] = 9; 

    ENGINESTATE->keymap[0x49] = 10;   
    ENGINESTATE->keymap[0x4B] = 11;
    ENGINESTATE->keymap[0x4A] = 12;
    ENGINESTATE->keymap[0x4C] = 13;
    ENGINESTATE->keymap[0x55] = 14;
    ENGINESTATE->keymap[0x4F] = 15;
    
    
    // init z buffer
    for (s32 i = 0; i < FRAME_BUFFER_HEIGHT; i++)
        {
            r32* row = (r32*)(Z_BUFFER + i*FRAME_BUFFER_PITCH);
            for (s32 j = 0; j < FRAME_BUFFER_WIDTH; j++)
                {
                    *row = 1.0f;
                    row++;
                }
        }
        
    s32 concentric_count = ENGINESTATE->concentric_count;
    r32* concentric_z_values = ENGINESTATE->concentric_z_values;
    ASSERT(concentric_count <= CONCENTRIC_MAX);

    for (s32 i = 0; i < MAX_BRUSHES; i++)
        {
            ENGINESTATE->brushes[i] = (((u32)255 << 24) |  // a
                                       ((u32)120 << 16) |  // R
                                       ((u32)0 << 8) |     // G
                                       (u32)120);          // B
        }
    ENGINESTATE->brushes[BRUSH_SCANLINE] = (((u32)255 << 24) |
                                            ((u32)0 << 16) |
                                            ((u32)0 << 8) |   
                                            0);        

    ENGINESTATE->brushes[BRUSH_SCANLINE2] = (((u32)255 << 24) |
                                             ((u32)0 << 16) |
                                             ((u32)0 << 8) |
                                             255);
            
    for (s32 i = 0; i < concentric_count; i++)
        {
            concentric_z_values[i] = Floor(concentric_count/2.0 - 1 - i);
        }
}

// #include "test.h"

void update_and_render(void)
{
    if (!ENGINESTATE->inited)
        {
            init_memory();
            init_engine_state();
            
            ENGINESTATE->inited = true;
        }
    // test();
}

