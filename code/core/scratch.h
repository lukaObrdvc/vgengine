#ifndef SCRATCH_H
#define SCRATCH_H

#define SCRATCH_ARENA globals->scratch_arena
#define SCRATCH_POOL_SIZE 8
#define SCRATCH_CAPACITY 2048 // 2KB

// @todo maybe don't use Scratch* ?

// @todo do we zero out scratch when releasing??

struct Scratch
{
    u8* base;
    s32 size;
    s32 index;
};



template<typename T>
T* scratch_push(Scratch* scratch, u32 count = 1)
{
    return (T*) scratch_push_size(scratch, sizeof(T)*count, alignof(T));
}


#endif
