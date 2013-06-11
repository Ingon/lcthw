#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

void die(const char *message)
{
    if(errno) {
        perror(message);
    } else {
        printf("ERROR: %s\n", message);
    }

    exit(1);
}

typedef int (*compare_cb)(int a, int b);
typedef int *(*sort_cb)(int *numbers, int count, compare_cb cmp);

int *bubble_sort(int *numbers, int count, compare_cb cmp)
{
    int temp = 0;
    int i = 0;
    int j = 0;
    int *target = malloc(sizeof(int) * count);

    if(! target) die("Memory error.");

    memcpy(target, numbers, count * sizeof(int));

    for(i = 0; i < count; i++) {
        for(j = 0; j < count - 1; j++) {
            if(cmp(target[j], target[j + 1]) > 0) {
                temp = target[j + 1];
                target[j + 1] = target[j];
                target[j] = temp;
            }
        }
    }

    return target;
}

int quick_sort_partition(int *numbers, int left, int right, compare_cb cmp)
{
    int pivotValue = numbers[right];
    int storeIndex = left;

    int tmp = 0;
    for(int i = left; i < right; i++) {
        if(cmp(numbers[i], pivotValue) <= 0) {
            tmp = numbers[i];
            numbers[i] = numbers[storeIndex];
            numbers[storeIndex] = tmp;

            storeIndex++;
        }
    }

    tmp = numbers[right];
    numbers[right] = numbers[storeIndex];
    numbers[storeIndex] = tmp;

    return storeIndex;
}

void quick_sort_recur(int *numbers, int left, int right, compare_cb cmp)
{
    if(left >= right)
        return;

    int newPivotIndex = quick_sort_partition(numbers, left, right, cmp);

    quick_sort_recur(numbers, left, newPivotIndex - 1, cmp);
    quick_sort_recur(numbers, newPivotIndex + 1, right, cmp);
}

int *quick_sort(int *numbers, int count, compare_cb cmp) 
{
    int *target = malloc(sizeof(int) * count);
    if(! target) die("Memory error.");
    memcpy(target, numbers, count * sizeof(int));

    quick_sort_recur(target, 0, count - 1, cmp);

    return target;
}

int sorted_order(int a, int b) 
{
    return a - b;
}

int reverse_order(int a, int b)
{
    return b - a;
}

int strange_order(int a, int b)
{
    if(a == 0 || b == 0) {
        return 0;
    }
    return a % b;
}

void test_sorting(int *numbers, int count, sort_cb srt, compare_cb cmp)
{
    int i = 0;
    int *sorted = srt(numbers, count, cmp);

    if(!sorted) die("Failed to sort as requested");

    for(i = 0; i < count; i++) {
        printf("%d ", sorted[i]);
    }
    printf("\n");

    free(sorted);
}

int main(int argc, char const *argv[])
{
    if(argc < 2) die("USAGE: ex18 4 3 1 5 6");

    int count = argc - 1;
    int i = 0;
    const char **inputs = argv + 1;

    int *numbers = malloc(sizeof(int) * count);
    if(! numbers) die("Memory error");

    for(i = 0; i < count; i++) {
        numbers[i] = atoi(inputs[i]);
    }

    test_sorting(numbers, count, bubble_sort, sorted_order);
    test_sorting(numbers, count, quick_sort, sorted_order);

    test_sorting(numbers, count, bubble_sort, reverse_order);
    test_sorting(numbers, count, quick_sort, reverse_order);
    
    test_sorting(numbers, count, bubble_sort, strange_order);
    test_sorting(numbers, count, quick_sort, strange_order);

    free(numbers);

    return 0;
}