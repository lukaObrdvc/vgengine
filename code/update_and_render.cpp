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
    result->window_offs_x = 0;
    result->window_offs_y = 0;
    result->window_width = 1920;
    result->window_height = 1080;
    result->cursor_x = FRAMEBUFFER_WIDTH / 2;
    result->cursor_y = FRAMEBUFFER_HEIGHT / 2;
    result->change_display = true;
    result->fullscreen = true;
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
    
    fill_background();
    
    Matrix4* view = view_tmatrix_for_camera();
    Matrix4* proj = perspective_tmatrix_for_camera();
    
    model_matrix_test(view, proj);


    // @todo load/get font at specific size instead, so scaling is
    // not garbage
    
    Font* consolas = get_font(MYFONT_CONSOLAS);
    u8* glyph = get_glyph_bmp(consolas, 'a');

    r32 scale = 1.5f;
    
    // this is texel space
    s32 glyph_w = consolas->glyph_width + 2 * consolas->glyph_padding;
    s32 glyph_h = consolas->glyph_height + 2 * consolas->glyph_padding;
    s32 stride = FONT_BMP_ROW_COUNT * glyph_w * BYTPP;

    // this is pixel space
    s32 scaled_w = floori(glyph_w * scale);
    s32 scaled_h = floori(glyph_h * scale);

    // drawing is done in pixel space
    for (s32 j = 0; j < scaled_h; j++)
    {
        for (s32 i = 0; i < scaled_w; i++)
        {
            // you want to sample from a texture when scale != 1, because then num_texels != num_pixels,
            // and then you have to do texture filtering so that texels map to pixels in a good-looking way
            
            // texel coordinates (but not normalized in this case)
            r32 u = i / scale;
            r32 v = (scaled_h - 1 - j) / scale;
            
            Color src = bilinear_sample_premultiplied(glyph, glyph_w, glyph_h, stride, u, v);
            Color dest = u32_to_color(*framebuffer_access(i + 30, j + 30)); // this is not premultiplied actually, but it doesn't matter I think...?
            Color blended = color_blend_premultiplied(src, dest);
            *framebuffer_access(i + 30, j + 30) = color_to_u32(blended);
        }
    }    
    
    temp_reset();
    engine_state->normalization_counter++;

    bit_array_unset_all(INPUT->keys_pressed);
    bit_array_unset_all(INPUT->keys_released);
    INPUT->mkeys_pressed = unset_all_flags();
    INPUT->mkeys_released = unset_all_flags();
    INPUT->moved_mouse = false;
}
