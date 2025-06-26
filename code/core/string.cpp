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
