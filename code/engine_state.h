#ifndef ENGINE_STATE_H
#define ENGINE_STATE_H

// @todo does this respect alignment??????
#define ENGINE_STATE ((Engine_state*)(PERMANENT_ARENA->base))

// @todo place these in framebuffer.cpp?
#define FRAMEBUFFER_WIDTH (ENGINE_STATE->framebuffer_width)
#define FRAMEBUFFER_HEIGHT (ENGINE_STATE->framebuffer_height)
#define FRAMEBUFFER_PITCH (FRAMEBUFFER_WIDTH * BYTPP) // @todo maybe calculate these once, cache them in engine_state
#define FRAMEBUFFER_BYTESIZE (FRAMEBUFFER_WIDTH * FRAMEBUFFER_HEIGHT * BYTPP) // @todo same?
#define FRAMEBUFFER_BASE (ENGINE_STATE->framebuffer_base)

#define ZBUFFER (ENGINE_STATE->zbuffer)

#define ASPECT_RATIO (ENGINE_STATE->aspect_ratio)



// @doc mainly for permanent information and convinience so you can
// globally access some things, instead of passing them to million
// scopes, and then make aliases for these things so you don't
// type the same thing a million times, and god forbid have to
// refactor all that

struct Engine_state
{
    // @todo use flags eventually instead of bool for efficiency
    b32 running;
    b32 fullscreen;
    b32 exclusive_fullscreen;
    
    Input input;
    u8* framebuffer_base;
    s32 framebuffer_width;
    s32 framebuffer_height;
    r32* zbuffer;
    Camera main_camera;
    r32 aspect_ratio;
    u8 normalization_counter;
    u32 prng_seed;
    // @todo use Array instead?
    Font fonts[NUM_FONTS];
    Profiler profilers[NUM_PROFILERS];
    s32 profiler_stack[NUM_PROFILERS];
    s32 profiler_stack_counter;

    // temporary-----------------------------
    r32 camera_angle;
    r32 line_angle;
    r32 spin_angle;
    b32 reverse_winding;
    Transform cube_transform;
    r32 cube_scaling_factor;
    b32 cube_scale_up;
    Vector2 font_offset;
    // temporary-----------------------------
    
    b32 tested_once;
};

#endif
