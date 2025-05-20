#ifndef ENGINE_H
#define ENGINE_H

// @TODO when converting to cpp, initialize this in the actual structs...
// well you should have a function that initializes these to stubs

// @TODO same thing about whether you want EngineAPI struct to use -> or .

#if USE_DLL

void platform_init_memory_base_stub(Globals* memory_base)
{
    return;
}
void update_and_render_stub(void)
{
    return;
}
b32 process_input_stub(u64 kts, u64 ktus, u8 mts, u8 mtus, r32 cursor_x, r32 cursor_y)
{
    return false;
}

typedef void (*Platform_init_memory_base) (Globals*);
typedef void (*Update_and_render) (void);
typedef b32 (*Process_input) (u64, u64, u8, u8, r32, r32);

struct Engine_api
{
    Platform_init_memory_base platform_init_memory_base;
    Update_and_render update_and_render;
    Process_input process_input;
};

global_variable Engine_api engine_api = {
    .platform_init_memory_base = platform_init_memory_base_stub,
    .update_and_render = update_and_render_stub,
    .process_input = process_input_stub
    };

#define UPDATE_AND_RENDER() engine_api.update_and_render()
#define PLATFORM_INIT_MEMORY_BASE(memoryBase) engine_api.platform_init_memory_base((memory_base))
#define PROCESS_INPUT(kts, ktus, mts, mtus, cX, cY) engine_api.process_input((kts), (ktus), (mts), (mtus), (cX), (cY))

#else

extern "C" void update_and_render(void);
extern "C" b32 process_input(u64, u64, u8, u8, r32, r32);

#define UPDATE_AND_RENDER() update_and_render()
// this should probably expand to something that just directly sets up the memory_base for the entire monolithic build
#define PLATFORM_INIT_MEMORY_BASE(memory_base) globals = (memory_base)
#define PROCESS_INPUT(kts, ktus, mts, mtus, cX, cY) process_input((kts), (ktus), (mts), (mtus), (cX), (cY))

#endif

#endif
