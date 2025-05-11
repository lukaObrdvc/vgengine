#ifndef NEW_MANI_H
#define NEW_MANI_H


struct EngineState
{
    b32 inited;
    FrameBuffer frameBuffer;
    u8* zBuffer;
    Camera mainCamera;

    // shize ------------------------------------
    // @TODO methinks VK combines mouse keys with keyboard keys.....
    u64 keyflags;
    u8 keymap[256]; // @Note 254 is the max VK code
    u8 mouseflags;
    Vec2f cursor;
    // shize ------------------------------------

    // temporary-----------------------------

    b32 tested_once;
    r32 camera_angle;
    b32 log_to_file_once;
    b32 reverse_winding;

    r32 line_angle;
    r32 rect_angle;

    r32 camera_offs_x;
    r32 camera_offs_y;
    
    // temporary-----------------------------
};

#define ENGINESTATE ((EngineState*)&PERM_ARENA)

#define FRAME_BUFFER ENGINESTATE->frameBuffer
#define FRAME_BUFFER_WIDTH FRAME_BUFFER.width
#define FRAME_BUFFER_HEIGHT FRAME_BUFFER.height
#define FRAME_BUFFER_BYTPP PLATFORM_API.platformDisplay.bytesPerPixel
#define FRAME_BUFFER_PITCH (FRAME_BUFFER_HEIGHT * FRAME_BUFFER_BYTPP)
#define FRAME_BUFFER_BYTESIZE (FRAME_BUFFER_WIDTH * FRAME_BUFFER_WIDTH * FRAME_BUFFER_BYTPP)

// @doc when we offset these like this, then we can - on y coordinate
// which allows the rest of the code to use y is up, and then this
// will map it to y is down
#define FRAME_BUFFER_BASE (FRAME_BUFFER.base + FRAME_BUFFER_BYTESIZE - FRAME_BUFFER_PITCH)
#define Z_BUFFER (ENGINESTATE->zBuffer + FRAME_BUFFER_BYTESIZE - FRAME_BUFFER_PITCH)



// @Note the cursor will always be in the windows' coordsys
// so if you want to draw around it, you have to translate
// to that coordsys

// @Note better name?
// @TODO do I make one for converting a wndrect (has to switch top and bottom)
#define to_yisdown(y) (FRAME_BUFFER_HEIGHT - (y))
#define to_yisup(y)   (FRAME_BUFFER_HEIGHT - (y))
//#define wndrect_yisdown(y)


// @TODO figure out default rotation direction cw or ccw and transforms...

// @Note converting when highest bit is 1 will result into wrapping to
// negative, but we don't care since we only use it in if statements ?
// @Note maybe make this an inline function instead?
#define ExtractKey(keyflags, keycode) ((b64) ((keyflags) & (keycode)) )

#endif
