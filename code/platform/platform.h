#ifndef PLATFORM_H
#define PLATFORM_H

#define INITIAL_COMMIT_SIZE_BY_PLATFORM (u64)5368709120 // 5 GB

// @todo should all of these be declared inline?

#if USE_DLL

typedef b32 (*Get_file_size) (const u8*, u32*);
typedef b32 (*Read_entire_file) (const u8*, void*, u32);
typedef void (*Write_entire_file) (const u8*, void*, u32);
typedef b32 (*Write_entire_existing_file) (const u8*, void*, u32);
typedef b32 (*Copy_file) (const u8*, const u8*);
typedef b32 (*Copy_and_maybe_overwrite_file) (const u8*, const u8*);
typedef b32 (*Move_file) (const u8*, const u8*);
typedef b32 (*Move_and_maybe_overwrite_file) (const u8*, const u8*);
typedef b32 (*Delete_file) (const u8*);
typedef b32 (*Create_directory) (const u8*);
typedef b32 (*Delete_directory) (const u8*);
typedef b32 (*Directory_exists) (const u8*);
typedef r64 (*Read_time_counter) ();
typedef u64 (*Read_cycle_counter) ();
typedef void (*Get_time) (Time*);
typedef Thread (*Start_thread) (Thread_procedure, void*);
typedef void (*Close_thread) (Thread*);
typedef void (*Wait_for_thread) (Thread*);
typedef void (*Sleep_current_thread) (u32);

#else

inline b32 get_file_size(const u8* filename, u32* size);
inline b32 read_entire_file(const u8* filename, void* buffer, u32 buffer_size);
inline void write_entire_file(const u8* filename, void* buffer, u32 buffer_size);
inline b32 write_entire_existing_file(const u8* filename, void* buffer, u32 buffer_size);
inline b32 copy_file(const u8* src, const u8* dest);
inline b32 copy_and_maybe_overwrite_file(const u8* src, const u8* dest);
inline b32 move_file(const u8* src, const u8* dest);
inline b32 move_and_maybe_overwrite_file(const u8* src, const u8* dest);
inline b32 delete_file(const u8* filename);
inline b32 create_directory(const u8* dirname);
inline b32 delete_directory(const u8* dirname);
inline b32 directory_exists(const u8* dirname);
inline r64 read_time_counter();
inline u64 read_cycle_counter();
inline void get_time(Time* time);
inline Thread start_thread(Thread_procedure proc, void* data);
inline void close_thread(Thread* t);
inline void wait_for_thread(Thread* t);
inline void sleep_current_thread(u32 ms);

#endif

struct Platform_api
{
#if USE_DLL
    Get_file_size get_file_size;
    Read_entire_file read_entire_file;
    Write_entire_file write_entire_file;
    Write_entire_existing_file write_entire_existing_file;
    Copy_file copy_file;
    Copy_and_maybe_overwrite_file copy_and_maybe_overwrite_file;
    Move_file move_file;
    Move_and_maybe_overwrite_file move_and_maybe_overwrite_file;
    Delete_file delete_file;
    Create_directory create_directory;
    Delete_directory delete_directory;
    Directory_exists directory_exists;
    Read_time_counter read_time_counter;
    Read_cycle_counter read_cycle_counter;
    Get_time get_time;
    Start_thread start_thread;
    Close_thread close_thread;
    Wait_for_thread wait_for_thread;
    Sleep_current_thread sleep_current_thread;
#endif
    u64 total_program_memory;
    u64 allocation_step; // @cleanup don't need this anymore
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
    s32 scratch_highest_size;
    s32 scratch_highest_index;
#endif
};

global_variable Globals* globals;


#define PLATFORM_API globals->platform_api
#define TOTAL_PROGRAM_MEMORY PLATFORM_API.total_program_memory
#define ALLOCATION_STEP PLATFORM_API.allocation_step


#if USE_DLL

#define GET_FILE_SIZE(filename, size) PLATFORM_API.get_file_size((filename), (size))
#define READ_ENTIRE_FILE(path, buff, buff_size) PLATFORM_API.read_entire_file((path), (buff), (buff_size))
#define WRITE_ENTIRE_FILE(path, buff, buff_size) PLATFORM_API.write_entire_file((path), (buff), (buff_size))
#define WRITE_ENTIRE_EXISTING_FILE(path, buff, buff_size) PLATFORM_API.write_entire_existing_file((path), (buff), (buff_size))
#define COPY_FILE(src, dest) PLATFORM_API.copy_file((src), (dest))
#define COPY_AND_MAYBE_OVERWRITE_FILE(src, dest) PLATFORM_API.copy_and_maybe_overwrite_file((src), (dest))
#define MOVE_FILE(src, dest) PLATFORM_API.move_file((src), (dest))
#define MOVE_AND_MAYBE_OVERWRITE_FILE(src, dest) PLATFORM_API.move_and_maybe_overwrite_file((src), (dest))
#define DELETE_FILE(filename) PLATFORM_API.delete_file((filename))
#define CREATE_DIRECTORY(dirname) PLATFORM_API.create_directory((dirname))
#define DELETE_DIRECTORY(dirname) PLATFORM_API.delete_directory((dirname))
#define DIRECTORY_EXISTS(dirname) PLATFORM_API.directory_exists((dirname))
#define READ_TIME_COUNTER() PLATFORM_API.read_time_counter()
#define READ_CYCLE_COUNTER() PLATFORM_API.read_cycle_counter()
#define GET_TIME(time) PLATFORM_API.get_time((time))
#define START_THREAD(f, d) PLATFORM_API.start_thread((f), (d))
#define CLOSE_THREAD(t) PLATFORM_API.close_thread((t))
#define WAIT_FOR_THREAD(t) PLATFORM_API.wait_for_thread((t))
#define SLEEP_CURRENT_THREAD(ms) PLATFORM_API.sleep_current_thread((ms))

#else

#define GET_FILE_SIZE(filename, size) get_file_size((filename), (size))
#define READ_ENTIRE_FILE(path, buff, buff_size) read_entire_file((path), (buff), (buff_size))
#define WRITE_ENTIRE_FILE(path, buff, buff_size) write_entire_file((path), (buff), (buff_size))
#define WRITE_ENTIRE_EXISTING_FILE(path, buff, buff_size) write_entire_existing_file((path), (buff), (buff_size))
#define COPY_FILE(src, dest) copy_file((src), (dest))
#define COPY_AND_MAYBE_OVERWRITE_FILE(src, dest) copy_and_maybe_overwrite_file((src), (dest))
#define MOVE_FILE(src, dest) move_file((src), (dest))
#define MOVE_AND_MAYBE_OVERWRITE_FILE(src, dest) move_and_maybe_overwrite_file((src), (dest))
#define DELETE_FILE(filename) delete_file((filename))
#define CREATE_DIRECTORY(dirname) create_directory((dirname))
#define DELETE_DIRECTORY(dirname) delete_directory((dirname))
#define DIRECTORY_EXISTS(dirname) directory_exists((dirname))
#define READ_TIME_COUNTER() read_time_counter()
#define READ_CYCLE_COUNTER() read_cycle_counter()
#define GET_TIME(time) get_time((time))
#define START_THREAD(f, d) start_thread((f), (d))
#define CLOSE_THREAD(t) close_thread((t))
#define WAIT_FOR_THREAD(t) wait_for_thread((t))
#define SLEEP_CURRENT_THREAD(ms) sleep_current_thread((ms))

#endif



#endif
