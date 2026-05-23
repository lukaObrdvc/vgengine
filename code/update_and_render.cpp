struct Job_Data
{
    int* chunk;
    int size;
};

JOB(testing)
{
    int* chunk = ((Job_Data*)data)->chunk;
    int size = ((Job_Data*)data)->size;

    for (int i = 0; i < size; i++)
    {
        chunk[i]++;
    }
}

extern "C" void update_and_render(Platform_frame_pass* pass, Engine_frame_result* result)
{
    // exclusive fullscreen: window dims and framebuffer dims equal
    // borderless fullscreen: window dims native, framebuffer dims separate

    // you don't have to exactly set window dims not native for exclusive
    // I guess, what happens is when you go exclusive fullscreen
    // windows will set the default resolution for the monitor
    // to the dimensions of the window

    // platform stretches framebuffer onto window

    // @todo cache framebuffer stuff instead (and other things like input ??)

    Engine_state* engine_state = ENGINE_STATE;
    FRAMEBUFFER_WIDTH = 860;
    FRAMEBUFFER_HEIGHT = 420;
    
    FRAMEBUFFER_BASE = temp_alloc(u8, FRAMEBUFFER_BYTESIZE);
    ZBUFFER = temp_alloc(r32, FRAMEBUFFER_WIDTH * FRAMEBUFFER_HEIGHT);
    // @doc this guarantees Y is up when accessing these buffers
    FRAMEBUFFER_BASE += FRAMEBUFFER_BYTESIZE - FRAMEBUFFER_PITCH;
    ZBUFFER += FRAMEBUFFER_WIDTH * (FRAMEBUFFER_HEIGHT - 1);

    
   // @todo engine should pass information about whether to set
   // the cursor? Or just set it to what the engine says every time?
    
    result->cap_frame_rate = true;
    result->target_frame_rate = 60;
    result->show_cursor = false;
    // this passes back the beginning of the buffer to the platform
    result->window_buffer_base = (void*)(FRAMEBUFFER_BASE - FRAMEBUFFER_BYTESIZE + FRAMEBUFFER_PITCH);
    result->window_buffer_width = FRAMEBUFFER_WIDTH;
    result->window_buffer_height = FRAMEBUFFER_HEIGHT;
    result->resize = true;
    result->window_offs_x = 300;
    result->window_offs_y = 150;
    result->window_width = 960;
    result->window_height = 540;
    result->cursor_x = FRAMEBUFFER_WIDTH / 2;
    result->cursor_y = FRAMEBUFFER_HEIGHT / 2;
    result->change_display = true;
    result->fullscreen = false;
    result->exclusive_fullscreen = false;
    // @todo how do I get into the normal mode with cursor?
    
    zbuffer_reset(); // this is here why exactly? something to do with initialization or ordering or something..?
    
    // turns y coordinate from y is down to y is up (ll corner is origin)
    INPUT->cursor_y = FRAMEBUFFER_HEIGHT - INPUT->cursor_y; // do I abstract this through a function call?
    
    Vector2 cursor_difference;
    cursor_difference.x = -(CURSOR_X - FRAMEBUFFER_WIDTH/2);
    cursor_difference.y = CURSOR_Y - FRAMEBUFFER_HEIGHT/2;

    Camera* camera = MAIN_CAMERA;
    r32 x_angle = cursor_difference.y / (2 * KB);
    r32 y_angle = cursor_difference.x / (2 * KB);

    // rotating around world Y because we don't wanna move diagonally when moving mouse left/right, but always around world Y
    // this is not the case when rotating around X, we wanna rotate around local X
    Quaternion rot_around_y = quaternion_from_axis(vec_up(), y_angle);
    Quaternion rot_around_x = quaternion_from_axis(quaternion_rot_vector(vec_right(), camera->orientation), x_angle);
    
    camera->orientation = quaternion_chain(quaternion_chain(camera->orientation, rot_around_x), rot_around_y);

    if (!engine_state->normalization_counter)
    {
        camera->orientation = quaternion_normalize(camera->orientation);
    }

    // @todo fix moving faster diagonally if you care...
    Vector3 camera_movement = vec_zero3();
    if (key_down(KEY_W))
    {
        camera_movement.z -= 2.0f;
    }
    if (key_down(KEY_S))
    {
        camera_movement.z += 2.0f;
    }
    if (key_down(KEY_A))
    {
        camera_movement.x -= 2.0f;
    }
    if (key_down(KEY_D))
    {
        camera_movement.x += 2.0f;
    }
    if (key_down(KEY_Q))
    {
        camera_movement.y -= 2.0f;
    }
    if (key_down(KEY_E))
    {
        camera_movement.y += 2.0f;
    }

    camera_movement = quaternion_rot_vector(camera_movement, camera->orientation);
    camera->position = vec_add(camera->position, camera_movement);

    if (key_pressed(KEY_U))
    {
        result->resize = true;
        FRAMEBUFFER_WIDTH = 500;
        FRAMEBUFFER_HEIGHT = 500;
        result->window_width = 500;
        result->window_height = 500;
    }
    if (key_released(KEY_U))
    {
        result->resize = true;
        FRAMEBUFFER_WIDTH = 1280;
        FRAMEBUFFER_HEIGHT = 720;
        result->window_width = 1280;
        result->window_height = 720;
    }
    
    Matrix4* view = view_tmatrix_for_camera();
    Matrix4* proj = perspective_tmatrix_for_camera();

    
    BEGIN_PROFILING(str("demo profiling"));

    fill_background_parallel();
    // fill_background_sequentially();
    
    model_matrix_test(view, proj);

    END_PROFILING();

    
    Profiler* p = &PROFILERS[0];
    String string = concat(7, p->name, str("\n"), to_string(p->cycle_diff / (r64)1000000, 4),
                           str(" MegaCycles"), str("\n"), to_string(p->time_diff, 2), str(" ms"));
    Font* font = get_font(MYFONT_CONSOLAS16);
    Vector2 offset = vec_make(5.0f, 390.0f);
    r32 line_spacing = 0.0f;
    draw_string(string, font, offset, line_spacing);
    
    engine_state->font_offset.x -= 1.0f;
    engine_state->font_offset.y += 1.0f;
    
    temp_reset();
    engine_state->normalization_counter++;
    engine_state->frame_counter++;

    bit_array_unset_all(INPUT->keys_pressed);
    bit_array_unset_all(INPUT->keys_released);
    INPUT->mkeys_pressed = unset_all_flags();
    INPUT->mkeys_released = unset_all_flags();
    INPUT->moved_mouse = false;
}
