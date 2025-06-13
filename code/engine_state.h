#ifndef ENGINE_STATE_H
#define ENGINE_STATE_H


// @doc mainly for permanent information and convinience so you can
// globally access some things, instead of passing them to million
// scopes, and then make aliases for these things so you don't
// type the same thing a million times, and god forbid have to
// refactor all that

struct Engine_state
{
    Input input;
    Framebuffer framebuffer;
    r32* zbuffer;
    Camera main_camera;
    r32 aspect_ratio;
    u8 normalization_counter;
    u32 prng_seed;

    // temporary-----------------------------
    r32 camera_angle;
    r32 line_angle;
    r32 spin_angle;
    b32 reverse_winding;
    Transform cube_transform;
    r32 cube_scaling_factor;
    b32 cube_scale_up;
    // temporary-----------------------------
    
    b32 tested_once;
};

#define ENGINE_STATE ((Engine_state*)(PERMANENT_ARENA->base))

#define FRAMEBUFFER (ENGINE_STATE->framebuffer)
#define FRAMEBUFFER_WIDTH (FRAMEBUFFER.width)
#define FRAMEBUFFER_HEIGHT (FRAMEBUFFER.height)
#define FRAMEBUFFER_PITCH (FRAMEBUFFER_WIDTH * BYTPP) // @todo maybe calculate these once, cache them in engine_state
#define FRAMEBUFFER_BYTESIZE (FRAMEBUFFER_WIDTH * FRAMEBUFFER_HEIGHT * BYTPP) // @todo same?
#define FRAMEBUFFER_BASE (FRAMEBUFFER.base)

#define ZBUFFER (ENGINE_STATE->zbuffer)

#define ASPECT_RATIO (ENGINE_STATE->aspect_ratio)

#endif
