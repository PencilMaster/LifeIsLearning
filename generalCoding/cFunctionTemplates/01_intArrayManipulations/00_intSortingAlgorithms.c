// Here we have an implemented quickSort function
//
#include <stdlib.h>

void quickSortInt(int* intArray, const size_t intArraySize) {
    if (intArraySize < 2) return; // trivial case

    int pivot = intArraySize - 1;
    size_t r = intArraySize - 2;
    for (size_t l = 0; l <= r; ++l) {
        if (intArray[l] < intArray[pivot]) continue;
        else {
            int temp = intArray[l];
            if (l != r) intArray[l] = intArray[r];
            intArray[r] = intArray[pivot];
            intArray[pivot] = temp;
            --r;
            --pivot;
        }
    }
    quickSortInt(intArray, (size_t)pivot); // left 
    quickSortInt(intArray+pivot+1, (size_t)(intArraySize-pivot)); // right, does it need to know if swap occurred? don't think so

    return;
}
