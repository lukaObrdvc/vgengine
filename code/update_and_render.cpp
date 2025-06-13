extern "C" Engine_frame_result update_and_render()
{
    // turns y coordinate from y is down to y is up (ll corner is origin)
    INPUT->cursor_y = FRAMEBUFFER_HEIGHT - INPUT->cursor_y; // do I abstract this through a function call?

    Engine_frame_result result;
    result.show_cursor = false;
    
    Engine_state* engine_state = ENGINE_STATE;
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
    if (get_key(KEY_W))
    {
        camera_movement.z -= 2.0f;
    }
    if (get_key(KEY_S))
    {
        camera_movement.z += 2.0f;
    }
    if (get_key(KEY_A))
    {
        camera_movement.x -= 2.0f;
    }
    if (get_key(KEY_D))
    {
        camera_movement.x += 2.0f;
    }
    if (get_key(KEY_Q))
    {
        camera_movement.y -= 2.0f;
    }
    if (get_key(KEY_E))
    {
        camera_movement.y += 2.0f;
    }

    camera_movement = quaternion_rot_vector(camera_movement, camera->orientation);
    camera->position = vec_add(camera->position, camera_movement);
    
    
    Framebuffer framebuffer = engine_state->framebuffer;
    r32* zbuffer = engine_state->zbuffer;
    s32 bytpp = BYTPP;
    s32 pitch = framebuffer_pitch(framebuffer.height, bytpp);

    fill_background();
    
    Matrix4* view = view_tmatrix_for_camera();
    Matrix4* proj = perspective_tmatrix_for_camera();
    
    model_matrix_test(view, proj);
    
    zbuffer_reset(zbuffer, framebuffer.width, framebuffer.height);
    temp_reset();
    engine_state->normalization_counter++;

    bit_array_unset_all(INPUT->keys_pressed);
    bit_array_unset_all(INPUT->keys_released);
    INPUT->mkeys_pressed = unset_all_flags();
    INPUT->mkeys_released = unset_all_flags();
    INPUT->moved_mouse = false;
    
    return result;
}
