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


union Triangle
{
    struct { Vector3 A, B, C; };
    Vector3 v[3];
};

union TriangleHom
{
    struct { Vector4 A, B, C; };
    Vector4 v[3];
};

#include "graphics/clip.h"


// inline r32 slope2(v2 vec1, v2 vec2)
// {
    // r32 result;

    // result = (vec1.y - vec2.y) / (vec1.x / vec2.x);
    
    // return result;
// }

inline r32 EdgeFunction(Vector3 c, Vector3 b, Vector3 a)
{
    return (c.x-a.x)*(b.y-a.y)-(c.y-a.y)*(b.x-a.x);
}

inline Rect ObtainTriangleBBox(Triangle tri)
{
    Rect result;

    result.min_x = Min(Min(tri.A.x, tri.B.x), tri.C.x);
    result.max_x = Max(Max(tri.A.x, tri.B.x), tri.C.x);
    result.min_y = Min(Min(tri.A.y, tri.B.y), tri.C.y);
    result.max_y = Max(Max(tri.A.y, tri.B.y), tri.C.y);

    return result;
}

void RasterizeTriangle(Triangle tri, u32 color, b32 inv)
{
    // perspective-correct interpolation
    // top-left rule
    // anti-aliasing

    Vector3 p0 = tri.A;
    Vector3 p1 = tri.B;
    Vector3 p2 = tri.C;
    
    Rect bbox = ObtainTriangleBBox(tri);
    r32 area = EdgeFunction(p2, p1, p0);

    for (s32 j = Floor(bbox.min_y); j < Floor(bbox.max_y); j++)
    {
            
        for (s32 i = Floor(bbox.min_x); i < Floor(bbox.max_x); i++)
        {
            Vector3 p = vec_make(i+0.5f, j+0.5f, 0.0f);
            r32 w0 = EdgeFunction(p, p2, p1);
            r32 w1 = EdgeFunction(p, p0, p2);
            r32 w2 = EdgeFunction(p, p1, p0);

            if (w0 >= 0 && w1 >= 0 && w2 >= 0) // inside test
            {
                w0 /= area; // these are barycentric coords
                w1 /= area;
                w2 /= area;

                r32 z = 1/(w0/p0.z+w1/p1.z+w2/p2.z);
                r32* zbuffer_point = zbuffer_access(i, j);

                u32 r = (u32)(w0*255);
                u32 g = (u32)(w1*255);
                u32 b = (u32)(w2*255);

                u32 intColor = (((u32)255 << 24) | (r << 16) | (g << 8) | b);
                            
                // if (z >= *zbuffer_point)
                if (z < *zbuffer_point)
                {
                    *framebuffer_access(i, j) = (inv ? intColor : color);
                    *zbuffer_point = z;
                }
            }
        }
    }
}

Triangle TriangleWorldToRaster(Triangle tri)
{
    Triangle result;

    // camera space
    
    Vector3 A = tri.A;
    Vector3 B = tri.B;
    Vector3 C = tri.C;
    
    r32 Near = Z_NEAR;
    
    r32 invA = Near/A.z;
    r32 invB = Near/B.z;
    r32 invC = Near/C.z;
    Vector3 A_s = vec_make(A.x*invA, A.y*invA, A.z);
    Vector3 B_s = vec_make(B.x*invB, B.y*invB, B.z);
    Vector3 C_s = vec_make(C.x*invC, C.y*invC, C.z);
    
    r32 fov = FOV;
    r32 aspect_ratio = (FRAMEBUFFER_WIDTH*1.0f)/FRAMEBUFFER_HEIGHT;

    r32 r = -Near*tan(radians(fov/2));
    r32 l = -r;
    r32 t = r/aspect_ratio; // is it the opposite here actually, first t then r?
    r32 b = -t;
    
    // clip space (before converting from homogeneous to cartesian, which
    // does the perspective divide essentialy)

    // ndc space
    
    Vector3 A_ndc = vec_make(A_s.x/r, A_s.y/t, A_s.z);
    Vector3 B_ndc = vec_make(B_s.x/r, B_s.y/t, B_s.z);
    Vector3 C_ndc = vec_make(C_s.x/r, C_s.y/t, C_s.z);

    /* v3 A_r = V3((A_ndc.x+1)/2*(FRAME_BUFFER_WIDTH-1), (1-(A_ndc.y+1)/2)*(FRAME_BUFFER_HEIGHT-1), A_ndc.z); */
    /* v3 B_r = V3((B_ndc.x+1)/2*(FRAME_BUFFER_WIDTH-1), (1-(B_ndc.y+1)/2)*(FRAME_BUFFER_HEIGHT-1), B_ndc.z); */
    /* v3 C_r = V3((C_ndc.x+1)/2*(FRAME_BUFFER_WIDTH-1), (1-(C_ndc.y+1)/2)*(FRAME_BUFFER_HEIGHT-1), C_ndc.z); */

    // raster space
    
    Vector3 A_r = vec_make((A_ndc.x+1)/2*(FRAMEBUFFER_WIDTH-1), (A_ndc.y+1)/2*(FRAMEBUFFER_HEIGHT-1), A_ndc.z);
    Vector3 B_r = vec_make((B_ndc.x+1)/2*(FRAMEBUFFER_WIDTH-1), (B_ndc.y+1)/2*(FRAMEBUFFER_HEIGHT-1), B_ndc.z);
    Vector3 C_r = vec_make((C_ndc.x+1)/2*(FRAMEBUFFER_WIDTH-1), (C_ndc.y+1)/2*(FRAMEBUFFER_HEIGHT-1), C_ndc.z);

    result.A = A_r;
    result.B = B_r;
    result.C = C_r;
    
    return result;
}


inline void matrix_proj(Matrix4* proj)
{
    matrix_unit(proj);

    // @todo unsigned vs signed mismatch?
    // make rad, deg, kb, mb, gb..?
    r32 aspectRatio = (FRAMEBUFFER_WIDTH*1.0f) / FRAMEBUFFER_HEIGHT;
    r32 fov = FOV;
    
    r32 n = Z_NEAR;
    r32 f = Z_FAR;
    r32 t = tan(radians(fov / 2)) * n;
    r32 r = t * aspectRatio;

    proj->e[0][0] = n / r;
    proj->e[1][1] = n / t;
    proj->e[2][2] = - (f + n) / (f - n);
    proj->e[3][3] = 0;
    proj->e[2][3] = - 1;
    proj->e[3][2] = - (2 * f * n) / (f - n);
}

s32 TriangleWorldToRasterPROJ(Triangle* tri, TriangleHom* out_triangles)
{
    Matrix4* PROJ = arena_push<Matrix4>(&TEMPORARY_ARENA);
    matrix_proj(PROJ);

    Vector3 A_clip3 = matrix_mul(PROJ, tri->A);
    Vector3 B_clip3 = matrix_mul(PROJ, tri->B);
    Vector3 C_clip3 = matrix_mul(PROJ, tri->C);

    Vector4 A_clip = vec_make(A_clip3.x, A_clip3.y, A_clip3.z, -tri->A.z);
    Vector4 B_clip = vec_make(B_clip3.x, B_clip3.y, B_clip3.z, -tri->B.z);
    Vector4 C_clip = vec_make(C_clip3.x, C_clip3.y, C_clip3.z, -tri->C.z);

    TriangleHom* tri_clip = arena_push<TriangleHom>(&TEMPORARY_ARENA);
    s32 count = ClipTriangle(tri_clip, out_triangles);
    for (s32 i = 0; i < count; i++)
    {
        TriangleHom t = out_triangles[i];
            
        Vector4 A_ndc = vec_make(t.A.x/t.A.w, t.A.y/t.A.w, t.A.z/t.A.w, 1.0f);
        Vector4 B_ndc = vec_make(t.B.x/t.B.w, t.B.y/t.B.w, t.B.z/t.B.w, 1.0f);
        Vector4 C_ndc = vec_make(t.C.x/t.C.w, t.C.y/t.C.w, t.C.z/t.C.w, 1.0f);

        Vector4 A_r = vec_make((A_ndc.x+1)/2*(FRAMEBUFFER_WIDTH-1), (A_ndc.y+1)/2*(FRAMEBUFFER_HEIGHT-1), A_ndc.z, 1.0f);
        Vector4 B_r = vec_make((B_ndc.x+1)/2*(FRAMEBUFFER_WIDTH-1), (B_ndc.y+1)/2*(FRAMEBUFFER_HEIGHT-1), B_ndc.z, 1.0f);
        Vector4 C_r = vec_make((C_ndc.x+1)/2*(FRAMEBUFFER_WIDTH-1), (C_ndc.y+1)/2*(FRAMEBUFFER_HEIGHT-1), C_ndc.z, 1.0f);

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




#include "test2.cpp"

void fill_background()
{
    for (s32 y = 0; y < FRAMEBUFFER_HEIGHT; y++)
    {
        for (s32 x = 0; x < FRAMEBUFFER_WIDTH; x++)
        {
            *framebuffer_access(x, y) = ((u32)255 << 24) | ((u32)125 << 16) | ((u32)0 << 8) | ((u32)125);
        }
    }
}

extern "C" void update_and_render()
{
    Engine_state* engine_state;
    b32 inited = false;
    if (!inited)
    {
        init_memory();
        init_engine_state();

        engine_state = ENGINE_STATE;
        engine_state->inited = true;
        inited = true;
    }
    engine_state = ENGINE_STATE;
    
    // @todo maybe just cache through engineState directly
    Framebuffer framebuffer = FRAMEBUFFER;
    r32* zbuffer = ZBUFFER;
    s32 bytpp = FRAMEBUFFER_BYTPP;
    s32 pitch = framebuffer_pitch(framebuffer.height, bytpp);
    
    fill_background();
    
    test();
    
    zbuffer_reset(zbuffer, pitch, framebuffer.width, framebuffer.height);
}

