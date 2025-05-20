extern "C" b32 process_input(u64 curr_keyflags_to_set,
                             u64 curr_keyflags_to_unset,
                             u8 curr_mouseflags_to_set,
                             u8 curr_mouseflags_to_unset,
                             r32 curr_cursorX,
                             r32 curr_cursorY)
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
    
    u64 prev_kflags = ENGINE_STATE->keyflags;
    ENGINE_STATE->keyflags |= curr_keyflags_to_set;
    ENGINE_STATE->keyflags &= ~curr_keyflags_to_unset;
    u64 kflags = ENGINE_STATE->keyflags;
    u64 kflags_trans = kflags ^ prev_kflags;
    u64 kflags_trans_to_up = kflags_trans & prev_kflags;
    u64 kflags_trans_to_down = kflags_trans & (~prev_kflags);
    
    u8 prev_mflags = ENGINE_STATE->mouseflags;
    ENGINE_STATE->mouseflags |= curr_mouseflags_to_set;
    ENGINE_STATE->mouseflags &= ~curr_mouseflags_to_unset;
    u8 mflags = ENGINE_STATE->mouseflags;
    u8 mflags_trans = mflags ^ prev_mflags;
    u8 mflags_trans_to_up = mflags_trans & prev_mflags;
    u8 mflags_trans_to_down = mflags_trans & (~prev_mflags);

    Camera camera = MAIN_CAMERA;
    curr_cursorY = to_yisup(curr_cursorY);
    Vector2 cursor_difference;
    cursor_difference.x = curr_cursorX - FRAMEBUFFER_WIDTH/2;
    cursor_difference.y = curr_cursorY - FRAMEBUFFER_HEIGHT/2;
    // camera.roll += cursor_difference.y / kilobytes(1);
    // camera.pitch -= cursor_difference.x / kilobytes(1);
    ENGINE_STATE->cursor.x = curr_cursorX;
    ENGINE_STATE->cursor.y = curr_cursorY;

    if (ExtractKey(kflags_trans_to_up, KEY_U))
        {
            if (ENGINE_STATE->reverse_winding)
                {
                    ENGINE_STATE->reverse_winding = false;
                }
            else
                {
                    ENGINE_STATE->reverse_winding = true;
                }
        }
    
    if (ExtractKey(kflags, KEY_D))
        {
            // ENGINE_STATE->camera_angle += pi/256;
        }

    if (ExtractKey(kflags, KEY_A))
        {            
            // ENGINE_STATE->camera_angle -= pi/256;
        }
    
    if (ExtractKey(kflags, KEY_UP))
        {
            ENGINE_STATE->camera_offs_y += 5.0f;
        }
    if (ExtractKey(kflags, KEY_DOWN))
        {
            ENGINE_STATE->camera_offs_y -= 5.0f;            
        }
    if (ExtractKey(kflags, KEY_LEFT))
        {
            ENGINE_STATE->camera_offs_x -= 5.0f;
        }
    if (ExtractKey(kflags, KEY_RIGHT))
        {
            ENGINE_STATE->camera_offs_x += 5.0f;
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

        }
    if (ExtractKey(mflags_trans_to_up, MOUSE_M2))
        {

        }

    return result;
}
