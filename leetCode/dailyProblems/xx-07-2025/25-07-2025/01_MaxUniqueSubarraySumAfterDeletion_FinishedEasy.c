// Solved this during Italy vacation with friends, uploading all solved exercises now.
//

#include <stdlib.h>

int compareInt(const void* a, const void* b) {
    int* A = (int*) a;
    int* B = (int*) b;

    return (*A > *B) - (*A < *B);
}

int maxSum(int* nums, int numsSize) {
    qsort(nums, numsSize, sizeof(*nums), compareInt);

    int result = 0;
    int* iteratorInt = nums;
    if (numsSize == 1 || *iteratorInt > 0) result += *iteratorInt;
    ++iteratorInt;
    for (int i = 1; i < numsSize; ++iteratorInt, ++i) {
        if (i == numsSize-1 && *iteratorInt <= 0) result += *iteratorInt;
        else if (*iteratorInt == *(iteratorInt-1) || *iteratorInt < 0) continue;
        else result += *iteratorInt;
    }

    return result;
}
