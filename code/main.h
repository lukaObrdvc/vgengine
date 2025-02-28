#ifndef MAIN_H
#define MAIN_H

#include "vector.h"

// @Fail what if file not exist?
typedef b32 (*fp_dbg_read_file) (u8*, void*, u32*);
typedef b32 (*fp_dbg_write_file) (u8*, void*, u32);

// @TODO figure out byte sizes of these fields for alignment optimization and to not waste space

typedef enum tagMOUSECODE
    {
        MOUSE_NONE = 0,
        MOUSE_MOVE = (1 << 0), // @Noted MOUSE_MOVED is defined in windows...
        MOUSE_M1   = (1 << 1),
        MOUSE_M2   = (1 << 2),
        MOUSE_M3   = (1 << 3)
    } MOUSECODE;

typedef enum tagKEYCODe
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
    } KEYCODE;

// @TODO how do I keep track of sizes of memories??
typedef struct
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
#define MAX_BRUSHES 16

typedef enum
    {
        BRUSH_NONE = 0,
        BRUSH_SCANLINE = 1,
        BRUSH_SCANLINE2 = 2
    } BRUSH;

typedef enum
    {
        ORTHOGRAPHIC,
        PERSPECTIVE
    } PROJECTION;

typedef struct
{
    v3 fpoint;
    r32 yaw;
    r32 pitch;
    r32 roll;
} Camera;

#pragma pack(push, 1)
typedef struct
{
    b32 inited;
    b32 tested_once;


    s32 wndbuffer_width;  // access through macro
    s32 wndbuffer_height; // access through macro

    // @TODO methinks VK combines mouse keys with keyboard keys.....
    u64 keyflags;
    u8 keymap[256]; // @Note 254 is the max VK code
    u8 mouseflags;
    v2 cursor;

    // @TODO these can be designed better probably
    r32 eye_x;
    r32 eye_y;
    r32 screen_z;
    r32 new_screen_z;
    r32 nearclip;
    r32 farclip;
    Camera camera;

    u32 brushes[MAX_BRUSHES];

    // @Note this is what the input changes,
    // probably think of something better
    r32 wnd_center_x; 
    r32 wnd_center_y;
    
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
#define wnd_bytesize (wnd_width*wnd_height*wnd_bytpp)      // cache in scope ?

#define wnd_pitch    (-wnd_width*wnd_bytpp)                // cache in scope ?
#define wnd_buffer   (((u8*)(memory_base->perm_mem) +   \
                       sizeof(game_state) +             \
                       wnd_bytesize + wnd_pitch))

#define wnd_nearclip (((game_state*)(memory_base->perm_mem))->nearclip)
#define wnd_farclip  (((game_state*)(memory_base->perm_mem))->farclip)

// @Note the cursor will always be in the windows' coordsys
// so if you want to draw around it, you have to translate
// to that coordsys

// @Note better name?
// @TODO do I make one for converting a wndrect (has to switch top and bottom)
#define to_yisdown(y) (wnd_height - (y))
//#define wndrect_yisdown(y)

// cast to r32* after you offset...
#define zbuffer (((u8*)(memory_base->perm_mem) +    \
                  sizeof(game_state) +              \
                  wnd_bytesize +                    \
                  wnd_bytesize + wnd_pitch))


// @TODO figure out default rotation direction cw or ccw and transforms...

#define GetBrush(type) (Gamestate->brushes[(type)])
// @Note maybe not a good idea
#define SetBrush(type, color) Gamestate->brushes[(type)] = (color)

// @Note converting when highest bit is 1 will result into wrapping to
// negative, but we don't care since we only use it in if statements ?
#define ExtractKey(keyflags, keycode) ((b64) ((keyflags) & (keycode)) )

#endif
