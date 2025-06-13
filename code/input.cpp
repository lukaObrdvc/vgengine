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
