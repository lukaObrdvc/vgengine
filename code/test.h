#define CURRENTLY_TESTING concentric    ##_test
#define TEST_ONLY_ONCE Gamestate->tested_once = true;

void seethislater_test()
{
    //draw_line();
    //if (linex == 0) a+= PI/8;
    //linex++;
    //offsetedcs.offset = a;
    //wndrect outline;
    //outline = obtain_wndrect(grect);
    //clamp_wndrect(&outline);
    //draw_rotated_rect(outline);
    
    //a+= PI/(megabytes(1));
    //linex++;
    //liney++;
    //liney++;
    //v2 ll;
    
    /*if (counter <= 300)
      {
      s-= 5.0f/megabytes(1);
      }
      else
      {
      s+= 5.0f/megabytes(1);
      }
      counter++;
      if (counter == 600)
      {
      s = 0;
      counter = 0;
      }*/
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
