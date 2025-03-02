#include "utils.h"
#include "math.h"
#include "vector.h"
#include "pixel.h"
#include "renderer.h"

#include "main.h"

// @TODO I think making a origin as alias of v3 is useful visually

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

v2 clamp_line(v2 vec, v2 other)
{
    // @IMPORTANT methinks it is assumed that vector is in wnd coordsys
    v2 result = zero2();

    // horizontal => k=0
    // vectical => k=nan
    
    // @TODO probably put these in their respective branches instead
    r32 x = vec.x;
    r32 y = vec.y;
    r32 k = (vec.y - other.y) / (vec.x - other.x); // dividing by zero everywhere....
    r32 y0 = y - x * k;
    r32 x0 = x - y / k;
    r32 yw = y - (x - wnd_width)  * k;
    r32 xh = x - (y - wnd_height) / k;

    // @TODO
    // added -1 on wnd_width and wnd_height (but is this a good solution??)
    // essentially it's still drawing around outline of the window
    // even though this kind of clamping (towards line not perpendicular)
    // is still desirable but figure out how to prevent that draw.....??
    
    if ((vec.y - other.y) == 0 ||
        (vec.x - other.x) == 0) // horizontal or vectial line
        {
            result.x = clamp32(x, 0, wnd_width - 1);
            result.y = clamp32(y, 0, wnd_height - 1);
        }
    else if (x < 0 && y > wnd_height && y0 > wnd_height) // ul corner 2nd quadrant and y0 > wnd_height
        {
            r32 xhc = clamp32(xh, 0, wnd_width - 1);
            result = V2(xhc, wnd_height - 1);
        }
    else if ((x < 0 && y > 0) ||
             (x < 0 && y < 0 && y0 < 0)) // bl corner 2nd quadrant or 3rd and y0 < 0
        {
            r32 y0c = clamp32(y0, 0, wnd_height - 1);
            result = V2(0, y0c);
        }
    else if (x > wnd_width && y < 0 && yw < 0) // br corner 4th quadrant and yw < 0
        {
            r32 x0c = clamp32(x0, 0, wnd_width - 1);
            result = V2(x0c, 0);
        }
    else if ((x > 0 && y < 0) ||
             (x < 0 && y < 0 && y0 >= 0)) // bl corner 4th quadrant or 3rd and y0 >= 0
        {
            r32 x0c = clamp32(x0, 0, wnd_width - 1);
            result = V2(x0c, 0);
        }
    else if ((x < wnd_width && y > wnd_height) ||  // ur corner 2nd quadrant or 3rd and yw > wnd_height
             (x > wnd_width && y > wnd_height && yw > wnd_height))
        {
            r32 xhc = clamp32(xh, 0, wnd_width - 1);
            result = V2(xhc, wnd_height - 1);
        }
    else if ((x > wnd_width && y < wnd_height) ||  // ur corner 4th quadrant or 3rd and yw <= wnd_height
             (x > wnd_width && y > wnd_height && yw <= wnd_height))
        {
            r32 ywc = clamp32(yw, 0, wnd_height - 1);
            result = V2(wnd_width - 1, ywc);
        }
    
    return result;
}

// @Note this is actually bad because it clamps perpendicular
// to the window, instead of toward the direction of the vector

// so you actually have to clamp towards some other origin
inline v2 clamp2(v2 vec)
{
    v2 result = vec;

    // @Note this is iffy (of by one andstuffs...)
    clamp(result.x, 0, wnd_width - 1);
    clamp(result.y, 0, wnd_height - 1);
    
    return result;
}

// @Note this is actually bad, because of the previous, but also
// because clamping Z like this is a special case where you are
// XY-plane aligned, I explained this in another note
inline v3 clamp3(v3 vec)
{
    v3 result = vec;

    // @Note this is iffy (of by one andstuffs...)
    clamp(result.x, 0, wnd_width - 1);
    clamp(result.y, 0, wnd_height - 1);
    clamp(result.z, wnd_nearclip, wnd_farclip); // @TODO is this correct?
    
    return result;
}

inline wndrect clamp_wndrect(wndrect rect)
{
    wndrect result = rect;

    // @Note this is iffy (of by one andstuffs...)
    clamp(result.left, 0, wnd_width - 1);
    clamp(result.bottom, 0, wnd_height - 1);
    clamp(result.right, 0, wnd_width - 1);
    clamp(result.top, 0, wnd_height - 1);
    
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

    
    result.x = x * (cA*cB) + y * (cA*sB*sG-sA*cG) + z * (cA*sB*cG+sA*sG);
    result.y = x * (sA*cB) + y * (sA*sB*sG+cA*cG) + z * (sA*sB*cG-cA*sG);
    result.z = x * (-sB)   + y * (cB*sG)          + z * (cB*cG);
    
    return result;
}

void draw_clamped_zbuffered_wndrect(wndrect rect, r32 z, pxl color)
{
    s32 offset = wnd_pitch*round32(rect.bottom) +
        round32(rect.left)*wnd_bytpp;
    s32 height = round32(wndrect_height(rect));
    s32 width = round32(wndrect_width(rect));

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
    s32 offset = wnd_pitch*round32(rect.bottom) +
        round32(rect.left)*wnd_bytpp;
    s32 height = round32(wndrect_height(rect));
    s32 width = round32(wndrect_width(rect));
    
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
                    pxl_point.x = floor32(pxl_point.x);
                    pxl_point.y = floor32(pxl_point.y);
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
                InvalidDefaultCase;
            }
        }
    
    return result;
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

    s32 xstart = floor32(P0.x); // round here?
    s32 xend = floor32(P1.x);
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
         
            
            u32* drawing_point1 = (u32*)(wnd_buffer + floor32(intersectY) * yoffset + x * xoffset);
            u32* drawing_point2 = (u32*)(wnd_buffer + (floor32(intersectY) - 1) * yoffset + x * xoffset);

            r32 alpha1 = 1 - decimal32(intersectY);
            r32 alpha2 = decimal32(intersectY);

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
    
    u8* offset = wnd_buffer + wnd_pitch*round32(origin.y) + wnd_bytpp*round32(origin.x);
    u8* drawing_point = 0;
    
    r32 dX = point.x;
    r32 dY = point.y;
    s32 movebyX = 0;
    s32 movebyY = 0;
    r32 len = edist2(origin, point);
    r32 whenX = len / dX; // @Note divide by zero here??
    r32 whenY = len / dY;

    
    for (s32 i = 1; i <= floor32(len); i++)
        {
            movebyX = floor32(i/whenX);
            movebyY = floor32(i/whenY);
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

                    *row = literal(pxl) {.R=0, .G=g, .B=b, .A=255};
                    row++;
                }
        }    
}

void dbg_draw_square_around_cursor(r32 square_length)
{
    pxl color = {.R=0, .G=0, .B=0, .A=255};
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

    b32 result = false;

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

    curr_cursor.y = to_yisup(curr_cursor.y);
    Gamestate->cursor = curr_cursor;
    if (result)
        {
            Camera camera = Gamestate->camera;
    
            /* if (ExtractKey(mflags_trans_to_up, MOUSE_MOVE)) // this does not work.... */
            /* { */
            v2 cursor_difference = transpose2(curr_cursor, zero2(), Gamestate->cursor); // V2(wnd_width/2.0f, wnd_height/2.0f)
            r32 roll_camera_by = cursor_difference.y;
            r32 pitch_camera_by = -cursor_difference.x;
            Gamestate->camera.roll += roll_camera_by / 256;
            Gamestate->camera.pitch += pitch_camera_by / 256;
            /* } */
            v2 prev_cursor = Gamestate->cursor;

        }
    
    if (ExtractKey(kflags, KEY_UP))
        {
            Gamestate->wnd_center_y+= 5;
            Gamestate->dbg_render_y_offset+= 10;
        }
    if (ExtractKey(kflags, KEY_DOWN))
        {
            Gamestate->wnd_center_y-= 5;
            Gamestate->dbg_render_y_offset-= 10;
        }
    if (ExtractKey(kflags, KEY_LEFT))
        {
            Gamestate->wnd_center_x-= 5;
            Gamestate->dbg_render_x_offset-= 10;
        }
    if (ExtractKey(kflags, KEY_RIGHT))
        {
            Gamestate->wnd_center_x+= 5;
            Gamestate->dbg_render_x_offset+= 10;
        }
    
    /* if (ExtractKey(kflags, KEY_W)) */
    /*     { */
    /*         Gamestate->camera.roll += PI/256; */
    /*     } */
    /* if (ExtractKey(kflags, KEY_S)) */
    /*     { */
    /*         Gamestate->camera.roll -= PI/256;             */
    /*     } */
    /* if (ExtractKey(kflags, KEY_A)) */
    /*     { */
    /*         Gamestate->camera.pitch += PI/256; */
    /*     } */
    /* if (ExtractKey(kflags, KEY_D)) */
    /*     { */
    /*         Gamestate->camera.pitch -= PI/256; */
    /*     } */
// figure out keys for these four
    /* if (ExtractKey(kflags, KEY_Q)) */
    /*     { */
    /*         Gamestate->camera.yaw += PI/256; */
    /*     } */
    /* if (ExtractKey(kflags, KEY_E)) */
    /*     { */
    /*         Gamestate->camera.yaw -= PI/256; */
    /*     } */
    
    /* if (ExtractKey(kflags, KEY_I)) */
    /*     { */
    /*         Gamestate->camera.fpoint.y += 5; */
    /*     } */
    /* if (ExtractKey(kflags, KEY_K)) */
    /*     { */
    /*         Gamestate->camera.fpoint.y -= 5; */
    /*     } */

    v3 camera_movement = zero3();
    if (ExtractKey(kflags, KEY_W))
        {
            camera_movement.z += 5;
            /* Gamestate->camera.fpoint.x += 0.5; */
        }
    if (ExtractKey(kflags, KEY_S))
        {
            camera_movement.z -= 5;
            /* Gamestate->camera.fpoint.x -= 0.5; */
        }
    if (ExtractKey(kflags, KEY_A))
        {
            camera_movement.x -= 5;
            /* Gamestate->camera.fpoint.z += 0.5; */
        }
    if (ExtractKey(kflags, KEY_D))
        {
            camera_movement.x += 5;
            /* Gamestate->camera.fpoint.z -= 0.5; */
        }

    
    if (result)
        {
            camera_movement = rotate3(camera_movement,
                                      Gamestate->camera.yaw,
                                      Gamestate->camera.pitch,
                                      Gamestate->camera.roll);
            Gamestate->camera.fpoint = add3(Gamestate->camera.fpoint, camera_movement);
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

void platform_init_memory_base(void* memory_base_ptr)
{    
    memory_base = (platform_provides*) memory_base_ptr;
}

// @TODO you can probably export this and call it once
void init_game_state(void)
{
    if (!Gamestate->inited)
        {
            s32 init_wnd_width     = 1280;
            s32 init_wnd_height    = 720;
            s32 init_wnd_center_x  = 640;
            s32 init_wnd_center_y  = 360;
            
            *(Gamestate) = literal(game_state) {

                    .inited = true,
                    .tested_once = 0,

                    // @Note do I want this now??
                    .cursor = V2(init_wnd_center_x,
                                 init_wnd_center_y),
                
                    .wndbuffer_width = init_wnd_width,
                    .wndbuffer_height = init_wnd_height,

                    .keyflags = 0,
                    .mouseflags = 0,
                    
                    .eye_x = init_wnd_center_x,
                    .eye_y = init_wnd_center_y,
                    .screen_z = 30.0f,   // was 0.6f
                    .new_screen_z = 10.0f,
                    .nearclip = -500.0f,  // was 0.7f
                    .farclip = 500.0f,  // was 9.8f

                    .wnd_center_x = init_wnd_center_x,
                    .wnd_center_y = init_wnd_center_y,
                    
                    .dbg_render_x_offset = 0, 
                    .dbg_render_y_offset = 0, 
                    .square_length = 10,       
                    .concentric_thickness = 5,
                    .concentric_count = 10,     // must be less than CONCENTRIC_MAX
                    .concentric_spread_x = 50, 
                    .concentric_spread_y = 50,
                    .concentric_current_z = 0,
                    .line_angle = 0,
                    .line_scaling_factor = 1,
                    .rect_angle = 0,
                    .rect_scaling_factor = 1 };


            
            // @TODO you should probably have a default for everything but whatever
            // @TODO is this a good way to set a keymap, just setting powers of two.............
            Gamestate->keymap[0x25] = 0;
            Gamestate->keymap[0x26] = 1; 
            Gamestate->keymap[0x27] = 2; 
            Gamestate->keymap[0x28] = 3; 

            Gamestate->keymap[0x57] = 4;   
            Gamestate->keymap[0x53] = 5; 
            Gamestate->keymap[0x41] = 6; 
            Gamestate->keymap[0x44] = 7; 
            Gamestate->keymap[0x51] = 8; 
            Gamestate->keymap[0x45] = 9; 

            Gamestate->keymap[0x49] = 10;   
            Gamestate->keymap[0x4B] = 11;
            Gamestate->keymap[0x4A] = 12;
            Gamestate->keymap[0x4C] = 13;
            Gamestate->keymap[0x55] = 14;
            Gamestate->keymap[0x4F] = 15;

            
            for (s32 i = 0; i < wnd_height; i++)
                {
                    r32* row = (r32*)(zbuffer + i*wnd_pitch);
                    for (s32 j = 0; j < wnd_width; j++)
                        {
                            *row = Gamestate->farclip; // smth else??
                            row++;
                        }
                }
            
            Gamestate->camera.fpoint = V3(640, 360, 5); // was 680
            Gamestate->camera.yaw    = 0;
            Gamestate->camera.pitch  = 0;
            //Gamestate->camera.pitch  = PI/4;
            Gamestate->camera.roll   = 0;
            
            s32 concentric_count = Gamestate->concentric_count;
            r32* concentric_z_values = Gamestate->concentric_z_values;
            Assert(concentric_count <= CONCENTRIC_MAX);

            for (s32 i = 0; i < MAX_BRUSHES; i++)
                {
                    Gamestate->brushes[i] = (((u32)255 << 24) |  // a
                                             ((u32)120 << 16) |  // R
                                             ((u32)0 << 8) |     // G
                                             (u32)120);          // B
                }
            Gamestate->brushes[BRUSH_SCANLINE] = (((u32)255 << 24) |
                                                  ((u32)0 << 16) |
                                                  ((u32)0 << 8) |   
                                                  0);        

            Gamestate->brushes[BRUSH_SCANLINE2] = (((u32)255 << 24) |
                                                   ((u32)0 << 16) |
                                                   ((u32)0 << 8) |
                                                   255);
            
            for (s32 i = 0; i < concentric_count; i++)
                {
                    concentric_z_values[i] = floor32(concentric_count/2.0 - 1 - i);
                }

        }
}

// @TODO declare in header instead
#include "test.h"

void update_and_render(void)
{
    init_game_state();
    test();    
}

