#ifndef RANDOM_H
#define RANDOM_H

// @todo replace this with datetime once we can read it
// then you can in your init function, init the state to datetime,
// to get a different state every time
#define RANDOM_DEFAULT_SEED 0xA5A5A5A5
#define PRNG_SEED (&ENGINE_STATE->prng_seed)

u32 prng_xorshift32()
{
    u32* result = PRNG_SEED;
    ASSERT(*result != 0);
    
    *result ^= *result << 13;
    *result ^= *result >> 17;
    *result ^= *result << 5;
    return *result;
}

void random_set_seed(u32 seed)
{
    *PRNG_SEED = seed;
}

// @doc [0, 1] (can never exactly return 1 but effectively inclusive)
r32 random_range01()
{
    return (prng_xorshift32() >> 8) * (1.0f / 16777216.0f);
}

// @doc [min, max]
s32 random_range(s32 min, s32 max)
{
    ASSERT(max >= min);
    return min + (s32)(prng_xorshift32() % (max - min + 1));
}

// @doc [min, max] (can never exactly return max but effectively inclusive)
r32 random_range(r32 min, r32 max)
{
    ASSERT(max >= min);
    return min + random_range01() * (max - min);
}

#endif
