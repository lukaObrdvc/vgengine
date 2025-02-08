#ifndef CARDCRAFTER_H
#define CARDCRAFTER_H


// @Fail what if file not exist?
typedef b32 (*dbg_read_file) (u8* filename, void* buffer, u32* buffer_size);
typedef b32 (*dbg_write_file) (u8* filename, void* buffer, u32 buffer_size);

// @TODO figure out byte sizes of these fields for alignment optimization and to not waste space

typedef struct Window_buffer
{
    u8* memory;
    u32 width;
    u32 height;
    u8 bytpp;
} window_buffer;

// @Note do I want a v2 here?
typedef struct Wndrect
{
    v2 topleft;
    r32 width;
    r32 height;
} wndrect;

// @Failure do I want unsigned here?
typedef struct Cursor
{
    u32 x;
    u32 y;
} cursor;

typedef enum Mouse_key_code
    {
        M_NONE,
        M1,
        M2,
        M3
    } mouse_key_code;

typedef struct Mouse_key
{
    b32 mouse_moved;
    b32 is_down;
    mouse_key_code code;
} mouse_key;

typedef struct Mouse
{
    cursor cursor;
    mouse_key key;
} mouse;

typedef enum Key_code
    {
        KEY_NONE,
        KEY_UP,
        KEY_RIGHT,
        KEY_DOWN,
        KEY_LEFT
    } key_code;

// @Note do I need repeat count, syskey?
typedef struct Keyboard_key
{
    key_code code;
    b32 is_down;
    b32 was_down;
} keyboard_key;

typedef struct Game_memory
{
    void* base;
    u64 size;
    u64 capacity;
} game_memory;

void dbg_render (s8 x_offset, s8 y_offset);
void dbg_draw_square(u8 square_length);
void invalidate_player_input();
void process_player_input();
void update_and_render();


b32 dbg_readfile_stub (u8* filename, void* buffer, u32* buffer_size)
{
    return false;
}

b32 dbg_writefile_stub (u8* filename, void* buffer, u32 buffer_size)
{
    return false;
}

global_variable dbg_read_file dbg_readfile = dbg_readfile_stub;
global_variable dbg_write_file dbg_writefile = dbg_writefile_stub;

global_variable window_buffer wndbuffer = {};
global_variable cursor cursor_ = {};
global_variable mouse_key mouse_frame_key = {};
global_variable keyboard_key frame_key = {};
global_variable s8 x_offset = 0;
global_variable s8 y_offset = 0;
global_variable u8 square_length = 10;
global_variable game_memory memory = {};
global_variable u32 wndpitch = 0;



#endif
