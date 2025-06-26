String to_string(s32 n, Arena* arena = TEMPORARY_ARENA)
{
    String result = empty_str();
    result.length = num_digits(n);
    
    b32 is_negative = n < 0;
    
    if (is_negative) result.length++;
    
    result.base = arena_push<u8>(arena, result.length + 1); // +1 for null termination

    if (is_negative) result.base[0] = '-';

    s32 i = is_negative ? 1 : 0;

    if (n == 0) result.base[0] = '0';
    
    n = abs(n);
    while (n > 0)
    {
        s32 digit = n % 10;
        n /= 10;
        result.base[result.length - 1 - i] = '0' + digit;
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

    if (n == 0) result.base[0] = '0';
    
    s32 i = 0;
    while (n > 0)
    {
        s32 digit = n % 10;
        n /= 10;
        result.base[result.length - 1 - i] = '0' + digit;
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

// @doc returns 0 when string is not alphanumeric (or doesn't have - or +),
// or when number is too big so it would overflow, or when empty string
s32 string_to_s32(String str)
{
    if (string_compare(str, empty_str())) return 0;
    
    for (s32 i = 0; i < str.length; i++)
    {
        if (!is_alpha(str.base[i]) && str.base[i] != '-' && str.base[i] != '+')
        {
            return 0;
        }
    }

    b32 is_negative = (str.base[0] == '-') ? true : false;
    b32 skip_first = (is_alpha(str.base[0])) ? false : true;
    
    if (str.length - skip_first > 17)
    {
        return 0;
    }

    s32 result = 0;

    for (s32 i = 0; i < str.length - skip_first; i++)
    {
        result += to_alpha(str.base[i + skip_first]) * pow10(str.length - skip_first - i - 1);
    }

    if (is_negative) result = -result;
    
    return result;
}

// @doc returns 0.0f when string is not alphanumeric (or doesn't have - or + or .)
// or when number of digits is too big so it would overflow, or when too many dots
r32 string_to_r32(String str)
{
    s32 dot = -1;
    s32 num_dots = 0;
    
    for (s32 i = 0; i < str.length; i++)
    {
        if (str.base[i] == '.')
        {
            dot = i;
            num_dots++;
        }
        
        if (!is_alpha(str.base[i]) && str.base[i] != '-' && str.base[i] != '+' && str.base[i] != '.')
        {
            return 0;
        }
    }

    if (dot == -1)
    {
        return (r32)string_to_s32(str);
    }

    if (num_dots > 1) return 0.0f;

    b32 is_negative = (str.base[0] == '-') ? true : false;
    b32 skip_first = (!is_alpha(str.base[0])) ? true : false;
    b32 skip_second = skip_first && dot == 1 ? true : false;
    b32 skip_last = (str.base[str.length - 1] == '.') ? true : false;

    if (skip_last)
    {
        return (r32)string_to_s32({str.base, str.length - 1});
    }
    
    String int_str = empty_str();
    if (!skip_second || dot != 0)
    {
        int_str = {str.base + skip_first, dot - skip_first};
    }
    
    b32 int_str_too_big = (int_str.length - skip_first > 17) ? true : false;

    String dec_str = {str.base + dot + 1, str.length - 1 - dot};

    s32 int_part = string_to_s32(int_str);
    s32 dec_part = int_str_too_big ? 0 : string_to_s32(dec_str);

    // avoids overflow with pow10
    if (dec_str.length > 17 || int_str_too_big) return 0.0f;

    r32 result = (r32)int_part + dec_part / (r32)pow10(dec_str.length);
    
    if (is_negative) result = -result;
    
    return result;
}
