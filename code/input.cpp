extern "C" b32 process_input(u64 curr_keyflags_to_set,
                             u64 curr_keyflags_to_unset,
                             u8 curr_mouseflags_to_set,
                             u8 curr_mouseflags_to_unset,
                             r32 curr_cursorX,
                             r32 curr_cursorY)
{
    b32 result = true;

    Engine_state* engine_state = ENGINE_STATE;

    // @todo fix moving faster diagonally if you care...

    // @todo use bitfields instead

    // @todo process_input should just set flags and cursor per frame
    // according to what the platform passes, and then these flags
    // should be globally available so you can "update" based on them
    // whenever you want

    u64 prev_kflags = engine_state->keyflags;
    engine_state->keyflags |= curr_keyflags_to_set;
    engine_state->keyflags &= ~curr_keyflags_to_unset;
    u64 kflags = engine_state->keyflags;
    u64 kflags_trans = kflags ^ prev_kflags;
    u64 kflags_trans_to_up = kflags_trans & prev_kflags;
    u64 kflags_trans_to_down = kflags_trans & (~prev_kflags);
    
    u8 prev_mflags = engine_state->mouseflags;
    engine_state->mouseflags |= curr_mouseflags_to_set;
    engine_state->mouseflags &= ~curr_mouseflags_to_unset;
    u8 mflags = engine_state->mouseflags;
    u8 mflags_trans = mflags ^ prev_mflags;
    u8 mflags_trans_to_up = mflags_trans & prev_mflags;
    u8 mflags_trans_to_down = mflags_trans & (~prev_mflags);

    curr_cursorY = to_yisup(curr_cursorY);
    Vector2 cursor_difference;
    cursor_difference.x = -(curr_cursorX - FRAMEBUFFER_WIDTH/2);
    cursor_difference.y = curr_cursorY - FRAMEBUFFER_HEIGHT/2;
    engine_state->cursor.x = -curr_cursorX; // @todo is this goodio?
    engine_state->cursor.y = curr_cursorY;

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
    
    if (ExtractKey(kflags_trans_to_up, KEY_U))
    {
        if (engine_state->reverse_winding)
        {
            engine_state->reverse_winding = false;
        }
        else
        {
            engine_state->reverse_winding = true;
        }
    }
    
    if (ExtractKey(kflags, KEY_D))
    {
        engine_state->camera_angle += PI/256;
    }

    if (ExtractKey(kflags, KEY_A))
    {            
        engine_state->camera_angle -= PI/256;
    }
    
    Vector3 camera_movement = vec_zero3();
    if (ExtractKey(kflags, KEY_W))
    {
        camera_movement.z -= 2.0f;
    }
    if (ExtractKey(kflags, KEY_S))
    {
        camera_movement.z += 2.0f;
    }
    if (ExtractKey(kflags, KEY_A))
    {
        camera_movement.x -= 2.0f;
    }
    if (ExtractKey(kflags, KEY_D))
    {
        camera_movement.x += 2.0f;
    }
    if (ExtractKey(kflags, KEY_Q)) // @todo y movement should be in world y, not based on camera orientation
    {
        camera_movement.y -= 2.0f;
    }
    if (ExtractKey(kflags, KEY_E))
    {
        camera_movement.y += 2.0f;
    }

    camera_movement = quaternion_rot_vector(camera_movement, camera->orientation);
    camera->position = vec_add(camera->position, camera_movement);

    if (ExtractKey(mflags_trans_to_up, MOUSE_M1))
    {
        
    }
    if (ExtractKey(mflags_trans_to_up, MOUSE_M2))
    {

    }

    return result;
}
