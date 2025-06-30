// Here I took the quickSort template - fixed it - and then used sliding window to
// measure the longest harmonious subsequence.
//

#include <stdlib.h>
static inline void swap(int* const a, int* const b) {
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

void quickSortInt(int* intArray, const size_t intArraySize) {
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

int findLHS(int* nums, int numsSize) {
    quickSortInt(nums, (size_t) numsSize);

    int left = 0, right = 0, maxLength = 0;
    printf("[");

    while (left < numsSize) {
        while (right < numsSize && nums[right] - nums[left] <= 1) {
            printf(",%d", nums[right]);
            ++right;
        }
        if (nums[right-1] != nums[left]) maxLength = (right-left > maxLength) ? right-left : maxLength ; // In c, if ternary is evaluated true, then the part after "?" is executed in parantheses.
        ++left;
    }
    printf("]");

    return maxLength;
}
