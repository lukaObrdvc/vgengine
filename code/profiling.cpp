s32 register_profiler(String name)
{
    for (s32 i = 0; i < NUM_PROFILERS; i++)
    {
        if (PROFILERS[i].name.base && string_compare(PROFILERS[i].name, name))
        {
            return i;
        }
    }

    for (s32 i = 0; i < NUM_PROFILERS; i++)
    {
        if (!PROFILERS[i].name.base)
        {
            PROFILERS[i].name = name;
            PROFILERS[i].min_cycles = MAX_U64;
            PROFILERS[i].min_time = MAX_R64;
            return i;
        }
    }

    // exceeded NUM_PROFILERS
    ASSERT(false);
    return -1;    
}

void push_profiler_index(s32 index)
{
    ASSERT(index >= 0 && index < NUM_PROFILERS);
    ASSERT(PROFILER_STACK_COUNTER < NUM_PROFILERS);
    
    PROFILER_STACK[PROFILER_STACK_COUNTER++] = index;
}

s32 pop_profiler_index()
{
    ASSERT(PROFILER_STACK_COUNTER > 0);

    return PROFILER_STACK[--PROFILER_STACK_COUNTER];
}

void profiler_push(s32 index)
{
    Profiler* profiler = &PROFILERS[index];
    
    profiler->cycle_start = read_cycle_counter();
    profiler->time_start = READ_TIME_COUNTER();
    
    push_profiler_index(index);
}

void profiler_pop()
{
    s32 index = pop_profiler_index();
    Profiler* profiler = &PROFILERS[index];

    profiler->cycle_diff = read_cycle_counter() - profiler->cycle_start;
    profiler->time_diff = read_time_counter() - profiler->time_start;
    profiler->min_cycles = Min(profiler->cycle_diff, profiler->min_cycles);
    profiler->max_cycles = Max(profiler->cycle_diff, profiler->max_cycles);
    profiler->min_time = Min(profiler->time_diff, profiler->min_time);
    profiler->max_time = Max(profiler->time_diff, profiler->max_time);
    profiler->total_cycles += profiler->cycle_diff;
    profiler->total_time += profiler->time_diff;
    profiler->count++;
}
