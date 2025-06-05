#ifndef SCRATCH_H
#define SCRATCH_H

#define SCRATCH_ARENA (&globals->scratch_arena)
#define SCRATCH_POOL_SIZE 8
#define SCRATCH_CAPACITY 2048 // 2KB


struct Scratch
{
    u8* base;
    s32 size;
    s32 index;
};

inline Scratch* get_scratch();
inline void release_scratch(Scratch* scratch);
inline void* scratch_push_size(Scratch* scratch, s32 size, s32 alignment);


template<typename T>
T* scratch_push(Scratch* scratch, u32 count = 1)
{
    return (T*) scratch_push_size(scratch, sizeof(T)*count, alignof(T));
}


#endif
