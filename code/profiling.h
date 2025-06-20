#ifndef PROFILING_H
#define PROFILING_H

inline u64 read_cycle_counter()
{
    // this is compiler dependant, because this is an intrinsic

    // this should be in like megacycles
    return __rdtsc();
}


#endif
