#ifndef SORT_H
#define SORT_H

// @todo reverse sort versions

void insertion_sort(s32* array, s32 count);
void insertion_sort(r32* array, s32 count);
void insertion_sort(u8* array, s32 count);
void quicksort(s32* array, s32 l, s32 h);
void quicksort(r32* array, s32 l, s32 h);
void quicksort(u8* array, s32 l, s32 h);

inline s32 quicksort_partition(s32* array, s32 l, s32 h)
{
	s32 x = array[h];
	s32 i = (l - 1);

	for (s32 j = l; j <= h - 1; j++)
    {
		if (array[j] <= x)
        {
			i++;
			swap(array[i], array[j]);
		}
	}
	swap(array[i + 1], array[h]);
	return (i + 1);
}
inline s32 quicksort_partition(r32* array, s32 l, s32 h)
{
	r32 x = array[h];
	s32 i = (l - 1);

	for (s32 j = l; j <= h - 1; j++)
    {
		if (array[j] <= x)
        {
			i++;
			swap(array[i], array[j]);
		}
	}
	swap(array[i + 1], array[h]);
	return (i + 1);
}
inline s32 quicksort_partition(u8* array, s32 l, s32 h)
{
	u8 x = array[h];
	s32 i = (l - 1);

	for (s32 j = l; j <= h - 1; j++)
    {
		if (array[j] <= x)
        {
			i++;
			swap(array[i], array[j]);
		}
	}
	swap(array[i + 1], array[h]);
	return (i + 1);
}

inline void quicksort(s32* array, s32 count)
{
    quicksort(array, 0, count - 1);
}
inline void quicksort(r32* array, s32 count)
{
    quicksort(array, 0, count - 1);
}
inline void quicksort(u8* array, s32 count)
{
    quicksort(array, 0, count - 1);
}

#endif
