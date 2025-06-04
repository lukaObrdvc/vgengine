#ifndef PLATFORM_H
#define PLATFORM_H

#define INITIAL_COMMIT_SIZE_BY_PLATFORM 5368709120 // 5 GB


#if USE_DLL

typedef b32 (*Read_file) (u8*, void*, u32*);
typedef b32 (*Write_file) (u8*, void*, u32);

#else

b32 read_file(u8* path, void* buff, u32* buff_size);
b32 write_file(u8* path, void* buff, u32 buff_size);

#endif

struct Platform_api
{
#if USE_DLL
    Read_file read_file;
    Write_file write_file;
#endif
    u64 total_program_memory;
    u64 allocation_step;
    s32 bytes_per_pixel;
};

struct Globals
{
    Platform_api platform_api;
    Arena managing_arena;
    Arena permanent_arena;
    Arena temporary_arena;
    Arena scratch_arena;
    // @todo abstract into general pool and use that instead
    Scratch scratch_pool[SCRATCH_POOL_SIZE];
    // @todo turn into a bitfield instead
    b32 scratch_in_use[SCRATCH_POOL_SIZE];
    
#if DEVELOPER
    // @todo also make scratch_pool_most_used
    s32 scratch_highest_size;
#endif
};

global_variable Globals* globals;


#define PLATFORM_API globals->platform_api
#define TOTAL_PROGRAM_MEMORY PLATFORM_API.total_program_memory
#define ALLOCATION_STEP PLATFORM_API.allocation_step


#if USE_DLL

#define READ_FILE(path, buff, buff_size) PLATFORM_API.read_file((path), (buff), (buff_size))
#define WRITE_FILE(path, buff, buff_size) PLATFORM_API.write_file((path), (buff), (buff_size))

#else

#define READ_FILE(path, buff, buff_size) read_file((path), (buff), (buff_size))
#define WRITE_FILE(path, buff, buff_size) write_file((path), (buff), (buff_size))

#endif



#endif
