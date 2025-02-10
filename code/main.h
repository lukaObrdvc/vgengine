#ifndef CARDCRAFTER_H
#define CARDCRAFTER_H

#include "linalg.h"

// @Fail what if file not exist?
typedef b32 (*fp_dbg_read_file) (u8* filename, void* buffer, u32* buffer_size);
typedef b32 (*fp_dbg_write_file) (u8* filename, void* buffer, u32 buffer_size);

// @TODO figure out byte sizes of these fields for alignment optimization and to not waste space

typedef struct platform_resized_window_Buffer
{
    b32 to_resize;
    u8* memory_base;
    u32 width;
    u32 height;
} platform_resized_window_buffer;

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

/* // @Failure do I want unsigned here? */
/* typedef struct Cursor */
/* { */
/*     u32 x; */
/*     u32 y; */
/* } cursor; */

typedef enum Mouse_key_code
    {
        M_NONE,
        M1,
        M2,
        M3
    } mouse_key_code;

typedef struct Mouse
{
    v2 cursor;
    b32 mouse_moved;
    b32 is_down;
    mouse_key_code code;
} mouse;

typedef enum Key_code
    {
        KEY_NONE,
        KEY_UP,
        KEY_RIGHT,
        KEY_DOWN,
        KEY_LEFT
    } key_code;

typedef struct Keyboard_key
{
    key_code code;
    b32 is_down;
    b32 was_down;
} key;

/* typedef struct Input */
/* { */
/*     mouse mouse; */
/*     key key; */
/* } input; */

/* typedef struct Game_memory */
/* { */
/*     void* base; */
/*     u64 size;        // @Note should be in bytes */
/*     u64 capacity; */
/* } game_memory; */


void dbg_render (s8 x_offset, s8 y_offset);
void dbg_draw_square(u8 square_length);


b32 dbg_read_file_stub (u8* filename, void* buffer, u32* buffer_size)
{
    return false;
}

b32 dbg_write_file_stub (u8* filename, void* buffer, u32 buffer_size)
{
    return false;
}

global_variable fp_dbg_read_file dbg_read_file = dbg_read_file_stub;
global_variable fp_dbg_write_file dbg_write_file = dbg_write_file_stub;


global_variable window_buffer wndbuffer = {};        // now this has to be allocated
//global_variable cursor cursor_ = {};
//global_variable mouse_key mouse_frame_key = {};
//global_variable keyboard_key frame_key = {};
global_variable s8 x_offset = 0; // using
global_variable s8 y_offset = 0; // using
global_variable u8 square_length = 10; // using
global_variable void* memory_base;
global_variable u32 wndpitch = 0;                   // and this

inline void* dbg_alloc(u32 size)
{
    void* result;
    result = (u8*)memory_base+ *((u64)memory_base);
    *((u64)memory_base) += size;
    return result;
}

// @IMPORTANT you cannot inline if you want to export with DLL ???
void platform_init_game(void* memory_base_ptr,
                        u64 memory_capacity,
                        u8 bytpp,
                        fp_dbg_read_file read_file_proc,
                        fp_dbg_write_file write_file_proc)
{
    
    memory_base = memory_base_ptr;
    if (memory_capacity)
        {
            dbg_read_file = read_file_proc;
            dbg_write_file = write_file_proc;
            *((u64)memory_base) = 16; // memory_size
            *((u64)memory_base + 1) = memory_capacity;
            wndbuffer.bytpp = bytpp;
        }
}


#endif
