#define INITIAL_COMMIT_SIZE_BY_PLATFORM 8388608 // 8MB

// possibilities...:

// @Note this is probably a bad idea
/* #define normalize(n, max) ((n)*1.0f/(max)*1.0f) */
/* #define renormalize(n, max) ((n)*(max)) */


// @TODO figure out what you actually want from these from real examples
// once you have some, until then don't use these
/* inline degree moduo_degree(degree angle) */
/* { */
/*     return (r32)(((s32)angle)%DEGREE_MAX) + angle-(s32)angle; */
/* } */

/* inline rad moduo_rad(rad angle) */
/* { */
/*     return (r32)(((s32)(angle/pi))%((s32)(RAD_MAX/pi))) + angle-(s32)angle; */
/* } */



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




// #include "test.h"

void update_and_render(void)
{
    FrameBuffer frameBuffer = get_frame_buffer();
    u8* zBuffer = get_z_buffer();
    u32 bytpp = get_frame_buffer_bytpp();
    u32 pitch = framebuffer_pitch(frameBuffer.height, bytpp);
    
    if (!ENGINESTATE->inited)
        {
            init_memory();
            init_engine_state();
            
            ENGINESTATE->inited = true;
        }
    // test();
    
    zbuffer_reset(zBuffer, pitch, bytpp, frameBuffer.width, frameBuffer.height);
}

