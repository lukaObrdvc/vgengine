#define CURRENTLY_TESTING perspective_projection_test      //##_test
#define TEST_ONLY_ONCE Gamestate->tested_once = true;


// @TODO simple dynamic arrays, so you don't have to make a #define
// for array sizes...
void perspective_projection_test()
{
    Gamestate->concentric_current_z = 0;
    s32 concentric_count = Gamestate->concentric_count;
    r32* concentric_z_values = Gamestate->concentric_z_values;
    
    pxl color = {.R=0, .G=0, .B=0, .A=255};
    s32 thickness = Gamestate->concentric_thickness;
    
    s32 origin_x = Gamestate->wnd_center_x;
    s32 origin_y = Gamestate->wnd_center_y;
    s32 spread_x = Gamestate->concentric_spread_x;
    s32 spread_y = Gamestate->concentric_spread_y;

    fill_background();

    for (s32 i=1; i<=concentric_count; i++)
        {
            r32 top = (r32)(origin_y-i*(spread_y+thickness));
            r32 left = (r32)(origin_x-i*(spread_x+thickness));
            r32 width = (r32)(i*2*(spread_x+thickness));
            r32 height = (r32)(i*2*(spread_y+thickness));

            wndrect rect = {
                .topleft = literal(v2) {.x=left, .y=top},
                .width = width,
                .height = height
            };

            if (concentric_z_values[i] >= Gamestate->nearclip &&
                concentric_z_values[i] <= Gamestate->farclip)
                {
                    draw_wndrect_outline_projected(rect, thickness, color);
                }
            Gamestate->concentric_current_z++;
        }

    //Gamestate->concentric_count = 7;
}



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
    pxl color = {.R=0, .G=0, .B=0, .A=255};
    s32 thickness = Gamestate->concentric_thickness;
    
    s32 origin_x = Gamestate->wnd_center_x;
    s32 origin_y = Gamestate->wnd_center_y;
    s32 spread_x = Gamestate->concentric_spread_x;
    s32 spread_y = Gamestate->concentric_spread_y;
    s32 concentric_count = Gamestate->concentric_count;
    
    fill_background();

    for (s32 i=1; i<=concentric_count; i++)
        {
            r32 top = (r32)(origin_y-i*(spread_y+thickness));
            r32 left = (r32)(origin_x-i*(spread_x+thickness));
            r32 width = (r32)(i*2*(spread_x+thickness));
            r32 height = (r32)(i*2*(spread_y+thickness));

            wndrect rect = {
                .topleft = literal(v2) {.x=left, .y=top},
                .width = width,
                .height = height
            };

            draw_wndrect_outline(rect, thickness, color);
        }
    //Gamestate->concentric_count = 10;
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
