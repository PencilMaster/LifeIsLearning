// Here we have an implemented quickSort function for integers
//
#include <stdlib.h>
#include <assert.h>

static inline void swap(int* const a, int* const b) {
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

void quickSortInt(int* intArray, const size_t intArraySize) {
    assert(0 <= intArraySize && "intArraySize cannot be negative");
    if (intArraySize < 2) return; // trivial case

    int pivot = intArraySize - 1;
    size_t l = 0;
    for (size_t r = 0; r < pivot; ++r) {
        if (intArray[r] < intArray[pivot]) swap(intArray+(l++), intArray+r);
    }
    swap(intArray+l, intArray+pivot);

    quickSortInt(intArray, (size_t)l); // left 
    quickSortInt(intArray+l+1, (size_t)(intArraySize-l-1)); // right, does it need to know if swap occurred? don't think so
    return;
}

