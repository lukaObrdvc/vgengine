#ifndef UPDATE_AND_RENDER_H
#define UPDATE_AND_RENDER_H

struct Platform_frame_pass
{
    b32 running;
};

struct Engine_frame_result
{
    b32 cap_frame_rate;
    s32 target_frame_rate;
    b32 show_cursor;
    void* window_buffer_base;
    s32 window_buffer_width;
    s32 window_buffer_height;
    // resize framebuffer in fullscreen, but also window in windowed
    b32 resize;
    b32 change_display;
    b32 fullscreen;
    b32 exclusive_fullscreen; // only valid if fullscreen == true
    s32 window_offs_x;
    s32 window_offs_y;
    s32 window_width;
    s32 window_height;
    s32 cursor_x;
    s32 cursor_y;
};


extern "C" void update_and_render(Platform_frame_pass*, Engine_frame_result*);

#endif
