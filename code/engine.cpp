// @Note about 5.8 MB will be unused when windowed on low resolution..

// @TODO I think making a origin as alias of v3 is useful visually

inline void MatrixToStr(u8* buff, m4 M)
{
    s32 pos = 0;
    for (s32 i = 0; i < 4; i++)
        {
            for (s32 j = 0; j < 4; j++)
                {
                    pos += sprintf((char* const)(buff+pos), "%8.2f", M.m[i][j]);
                }
            pos += sprintf((char* const)(buff+pos), "\n");
        }
}

inline r32 slope2(v2 vec1, v2 vec2)
{
    r32 result;

    result = (vec1.y - vec2.y) / (vec1.x / vec2.x);
    
    return result;
}

/* else if (x < 0 && y < 0) // 3rd quadrant */
/*     { */
/*         if (y0 >= 0) */
/*             { */
/*                 r32 y0c = clamp(y0, 0, wnd_height); */
/*                 result = V2(0, y0c); */
/*             } */
/*         else */
/*             { */
/*                 r32 x0c = clamp(x0, 0, wnd_width); */
/*                 result = V2(x0c, 0); */
/*             } */
/*     } */

// @TODO potentially say if lines on the outline of wndrect don't draw
// or clamp lines in a different way

// essentialy the problem there is that with rectangles you draw based
// on whether width or height exists, for triangles you can draw based
// on whether all 3 points are on a line, but for a line you can't
// do these things because a line doesn't have an Area, only a Length

// so you need to clamp lines differently, by reducing their length instead,
// which you do by clamping towards the other end of the line, not
// perpendicularly towards the window

typedef union tagLine
{
    struct
    {
        r32 x0;
        r32 y0;
        r32 x1;
        r32 y1;
    };
    struct
    {
        v2 P;
        v2 Q;
    };
} line;

typedef enum tagREGIONCODE
    {
        REGIONCODE_INSIDE = 0,
        REGIONCODE_LEFT   = (1 << 0),
        REGIONCODE_RIGHT  = (1 << 1),
        REGIONCODE_BOTTOM = (1 << 2),
        REGIONCODE_TOP    = (1 << 3)
    } REGIONCODE;

inline u8 ObtainRegionFlags(v2 point)
{
    u8 result = REGIONCODE_INSIDE;

    if (point.x < 0)
        {
            result |= (u8)REGIONCODE_LEFT;
        }
    else if (point.x >= wnd_width)
        {
            result |= (u8)REGIONCODE_RIGHT;
        }
    if (point.y < 0)
        {
            result |= (u8)REGIONCODE_BOTTOM;
        }
    else if (point.y >= wnd_height)
        {
            result |= (u8)REGIONCODE_TOP;
        }

    return result;
}

b32 clip_line(line* Line)
{
    b32 to_draw = false;

    u8 P_rflags = ObtainRegionFlags(Line->P);
    u8 Q_rflags = ObtainRegionFlags(Line->Q);

    while (true)
        {
            if (!(P_rflags | Q_rflags))   // both inside viewport, draw
                {
                    to_draw = true;
                    break;
                }
            else if (P_rflags & Q_rflags) // both outside viewport, don't draw
                {
                    break;
                }
            else                          // one outside, clip then iterate
                {
                    r32 ClippedX, ClippedY;
                    u8 rflags;
                    if (P_rflags > Q_rflags) // figure out which is outside
                        {
                            rflags = P_rflags;
                        }
                    else
                        {
                            rflags = Q_rflags;
                        }
                    
                    if (rflags & REGIONCODE_TOP) // clip
                        {
                            ClippedX = Line->x0 + (Line->x1 - Line->x0) * (wnd_height-1 - Line->y0) / (Line->y1 - Line->y0);
                            ClippedY = wnd_height-1;
                        }
                    else if (rflags & REGIONCODE_BOTTOM)
                        {
                            ClippedX = Line->x0 + (Line->x1 - Line->x0) * (0 - Line->y0) / (Line->y1 - Line->y0);
                            ClippedY = 0;
                        }
                    else if (rflags & REGIONCODE_RIGHT)
                        {
                            ClippedX = wnd_width-1;
                            ClippedY = Line->y0 + (Line->y1 - Line->y0) * (wnd_width-1 - Line->x0) / (Line->x1 - Line->x0);
                        }
                    else if (rflags & REGIONCODE_LEFT)
                        {
                            ClippedX = 0;
                            ClippedY = Line->y0 + (Line->y1 - Line->y0) * (0 - Line->x0) / (Line->x1 - Line->x0);
                        }

                    if (rflags == P_rflags) // update
                        {
                            Line->P.x = ClippedX;
                            Line->P.y = ClippedY;
                            P_rflags = ObtainRegionFlags(Line->P);
                        }
                    else
                        {
                            Line->Q.x = ClippedX;
                            Line->Q.y = ClippedY;
                            Q_rflags = ObtainRegionFlags(Line->Q);
                        }
                }
        }
    
    return to_draw;
}


// @Note this is actually bad because it clamps perpendicular
// to the window, instead of toward the direction of the vector

// so you actually have to clamp towards some other origin
inline v2 clamp2(v2 vec)
{
    v2 result = vec;

    // @Note this is iffy (of by one andstuffs...)
    clamp(result.x, 0.0f, (r32)wnd_width - 1);
    clamp(result.y, 0.0f, (r32)wnd_height - 1);
    
    return result;
}

// @Note this is actually bad, because of the previous, but also
// because clamping Z like this is a special case where you are
// XY-plane aligned, I explained this in another note
inline v3 clamp3(v3 vec)
{
    v3 result = vec;

    // @Note this is iffy (of by one andstuffs...)
    clamp(result.x, 0.0f, (r32)wnd_width - 1);
    clamp(result.y, 0.0f, (r32)wnd_height - 1);
    clamp(result.z, (r32)wnd_nearclip, (r32)wnd_farclip); // @TODO is this correct?
    
    return result;
}

inline wndrect clamp_wndrect(wndrect rect)
{
    wndrect result = rect;

    // @Note this is iffy (of by one andstuffs...)
    clamp(result.left, 0.0f, (r32)wnd_width - 1);
    clamp(result.bottom, 0.0f, (r32)wnd_height - 1);
    clamp(result.right, 0.0f, (r32)wnd_width - 1);
    clamp(result.top, 0.0f, (r32)wnd_height - 1);
    
    return result;
}

// @TODO probably put this into .c instead and don't inline

                         //   z         y         x
inline v3 rotate3(v3 vec, r32 yaw, r32 pitch, r32 roll)
{
    v3 result = zero3();

    r32 x = vec.x;
    r32 y = vec.y;
    r32 z = vec.z;
    
    r32 sA = sin(yaw);
    r32 cA = cos(yaw);

    r32 sB = sin(pitch);
    r32 cB = cos(pitch);

    r32 sG = sin(roll);
    r32 cG = cos(roll);

    
    /* result.x = x * (cA*cB) + y * (cA*sB*sG-sA*cG) + z * (cA*sB*cG+sA*sG); */
    /* result.y = x * (sA*cB) + y * (sA*sB*sG+cA*cG) + z * (sA*sB*cG-cA*sG); */
    /* result.z = x * (-sB)   + y * (cB*sG)          + z * (cB*cG); */

    result.x = x * (cA*cB) + y * (cA*sB*sG-sA*cG) + z * (cA*sB*cG+sA*sG);
    result.y = x * (sA*cB) + y * (sA*sB*sG+cA*cG) + z * (sA*sB*cG-cA*sG);
    result.z = x * (-sB)   + y * (cB*sG)          + z * (cB*cG);
    
    return result;
}

void draw_clamped_zbuffered_wndrect(wndrect rect, r32 z, pxl color)
{
    s32 offset = wnd_pitch*Round(rect.bottom) +
        Round(rect.left)*wnd_bytpp;
    s32 height = Round(wndrect_height(rect));
    s32 width = Round(wndrect_width(rect));

    for (s32 i = 0; i < height; i++)
        {
            pxl* row = (pxl*)(wnd_buffer + wnd_pitch*i + offset);
            for (s32 j = 0; j < width; j++)
                {
                    r32* zbuffer_point = (r32*)(zbuffer + wnd_pitch*i + wnd_bytpp*j + offset);
                    if (z < *zbuffer_point)
                        {
                            *row = color;
                            *zbuffer_point = z;
                        }
                    row++;
                }
        }    
}

void draw_clamped_wndrect(wndrect rect, pxl color)
{
    // @TODO do I round?
    s32 offset = wnd_pitch*Round(rect.bottom) +
        Round(rect.left)*wnd_bytpp;
    s32 height = Round(wndrect_height(rect));
    s32 width = Round(wndrect_width(rect));
    
    for (s32 i = 0; i < height; i++)
        {
            pxl* row = (pxl*)(wnd_buffer + wnd_pitch*i + offset);
            for (s32 j = 0; j < width; j++)
                {
                    *row = color;
                    row++;
                }
        }
}

void draw_rotated_rect(s32 width, s32 height, v2 origin, pxl color)
{
    // @Note using clmap2 so you draw along the edge...
    for (s32 i = 0; i < height; i++)
        {
            for (s32 j = 0; j < width; j++)
                {
                    v2 pxl_point = V2(j, i - height); // do I want - here
                    pxl_point = rotate2(pxl_point, Gamestate->rect_angle);
                    pxl_point.x = Floor(pxl_point.x);
                    pxl_point.y = Floor(pxl_point.y);
                    pxl_point = add2(pxl_point, origin);
                    pxl_point = clamp2(pxl_point);
                    pxl* draw_point = (pxl*)(wnd_buffer +
                                             wnd_pitch*(s32)pxl_point.y +
                                             wnd_bytpp*(s32)pxl_point.x);
                    *draw_point = color;
                }
        }       
}


// transpose point to coordsys of camera
// by offseting by fpoint, then rotating
// in 3d by yaw, pitch, roll

v2 project(v3 point, PROJECTION P)
{
    v2 result = zero2();

    Camera camera = Gamestate->camera;
    
    point = transpose3(point, zero3(), camera.fpoint);
    point = rotate3(point, camera.yaw, camera.pitch, camera.roll);
    
    switch (P)
        {
        case ORTHOGRAPHIC:
            {
                result = V2(point.x, point.y);                        
            } break;
        case PERSPECTIVE:
            {
                r32 scaling_factor = Gamestate->new_screen_z / point.z;

                /* result.x = Gamestate->eye_x + scaling_factor * (point.x - Gamestate->eye_x); */
                /* result.y = Gamestate->eye_y + scaling_factor * (point.y - Gamestate->eye_y); */
                
                result.x = Gamestate->eye_x + scaling_factor * point.x;
                result.y = Gamestate->eye_y + scaling_factor * point.y;
                
                /* r32 scaling_factor = Gamestate->screen_z / point.z; */
                /* v2 focus = V2(Gamestate->eye_x, Gamestate->eye_y); */
                /* v2 point2 = V2(point.x, point.y); */

                /* // @TODO maybe it's actually better if you write two lines instead */
                /* result = sub2(focus, scale2(sub2(focus, point2), scaling_factor)); */

                /* result.left   = Gamestate->eye_x - scaling_factor * (Gamestate->eye_x - rect.left); */
                /* result.bottom = Gamestate->eye_y - scaling_factor * (Gamestate->eye_y - rect.bottom); */
                /* result.right  = Gamestate->eye_x - scaling_factor * (Gamestate->eye_x - rect.right); */
                /* result.top    = Gamestate->eye_y - scaling_factor * (Gamestate->eye_y - rect.top); */

            } break;
        default:
            {
                ASSERT(0); // invalid default case
            }
        }
    
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
                            r32* zbuffer_point = (r32*)(zbuffer+wnd_pitch*j+wnd_bytpp*i);

                            u32 r = (u32)(w0*255);
                            u32 g = (u32)(w1*255);
                            u32 b = (u32)(w2*255);

                            u32 intColor = (((u32)255 << 24) | (r << 16) | (g << 8) | b);
                            
                            // if (z >= *zbuffer_point)
                            if (z < *zbuffer_point)
                                {
                                    *((u32*)(wnd_buffer+j*wnd_pitch+i*wnd_bytpp)) = (inv ? intColor : color);
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
    
    r32 Near = Gamestate->cameraParams._near;

    r32 invA = Near/A.z;
    r32 invB = Near/B.z;
    r32 invC = Near/C.z;
    v3 A_s = V3(A.x*invA, A.y*invA, A.z);
    v3 B_s = V3(B.x*invB, B.y*invB, B.z);
    v3 C_s = V3(C.x*invC, C.y*invC, C.z);
    
    r32 Fov = Gamestate->cameraParams.fov;
    r32 aspect_ratio = (wnd_width*1.0f)/wnd_height;

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

    /* v3 A_r = V3((A_ndc.x+1)/2*(wnd_width-1), (1-(A_ndc.y+1)/2)*(wnd_height-1), A_ndc.z); */
    /* v3 B_r = V3((B_ndc.x+1)/2*(wnd_width-1), (1-(B_ndc.y+1)/2)*(wnd_height-1), B_ndc.z); */
    /* v3 C_r = V3((C_ndc.x+1)/2*(wnd_width-1), (1-(C_ndc.y+1)/2)*(wnd_height-1), C_ndc.z); */

    // raster space
    
    v3 A_r = V3((A_ndc.x+1)/2*(wnd_width-1), (A_ndc.y+1)/2*(wnd_height-1), A_ndc.z);
    v3 B_r = V3((B_ndc.x+1)/2*(wnd_width-1), (B_ndc.y+1)/2*(wnd_height-1), B_ndc.z);
    v3 C_r = V3((C_ndc.x+1)/2*(wnd_width-1), (C_ndc.y+1)/2*(wnd_height-1), C_ndc.z);

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

    r32 aspectRatio = (wnd_width*1.0f) / wnd_height;
    r32 fov = Gamestate->cameraParams.fov;
    
    r32 n = Gamestate->cameraParams._near;
    r32 f = Gamestate->cameraParams._far;
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

            v4 A_r = V4((A_ndc.x+1)/2*(wnd_width-1), (A_ndc.y+1)/2*(wnd_height-1), A_ndc.z, 1);
            v4 B_r = V4((B_ndc.x+1)/2*(wnd_width-1), (B_ndc.y+1)/2*(wnd_height-1), B_ndc.z, 1);
            v4 C_r = V4((C_ndc.x+1)/2*(wnd_width-1), (C_ndc.y+1)/2*(wnd_height-1), C_ndc.z, 1);

            t.A = A_r;
            t.B = B_r;
            t.C = C_r;
            out_triangles[i] = t;
        }
    
    return count;
}

v3 project_new2(v3 point, PROJECTION P)
{
    v3 result = zero3();

    Camera camera = Gamestate->camera;

    // this transform is not good.....
    point = transpose3(point, zero3(), camera.fpoint);
    point = rotate3(point, camera.yaw, camera.pitch, camera.roll);
    result.z = point.z;
    
    switch (P)
        {
        case ORTHOGRAPHIC:
            {
                result.x = point.x;
                result.y = point.y;
            } break;
        case PERSPECTIVE:
            {
                if (point.z > Gamestate->nearclip)
                    {
                        break;
                    }
                r32 scaling_factor = Gamestate->new_screen_z / point.z;
                
                result.x = Gamestate->eye_x + scaling_factor * point.x;
                result.y = Gamestate->eye_y + scaling_factor * point.y;
                
                /* result.x = scaling_factor * point.x; */
                /* result.y = scaling_factor * point.y; */
            } break;
        default:
            {
                ASSERT(0); // invalid default case
            }
        }
    
    return result;
}

void draw_wndline_zbuffered_aa(v2 P0, v2 P1, r32 z, u32 color)
{
    b32 steep = abs(P1.y - P0.y) > abs(P1.x - P0.x);

    if (steep)
        {
            r32 tmp = P0.x;
            P0.x = P0.y;
            P0.y = tmp;

            tmp = P1.x;
            P1.x = P1.y;
            P1.y = tmp;
        }
    if (P0.x > P1.x)
        {
            r32 tmp = P0.x;
            P0.x = P1.x;
            P1.x = tmp;
            
            tmp = P0.y;
            P0.y = P1.y;
            P1.y = tmp;
        }

    r32 dx = P1.x - P0.x;
    r32 dy = P1.y - P0.y;
    r32 k = dy / dx;
    if (dx == 0.0f)
        {
            k = 1; // why exactly?
        }

    s32 xstart = Floor(P0.x); // round here?
    s32 xend = Floor(P1.x);
    r32 intersectY = P0.y;

    s32 xoffset;
    s32 yoffset;
    
    if (steep)
        {
            xoffset = wnd_pitch;
            yoffset = wnd_bytpp;
        }
    else
        {
            xoffset = wnd_bytpp;
            yoffset = wnd_pitch;
        }

    // clip xstart and xend here
    // also clip intersectY every iteration
    // the tricky part is probably figuring out all of the proper swaps...
    
    for (s32 x = xstart; x <= xend; x++)
        {
            r32* Z = (r32*)(zbuffer + floor_to_int(intersectY)*yoffset + x*xoffset);
            if (z < *Z ||     // this stuff was other way when Z was other way, also >= on first condition
                z > Gamestate->nearclip ||
                z < Gamestate->farclip)
                {
                    continue;
                }
            else
                {
                    *Z = z;
                }
            
            // @IMPORTANT do we actually want to use 256 instead of 255, and in that case we can shift instead...
         
            
            u32* drawing_point1 = (u32*)(wnd_buffer + floor_to_int(intersectY) * yoffset + x * xoffset);
            u32* drawing_point2 = (u32*)(wnd_buffer + (floor_to_int(intersectY) - 1) * yoffset + x * xoffset);

            r32 alpha1 = 1 - decimal(intersectY);
            r32 alpha2 = decimal(intersectY);

            u32 color1 = *drawing_point1;
            u32 color2 = *drawing_point2;
                    
            r32 r1 = ((u8)((color1 & 0x00FF0000) >> 16)) / 255.0;
            r32 g1 = ((u8)((color1 & 0x0000FF00) >> 8)) / 255.0;
            r32 b1 = ((u8)(color1 & 0x000000FF)) / 255.0;

            r32 r2 = ((u8)((color2 & 0x00FF0000) >> 16)) / 255.0;
            r32 g2 = ((u8)((color2 & 0x0000FF00) >> 8)) / 255.0;
            r32 b2 = ((u8)(color2 & 0x000000FF)) / 255.0;

            r32 rc = ((u8)((color & 0x00FF0000) >> 16)) / 255.0;
            r32 gc = ((u8)((color & 0x0000FF00) >> 8)) / 255.0;
            r32 bc = ((u8)(color & 0x000000FF)) / 255.0;
                    
            r1 = lerp(rc, r1, alpha1);
            g1 = lerp(gc, g1, alpha1);
            b1 = lerp(bc, b1, alpha1);

            r2 = lerp(rc, r2, alpha2);
            g2 = lerp(gc, g2, alpha2);
            b2 = lerp(bc, b2, alpha2);

            u32 R1 = (u32)(r1 * 255);
            u32 G1 = (u32)(g1 * 255);
            u32 B1 = (u32)(b1 * 255);

            u32 R2 = (u32)(r2 * 255);
            u32 G2 = (u32)(g2 * 255);
            u32 B2 = (u32)(b2 * 255);
                    
            u32 final1 = ((255 << 24) |
                          (R1 << 16) |
                          (G1 << 8) |   
                          B1);        

            u32 final2 = ((255 << 24) |
                          (R2 << 16) |
                          (G2 << 8) |   
                          B2);        

            *drawing_point1 = final1;
            *drawing_point2 = final2;
                    
            intersectY += k;
        }
}

// here goes anti-aliased line draw.....

void draw_wndline_aa(v2 P0, v2 P1, u32 color)
{
    b32 steep = abs(P1.y - P0.y) > abs(P1.x - P0.x);

    if (steep)
        {
            r32 tmp = P0.x;
            P0.x = P0.y;
            P0.y = tmp;

            tmp = P1.x;
            P1.x = P1.y;
            P1.y = tmp;
        }
    if (P0.x > P1.x)
        {
            r32 tmp = P0.x;
            P0.x = P1.x;
            P1.x = tmp;
            
            tmp = P0.y;
            P0.y = P1.y;
            P1.y = tmp;
        }

    r32 dx = P1.x - P0.x;
    r32 dy = P1.y - P0.y;
    r32 k = dy / dx;
    if (dx == 0.0f)
        {
            k = 1; // why exactly?
        }

    s32 xstart = Floor(P0.x); // round here?
    s32 xend = Floor(P1.x);
    r32 intersectY = P0.y;

    s32 xoffset;
    s32 yoffset;
    
    if (steep)
        {
            xoffset = wnd_pitch;
            yoffset = wnd_bytpp;
        }
    else
        {
            xoffset = wnd_bytpp;
            yoffset = wnd_pitch;
        }

    // clip xstart and xend here
    // also clip intersectY every iteration
    // the tricky part is probably figuring out all of the proper swaps...
    
    for (s32 x = xstart; x <= xend; x++)
        {
            // @IMPORTANT do we actually want to use 256 instead of 255, and in that case we can shift instead...
         
            
            u32* drawing_point1 = (u32*)(wnd_buffer + floor_to_int(intersectY) * yoffset + x * xoffset);
            u32* drawing_point2 = (u32*)(wnd_buffer + (floor_to_int(intersectY) - 1) * yoffset + x * xoffset);

            r32 alpha1 = 1 - decimal(intersectY);
            r32 alpha2 = decimal(intersectY);

            u32 color1 = *drawing_point1;
            u32 color2 = *drawing_point2;
                    
            r32 r1 = ((u8)((color1 & 0x00FF0000) >> 16)) / 255.0;
            r32 g1 = ((u8)((color1 & 0x0000FF00) >> 8)) / 255.0;
            r32 b1 = ((u8)(color1 & 0x000000FF)) / 255.0;

            r32 r2 = ((u8)((color2 & 0x00FF0000) >> 16)) / 255.0;
            r32 g2 = ((u8)((color2 & 0x0000FF00) >> 8)) / 255.0;
            r32 b2 = ((u8)(color2 & 0x000000FF)) / 255.0;

            r32 rc = ((u8)((color & 0x00FF0000) >> 16)) / 255.0;
            r32 gc = ((u8)((color & 0x0000FF00) >> 8)) / 255.0;
            r32 bc = ((u8)(color & 0x000000FF)) / 255.0;
                    
            r1 = lerp(rc, r1, alpha1);
            g1 = lerp(gc, g1, alpha1);
            b1 = lerp(bc, b1, alpha1);

            r2 = lerp(rc, r2, alpha2);
            g2 = lerp(gc, g2, alpha2);
            b2 = lerp(bc, b2, alpha2);

            u32 R1 = (u32)(r1 * 255);
            u32 G1 = (u32)(g1 * 255);
            u32 B1 = (u32)(b1 * 255);

            u32 R2 = (u32)(r2 * 255);
            u32 G2 = (u32)(g2 * 255);
            u32 B2 = (u32)(b2 * 255);
                    
            u32 final1 = ((255 << 24) |
                          (R1 << 16) |
                          (G1 << 8) |   
                          B1);        

            u32 final2 = ((255 << 24) |
                          (R2 << 16) |
                          (G2 << 8) |   
                          B2);        

            *drawing_point1 = final1;
            *drawing_point2 = final2;
                    
            intersectY += k;
        }
}

void draw_line(v2 origin, v2 point, u32 color)
{
    // @TODO potentially say if lines on the outline of wndrect don't draw
    // or clamp lines in a different way

    // @TODO implement anti-aliasing
    
    u8* offset = wnd_buffer + wnd_pitch*round_to_int(origin.y) + wnd_bytpp*round_to_int(origin.x);
    u8* drawing_point = 0;
    
    r32 dX = point.x;
    r32 dY = point.y;
    s32 movebyX = 0;
    s32 movebyY = 0;
    r32 len = edist2(origin, point);
    r32 whenX = len / dX; // @Note divide by zero here??
    r32 whenY = len / dY;

    
    for (s32 i = 1; i <= Floor(len); i++)
        {
            movebyX = Floor(i/whenX);
            movebyY = Floor(i/whenY);
            drawing_point = offset + wnd_pitch*movebyY + wnd_bytpp*movebyX;
                *((u32*)drawing_point) = color;
        }
}

void fill_background()
{
    for (s32 i = 0; i < wnd_height; i++)
        {
            u32* row = (u32*)(wnd_buffer + i*wnd_pitch);
            for (s32 j = 0; j < wnd_width; j++)
                {
                    *row = Gamestate->brushes[BRUSH_NONE];
                    row++;
                }
        }    
}

void dbg_render(s8 x_offset, s8 y_offset)
{
    for (s32 i = 0; i < wnd_height; i++)
        {
            pxl* row = (pxl*)(wnd_buffer + i*wnd_pitch);
            for (s32 j = 0; j < wnd_width; j++)
                {
                    u8 g = j + x_offset;
                    u8 b = i + y_offset;

                    // *row = literal(pxl) {.R=0, .G=g, .B=b, .A=255};
                    *row = PXL(0, g, b, 255);
                    
                    row++;
                }
        }    
}

void dbg_draw_square_around_cursor(r32 square_length)
{
    pxl color = PXL(0, 0, 0, 255);
    r32 half_square_length = square_length/2;
    
    r32 left   = Gamestate->cursor.x - half_square_length;
    r32 bottom = Gamestate->cursor.y - half_square_length;
    r32 right  = Gamestate->cursor.x + half_square_length;
    r32 top    = Gamestate->cursor.y + half_square_length;

    /* r32 tmp = top; */
    /* top = to_yisdown(bottom); */
    /* bottom = to_yisdown(tmp); */
    
    wndrect rect = Wndrect(left, bottom, right, top);
    rect = clamp_wndrect(rect);
    draw_clamped_wndrect(rect, color);
}

wndrect project_wndrect(wndrect rect)
{
    wndrect result;

    s32 concentric_current_z = Gamestate->concentric_current_z;
    r32 rect_z = Gamestate->concentric_z_values[concentric_current_z];
    r32 screen_z = Gamestate->screen_z;
    r32 scaling_factor = screen_z / rect_z;

    result.left   = Gamestate->eye_x - scaling_factor * (Gamestate->eye_x - rect.left);
    result.bottom = Gamestate->eye_y - scaling_factor * (Gamestate->eye_y - rect.bottom);
    result.right  = Gamestate->eye_x - scaling_factor * (Gamestate->eye_x - rect.right);
    result.top    = Gamestate->eye_y - scaling_factor * (Gamestate->eye_y - rect.top);
    
    return result;
}

void draw_wndrect_outline_projected(wndrect rect, s32 thickness, pxl color)
{
    wndrect rect_left   = Wndrect(rect.left,
                                  rect.bottom,
                                  rect.left + thickness,
                                  rect.top);

    rect_left = project_wndrect(rect_left);
    rect_left = clamp_wndrect(rect_left);
    
    wndrect rect_bottom = Wndrect(rect.left,
                                  rect.bottom,
                                  rect.right,
                                  rect.bottom + thickness);

    rect_bottom = project_wndrect(rect_bottom);
    rect_bottom = clamp_wndrect(rect_bottom);
 
    wndrect rect_right  = Wndrect(rect.right - thickness,
                                  rect.bottom,
                                  rect.right,
                                  rect.top);

    rect_right = project_wndrect(rect_right);
    rect_right = clamp_wndrect(rect_right);
    
    wndrect rect_top    = Wndrect(rect.left,
                                  rect.top - thickness,
                                  rect.right,
                                  rect.top);

    rect_top = project_wndrect(rect_top);
    rect_top = clamp_wndrect(rect_top);

    draw_clamped_wndrect(rect_left, color);
    draw_clamped_wndrect(rect_bottom, color);
    draw_clamped_wndrect(rect_right, color);
    draw_clamped_wndrect(rect_top, color);
}

void draw_wndrect_outline(wndrect rect, s32 thickness, pxl color)
{
    wndrect rect_left   = Wndrect(rect.left,
                                  rect.bottom,
                                  rect.left + thickness,
                                  rect.top);
    
    rect_left = clamp_wndrect(rect_left);
    
    wndrect rect_bottom = Wndrect(rect.left,
                                  rect.bottom,
                                  rect.right,
                                  rect.bottom + thickness);
    
    rect_bottom = clamp_wndrect(rect_bottom);
 
    wndrect rect_right  = Wndrect(rect.right - thickness,
                                  rect.bottom,
                                  rect.right,
                                  rect.top);
    
    rect_right = clamp_wndrect(rect_right);
    
    wndrect rect_top    = Wndrect(rect.left,
                                  rect.top - thickness,
                                  rect.right,
                                  rect.top);
    
    rect_top = clamp_wndrect(rect_top);

    draw_clamped_wndrect(rect_left, color);
    draw_clamped_wndrect(rect_bottom, color);
    draw_clamped_wndrect(rect_right, color);
    draw_clamped_wndrect(rect_top, color);
}

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
    
    u64 prev_kflags = Gamestate->keyflags;
    Gamestate->keyflags |= curr_keyflags_to_set;
    Gamestate->keyflags &= ~curr_keyflags_to_unset;
    u64 kflags = Gamestate->keyflags;
    u64 kflags_trans = kflags ^ prev_kflags;
    u64 kflags_trans_to_up = kflags_trans & prev_kflags;
    u64 kflags_trans_to_down = kflags_trans & (~prev_kflags);
    
    u8 prev_mflags = Gamestate->mouseflags;
    Gamestate->mouseflags |= curr_mouseflags_to_set;
    Gamestate->mouseflags &= ~curr_mouseflags_to_unset;
    u8 mflags = Gamestate->mouseflags;
    u8 mflags_trans = mflags ^ prev_mflags;
    u8 mflags_trans_to_up = mflags_trans & prev_mflags;
    u8 mflags_trans_to_down = mflags_trans & (~prev_mflags);

    Camera camera = Gamestate->camera;
    curr_cursor.y = to_yisup(curr_cursor.y);
    v2 cursor_difference;
    cursor_difference.x = curr_cursor.x - wnd_width/2;
    cursor_difference.y = curr_cursor.y - wnd_height/2;
    camera.roll += cursor_difference.y / kilobytes(1);
    camera.pitch -= cursor_difference.x / kilobytes(1);
    Gamestate->cursor = curr_cursor;

    if (ExtractKey(kflags_trans_to_up, KEY_U))
        {
            if (Gamestate->reverse_winding)
                {
                    Gamestate->reverse_winding = false;
                }
            else
                {
                    Gamestate->reverse_winding = true;
                }
        }
    
    if (ExtractKey(kflags, KEY_D))
        {
            // Gamestate->camera_angle += pi/256;
            Gamestate->log_to_file_once = true;
        }

    if (ExtractKey(kflags, KEY_A))
        {            
            // Gamestate->camera_angle -= pi/256;
            Gamestate->log_to_file_once = true;
        }
    
    if (ExtractKey(kflags, KEY_UP))
        {
            Gamestate->camera_offs_y += 5.0f;
            Gamestate->wnd_center_y+= 5;
            Gamestate->dbg_render_y_offset+= 10;
        }
    if (ExtractKey(kflags, KEY_DOWN))
        {
            Gamestate->camera_offs_y -= 5.0f;            
            Gamestate->wnd_center_y-= 5;
            Gamestate->dbg_render_y_offset-= 10;
        }
    if (ExtractKey(kflags, KEY_LEFT))
        {
            Gamestate->camera_offs_x -= 5.0f;
            Gamestate->wnd_center_x-= 5;
            Gamestate->dbg_render_x_offset-= 10;
        }
    if (ExtractKey(kflags, KEY_RIGHT))
        {
            Gamestate->camera_offs_x += 5.0f;
            Gamestate->wnd_center_x+= 5;
            Gamestate->dbg_render_x_offset+= 10;
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

    m4 rotationMatrix = M4Compose(2,
                                  M4RotX(camera.roll),
                                  M4RotY(camera.pitch)
                                  );
    camera_movement = M4Mul(camera_movement, rotationMatrix);
    camera.fpoint = add3(camera.fpoint, camera_movement);
    
    if (result)
        {
            Gamestate->camera = camera;
        }
    
    if (ExtractKey(mflags_trans_to_up, MOUSE_M1))
        {
            Gamestate->square_length += 50;
            Gamestate->concentric_spread_x+=10;
            Gamestate->concentric_spread_y+=4;
            Gamestate->line_scaling_factor += 0.1f;
            Gamestate->rect_scaling_factor += 0.1f;
            for (s32 i = 0; i < Gamestate->concentric_count; i++)
                {
                    Gamestate->concentric_z_values[i]++;
                }
        }
    if (ExtractKey(mflags_trans_to_up, MOUSE_M2))
        {
            Gamestate->square_length -= 50;
            Gamestate->concentric_spread_x-=10;
            Gamestate->concentric_spread_y-=4;
            Gamestate->line_scaling_factor -= 0.1f;
            Gamestate->rect_scaling_factor -= 0.1f;
            for (s32 i = 0; i < Gamestate->concentric_count; i++)
                {
                    Gamestate->concentric_z_values[i]--;
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
    ArenaManager arenaManager;
    arenaManager.base = globals + 1;
    arenaManager.virtualMemoryUsed = sizeof(Globals);

    // @pot do I need to align down/up, if sizeof(Globals) makes it
    // unaligned or something?
    u64 firstArenaSize = INITIAL_COMMIT_SIZE_BY_PLATFORM - sizeof(Globals);
    arena_init(PERM_ARENA, firsArenaSize, firsArenaSize);
    arena_init(FRAME_ARENA, gigabytes(4));
            
    FRAME_BUFFER = arena_push<u8>(PERM_ARENA, MAX_FRAMEBUFFER_SIZE * BYTPP);
    Z_BUFFER = arena_push<r32>(PERM_ARENA, MAX_FRAMEBUFFER_SIZE);    
}

// @todo you can probably export this and call it once
void init_engine_state(void)
{
    s32 init_wnd_width    = 1280;
    s32 init_wnd_height   = 720;
    s32 init_wnd_center_x = 640;
    s32 init_wnd_center_y = 360;
            

    ENGINESTATE->tested_once = 0;

    // @Note do I want this now??
    ENGINESTATE->cursor = V2(init_wnd_center_x,
                             init_wnd_center_y);
                
    ENGINESTATE->wndbuffer_width = init_wnd_width;
    ENGINESTATE->wndbuffer_height = init_wnd_height;
                    
    ENGINESTATE->cameraParams._near = 5; // do we want -1?
    ENGINESTATE->cameraParams._far = 500; // -?
    ENGINESTATE->cameraParams.fov = 120; // in degrees I quess?

    ENGINESTATE->camera_angle = 0;
    ENGINESTATE->log_to_file_once = false;
    ENGINESTATE->reverse_winding = false;

    ENGINESTATE->keyflags = 0;
    ENGINESTATE->mouseflags = 0;
                    
    ENGINESTATE->eye_x = init_wnd_center_x;
    ENGINESTATE->eye_y = init_wnd_center_y;
    ENGINESTATE->screen_z = 0.5f;   // was 0.6f ; 30
    ENGINESTATE->new_screen_z = 30.0f;        // - 0.5  ; 10 ; 0.5
    ENGINESTATE->nearclip = 0.7f;  // was  -     ; 0.7f ; -500 ; 0.7
    ENGINESTATE->farclip = 10000.0f;  // was  -  ;  9.8f ; 500 ; 100

    ENGINESTATE->wnd_center_x = init_wnd_center_x;
    ENGINESTATE->wnd_center_y = init_wnd_center_y;
                    
    ENGINESTATE->dbg_render_x_offset = 0; 
    ENGINESTATE->dbg_render_y_offset = 0; 
    ENGINESTATE->square_length = 10;       
    ENGINESTATE->concentric_thickness = 5;
    ENGINESTATE->concentric_count = 10;     // must be less than CONCENTRIC_MAX
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

            
    // for (s32 i = 0; i < wnd_height; i++)
    //     {
    //         r32* row = (r32*)(zbuffer + i*wnd_pitch);
    //         for (s32 j = 0; j < wnd_width; j++)
    //             {
    //                 *row = ENGINESTATE->cameraParams._far; // smth else??
    //                 row++;
    //             }
    //     }
    for (s32 i = 0; i < wnd_height; i++)
        {
            r32* row = (r32*)(zbuffer + i*wnd_pitch);
            for (s32 j = 0; j < wnd_width; j++)
                {
                    *row = 1.0f;
                    row++;
                }
        }

            
    ENGINESTATE->camera.fpoint = V3(640, 360, 0); // was 680  ; z=5
    ENGINESTATE->camera.yaw    = 0;
    ENGINESTATE->camera.pitch  = 0;
    //ENGINESTATE->camera.pitch  = pi/4;
    ENGINESTATE->camera.roll   = 0;
            
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

#include "test.h"

void update_and_render(void)
{
    if (!ENGINESTATE->inited)
        {
            init_memory();
            init_engine_state();
            
            ENGINESTATE->inited = true;
        }
    test();
}

