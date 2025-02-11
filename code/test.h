#define CURRENTLY_TESTING draw_rotated_rect_2    ##_test
#define TEST_ONLY_ONCE Gamestate->tested_once = true;


void draw_rotated_rect_2_test()
{
    fill_background();
    pxl color = {.R = 0, .G = 0, .B = 0, .A = 0};
    v2 origin = {.x = Gamestate->wnd_center_x, .y = Gamestate->wnd_center_y};
    rect2 rect = {
        .A = {.x=0, .y=50},
        .B = {.x=50, .y=50},
        .C = {.x=50, .y=0},
        .D = {.x=0, .y=0} };

    rect.A = scale2(rect.A, Gamestate->rect_scaling_factor);
    rect.B = scale2(rect.B, Gamestate->rect_scaling_factor);
    rect.C = scale2(rect.C, Gamestate->rect_scaling_factor);
    rect.D = scale2(rect.D, Gamestate->rect_scaling_factor);
    
    draw_rotated_rect_2(rect, origin, color);

    Gamestate->rect_angle += PI / 256;
}

void draw_rotated_rect_test()
{
    // @TODO this is probably a better method to draw than _2 which is kind of
    // naive, but this is harder to figure out, since this is kind of like
    // implicit rasterization/masking, which GPU also does and so on....
    // basically I have not yet solved this problem on paper, so I can't
    // program the solution yet (either figure out, or watch casey do it....)
    // (or ignore for now, when I gain more knowledge maybe later attempt bababa)

    fill_background();

    pxl color = {.R = 0, .G = 0, .B = 0, .A = 0};
    v2 origin = {.x = Gamestate->wnd_center_x, .y = Gamestate->wnd_center_y};
    rect2 rect = {
        .A = {.x=0, .y=50},
        .B = {.x=50, .y=50},
        .C = {.x=50, .y=0},
        .D = {.x=0, .y=0} };
    
    rect.A = rotate2(rect.A, Gamestate->rect_angle);
    rect.B = rotate2(rect.B, Gamestate->rect_angle);
    rect.C = rotate2(rect.C, Gamestate->rect_angle);
    rect.D = rotate2(rect.D, Gamestate->rect_angle);

    wndrect outline = obtain_wndrect(rect, origin);
    // @TODO clamping

    draw_rotated_rect(rect, outline, color);

    Gamestate->rect_angle += PI / 256;
    
}

void draw_rotated_line_test()
{
    // @TODO prevent drawing along left and top wndrect edges when coords are 0...
    fill_background();
    
    pxl color = {.R = 0, .G = 0, .B = 0, .A = 0};
    v2 origin = {.x = Gamestate->wnd_center_x, .y = Gamestate->wnd_center_y};
    v2 offset_line = {.x = 100, .y = 0};
    v2 line = {.x = 50, .y = 0};
    
    line = scale2(line, Gamestate->line_scaling_factor);
    line = rotate2(line, Gamestate->line_angle);
    offset_line = scale2(offset_line, Gamestate->line_scaling_factor);
    offset_line = rotate2(offset_line, Gamestate->line_angle*1.5);
    offset_line.y = -offset_line.y;
    v2 start = {.x = origin.x+offset_line.x, .y=origin.y-offset_line.y};
    v2 end = {.x = origin.x+line.x+offset_line.x, .y=origin.y-line.y-offset_line.y};

    origin = clamp_point(origin);
    start = clamp_point(start);
    end = clamp_point(end);

    origin.y = -origin.y;
    start.y = -start.y;
    end.y = -end.y;
    line = transpose2(end, start);
    offset_line = transpose2(start, origin);
    origin.y = -origin.y;
    start.y = -start.y;
    end.y = -end.y;

    draw_line(start, line, color);
    draw_line(origin, offset_line, color);

    Gamestate->line_angle += PI / 256;
}

void draw_sqaure_around_cursor_test()
{
    dbg_render(Gamestate->dbg_render_x_offset, Gamestate->dbg_render_y_offset);
    dbg_draw_square_around_cursor(Gamestate->square_length);
}

void concentric_test()
{
    fill_background();
    u32 thickness = Gamestate->concentric_thickness;
    u32 spread_x = Gamestate->concentric_spread_x;
    u32 spread_y = Gamestate->concentric_spread_y;
    u32 count = Gamestate->concentric_count;
    
    concentric_rectangles_around_origin(thickness, spread_x, spread_y, count);

    //Gamestate->concentric_count = 3;
}

void file_test()
{
    u32 readsize;
    DBG_READ_FILE((u8*)"W:\\cardcrafter\\data\\readtest.txt", (u8*)memory_base->temp_mem, &readsize);
    DBG_WRITE_FILE((u8*)"W:\\cardcrafter\\data\\writetest.txt", (u8*)memory_base->temp_mem, readsize);
    TEST_ONLY_ONCE
}

void none_test()
{
    TEST_ONLY_ONCE
}

#define TEST_ CURRENTLY_TESTING();
void test()
{
    if (!Gamestate->tested_once)
        {
            TEST_
        }
}
