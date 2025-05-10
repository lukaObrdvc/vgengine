// @todo do I use defines instead of enums at this point??

enum
{
    MOUSE_NONE = 0,
    MOUSE_MOVE = (1 << 0), // @fail MOUSE_MOVED is defined in windows...
    MOUSE_M1   = (1 << 1),
    MOUSE_M2   = (1 << 2),
    MOUSE_M3   = (1 << 3)
};

enum
{
    KEY_NONE  = 0,
    KEY_LEFT  = (1 << 0),
    KEY_UP    = (1 << 1), 
    KEY_RIGHT = (1 << 2), 
    KEY_DOWN  = (1 << 3), 
    KEY_W     = (1 << 4), 
    KEY_S     = (1 << 5), 
    KEY_A     = (1 << 6), 
    KEY_D     = (1 << 7), 
    KEY_Q     = (1 << 8), 
    KEY_E     = (1 << 9), 
    KEY_I     = (1 << 10),
    KEY_K     = (1 << 11),
    KEY_J     = (1 << 12),
    KEY_L     = (1 << 13),
    KEY_U     = (1 << 14),
    KEY_O     = (1 << 15)
};

b32 process_input(u64 curr_keyflags_to_set,
                  u64 curr_keyflags_to_unset,
                  u8 curr_mouseflags_to_set,
                  u8 curr_mouseflags_to_unset,
                  v2 curr_cursor);

