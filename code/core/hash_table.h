#ifndef HASH_TABLE_H
#define HASH_TABLE_H

// @doc keys can be all integral types, pointers, strings

// @doc tombstone means there was a key there before, but was removed, we need to differentiate between this and an empty slot so as
// not to put duplicates in slots where we removed some key, but there already was a key further along if we continued probing

// @doc used_count does not track tombstone and empty slots

// @doc we do not resize, just assert when full

// @doc we can't track whether it's full with this, we just assert when it's full and trying to insert a key

// @doc key values for empty and tombstone are reserved


// @todo probably don't use Hash_table* now?
// @todo rehash when tombstone_count reaches like capacity/2 or something, by compacting memory basically
// @todo fast mod


#define HASH_TABLE_LOAD_FACTOR 0.7f

template<typename TKey, typename TValue>
struct Hash_table
{
    TKey* keys;
    TValue* values;
    s32 capacity; // @doc must be power of 2
    s32 tombstone_count;
};

template<typename T>
T empty_key();

template<typename T>
T tombstone_key();

// golden ratio hash for smaller keys
inline u64 hash(u8 x)  { return (u64)x * 0x9E3779B97F4A7C15ull; }
inline u64 hash(u16 x) { return (u64)x * 0x9E3779B97F4A7C15ull; }
inline u64 hash(s8 x)  { return (u64)x * 0x9E3779B97F4A7C15ull; }
inline u64 hash(s16 x) { return (u64)x * 0x9E3779B97F4A7C15ull; }
// murmur3 hash for bigger keys
inline u64 hash(u32 x)
{
    x ^= x >> 16;
    x *= 0x85ebca6b;
    x ^= x >> 13;
    x *= 0xc2b2ae35;
    x ^= x >> 16;
    return (u64)x;
}
inline u64 hash(s32 x)
{
    x ^= x >> 16;
    x *= 0x85ebca6b;
    x ^= x >> 13;
    x *= 0xc2b2ae35;
    x ^= x >> 16;
    return (u64)x;
}
inline u64 hash(u64 x)
{
    x ^= x >> 33;
    x *= 0xff51afd7ed558ccd;
    x ^= x >> 33;
    x *= 0xc4ceb9fe1a85ec53;
    x ^= x >> 33;
    return x;
}
inline u64 hash(s64 x)
{
    x ^= x >> 33;
    x *= 0xff51afd7ed558ccd;
    x ^= x >> 33;
    x *= 0xc4ceb9fe1a85ec53;
    x ^= x >> 33;
    return (u64)x;
}
// FNV-1a hash for strings
inline u64 hash(String str)
{
    u64 h = 14695981039346656037ull;
    for (s32 i = 0; i < str.length; i++)
    {
        h = (h ^ str.base[i]) * 1099511628211ull;
    }
    return h;
}
template<typename T>
u64 hash(T* ptr)
{
    return hash((u64)ptr);
}

template<typename T>
b32 keys_equal(T a, T b)
{
    return a == b;
}
inline b32 keys_equal(String a, String b)
{
    return string_compare(a, b);
}

template<> u8 empty_key<u8>()   { return MAX_U8; }
template<> u16 empty_key<u16>() { return MAX_U16; }
template<> u32 empty_key<u32>() { return MAX_U32; }
template<> u64 empty_key<u64>() { return MAX_U64; }
template<> s8 empty_key<s8>()   { return MAX_S8; }
template<> s16 empty_key<s16>() { return MAX_S16; }
template<> s32 empty_key<s32>() { return MAX_S32; }
template<> s64 empty_key<s64>() { return MAX_S64; }
template<> void* empty_key<void*>() { return nullptr; }
template<> String empty_key<String>() { return {}; }

template<> u8 tombstone_key<u8>()   { return MAX_U8 - 1; }
template<> u16 tombstone_key<u16>() { return MAX_U16 - 1; }
template<> u32 tombstone_key<u32>() { return MAX_U32 - 1; }
template<> u64 tombstone_key<u64>() { return MAX_U64 - 1; }
template<> s8 tombstone_key<s8>()   { return MAX_S8 - 1; }
template<> s16 tombstone_key<s16>() { return MAX_S16 - 1; }
template<> s32 tombstone_key<s32>() { return MAX_S32 - 1; }
template<> s64 tombstone_key<s64>() { return MAX_S64 - 1; }
template<> void* tombstone_key<void*>() { return (void*)1; }
template<> String tombstone_key<String>() { return String{(u8*)"<tombstone>", 11}; }

template<typename TKey, typename TValue>
s32 hash_table_find_slot(Hash_table<TKey, TValue>* table, TKey key, b32* found)
{
    ASSERT(!(keys_equal(key, empty_key<TKey>()) || keys_equal(key, tombstone_key<TKey>())));
    
    s32 mask = table->capacity - 1;
    s32 index = hash(key) & mask; // hash and mod by capacity (fast mod because capacity is pow of 2)
    s32 tombstone = -1; // is there a tombstone while we linearly probe to find if key is already inserted
    // found means key was already inserted
    for (int i = 0; i < table->capacity; i++)
    {
        TKey current_key = table->keys[index];

        if (keys_equal(current_key, empty_key<TKey>()))
        {
            // if there wasn't a tombstone, then insert a new key at the current index
            *found = false;
            return (tombstone != -1) ? tombstone : index;
        }
        
        if (keys_equal(current_key, key))
        {
            *found = true;
            return index;
        }

        if (keys_equal(current_key, tombstone_key<TKey>()) && tombstone == -1)
        {
            tombstone = index;
        }

        index = (index + 1) & mask; // increment and mod by capacity (fast mod because capacity is pow of 2)
    }

    ASSERT(tombstone != -1); // no empty or tombstone slots, so no space left

    *found = false;
    return tombstone;
}


template<typename TKey, typename TValue>
void hash_table_insert(Hash_table<TKey, TValue>* table, TKey key, const TValue& value)
{
    b32 found;
    s32 index = hash_table_find_slot(table, key, &found);

    table->keys[index] = key;
    table->values[index] = value;
}

template<typename TKey, typename TValue>
void hash_table_remove(Hash_table<TKey, TValue>* table, TKey key)
{
    b32 found;
    s32 index = hash_table_find_slot(table, key, &found);
    if (!found) return; // do nothing when key was never inserted in the first place; maybe assert instead?

    table->keys[index] = tombstone_key<TKey>();
    table->tombstone_count++;
}

template<typename TKey, typename TValue>
b32 hash_table_get_value(Hash_table<TKey, TValue>* table, TKey key, TValue* out_value)
{
    b32 found;
    s32 index = hash_table_find_slot(table, key, &found);
    if (found)
    {
        *out_value = table->values[index];
        return true;
    }
    return false;
}

template<typename TKey, typename TValue>
b32 hash_table_has_key(Hash_table<TKey, TValue>* table, TKey key)
{
    b32 found;
    hash_table_find_slot(table, key, &found);
    return found ? true : false;
}

template<typename TKey, typename TValue>
void hash_table_init(Hash_table<TKey, TValue>* table, TKey* keys, TValue* values, s32 capacity)
{
    ASSERT(pow_of_2(capacity));

    s32 new_capacity = align_up(floori(capacity * (1 + HASH_TABLE_LOAD_FACTOR)), 2); 
    
    table->keys = keys;
    table->values = values;
    table->capacity = new_capacity;
    table->tombstone_count = 0;

    for (s32 i = 0; i < table->capacity; i++)
    {
        table->keys[i] = empty_key<TKey>();
    }
}

template<typename TKey, typename TValue>
void hash_table_clear(Hash_table<TKey, TValue>* table)
{
    for (s32 i = 0; i < table->capacity; i++)
    {
        table->keys[i] = empty_key<TKey>();
    }

    table->tombstone_count = 0;
}

#endif
