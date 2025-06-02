#ifndef NEW_MANI_H
#define NEW_MANI_H


struct Engine_state
{
    b32 tested_once;
    
    Framebuffer framebuffer;
    r32* zbuffer;
    Camera main_camera;

    // shize ------------------------------------
    // @TODO methinks VK combines mouse keys with keyboard keys.....
    u64 keyflags;
    u8 keymap[256]; // @Note 254 is the max VK code
    u8 mouseflags;
    Vector2 cursor;
    // shize ------------------------------------

    // temporary-----------------------------
    r32 camera_angle;
    r32 line_angle;
    r32 spin_angle;
    b32 reverse_winding;
    // temporary-----------------------------
};

#define ENGINE_STATE ((Engine_state*)(PERMANENT_ARENA.base))

#define FRAMEBUFFER (ENGINE_STATE->framebuffer)
#define FRAMEBUFFER_WIDTH (FRAMEBUFFER.width)
#define FRAMEBUFFER_HEIGHT (FRAMEBUFFER.height)
#define FRAMEBUFFER_BYTPP (PLATFORM_API.bytes_per_pixel)
// @todo maybe calculate these once, cache them in engine_state
#define FRAMEBUFFER_PITCH (FRAMEBUFFER_WIDTH * FRAMEBUFFER_BYTPP)
#define FRAMEBUFFER_BYTESIZE (FRAMEBUFFER_WIDTH * FRAMEBUFFER_HEIGHT * FRAMEBUFFER_BYTPP)

// @doc when we offset these like this, then we can - on y coordinate
// which allows the rest of the code to use y is up, and then this
// will map it to y is down
// #define FRAMEBUFFER_BASE (FRAMEBUFFER.base + FRAMEBUFFER_BYTESIZE - FRAMEBUFFER_PITCH)
#define FRAMEBUFFER_BASE (FRAMEBUFFER.base)
// #define Z_BUFFER (((r32*)ENGINESTATE->zBuffer) + FRAME_BUFFER_WIDTH*(FRAME_BUFFER_HEIGHT -1))
#define ZBUFFER (ENGINE_STATE->zbuffer)



// @Note the cursor will always be in the windows' coordsys
// so if you want to draw around it, you have to translate
// to that coordsys

// @Note better name?
// @TODO do I make one for converting a wndrect (has to switch top and bottom)
#define to_yisdown(y) (FRAMEBUFFER_HEIGHT - (y))
#define to_yisup(y)   (FRAMEBUFFER_HEIGHT - (y))
//#define wndrect_yisdown(y)


// @TODO figure out default rotation direction cw or ccw and transforms...

// @Note converting when highest bit is 1 will result into wrapping to
// negative, but we don't care since we only use it in if statements ?
// @Note maybe make this an inline function instead?
#define ExtractKey(keyflags, keycode) ((b64) ((keyflags) & (keycode)) )

#endif
