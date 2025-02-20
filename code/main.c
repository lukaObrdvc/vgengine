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

v2 clamp_line(v2 vec, v2 other)
{
    // @IMPORTANT methinks it is assumed that vector is in wnd coordsys
    v2 result = zero2();

    // @TODO probably put these in their respective branches instead
    r32 x = vec.x;
    r32 y = vec.y;
    r32 k = (vec.y - other.y) / (vec.x - other.x);
    r32 y0 = y - x * k;
    r32 x0 = x - y / k;
    r32 yw = y - (x - wnd_width)  * k;
    r32 xh = x - (y - wnd_height) / k;

    if (x < 0 && y > wnd_height && y0 > wnd_height) // ul corner 2nd quadrant and y0 > wnd_height
        {
            r32 xhc = clamp32(xhc, 0, wnd_height);
            result = V2(xhc, wnd_height);
        }
    else if ((x < 0 && y > 0) ||
             (x < 0 && y < 0 && y0 < 0)) // bl corner 2nd quadrant or 3rd and y0 < 0
        {
            r32 y0c = clamp32(y0, 0, wnd_height);
            result = V2(0, y0c);
        }
    else if (x > wnd_width && y < 0 && yw < 0) // br corner 4th quadrant and yw < 0
        {
            r32 x0c = clamp32(x0, 0, wnd_width);
            result = V2(x0c, 0);
        }
    else if ((x > 0 && y < 0) ||
             (x < 0 && y < 0 && y0 >= 0)) // bl corner 4th quadrant or 3rd and y0 >= 0
        {
            r32 x0c = clamp32(x0, 0, wnd_width);
            result = V2(x0c, 0);
        }
    else if ((x < wnd_width && y > wnd_height) ||  // ur corner 2nd quadrant or 3rd and yw > wnd_height
             (x > wnd_width && y > wnd_height && yw > wnd_height))
        {
            r32 xhc = clamp32(xh, 0, wnd_width);
            result = V2(xhc, wnd_height);
        }
    else if ((x > wnd_width && y < wnd_height) ||  // ur corner 4th quadrant or 3rd and yw <= wnd_height
             (x > wnd_width && y > wnd_height && yw <= wnd_height))
        {
            r32 ywc = clamp32(yw, 0, wnd_height);
            result = V2(wnd_width, ywc);
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

v2 project(v3 point, PROJECTION P)
{
    v2 result = zero2();
                
    switch (P)
        {
        case ORTHOGRAPHIC:
            {
                result = V2(point.x, point.y);                        
            } break;
        case PERSPECTIVE:
            {
                r32 scaling_factor = Gamestate->screen_z / point.z;
                v2 focus = V2(Gamestate->eye_x, Gamestate->eye_y);
                v2 point2 = V2(point.x, point.y);

                // @TODO maybe it's actually better if you write two lines instead
                result = sub2(focus, scale2(sub2(focus, point2), scaling_factor));
            } break;
        default:
            {
                InvalidDefaultCase;
            }
        }
    
    return result;
}

void draw_wndline(v2 point_A, v2 point_B, u32 color)
{
    // @TODO potentially say if lines on the outline of wndrect don't draw
    // or clamp lines in a different way

    // essentialy the problem there is that with rectangles you draw based
    // on whether width or height exists, for triangles you can draw based
    // on whether all 3 points are on a line, but for a line you can't
    // do these things because a line doesn't have an Area, only a Length

    // so you need to clamp lines differently, by reducing their length instead,
    // which you do by clamping towards the other end of the line, not
    // perpendicularly towards the window

    
    
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

    r32 tmp = top;
    top = to_yisdown(bottom);
    bottom = to_yisdown(tmp);
    
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

// @TODO make this work when multiple keys are down at the same time ??
void process_frame_input(key curr_frame_key,
                         b32 used_key,
                         mouse curr_frame_mouse,
                         b32 used_mouse)
{
    if (used_key)
        {
            switch(curr_frame_key.code)
                {            
                case KEY_UP:
                    {
                        Gamestate->wnd_center_y+= 20;
                        Gamestate->dbg_render_y_offset+= 10;
                    } break;
                case KEY_DOWN:
                    {
                        Gamestate->wnd_center_y-= 20;
                        Gamestate->dbg_render_y_offset-= 10;
                    } break;
                case KEY_LEFT:
                    {
                        Gamestate->wnd_center_x-= 20;
                        Gamestate->dbg_render_x_offset-= 10;
                    } break;
                case KEY_RIGHT:
                    {
                        Gamestate->wnd_center_x+= 20;
                        Gamestate->dbg_render_x_offset+= 10;
                    } break;
                default:
                    {}
                }
        }
    
    if (used_mouse)
        {
            if (curr_frame_mouse.code == M1 && curr_frame_mouse.is_down)
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
            if (curr_frame_mouse.code == M2 && curr_frame_mouse.is_down)
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
            Gamestate->cursor = curr_frame_mouse.cursor;
        }

    // @TODO do I need to invalidate think about it...
    // @Fail is this invalidation desirable    
    /* frame_key.code = KEY_NONE; */
    /* mouse_frame_key.code = M_NONE; */
    /* mouse_frame_key.is_down = false; */
    /* mouse_frame_key.mouse_moved = false;     */
}

void platform_init_memory_base(void* memory_base_ptr)
{    
    memory_base = (platform_provides*) memory_base_ptr;
}

void init_game_state(void)
{
    if (!Gamestate->inited)
        {
            s32 init_wnd_width     = 1280;
            s32 init_wnd_height    = 720;
            s32 init_wnd_center_x  = 640;
            s32 init_wnd_center_y  = 360;
            
            Assert(sizeof(game_state) + wnd_bytesize <= memory_base->perm_mem_cap);
            
            *(Gamestate) = literal(game_state) {
                .inited = true,
                .tested_once = 0,

                .cursor = V2(init_wnd_center_x,
                             init_wnd_center_y),
                
                .wndbuffer_width = init_wnd_width,
                .wndbuffer_height = init_wnd_height,

                .eye_x = init_wnd_center_x,
                .eye_y = init_wnd_center_y,
                .screen_z = 0.6f,
                .nearclip = 0.7f,
                .farclip = 9.8f,

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

            s32 concentric_count = Gamestate->concentric_count;
            r32* concentric_z_values = Gamestate->concentric_z_values;
            Assert(concentric_count <= CONCENTRIC_MAX);

            for (s32 i = 0; i < MAX_BRUSHES; i++)
                {
                    Gamestate->brushes[i] = ((256 << 24) |  // a
                                             (120 << 16) |  // R
                                             (0 << 8) |     // G
                                             120);          // B
                }
            Gamestate->brushes[BRUSH_SCANLINE] = ((256 << 24) |
                                                  (0 << 16) |
                                                  (0 << 8) |   
                                                  0);        
            
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

