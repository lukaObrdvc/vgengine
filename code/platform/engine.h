#ifndef ENGINE_H
#define ENGINE_H


#if USE_DLL

void platform_init_engine_stub(Platform_init_result* p)
{
    return;
}
void platform_init_memory_base_stub(Globals* memory_base)
{
    return;
}
void update_and_render_stub(Platform_frame_pass* p1, Engine_frame_result* p2)
{
    return;
}
void shut_down_all_threads_stub()
{
    return;
}

void start_up_all_threads_stub()
{
    return;
}

typedef void (*Platform_init_engine) (Platform_init_result*);
typedef void (*Platform_init_memory_base) (Globals*);
typedef void (*Update_and_render) (Platform_frame_pass*, Engine_frame_result*);
typedef void (*Shut_down_all_threads) ();
typedef void (*Start_up_all_threads) ();

struct Engine_api
{
    Platform_init_engine platform_init_engine;
    Platform_init_memory_base platform_init_memory_base;
    Update_and_render update_and_render;
    Shut_down_all_threads shut_down_all_threads;
    Start_up_all_threads start_up_all_threads;
};

GLOBAL Engine_api engine_api = {
    .platform_init_engine = platform_init_engine_stub,
    .platform_init_memory_base = platform_init_memory_base_stub,
    .update_and_render = update_and_render_stub,
    .shut_down_all_threads = shut_down_all_threads_stub,
    .start_up_all_threads = start_up_all_threads_stub
};

#define UPDATE_AND_RENDER(pass, result) engine_api.update_and_render((pass), (result))
#define PLATFORM_INIT_ENGINE(pass) engine_api.platform_init_engine((pass))
#define PLATFORM_INIT_MEMORY_BASE(memory_base) engine_api.platform_init_memory_base((memory_base))
#define SHUT_DOWN_ALL_THREADS() engine_api.shut_down_all_threads()
#define START_UP_ALL_THREADS() engine_api.start_up_all_threads()

#else

#define UPDATE_AND_RENDER(pass, result) update_and_render((pass), (result))
#define PLATFORM_INIT_ENGINE(pass) platform_init_engine((pass))
#define PLATFORM_INIT_MEMORY_BASE(memory_base) globals = (memory_base)
#define SHUT_DOWN_ALL_THREADS() shut_down_all_threads()
#define START_UP_ALL_THREADS() start_up_all_threads()

#endif


#endif
