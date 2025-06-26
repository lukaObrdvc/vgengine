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

// @todo optional data arg...

typedef void (*Thread_procedure) (void*);

#define THREAD_ENTRY(name) void (name)(void* data = 0)

struct Thread
{
    void* platform_handle;
    s32 id;
};

struct Thread_entry_arguments
{
    Thread_procedure procedure;
    void* data;
};



#endif
