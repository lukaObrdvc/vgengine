#ifndef PLATFORM_H
#define PLATFORM_H

// @TODO whether you use pointers here -> or not . depends on the size of
// the structs, and whether you want to copy them or not, figure out
// this later...


#if USE_DLL
typedef b32 (*fpReadFile) (u8*, void*, u32*);
typedef b32 (*fpWriteFile) (u8*, void*, u32);

typedef struct tagPlatformProcedures
{
    fpReadFile readFile;
    fpWriteFile writeFile;
} PlatformProcedures;

#else

b32 read_file(u8* path, void* buff, u32* buffSize);
b32 write_file(u8* path, void* buff, u32 buffSize);

#endif

typedef struct tagPlatformMemory
{
    /* void* memoryBase; */
    u64 memoryCapacity;
} PlatformMemory;

// @TODO not sure if you want a macro for something like this instead
// and then use preproc to make it portable
typedef struct tagPlatformDisplay
{
    u8 bytesPerPixel;
} PlatformDisplay;


#pragma pack(push, 1)
typedef struct tagPlatformAPI
{
#if USE_DLL
    PlatformProcedures platformProcedures;
#endif
    PlatformMemory platformMemory;
    PlatformDisplay platformDisplay;
} PlatformAPI;
#pragma pack(pop)

global_variable PlatformAPI* platformAPI;


#if USE_DLL

#define READ_FILE(path, buff, buffSize) platformAPI->platformProcedures.readFile((path), (buff), (buffSize))
#define WRITE_FILE(path, buff, buffSize) platformAPI->platformProcedures.writeFile((path), (buff), (buffSize))

#else

#define READ_FILE(path, buff, buffSize) read_file((path), (buff), (buffSize))
#define WRITE_FILE(path, buff, buffSize) write_file((path), (buff), (buffSize))

#endif


#endif
