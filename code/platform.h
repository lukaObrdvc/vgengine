#ifndef PLATFORM_H
#define PLATFORM_H

// @TODO whether you use pointers here -> or not . depends on the size of
// the structs, and whether you want to copy them or not, figure out
// this later...


#if USE_DLL
typedef b32 (*fpReadFile) (u8*, void*, u32*);
typedef b32 (*fpWriteFile) (u8*, void*, u32);
typedef b32 (*fpCommitMemory) (u8*, u64);

typedef struct tagPlatformProcedures
{
    fpReadFile readFile;
    fpWriteFile writeFile;
    fpCommitMemory commitMemory;
} PlatformProcedures;

#else

b32 read_file(u8* path, void* buff, u32* buffSize);
b32 write_file(u8* path, void* buff, u32 buffSize);
b32 commit_memory(u8* address, u64 size);

#endif

typedef struct tagPlatformMemory
{
    u64 reservedMemory;
    u64 pageSize;
} PlatformMemory;

// @TODO not sure if you want a macro for something like this instead
// and then use preproc to make it portable
typedef struct tagPlatformDisplay
{
    u8 bytesPerPixel;
} PlatformDisplay;


struct PlatformAPI
{
#if USE_DLL
    PlatformProcedures platformProcedures;
#endif
    PlatformMemory platformMemory;
    PlatformDisplay platformDisplay;
};



struct Globals
{
    PlatformAPI platformAPI;
    ArenaManager arenaManager;
    Arena permArena;
    Arena frameArena;
    u16 arenaCount;
    Arena arenas[MAX_ARENAS];
};

global_variable Globals* globals;


#if USE_DLL

#define READ_FILE(path, buff, buffSize) PLATFORM_API.platformProcedures.readFile((path), (buff), (buffSize))
#define WRITE_FILE(path, buff, buffSize) PLATFORM_API.platformProcedures.writeFile((path), (buff), (buffSize))
#define COMMIT_MEMORY(address, size) PLATFORM_API.platformProcedures.commitMemory((address), (size))

#else

#define READ_FILE(path, buff, buffSize) read_file((path), (buff), (buffSize))
#define WRITE_FILE(path, buff, buffSize) write_file((path), (buff), (buffSize))
#define COMMIT_MEMORY(address, size) commit_memory((address), (size))

#endif



#endif
