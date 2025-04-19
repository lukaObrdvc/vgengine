#ifndef NEW_MANI_H
#define NEW_MANI_H


// @TODO figure out byte sizes of these fields for alignment optimization and to not waste space

// @TODO do I use defines instead of enums at this point??
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

typedef struct
{
    void* perm_mem;
    u64 perm_mem_cap;
    void* temp_mem;
    u64 temp_mem_cap;
} EngineMemory;

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


typedef struct tagCamera_parameters
{
    // canvas is implicitly at the near clipping plane;
    
    // canvas center x,y are 0 relative to camera space origin;
    // eye is positioned at origin in camera space;
    // camera is looking down negative Z in camera space;
    // camera aspect ratio is implicitly 1
    r32 _near; // near clipping plane is on z=-1 in camera space
    r32 _far;
    r32 fov; // horizontal=vertical since aspect ratio is 1
} camera_parameters;

#pragma pack(push, 1)
typedef struct tagGame_state
{
    b32 inited;
    b32 tested_once;


    s32 wndbuffer_width;  // access through macro
    s32 wndbuffer_height; // access through macro
    /* r32 aspectRatio; */
    camera_parameters cameraParams;

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

    r32 camera_angle;
    b32 log_to_file_once;
    b32 reverse_winding;

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


#define ENGINE_MEMORY ((EngineMemory*)((u8*)(platformAPI) + sizeof(PlatformAPI)))
#define PERM_MEM      (ENGINE_MEMORY->perm_mem)
#define TEMP_MEM      (ENGINE_MEMORY->perm_mem)

#define Gamestate    ((game_state*)(PERM_MEM))
#define Assets       ((Mesh*)(TEMP_MEM))

#define wnd_width    (Gamestate->wndbuffer_width)
#define wnd_height   (Gamestate->wndbuffer_height)
#define wnd_bytpp    (platformAPI->platformDisplay.bytesPerPixel)
#define wnd_bytesize (wnd_width*wnd_height*wnd_bytpp)      // cache in scope ?

#define wnd_pitch    (-wnd_width*wnd_bytpp)                // cache in scope ?
#define wnd_buffer   (((u8*)(PERM_MEM) +                \
                       sizeof(game_state) +             \
                       wnd_bytesize + wnd_pitch))

#define wnd_nearclip (Gamestate->nearclip)
#define wnd_farclip  (Gamestate->farclip)

/* #define Gamestate    ((game_state*)(memory_base->perm_mem)) */

/* #define wnd_width    (((game_state*)(memory_base->perm_mem))->wndbuffer_width) */
/* #define wnd_height   (((game_state*)(memory_base->perm_mem))->wndbuffer_height) */
/* #define wnd_bytpp    (memory_base->bytpp)                                       */
/* #define wnd_bytesize (wnd_width*wnd_height*wnd_bytpp)      // cache in scope ? */

/* #define wnd_pitch    (-wnd_width*wnd_bytpp)                // cache in scope ? */
/* #define wnd_buffer   (((u8*)(memory_base->perm_mem) +   \ */
/*                        sizeof(game_state) +             \ */
/*                        wnd_bytesize + wnd_pitch)) */

/* #define wnd_nearclip (((game_state*)(memory_base->perm_mem))->nearclip) */
/* #define wnd_farclip  (((game_state*)(memory_base->perm_mem))->farclip) */

// @Note the cursor will always be in the windows' coordsys
// so if you want to draw around it, you have to translate
// to that coordsys

// @Note better name?
// @TODO do I make one for converting a wndrect (has to switch top and bottom)
#define to_yisdown(y) (wnd_height - (y))
#define to_yisup(y)   (wnd_height - (y))
//#define wndrect_yisdown(y)

// cast to r32* after you offset...
#define zbuffer (((u8*)(PERM_MEM) +                   \
                  sizeof(game_state) +                \
                  wnd_bytesize +                      \
                  wnd_bytesize + wnd_pitch))


// @TODO figure out default rotation direction cw or ccw and transforms...

#define GetBrush(type) (Gamestate->brushes[(type)])
// @Note maybe not a good idea
#define SetBrush(type, color) Gamestate->brushes[(type)] = (color)

// @Note converting when highest bit is 1 will result into wrapping to
// negative, but we don't care since we only use it in if statements ?
// @Note maybe make this an inline function instead?
#define ExtractKey(keyflags, keycode) ((b64) ((keyflags) & (keycode)) )

#endif
