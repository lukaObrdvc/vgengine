#define INPUT (&ENGINE_STATE->input)
#define MOVED_MOUSE (INPUT->moved_mouse)
#define CURSOR_X (INPUT->cursor_x)
#define CURSOR_Y (INPUT->cursor_y)

inline b32 get_key(KEYCODE key)
{
    return bit_array_get(INPUT->keys, key);
}
inline b32 key_changed(KEYCODE key)
{
    return bit_array_get(INPUT->keys_pressed, key) | bit_array_get(INPUT->keys_released, key);
}
inline b32 key_pressed(KEYCODE key)
{
    return bit_array_get(INPUT->keys_pressed, key);
}
inline b32 key_released(KEYCODE key)
{
    return bit_array_get(INPUT->keys_released, key);
}

inline b32 get_mkey(MKEYCODE mkey)
{
    return get_flags(INPUT->mkeys, mkey);
}
inline b32 mkey_changed(MKEYCODE mkey)
{
    return get_flags(INPUT->mkeys_pressed, mkey) | get_flags(INPUT->mkeys_released, mkey);
}
inline b32 mkey_pressed(MKEYCODE mkey)
{
    return get_flags(INPUT->mkeys_pressed, mkey);
}
inline b32 mkey_released(MKEYCODE mkey)
{
    return get_flags(INPUT->mkeys_released, mkey);
}

void process_input(Platform_input_pass args)
{
    Input* input = INPUT;

    Bit_array old_keys = input->keys;
    bit_array_toggle(input->keys, args.changed_keys);
    bit_array_diff_in_1(input->keys, old_keys, input->keys_pressed);
    bit_array_diff_in_0(input->keys, old_keys, input->keys_released);

    u8 old_mkeys = input->mkeys;
    input->mkeys = toggle_flags(input->mkeys, args.changed_mkeys);
    input->mkeys_pressed = diff_flags_in_1(input->mkeys, old_mkeys);
    input->mkeys_released = diff_flags_in_0(input->mkeys, old_mkeys);

    input->moved_mouse = args.moved_mouse;
    input->cursor_x = args.cursor_x;
    // turns y coordinate from y is down to y is up (ll corner is origin)
    input->cursor_y = FRAMEBUFFER_HEIGHT - args.cursor_y; // do I abstract this through a function call?
}
