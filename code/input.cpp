b32 process_input(u64 curr_keyflags_to_set,
                  u64 curr_keyflags_to_unset,
                  u8 curr_mouseflags_to_set,
                  u8 curr_mouseflags_to_unset,
                  v2 curr_cursor)
{
    // dude am I doing zbuffering correctly, maybe that's the problem??
    b32 result = true;

    // need to make WASD work in the direction camera is facing
    // moving by Y can be the same because we don't want to ever yaw
    // distance between prev and curr cursor gives you roll and pitch
    // by x component and y component;
    // but if you wanted to yaw (do it through keys)
    // then moving by Y is not the same
    
    // @TODO fix moving faster diagonally if you care...
    
    u64 prev_kflags = ENGINESTATE->keyflags;
    ENGINESTATE->keyflags |= curr_keyflags_to_set;
    ENGINESTATE->keyflags &= ~curr_keyflags_to_unset;
    u64 kflags = ENGINESTATE->keyflags;
    u64 kflags_trans = kflags ^ prev_kflags;
    u64 kflags_trans_to_up = kflags_trans & prev_kflags;
    u64 kflags_trans_to_down = kflags_trans & (~prev_kflags);
    
    u8 prev_mflags = ENGINESTATE->mouseflags;
    ENGINESTATE->mouseflags |= curr_mouseflags_to_set;
    ENGINESTATE->mouseflags &= ~curr_mouseflags_to_unset;
    u8 mflags = ENGINESTATE->mouseflags;
    u8 mflags_trans = mflags ^ prev_mflags;
    u8 mflags_trans_to_up = mflags_trans & prev_mflags;
    u8 mflags_trans_to_down = mflags_trans & (~prev_mflags);

    Camera camera = MAIN_CAMERA;
    curr_cursor.y = to_yisup(curr_cursor.y);
    v2 cursor_difference;
    cursor_difference.x = curr_cursor.x - FRAME_BUFFER_WIDTH/2;
    cursor_difference.y = curr_cursor.y - FRAME_BUFFER_HEIGHT/2;
    // camera.roll += cursor_difference.y / kilobytes(1);
    // camera.pitch -= cursor_difference.x / kilobytes(1);
    ENGINESTATE->cursor = curr_cursor;

    if (ExtractKey(kflags_trans_to_up, KEY_U))
        {
            if (ENGINESTATE->reverse_winding)
                {
                    ENGINESTATE->reverse_winding = false;
                }
            else
                {
                    ENGINESTATE->reverse_winding = true;
                }
        }
    
    if (ExtractKey(kflags, KEY_D))
        {
            // ENGINESTATE->camera_angle += pi/256;
            ENGINESTATE->log_to_file_once = true;
        }

    if (ExtractKey(kflags, KEY_A))
        {            
            // ENGINESTATE->camera_angle -= pi/256;
            ENGINESTATE->log_to_file_once = true;
        }
    
    if (ExtractKey(kflags, KEY_UP))
        {
            ENGINESTATE->camera_offs_y += 5.0f;
            ENGINESTATE->wnd_center_y+= 5;
            ENGINESTATE->dbg_render_y_offset+= 10;
        }
    if (ExtractKey(kflags, KEY_DOWN))
        {
            ENGINESTATE->camera_offs_y -= 5.0f;            
            ENGINESTATE->wnd_center_y-= 5;
            ENGINESTATE->dbg_render_y_offset-= 10;
        }
    if (ExtractKey(kflags, KEY_LEFT))
        {
            ENGINESTATE->camera_offs_x -= 5.0f;
            ENGINESTATE->wnd_center_x-= 5;
            ENGINESTATE->dbg_render_x_offset-= 10;
        }
    if (ExtractKey(kflags, KEY_RIGHT))
        {
            ENGINESTATE->camera_offs_x += 5.0f;
            ENGINESTATE->wnd_center_x+= 5;
            ENGINESTATE->dbg_render_x_offset+= 10;
        }
    
    v3 camera_movement = zero3();
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
    if (ExtractKey(kflags, KEY_Q))
        {
            camera_movement.y -= 2.0f;
        }
    if (ExtractKey(kflags, KEY_E))
        {
            camera_movement.y += 2.0f;
        }    

    // m4 rotationMatrix = M4Compose(2,
                                  // M4RotX(camera.roll),
                                  // M4RotY(camera.pitch)
                                  // );
    // camera_movement = M4Mul(camera_movement, rotationMatrix);
    // camera.fpoint = add3(camera.fpoint, camera_movement);
    
    if (result)
        {
            MAIN_CAMERA = camera;
        }
    
    if (ExtractKey(mflags_trans_to_up, MOUSE_M1))
        {
            ENGINESTATE->square_length += 50;
            ENGINESTATE->concentric_spread_x+=10;
            ENGINESTATE->concentric_spread_y+=4;
            ENGINESTATE->line_scaling_factor += 0.1f;
            ENGINESTATE->rect_scaling_factor += 0.1f;
            for (s32 i = 0; i < ENGINESTATE->concentric_count; i++)
                {
                    ENGINESTATE->concentric_z_values[i]++;
                }
        }
    if (ExtractKey(mflags_trans_to_up, MOUSE_M2))
        {
            ENGINESTATE->square_length -= 50;
            ENGINESTATE->concentric_spread_x-=10;
            ENGINESTATE->concentric_spread_y-=4;
            ENGINESTATE->line_scaling_factor -= 0.1f;
            ENGINESTATE->rect_scaling_factor -= 0.1f;
            for (s32 i = 0; i < ENGINESTATE->concentric_count; i++)
                {
                    ENGINESTATE->concentric_z_values[i]--;
                }
        }

    return result;
}
