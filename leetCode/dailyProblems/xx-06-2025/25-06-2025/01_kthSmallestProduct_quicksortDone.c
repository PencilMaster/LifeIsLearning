// Todays leetcode question gives us already sorted integer arrays. For traininig purposes I
// Implemented quicksort first anyways, to refresh recursive functions.
//

#include <assert.h>
#include <stdlib.h>

//  No error handling with try / catch yet. Refresh this, check if it adds new 
//  security vulnerabilities and implement it after getting the code to work without it
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

long long kthSmallestProduct(int* nums1, int nums1Size, int* nums2, int nums2Size, long long k) {
    //  1. Merge sort both arrays into one
    //  2. We cannot differentiate between both arrays if we merge them.

    //  After sorting 2 cases with quicksort:
    //  Case 1: One integer is negative, then pick the greatest integer from other Array.
                //  Afterwards, check if product of most negative and second most positive is 
                //  smaller than second most negative and most positive.
    //  Case 2: Both are positive || both are negative. In both cases, pick closest to zero.
    assert(nums1Size >= 0 && nums2Size >= 0 && "At least one initial Array size is negative");

    quickSortInt(nums1, (size_t)nums1Size);
    quickSortInt(nums2, (size_t)nums2Size);



    return 0;
}
