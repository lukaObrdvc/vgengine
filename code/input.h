#ifndef INPUT_H
#define INPUT_H

#define NUM_KEYS 30
#define NUM_MKEYS 3


// @todo init bit arrays by max_keys
// @todo [] operator for bit array
struct Input
{
    Bit_array keys;
    // these should be reset to 0 at the start of the frame
    // and then the platform passes information about which keys
    // changed and you just toggle that with keys, and then deduce
    // keys_pressed and keys_released from previous keys and new
    // keys and so on (by xor-ing)...
    Bit_array keys_pressed; // changed from 0 to 1
    Bit_array keys_released; // changed from 1 to 0
    
    u8 mkeys;
    u8 mkeys_pressed;
    u8 mkeys_released;
    
    b32 moved_mouse;
    r32 cursor_x;
    r32 cursor_y;
};

struct Platform_input_pass
{
    Bit_array changed_keys;
    u8 changed_mkeys;
    b32 moved_mouse;
    r32 cursor_x;
    r32 cursor_y;
};

enum KEYCODE
{
    KEY_NONE,
    KEY_LEFT,
    KEY_UP,
    KEY_RIGHT,
    KEY_DOWN,
    KEY_W,
    KEY_S,
    KEY_A,
    KEY_D,
    KEY_Q,
    KEY_E,
    KEY_I,
    KEY_K,
    KEY_J,
    KEY_L,
    KEY_U,
    KEY_O
};

enum MKEYCODE
{
    MKEY_NONE,
    MKEY_M1,
    MKEY_M2,
    MKEY_M3
};

#endif
