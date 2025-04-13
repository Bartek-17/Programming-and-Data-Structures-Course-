#include <stdio.h>

// size_t - size type (used for sizes and lengths)

void *bsearch(const void *key, const void *base, size_t num, size_t size, int (*compar)(const void *, const void *))
{
    size_t low = 0;
    size_t high = num;
    while (low < high)
    {
        size_t mid = low + (high - low) / 2;
        const void *mid_elem = (const char *)base + (mid * size);

        int result = compar(key, mid_elem);
        if (result < 0)
        {
            high = mid;
        }
        else if (result > 0)
        {
            low = mid + 1;
        }
        else
        {
            return (void *)mid_elem;
        }
    }
    return NULL;
}

int compare_ints(const void *a, const void *b)
{
    return (*(int *)a - *(int *)b);
}

int main()
{
    int arr[] = {1, 2, 4, 5, 6, 7, 8, 9, 10, 35, 47};
    int key = 6;
    size_t num_elements = sizeof(arr) / sizeof(arr[0]);

    int *found = (int *)bsearch(&key, arr, num_elements, sizeof(int), compare_ints);

    if (found)
    {
        printf("Found: %d\n", *found);
    }
    else
    {
        printf("Not found\n");
    }
    return 0;
}
