#include "utils.h"
#include "math.h"
#include "linalg.h"
#include "pixel.h"
#include "renderer.h"
#include "main.h"

// @TODO put void into functions that actually do not get passed anything

// @Failure IMPORTANT, avoid unsigned when you are subtracting because when you go past 0 you will overflow
// @Note LITERALLY DO NOT USE unsigned int (especially when you subtract and operate with signed int) unless you know it's fine to use....

// @IMPORTANT -wd4013 I'm getting for sqr probably something to do with including <math.h> orsmth......???

                      // using
//global_variable b32 inited = false; // @IMPORTANT this shite here will also resize the window on dll reload (because static)
//global_variable coordsys2 centeredcs;
//global_variable coordsys2 offsetedcs;
/* global_variable line2 line; */
/* global_variable s8 linex = 0; */
/* global_variable s8 liney = 0; */
/* global_variable rad a = 0; */
/* global_variable r32 s = 1; */
/* global_variable s32 counter = 0; */
/* global_variable rect2 grect; */

//global_variable pxl my;
//global_variable mouse_key_code mykey;


/* #define GRECT_WIDTH 50 */
/* #define GRECT_HEIGHT 50 */

/* wndrect obtain_wndrect(rect2 rectangle) */
/* { */
/*     wndrect result; */
/*     r32 min_x = rectangle.A.x; */
/*     r32 min_y = rectangle.A.y; */
/*     r32 max_x = rectangle.A.x; */
/*     r32 max_y = rectangle.A.y; */

/*     if (rectangle.B.x < min_x) min_x = rectangle.B.x; */
/*     if (rectangle.C.x < min_x) min_x = rectangle.B.x; */
/*     if (rectangle.D.x < min_x) min_x = rectangle.B.x; */
    
/*     if (rectangle.B.x > max_x) max_x = rectangle.B.x; */
/*     if (rectangle.C.x > max_x) max_x = rectangle.C.x; */
/*     if (rectangle.D.x > max_x) max_x = rectangle.D.x; */

/*     if (rectangle.B.y < min_y) min_y = rectangle.B.y; */
/*     if (rectangle.C.y < min_y) min_y = rectangle.C.y; */
/*     if (rectangle.D.y < min_y) min_y = rectangle.D.y; */

/*     if (rectangle.B.y > max_y) max_y = rectangle.B.y; */
/*     if (rectangle.C.y > max_y) max_y = rectangle.C.y; */
/*     if (rectangle.D.y > max_y) max_y = rectangle.D.y; */
    
/*     result.topleft = literal(v2) { */
/*         .x = min_x + offsetedcs.origin.x, */
/*         .y = -max_y + offsetedcs.origin.y }; */
/*     result.width = max_x - min_x; */
/*     result.height = max_y - min_y; */
/*     return result; */
/* } */

/* void draw_rotated_rect(wndrect outline) */
/* { */
/*     pxl color = {0, 0, 0, 255}; */

/*     coordsys2 from; */
/*     from.origin.x = (r32)outline.topleft.x;  */
/*     from.origin.y = (r32)outline.topleft.y + outline.height;  */
/*     from.offset = 0; */
/*     coordsys2 to; */
/*     to.origin = offsetedcs.origin; */
/*     to.offset = offsetedcs.offset; */
                    
/*     u32 offset = round32(outline.topleft.x)*wndbuffer.bytpp; */
/*     for (s32 i = 0; i < outline.height; i++) */
/*         { */
/*             pxl* row = (pxl*)(wndbuffer.memory + wndpitch*(round32(outline.topleft.y) + i) + offset); */
/*             for (s32 j = 0; j < outline.width; j++) */
/*                 { */
/*                     v2 curr; */
/*                     curr.x = (r32)j; */
/*                     curr.y = (r32)i; */

/*                     b32 inside_grect = false; */
/*                     transpose(&curr, from, to); */

/*                     if ((curr.x >= 0 && curr.x <= GRECT_WIDTH) && (curr.y >= 0 && curr.y <= GRECT_HEIGHT)) */
/*                         { */
/*                             inside_grect = true; */
/*                         } */
                    
/*                     if (inside_grect) */
/*                         { */
/*                             *row = color; */
/*                         } */
/*                     row++; */
/*                 } */
/*         } */
/* } */

/* void draw_line() */
/* { */
/*     u32 pitch = wndbuffer.width*wndbuffer.bytpp; */
/*     u32 offset = (s32)centeredcs.origin.x*wndbuffer.bytpp; */
/*     pxl* pixel = (pxl*)(wndbuffer.memory + pitch*(s32)centeredcs.origin.y + offset); */

/*     s32 dX = (s32)icomponent(line.B).x; */
/*     s32 dY = -(s32)jcomponent(line.B).y; */
/*     s32 movebyX = 0; */
/*     s32 movebyY = 0; */
/*     u32 len = (s32)edist(line.A, line.B); */
/*     r32 whenX = ((r32)len) / dX; */
/*     r32 whenY = ((r32)len) / dY; */

/*     for (u32 i = 1; i <= len; i++) */
/*         { */
/*             movebyX = floor32i(i/whenX); */
/*             movebyY = floor32i(i/whenY); */

/*             // 20 50.50 */
/*             // 54.3 */
/*             // 2.7 1.075 */

/*             pixel = (pxl*)(wndbuffer.memory + pitch*((s32)centeredcs.origin.y+movebyY) + ((s32)centeredcs.origin.x+movebyX)*wndbuffer.bytpp);// + movebyX*wndbuffer.bytpp + pitch*movebyY); */

/*             pixel->set(0, 0, 0, 255); */
/*         } */

/*     u32 len = (s32)edist(line.A, line.B); */
/*     for (u32 i = 0; i < len; i++) */
/*         { */
/*             pixel->set(0, 0, 0, 255); */
/*             pixel++; */
/*             } */
/* } */

v2 scale2(v2 vec, r32 scalar)
{
    v2 result;

    result.x = vec.x * scalar;
    result.y = vec.y * scalar;
    
    return result;
}

v2 rotate2(v2 vec, r32 angle)
{
    v2 result;

    result.x = cos(angle)*vec.x - sin(angle)*vec.y;
    result.y = sin(angle)*vec.x + cos(angle)*vec.y;
    
    return result;
}

void draw_line(v2 origin, v2 point, pxl color)
{
    u8* offset = wnd_buffer + wnd_pitch*round32(origin.y) + wnd_bytpp*round32(origin.x);
    u8* drawing_point = 0;
    
    r32 dX = point.x;
    r32 dY = -point.y;
    s32 movebyX = 0;
    s32 movebyY = 0;
    r32 len = edist2(origin, point);
    r32 whenX = len / dX;
    r32 whenY = len / dY;

    for (s32 i = 1; i <= floor32(len); i++)
        {
            movebyX = floor32(i/whenX);
            movebyY = floor32(i/whenY);
            drawing_point = offset + wnd_pitch*movebyY + wnd_bytpp*movebyX;
            *((pxl*)drawing_point) = color;
        }
}

void draw_clamped_wndrect(wndrect rectangle, pxl color)
{
    u32 offset = wnd_pitch*round32(rectangle.topleft.y) +
        round32(rectangle.topleft.x)*wnd_bytpp;
    for (s32 i = 0; i < rectangle.height; i++)
        {
            pxl* row = (pxl*)(wnd_buffer + wnd_pitch*i + offset);
            for (s32 j = 0; j < rectangle.width; j++)
                {
                    *row = color;
                    row++;
                }
        }
}

void clamp_wndrect(wndrect* rectangle)
{
    if (rectangle->topleft.x < 0)
        {
            rectangle->width += rectangle->topleft.x;
            rectangle->topleft.x = 0;
        }
    else if (rectangle->topleft.x > wnd_width)
        {
            rectangle->width = 0;
            rectangle->topleft.x = (r32)wnd_width;
        }
    if (rectangle->topleft.y < 0)
        {
            rectangle->height += rectangle->topleft.y;
            rectangle->topleft.y = 0;            
        }
    else if (rectangle->topleft.y > wnd_height)
        {
            rectangle->height = 0;
            rectangle->topleft.y = (r32)wnd_height;
        }
    
    if (rectangle->topleft.x + rectangle->width > wnd_width)
        {
            rectangle->width = wnd_width - rectangle->topleft.x;
        }
    if (rectangle->topleft.y + rectangle->height > wnd_height)
        {
            rectangle->height = wnd_height - rectangle->topleft.y;
        }    
}

void fill_background()
{
    s32 pitch = wnd_width*wnd_bytpp;
    for (s32 i = 0; i < wnd_height; i++)
        {
            pxl* row = (pxl*)(wnd_buffer + i*pitch);
            for (s32 j = 0; j < wnd_width; j++)
                {
                    *row = literal(pxl) {120, 0, 120, 255}; // in struct is r,g,b,a ..?
                    //*row = (a << 24) | (R << 16) | (G << 8) | B;
                    row++;
                }
        }    
}

void dbg_render(s8 x_offset, s8 y_offset)
{
    s32 pitch = wnd_width*wnd_bytpp;
    for (s32 i = 0; i < wnd_height; i++)
        {
            pxl* row = (pxl*)(wnd_buffer + i*pitch);
            for (s32 j = 0; j < wnd_width; j++)
                {
                    u8 g = j + Gamestate->dbg_render_x_offset;
                    u8 b = i + Gamestate->dbg_render_y_offset;

                    *row = literal(pxl) {.R=0, .G=g, .B=b, .A=255};
                    row++;
                }
        }    
}

void dbg_draw_square_around_cursor(u8 square_length)
{
    pxl color = {.R=0, .G=0, .B=0, .A=255};
    r32 top = Gamestate->cursor.y - (((r32)square_length)/2.0f);
    r32 left = Gamestate->cursor.x - (((r32)square_length)/2.0f);
    r32 width = (r32)square_length;
    r32 height = (r32)square_length;
    wndrect rect = {
        .topleft = literal(v2) {.x = left, .y = top},
        .width = width,
        .height = height };
    clamp_wndrect(&rect);
    draw_clamped_wndrect(rect, color);
}

void concentric_rectangles_around_origin(u32 thickness, u32 spread_x, u32 spread_y, u32 count)
{
    pxl color = {.R=0, .G=0, .B=0, .A=255};
    s32 origin_x = Gamestate->wnd_center_x;
    s32 origin_y = Gamestate->wnd_center_y;
    for (u32 i=1; i<=count; i++)
        {
            s32 top = origin_y-i*(spread_y+thickness);
            s32 left = origin_x-i*(spread_x+thickness);
            u32 width = i*2*(spread_x+thickness);
            u32 height = i*2*(spread_y+thickness);

            // @TODO write this in a cleaner way....
            wndrect wup, wleft, wdown, wright;
            
            wup.topleft.y = top;
            wup.topleft.x = left;
            wup.width = width;
            wup.height = thickness;

            wleft.topleft.y = top;
            wleft.topleft.x = left;
            wleft.width = thickness;
            wleft.height = height;

            wright.topleft.y = top;
            wright.topleft.x = left+width-thickness;
            wright.width = thickness;
            wright.height = height;

            wdown.topleft.y = top+height-thickness;
            wdown.topleft.x = left;
            wdown.width = width;
            wdown.height = thickness;

            clamp_wndrect(&wup);
            clamp_wndrect(&wleft);
            clamp_wndrect(&wright);
            clamp_wndrect(&wdown);

            draw_clamped_wndrect(wup, color);
            draw_clamped_wndrect(wleft, color);
            draw_clamped_wndrect(wright, color);
            draw_clamped_wndrect(wdown, color);            
        }
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
                        Gamestate->wnd_center_y-= 10;
                        Gamestate->dbg_render_y_offset-= 10;
                    } break;
                case KEY_DOWN:
                    {
                        Gamestate->wnd_center_y+= 10;
                        Gamestate->dbg_render_y_offset+= 10;
                    } break;
                case KEY_LEFT:
                    {
                        Gamestate->wnd_center_x-= 10;
                        Gamestate->dbg_render_x_offset-= 10;
                    } break;
                case KEY_RIGHT:
                    {
                        Gamestate->wnd_center_x+= 10;
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
                }
            if (curr_frame_mouse.code == M2 && curr_frame_mouse.is_down)
                {
                    Gamestate->square_length -= 50;
                    Gamestate->concentric_spread_x-=10;
                    Gamestate->concentric_spread_y-=4;
                    Gamestate->line_scaling_factor -= 0.1f;
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
            Assert(sizeof(game_state) <= memory_base->perm_mem_cap);
            
            *(Gamestate) = literal(game_state) {
                .inited = true,

                .tested_once = 0,
                .wndbuffer_width = 1280,
                .wndbuffer_height = 720,
                .cursor = literal(v2) {.x = 640, .y = 360},
                .dbg_render_x_offset = 0, 
                .dbg_render_y_offset = 0, 
                .square_length = 10,       
                .wnd_center_x = 640,        
                .wnd_center_y = 360,        
                .concentric_thickness = 5,
                .concentric_count = 10,    
                .concentric_spread_x = 50, 
                .concentric_spread_y = 50,
                .line_angle = 0,
                .line_scaling_factor = 1 };
        }
}

// @TODO declare in header instead
#include "test.h"

void update_and_render()
{    
    init_game_state();
    test();    
}

