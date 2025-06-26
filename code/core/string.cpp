String to_string(s32 n, Arena* arena = TEMPORARY_ARENA)
{
    String result;
    result.length = num_digits(n);
    
    b32 is_negative = n < 0;
    
    if (is_negative) result.length++;
    
    result.base = arena_push<u8>(arena, result.length + 1); // +1 for null termination

    if (is_negative) result.base[0] = '-';

    s32 i = is_negative ? 1 : 0;

    n = abs(n);
    while (n > 0)
    {
        s32 digit = n % 10;
        n /= 10;
        result.base[i] = '0' + digit;
        i++;
    }

    result.base[result.length] = '\0';
    return result;
}

String to_string(u64 n, Arena* arena = TEMPORARY_ARENA)
{
    String result;
    result.length = num_digits(n);
    
    result.base = arena_push<u8>(arena, result.length + 1); // +1 for null termination

    s32 i = 0;
    while (n > 0)
    {
        s32 digit = n % 10;
        n /= 10;
        result.base[i] = '0' + digit;
        i++;
    }
    
    result.base[result.length] = '\0';
    return result;
}

String to_string(r32 n, s32 decimals, Arena* arena = TEMPORARY_ARENA)
{
    u8 buffer[64];
    snprintf((char*)buffer, 64, "%.*f", decimals, n);
    String s = copy_str((const char*)buffer);
    return s;
}

String to_string(r64 n, s32 decimals, Arena* arena = TEMPORARY_ARENA)
{
    u8 buffer[64];
    snprintf((char*)buffer, 64, "%.*f", decimals, n);
    String s = copy_str((const char*)buffer);
    return s;
}

// @doc allocates on temp arena implicitly
String concat(s32 count, ...)
{
    ASSERT(count >= 2);

    va_list strings;
    va_start(strings, count);

    Scratch* scratch = get_scratch();
    String** buffer = scratch_push<String*>(scratch, count);

    for (s32 i = 0; i < count; i++)
    {
        buffer[i] = va_arg(strings, String*);
    }

    s32 total_len = 0;

    for (s32 i = 0; i < count; i++)
    {
        total_len += buffer[i]->length;
    }

    ASSERT(total_len != 0);

    String result;
    result.base = arena_push<u8>(TEMPORARY_ARENA, total_len + 1); // +1 for null termination
    result.length = total_len;
    result.base[result.length] = '\0';

    s32 offset = 0;
    for (s32 i = 0; i < count; i++)
    {
        memcpy(result.base + offset, buffer[i]->base, buffer[i]->length);
        offset += buffer[i]->length;
    }
    
    release_scratch(scratch);
    
    va_end(strings);

    return result;
}

// @doc allocates on temp arena implicitly
String concat(u32 count, ...)
{
    ASSERT(count >= 2);

    va_list strings;
    va_start(strings, count);

    Scratch* scratch = get_scratch();
    const char** buffer = scratch_push<const char*>(scratch, count);

    for (u32 i = 0; i < count; i++)
    {
        buffer[i] = va_arg(strings, const char*);
    }

    s32 total_len = 0;

    for (u32 i = 0; i < count; i++)
    {
        total_len += len_of_c_string(buffer[i]);
    }

    ASSERT(total_len != 0);

    String result;
    result.base = arena_push<u8>(TEMPORARY_ARENA, total_len + 1); // +1 for null termination
    result.length = total_len;
    result.base[result.length] = '\0';

    s32 offset = 0;
    for (u32 i = 0; i < count; i++)
    {
        s32 len = len_of_c_string(buffer[i]);
        memcpy(result.base + offset, (u8*)buffer[i], len);
        offset += len;
    }
    
    release_scratch(scratch);
    
    va_end(strings);

    return result;
}
