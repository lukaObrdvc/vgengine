#include "utils.h"
#include "math.h"
#include "vector.h"
#include "pixel.h"
#include "renderer.h"
#include "main.h"

// @TODO put void into functions that actually do not get passed anything

// @Failure IMPORTANT, avoid unsigned when you are subtracting because when you go past 0 you will overflow
// @Note LITERALLY DO NOT USE unsigned int (especially when you subtract and operate with signed int) unless you know it's fine to use....

// @IMPORTANT -wd4013 I'm getting for sqr probably something to do with including <math.h> orsmth......???

wndrect obtain_wndrect(rect2 rectangle, v2 origin)
{
    wndrect result;
    r32 min_x = rectangle.A.x;
    r32 min_y = rectangle.A.y;
    r32 max_x = rectangle.A.x;
    r32 max_y = rectangle.A.y;

    if (rectangle.B.x < min_x) min_x = rectangle.B.x;
    if (rectangle.C.x < min_x) min_x = rectangle.C.x;
    if (rectangle.D.x < min_x) min_x = rectangle.D.x;
    
    if (rectangle.B.x > max_x) max_x = rectangle.B.x;
    if (rectangle.C.x > max_x) max_x = rectangle.C.x;
    if (rectangle.D.x > max_x) max_x = rectangle.D.x;

    if (rectangle.B.y < min_y) min_y = rectangle.B.y;
    if (rectangle.C.y < min_y) min_y = rectangle.C.y;
    if (rectangle.D.y < min_y) min_y = rectangle.D.y;

    if (rectangle.B.y > max_y) max_y = rectangle.B.y;
    if (rectangle.C.y > max_y) max_y = rectangle.C.y;
    if (rectangle.D.y > max_y) max_y = rectangle.D.y;
    
    result.width = max_x - min_x;
    result.height = max_y - min_y;
        result.topleft = literal(v2) {
        .x = min_x + origin.x,
        .y = -min_y + origin.y - result.height };
    return result;
}

void draw_rotated_rect_2(rect2 rect, v2 origin, pxl color)
{
    // @Note round instead??
    // @TODO there are holes at certain times, due to numerical inaccuracy...
    s32 width = floor32(edist2(rect.A, rect.B));
    s32 height = floor32(edist2(rect.A, rect.D));
    for (s32 i = 0; i < height; i++)
        {
            for (s32 j = 0; j < width; j++)
                {
                    v2 pxl_point = {
                        .x = j,
                        .y =  - height + i };
                    pxl_point = rotate2(pxl_point, -Gamestate->rect_angle);
                    pxl_point.x = floor32(pxl_point.x) + origin.x;
                    pxl_point.y = floor32(pxl_point.y) + origin.y;
                    pxl_point = clamp_point(pxl_point);
                    pxl* draw_point = (pxl*)(wnd_buffer +
                                             wnd_pitch*(s32)pxl_point.y +
                                             wnd_bytpp*(s32)pxl_point.x);
                    *draw_point = color;
                }
        }       
}

v2 obtain_leftmost(rect2 rect)
{
    r32 min_x = rect.A.x;

    if (rect.B.x < min_x) min_x = rect.B.x;
    if (rect.C.x < min_x) min_x = rect.C.x;
    if (rect.D.x < min_x) min_x = rect.D.x;

    if (rect.A.x == min_x) return rect.A;
    if (rect.B.x == min_x) return rect.B;
    if (rect.C.x == min_x) return rect.C;
    if (rect.D.x == min_x) return rect.D;
}

void draw_rotated_rect(rect2 rect, wndrect outline, pxl color)
{    
    u32 offset = wnd_pitch*round32(outline.topleft.y) +
        round32(outline.topleft.x)*wnd_bytpp;
    for (s32 i = 0; i < outline.height; i++)
        {
            pxl* row = (pxl*)(wnd_buffer + wnd_pitch*i + offset);
            for (s32 j = 0; j < outline.width; j++)
                {                    
                    b32 inside = true;

                    v2 pixel_to_outline = { .x=j, .y=round32(outline.height)-i };
                    v2 rect_leftmost = obtain_leftmost(rect);
                    /* v2 rect_origin = {.x=0, */
                    /*                   .y=outline.height - (rect_leftmost.y - outline.topleft.y) }; */
                    v2 rect_origin = {.x=0,
                                      .y=round32(abs(rect_leftmost.y)) };


                    //pixel_to_outline = transpose2(pixel_to_outline, rect_origin);
                    pixel_to_outline = rotate2(pixel_to_outline, -PI/2 +Gamestate->rect_angle);
                    pixel_to_outline = transpose2(pixel_to_outline, rect_origin);
                    
                    if (pixel_to_outline.x < 0)
                        {
                            row++;
                            continue;
                        }
                    if (pixel_to_outline.y < 0)
                        {
                            row++;
                            continue;
                        }
                    if (pixel_to_outline.x > edist2(rect.A, rect.B))
                        {
                            row++;
                            continue;
                        }
                    if (pixel_to_outline.y > edist2(rect.C, rect.B))
                        {
                            row++;
                            continue;
                        }
                    *row = color;
                    row++;
                }
        }       
}

// nochecking
void draw_line(v2 origin, v2 point, pxl color)
{
    u8* offset = wnd_buffer + wnd_pitch*round32(origin.y) + wnd_bytpp*round32(origin.x);
    u8* drawing_point = 0;
    
    r32 dX = point.x;
    r32 dY = -point.y;
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
                *((pxl*)drawing_point) = color;
        }
}


/* v2 project_point(v2 point) */
/* { */
/*     v2 result; */

/*     r32 point_z = Gamestate->concentric_z_values[Gamestate->concentric_current_z]; */
/*     r32 screen_z = 0.1f; */
/*     result.x = point.x / point_z * screen_z; */
/*     result.y = point.y / point_z * screen_z; */
    
/*     return result; */
/* } */

wndrect project_wndrect(wndrect rect)
{
    wndrect result;

    s32 concentric_current_z = Gamestate->concentric_current_z;
    r32 rect_z = Gamestate->concentric_z_values[concentric_current_z];
    r32 screen_z = Gamestate->screen_z;
    r32 scaling_factor = screen_z / rect_z;
    
    v2 A = rect.topleft;
    v2 B = {.x = rect.topleft.x + rect.width , .y = rect.topleft.y};
    v2 D = {.x = rect.topleft.x, .y = rect.topleft.y + rect.height};

    A.x = Gamestate->eye_x - scaling_factor * (Gamestate->eye_x - A.x);
    A.y = Gamestate->eye_y - scaling_factor * (Gamestate->eye_y - A.y);
    B.x = Gamestate->eye_x - scaling_factor * (Gamestate->eye_x - B.x);
    D.y = Gamestate->eye_y - scaling_factor * (Gamestate->eye_y - D.y);
    result.topleft = A;
    result.width = B.x - A.x;
    result.height = D.y - A.y;
    
    return result;
}

void fill_background()
{
    for (s32 i = 0; i < wnd_height; i++)
        {
            pxl* row = (pxl*)(wnd_buffer + i*wnd_pitch);
            for (s32 j = 0; j < wnd_width; j++)
                {
                    *row = literal(pxl) {120, 0, 120, 255};
                    // in struct is r,g,b,a ..?
                    //*row = (a << 24) | (R << 16) | (G << 8) | B;
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

    wndrect rect = Wndrect(left, bottom, right, top);
    rect = clamp_wndrect(rect);
    draw_clamped_wndrect(rect, color);
}

void draw_wndrect_outline_projected(wndrect rect, s32 thickness, pxl color)
{
    wndrect rect_up = rect;
    rect_up.height = thickness;
    rect_up = project_wndrect(rect_up);
    clamp_wndrect(&rect_up);
    
    wndrect rect_left = rect;
    rect_left.width = thickness;
    rect_left.height -= thickness;
    rect_left.topleft.y += thickness;
    rect_left = project_wndrect(rect_left);
    clamp_wndrect(&rect_left);
    
    wndrect rect_down = rect;
    rect_down.height = thickness;
    rect_down.width -= thickness;
    rect_down.topleft.y += rect.height - thickness;
    rect_down.topleft.x += thickness;
    rect_down =  project_wndrect(rect_down);
    clamp_wndrect(&rect_down);
    
    wndrect rect_right = rect;
    rect_right.height -= thickness+thickness;
    rect_right.width = thickness;
    rect_right.topleft.y += thickness;
    rect_right.topleft.x += rect.width - thickness;
    rect_right = project_wndrect(rect_right);
    clamp_wndrect(&rect_right);
    
    draw_clamped_wndrect(rect_up, color);
    draw_clamped_wndrect(rect_left, color);
    draw_clamped_wndrect(rect_down, color);
    draw_clamped_wndrect(rect_right, color);
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
                        Gamestate->wnd_center_y-= 20;
                        Gamestate->dbg_render_y_offset-= 10;
                    } break;
                case KEY_DOWN:
                    {
                        Gamestate->wnd_center_y+= 20;
                        Gamestate->dbg_render_y_offset+= 10;
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

void init_game_state()
{
    if (!Gamestate->inited)
        {
            s32 init_wnd_width     = 1280;
            s32 init_wnd_height    = 720;
            s32 init_wnd_center_x  = 640;
            s32 init_wnd_center_y  = 360;
            
            // @TODO include wnd_buffer size in the assert
            Assert(sizeof(game_state) + wnd_bytesize <= memory_base->perm_mem_cap);
            
            *(Gamestate) = literal(game_state) {
                .inited = true,
                .tested_once = 0,

                .cursor = V2(init_wnd_center_x,
                             init_wnd_center_y),
                    
                .wndbuffer_width = init_wnd_width,
                .wndbuffer_height = init_wnd_height,

                .eye_x = init_wnd_width,
                .eye_y = init_wnd_height,
                .screen_z = 0.5f,
                .nearclip = 0.7f,
                .farclip = 9.8f,
                    
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
    
            for (s32 i = 0; i < concentric_count; i++)
                {
                    concentric_z_values[i] = floor32(concentric_count/2.0 - 1 - i);
                }

        }
}

// @TODO declare in header instead
#include "test.h"

void update_and_render()
{    
    init_game_state();
    test();    
}

