#ifndef INPUT_H
#define INPUT_H

#define NUM_KEYS 50 // actually 17
#define NUM_MKEYS 6 // actually 4

struct Input
{
    Bit_array keys;
    Bit_array keys_pressed;
    Bit_array keys_released;
    
    u8 mkeys;
    u8 mkeys_pressed;
    u8 mkeys_released;
    
    b32 moved_mouse;
    r32 cursor_x;
    r32 cursor_y;
};

enum KEYCODE
{
    KEY_NONE,  // 0
    KEY_LEFT,  // 1
    KEY_UP,    // 2
    KEY_RIGHT, // 3
    KEY_DOWN,  // 4
    KEY_W,     // 5
    KEY_S,     // 6
    KEY_A,     // 7
    KEY_D,     // 8
    KEY_Q,     // 9
    KEY_E,     // 10
    KEY_I,     // 11
    KEY_K,     // 12
    KEY_J,     // 13
    KEY_L,     // 14
    KEY_U,     // 15
    KEY_O      // 16
};

enum MKEYCODE
{
    MKEY_NONE,
    MKEY_M1,
    MKEY_M2,
    MKEY_M3
};

#endif
