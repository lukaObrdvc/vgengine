#ifndef THREAD_H
#define THREAD_H

// because the platform will always require some thread handle,
// you put a generic pointer to that in your Thread struct, so
// you can later pass that to functions that close the thread and
// other things

// the platform also expects a function pointer which will be the
// entry point for the thread, and to make that generic it will
// always have the same Thread_procedure signature, you pass it some
// data if it's needed (otherwise null pointer), and then inside
// the actualy function you can cast that data to whatever you need
// and call the functions inside there that you actually want

// when threads operate on some memory that exists in the same cache
// line this will slow down multithreading, because every time thread 1
// wants to access that memory it will invalidate this cache line for
// thread 2

// to avoid this you can add padding to structs, since cache lines
// are 64B, you can put alignas(64) in front of struct key word,
// and then add enough bytes of padding (is this necessary?) in the
// struct as a field

// Thread and Job things here are basically the same thing, but Job
// things refer to the Job system while Thread things are more general

// because we already have the main thread, we create one less than NUM_CORES
#define NUM_WORKERS (NUM_CORES - 1)
#define NUM_JOBS 256

typedef void (*Thread_procedure) (void*);
typedef void (*Job_procedure) (void*);

#define THREAD_ENTRY(name) void (name)(void* data = 0)
#define JOB(name) void (name)(void* data = 0)


struct Thread
{
    void* platform_handle;
};

struct Thread_entry_arguments
{
    Thread_procedure procedure;
    void* data;
};

struct Job
{
    Job_procedure proc;
    void* data;
};

struct Job_queue
{
    Job jobs[NUM_JOBS];
    s32 write;
    s32 read;
    s32 available_jobs;
};

void submit_job(Job job);
void worker_proc(void* data);
inline void wait_for_all_jobs();
inline s32 get_thread_index();

// @todo make an engine api function that is called
// in the platform after the main game loop finishes
// so you can close all threads there (you don't technically
// need to do that but it's clean)

// @todo add running per thread? (tracking working threads basically?)

#endif
