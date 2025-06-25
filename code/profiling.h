#ifndef PROFILING_H
#define PROFILING_H

#define NUM_PROFILERS 100
#define PROFILERS (ENGINE_STATE->profilers)
#define PROFILER_STACK (ENGINE_STATE->profiler_stack)
#define PROFILER_STACK_COUNTER (ENGINE_STATE->profiler_stack_counter)

// we push indices of profilers onto a stack, when begin_profiling
// and pop an index when end_profiling, this way if there is an if
// statement this will work correctly, and is easy to use

// @todo only profile in debug mode

struct Profiler
{
    String name;
    
    u64 cycle_diff;
    r64 time_diff;
    
    u64 min_cycles;
    u64 max_cycles;
    
    r64 min_time;
    r64 max_time;

    // for averaging
    u64 total_cycles;
    r64 total_time;

    // need to remember so you can calculate the diffs
    u64 cycle_start;
    r64 time_start;
    
    // total frames essentialy, for averaging
    u64 count;
};
s32 register_profiler(String name);
void push_profiler_index(s32 index);
s32 pop_profiler_index();
void profiler_push(s32 index);
void profiler_pop();

inline u64 read_cycle_counter()
{
    // this is compiler dependant, because this is an intrinsic

    // this should be in like megacycles
    return __rdtsc();
}

inline u64 profiler_avg_cycles(Profiler* p)
{
    if (p->count == 0) return 0;
    return roundi(p->total_cycles / (r32)p->count);
}

inline r64 profiler_avg_time(Profiler* p)
{
    if (p->count == 0) return 0;
    return p->total_time / p->count;
}

#if DEVELOPER
// static variable is initialized once, so register_profiler is called
// once, and you store these indices in scopes so they are unique per
// profiler
#define BEGIN_PROFILING(name) \
    static s32 profiler_index_##__LINE__ = register_profiler(name); \
    profiler_push(profiler_index_##__LINE__)

#define END_PROFILING() profiler_pop()

#else

#define BEGING_PROFILING(name) ((void)0)
#define END_PROFILING() ((void)0)

#endif


#endif
