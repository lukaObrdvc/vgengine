#ifndef MAIN_H
#define MAIN_H

#include "linalg.h"

// @Fail what if file not exist?
typedef b32 (*fp_dbg_read_file) (u8*, void*, u32*);
typedef b32 (*fp_dbg_write_file) (u8*, void*, u32);

// @TODO figure out byte sizes of these fields for alignment optimization and to not waste space

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

// @TODO how do I keep track of sizes of memories??
typedef struct platform_Provides
{
    void* perm_mem;
    u64 perm_mem_cap;
    void* temp_mem;
    u64 temp_mem_cap;

    s32 bytpp;            // access through macro
    // @TODO do I want stubs for these (and then
    // the question is does the platform or game
    // set the stub, and in what circumstances etc....)
    fp_dbg_read_file dbg_read_file;
    fp_dbg_write_file dbg_write_file;
} platform_provides;

global_variable platform_provides* memory_base;

#define DBG_READ_FILE memory_base->dbg_read_file
#define DBG_WRITE_FILE memory_base->dbg_write_file

#define CONCENTRIC_MAX 50 // must be positive integer less than 256

#pragma pack(push, 1)
typedef struct
{
    b32 inited;
    b32 tested_once;

    v2  cursor;

    s32 wndbuffer_width;  // access through macro
    s32 wndbuffer_height; // access through macro

    // @TODO these can designed better probably
    r32 eye_x;
    r32 eye_y;
    r32 screen_z;
    r32 nearclip;
    r32 farclip;
    
    s32 dbg_render_x_offset;
    s32 dbg_render_y_offset;
    r32 square_length;
    r32 concentric_thickness;
    s32 concentric_count;     // must be less than CONCENTRIC_MAX
    r32 concentric_spread_x;
    r32 concentric_spread_y;
    r32 concentric_z_values[CONCENTRIC_MAX];
    s32 concentric_current_z;
    r32 line_angle;
    r32 line_scaling_factor;
    r32 rect_angle;
    r32 rect_scaling_factor;
    
} game_state;
#pragma pack(pop)

#define Gamestate    ((game_state*)(memory_base->perm_mem))

#define wnd_width    (((game_state*)(memory_base->perm_mem))->wndbuffer_width)
#define wnd_height   (((game_state*)(memory_base->perm_mem))->wndbuffer_height)
#define wnd_bytpp    (memory_base->bytpp)                                      
#define wnd_bytesize (wnd_width*wnd_height*wnd_bytpp)

// @TODO need to have macros that assume a base coordinate system that
// is right handed and whose origin is in the lower left corner of the
// window, but that expand to things that transform into windows'
// actual base coordinate system (in upper left corner, y-is-down)

//#define wnd_pitch  (wnd_width*wnd_bytpp)
//#define wnd_buffer ((u8*)((u8*)(memory_base->perm_mem) + sizeof(game_state)))
#define wnd_pitch  (-wnd_width*wnd_bytpp)
#define wnd_buffer ((u8*)((u8*)(memory_base->perm_mem) +                \
                          sizeof(game_state) +                          \
                          wnd_bytesize + wnd_pitch))

#define wnd_nearclip (((game_state*)(memory_base->perm_mem))->nearclip)
#define wnd_farclip (((game_state*)(memory_base->perm_mem))->farclip)

#endif
