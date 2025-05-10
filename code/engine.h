#ifndef ENGINE_H
#define ENGINE_H

// @TODO when converting to cpp, initialize this in the actual structs...
// well you should have a function that initializes these to stubs

// @TODO same thing about whether you want EngineAPI struct to use -> or .

#if USE_DLL

void platform_init_memory_base_stub(PlatformAPI* memoryBase)
{
    return;
}
void update_and_render_stub(void)
{
    return;
}
b32 process_input_stub(u64 kts, u64 ktus, u8 mts, u8 mtus, v2 c)
{
    return false;
}

typedef void (*fpPlatformInitMemoryBase) (PlatformAPI*);
typedef void (*fpUpdateAndRender) (void);
typedef b32 (*fpProcessInput) (u64, u64, u8, u8, v2);

#pragma pack(push, 1)
typedef struct tagEngineAPI
{
    fpPlatformInitMemoryBase platformInitMemoryBase;
    fpUpdateAndRender updateAndRender;
    fpProcessInput processInput;
} EngineAPI;
#pragma pack(pop)

global_variable EngineAPI engineAPI = {
    .platformInitMemoryBase = platform_init_memory_base_stub,
    .updateAndRender = update_and_render_stub,
    .processInput = process_input_stub
    };

#define UPDATE_AND_RENDER() engineAPI.updateAndRender()
#define PLATFORM_INIT_MEMORY_BASE(memoryBase) engineAPI.platformInitMemoryBase((memoryBase))
#define PROCESS_INPUT(kts, ktus, mts, mtus, c) engineAPI.processInput((kts), (ktus), (mts), (mtus), (c))

#else

/* void platform_init_memory_base(EngineAPI*); */
void update_and_render(void);
b32 process_input(u64, u64, u8, u8, v2);

#define UPDATE_AND_RENDER() update_and_render()
// this should probably expand to something that just directly sets up the memoryBase for the entire monolithic build
#define PLATFORM_INIT_MEMORY_BASE(memoryBase) globals = memoryBase
#define PROCESS_INPUT(kts, ktus, mts, mtus, c) process_input((kts), (ktus), (mts), (mtus), (c))

#endif

#endif
