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

String to_string(r32 n, Arena* arena = TEMPORARY_ARENA)
{
    String result;

    b32 is_negative = n < 0;
    n = abs(n);
    
    s32 int_part = floori(n);
    
    s32 ndigits = num_digits(int_part);
    s32 ndecimals = num_decimal_digits(n);
    
    s32 decimal_part = roundi(decimal(n) * pow10(ndecimals));

    if (int_part == 0 && decimal_part == 0)
    {
        result.length = 1;
        result.base = arena_push<u8>(arena, 2);
        result.base[0] = '0';
        result.base[1] = '\0';
        return result;
    }
    
    result.length += ndigits;
    if (ndecimals != 0) result.length += ndecimals + 1; // +1 for '.'
    
    if (is_negative) result.length++;

    result.base = arena_push<u8>(arena, result.length + 1); // +1 for null termination

    if (is_negative) result.base[0] = '-';

    s32 i = result.length;
    
    if (ndecimals != 0)
    {
        for (s32 d = 0; d < ndecimals; d++)
        {
            result.base[--i] = '0' + (decimal_part % 10);
            decimal_part /= 10;
        }
        result.base[--i] = '.';
    }
    
    if (int_part == 0)
    {
        result.base[--i] = '0';
    }
    else
    {
        for (s32 d = 0; d < ndigits; d++)
        {
            result.base[--i] = '0' + (int_part % 10);
            int_part /= 10;
        }
    }

    result.base[result.length] = '\0';
    return result;    
}

String to_string(r64 n, Arena* arena = TEMPORARY_ARENA)
{
    String result;

    b32 is_negative = n < 0;
    n = abs(n);
    
    s64 int_part = floori(n);
    
    s32 ndigits = num_digits(int_part);
    s32 ndecimals = num_decimal_digits(n);
    
    s64 decimal_part = roundi(decimal(n) * pow10(ndecimals));

    if (int_part == 0 && decimal_part == 0)
    {
        result.length = 1;
        result.base = arena_push<u8>(arena, 2);
        result.base[0] = '0';
        result.base[1] = '\0';
        return result;
    }
    
    result.length += ndigits;
    if (ndecimals != 0) result.length += ndecimals + 1; // +1 for '.'
    
    if (is_negative) result.length++;

    result.base = arena_push<u8>(arena, result.length + 1); // +1 for null termination

    if (is_negative) result.base[0] = '-';

    s32 i = result.length;
    
    if (ndecimals != 0)
    {
        for (s32 d = 0; d < ndecimals; d++)
        {
            result.base[--i] = '0' + (decimal_part % 10);
            decimal_part /= 10;
        }
        result.base[--i] = '.';
    }
    
    if (int_part == 0)
    {
        result.base[--i] = '0';
    }
    else
    {
        for (s32 d = 0; d < ndigits; d++)
        {
            result.base[--i] = '0' + (int_part % 10);
            int_part /= 10;
        }
    }

    result.base[result.length] = '\0';
    return result;    
}
