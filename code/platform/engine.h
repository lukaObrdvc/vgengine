#ifndef ENGINE_H
#define ENGINE_H


#if USE_DLL

void platform_init_engine_stub(Platform_init_result*)
{
    return;
}
void platform_init_memory_base_stub(Globals* memory_base)
{
    return;
}
void update_and_render_stub(Platform_frame_pass*, Engine_frame_result*)
{
    return;
}

typedef void (*Platform_init_engine) (Platform_init_out*);
typedef void (*Platform_init_memory_base) (Globals*);
typedef void (*Update_and_render) (Platform_frame_pass*, Engine_frame_result*);

struct Engine_api
{
    Platform_init_engine platform_init_engine;
    Platform_init_memory_base platform_init_memory_base;
    Update_and_render update_and_render;
};

global_variable Engine_api engine_api = {
    .platform_init_engine = platform_init_engine_stub,
    .platform_init_memory_base = platform_init_memory_base_stub,
    .update_and_render = update_and_render_stub,
};

#define UPDATE_AND_RENDER(pass, result) engine_api.update_and_render((pass), (result))
#define PLATFORM_INIT_ENGINE(pass) engine_api.platform_init_engine((pass))
#define PLATFORM_INIT_MEMORY_BASE(memory_base) engine_api.platform_init_memory_base((memory_base))

#else

#define UPDATE_AND_RENDER(pass, result) update_and_render((pass), (result))
#define PLATFORM_INIT_ENGINE(pass) platform_init_engine((pass))
#define PLATFORM_INIT_MEMORY_BASE(memory_base) globals = (memory_base)

#endif


#endif
