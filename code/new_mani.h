#ifndef NEW_MANI_H
#define NEW_MANI_H



#define CONCENTRIC_MAX 50 // must be positive integer less than 256
#define MAX_BRUSHES 16

typedef enum
    {
        BRUSH_NONE = 0,
        BRUSH_SCANLINE = 1,
        BRUSH_SCANLINE2 = 2
    } BRUSH;

// @doc canvas plane is implicitly at zNear;
// canvas center is (0,0) relative to camera space origin;
// eye is positioned at camera space origin;
// camera is looking down negative Z in camera space;
// canvas aspect ration is implicitly 1, this also means FOVh = FOVv;
struct Camera
{
    Vec3f position;
    Quaternion orientation;
    r32 zNear;
    r32 zFar;
    r32 fov;
};

struct EngineState
{
    b32 inited;
    u16 frameBufferWidth;
    u16 frameBufferHeight;
    u8* frameBuffer;
    r32* Z_BUFFER;
    Camera mainCamera;

    // shize ------------------------------------
    // @TODO methinks VK combines mouse keys with keyboard keys.....
    u64 keyflags;
    u8 keymap[256]; // @Note 254 is the max VK code
    u8 mouseflags;
    v2 cursor;
    // shize ------------------------------------

    // temporary-----------------------------

    b32 tested_once;
    r32 camera_angle;
    b32 log_to_file_once;
    b32 reverse_winding;

    u32 brushes[MAX_BRUSHES];

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

    r32 camera_offs_x;
    r32 camera_offs_y;
    
    // temporary-----------------------------
};

// @todo change all aliases to inline functions bro...
// @todo change ENGINESTATE name to EngineState

#define ENGINESTATE ((EngineState*)PERM_ARENA)

#define FRAME_BUFFER ENGINESTATE->frameBuffer
#define FRAME_BUFFER_WIDTH ENGINESTATE->frameBufferWidth
#define FRAME_BUFFER_HEIGHT ENGINESTATE->frameBufferHeight
#define FRAME_BUFFER_PITCH (-FRAME_BUFFER_HEIGHT * BYTPP) // @doc ...
#define FRAME_BUFFER_BYTESIZE (FRAME_BUFFER_WIDTH * FRAME_BUFFER_HEIGHT * BYTPP)

#define Z_BUFFER ENGINESTATE->Z_BUFFER

#define MAIN_CAMERA ENGINESTATE->mainCamera
#define Z_NEAR MAIN_CAMERA.zNear
#define Z_FAR MAIN_CAMERA.zFar
#define FOV MAIN_CAMERA.fov

// @Note the cursor will always be in the windows' coordsys
// so if you want to draw around it, you have to translate
// to that coordsys

// @Note better name?
// @TODO do I make one for converting a wndrect (has to switch top and bottom)
#define to_yisdown(y) (FRAME_BUFFER_HEIGHT - (y))
#define to_yisup(y)   (FRAME_BUFFER_HEIGHT - (y))
//#define wndrect_yisdown(y)


// @TODO figure out default rotation direction cw or ccw and transforms...

#define GetBrush(type) (ENGINESTATE->brushes[(type)])
// @Note maybe not a good idea
#define SetBrush(type, color) ENGINESTATE->brushes[(type)] = (color)

// @Note converting when highest bit is 1 will result into wrapping to
// negative, but we don't care since we only use it in if statements ?
// @Note maybe make this an inline function instead?
#define ExtractKey(keyflags, keycode) ((b64) ((keyflags) & (keycode)) )

#endif
