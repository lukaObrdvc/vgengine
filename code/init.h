#ifndef INIT_H
#define INIT_H

struct Platform_init_out
{
    Input* input_address;
    s32 window_width;
    s32 window_height;
    s32 window_offs_x;
    s32 window_offs_y;
    s32 framebuffer_width;
    s32 framebuffer_height;
    void* framebuffer_mem_begin;
};

void init_memory();
void init_engine_state();

extern "C" void platform_init_engine(Platform_init_out*);
#if USE_DLL
extern "C" void platform_init_memory_base(Globals* memory_base);
#endif

#endif
