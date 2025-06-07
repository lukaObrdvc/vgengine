void insertion_sort(s32* array, s32 count)
{
    for (s32 i = 1; i < count; ++i)
    {
        s32 value = array[i];
        s32 j = i;
        while (j > 0 && array[j - 1] > value)
        {
            array[j] = array[j - 1];
            --j;
        }
        array[j] = value;
    }
}
void insertion_sort(r32* array, s32 count)
{
    for (s32 i = 1; i < count; ++i)
    {
        r32 value = array[i];
        s32 j = i;
        while (j > 0 && array[j - 1] > value)
        {
            array[j] = array[j - 1];
            --j;
        }
        array[j] = value;
    }
}
void insertion_sort(u8* array, s32 count)
{
    for (s32 i = 1; i < count; ++i)
    {
        u8 value = array[i];
        s32 j = i;
        while (j > 0 && array[j - 1] > value)
        {
            array[j] = array[j - 1];
            --j;
        }
        array[j] = value;
    }
}

void quicksort(s32* array, s32 l, s32 h)
{
    ASSERT(h >= l);
    Scratch* scratch = get_scratch();
    s32* temp = scratch_push<s32>(scratch, h - l + 1);

	s32 top = -1;

	temp[++top] = l;
	temp[++top] = h;

	while (top >= 0)
    {
		h = temp[top--];
		l = temp[top--];

		s32 p = quicksort_partition(array, l, h);

		if (p - 1 > l)
        {
			temp[++top] = l;
			temp[++top] = p - 1;
		}

		if (p + 1 < h)
        {
			temp[++top] = p + 1;
			temp[++top] = h;
		}
	}

    release_scratch(scratch);
} 
void quicksort(r32* array, s32 l, s32 h)
{
    ASSERT(h >= l);
    Scratch* scratch = get_scratch();
    r32* temp = scratch_push<r32>(scratch, h - l + 1);

	s32 top = -1;

	temp[++top] = l;
	temp[++top] = h;

	while (top >= 0)
    {
		h = temp[top--];
		l = temp[top--];

		s32 p = quicksort_partition(array, l, h);

		if (p - 1 > l)
        {
			temp[++top] = l;
			temp[++top] = p - 1;
		}

		if (p + 1 < h)
        {
			temp[++top] = p + 1;
			temp[++top] = h;
		}
	}

    release_scratch(scratch);
} 
void quicksort(u8* array, s32 l, s32 h)
{
    ASSERT(h >= l);
    Scratch* scratch = get_scratch();
    u8* temp = scratch_push<u8>(scratch, h - l + 1);

	s32 top = -1;

	temp[++top] = l;
	temp[++top] = h;

	while (top >= 0)
    {
		h = temp[top--];
		l = temp[top--];

		s32 p = quicksort_partition(array, l, h);

		if (p - 1 > l)
        {
			temp[++top] = l;
			temp[++top] = p - 1;
		}

		if (p + 1 < h)
        {
			temp[++top] = p + 1;
			temp[++top] = h;
		}
	}

    release_scratch(scratch);
} 
