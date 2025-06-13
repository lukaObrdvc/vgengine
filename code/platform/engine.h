#ifndef ENGINE_H
#define ENGINE_H


#if USE_DLL

void platform_init_engine_stub()
{
    return;
}
void platform_init_memory_base_stub(Globals* memory_base)
{
    return;
}
Engine_frame_result update_and_render_stub(Platform_input_pass)
{
    return;
}

typedef void (*Platform_init) ();
typedef void (*Platform_init_memory_base) (Globals*);
typedef Engine_frame_result (*Update_and_render) (Platform_input_pass);

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

#define UPDATE_AND_RENDER(input) engine_api.update_and_render((input))
#define PLATFORM_INIT_ENGINE() engine_api.platform_init_engine()
#define PLATFORM_INIT_MEMORY_BASE(memory_base) engine_api.platform_init_memory_base((memory_base))

#else

extern "C" Engine_frame_result update_and_render(Platform_input_pass);
extern "C" void platform_init_engine();

#define UPDATE_AND_RENDER(input) update_and_render((input))
#define PLATFORM_INIT_ENGINE() platform_init_engine()
#define PLATFORM_INIT_MEMORY_BASE(memory_base) globals = (memory_base)  // this should probably expand to something that just directly sets up the memory_base for the entire monolithic build

#endif


#endif
