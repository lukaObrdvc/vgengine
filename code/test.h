#define CURRENTLY_TESTING perspective_projection_test
#define TEST_ONLY_ONCE Gamestate->tested_once = true;

// @TODO make a variable brush (multiple of them ?) that's RGBA,
// that you can set and get with macros, so you don't have to make
// colors everywhere...

// @TODO simple dynamic arrays, so you don't have to make a #define
// for array sizes...

void draw_rotated_line_test(void)
{
    // @TODO prevent drawing along left and top wndrect edges when coords are 0...
    fill_background();
    
    pxl color      = {.R = 0, .G = 0, .B = 0, .A = 0};
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

    draw_line(start, line, color);
    draw_line(origin, offset_line, color);

    Gamestate->line_angle += PI / 256;
}

void perspective_projection_test(void)
{
    Gamestate->concentric_current_z = 0;
    r32* concentric_z_values = Gamestate->concentric_z_values;

    pxl color     = {.R=0, .G=0, .B=0, .A=255};
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
    pxl color     = {.R=0, .G=0, .B=0, .A=255};
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
    pxl color = {.R = 0, .G = 0, .B = 0, .A = 0};
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
    DBG_READ_FILE((u8*)"W:\\cardcrafter\\data\\readtest.txt", (u8*)memory_base->temp_mem, &readsize);
    DBG_WRITE_FILE((u8*)"W:\\cardcrafter\\data\\writetest.txt", (u8*)memory_base->temp_mem, readsize);
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
