// In this code we exceed the time limit, next iteration I will just use 
// do obj->nums2 = nums2 to check whether the intended this unsafe instantiation.
//
//
//

#include <stdlib.h>


typedef struct {
    int* nums1;
    size_t nums1Size;

    int* nums2;
    size_t nums2Size;
} FindSumPairs;

int cmpfunction(const void* A, const void* B) { // In case we use qsort()
    int* a = (int*) A;
    int* b = (int*) B;
    return (*a > *b) - (*a < *b);
}


FindSumPairs* findSumPairsCreate(const int* const nums1, const int nums1Size, const int* const nums2, const int nums2Size) {
    FindSumPairs* obj = malloc(sizeof(*obj));
    (*obj).nums1 = malloc(nums1Size * sizeof(*nums1));
    (*obj).nums1Size = (size_t) nums1Size;
    (*obj).nums2 = malloc(nums2Size * sizeof(*nums2));
    (*obj).nums2Size = (size_t) nums2Size;

    size_t cpIterator = 0;
    while (cpIterator < nums1Size && cpIterator < nums2Size) {
        ((*obj).nums1)[cpIterator] = nums1[cpIterator];
        ((*obj).nums2)[cpIterator] = nums2[cpIterator];
        ++cpIterator;
    }

    while (cpIterator < nums1Size)
        ((*obj).nums1)[cpIterator++] = nums1[cpIterator]; //right to left precedence
    while (cpIterator < nums2Size)
        ((*obj).nums2)[cpIterator++] = nums2[cpIterator]; //right to left precedence

    qsort(obj->nums1, obj->nums1Size, sizeof(*(obj->nums1)), cmpfunction);
    
    return obj;
}

void findSumPairsAdd(FindSumPairs* obj, const int index, const int val) {
    obj->nums2[index] += val; // think using "->" makes compilation slower than writing *ob.
}

// Might want to use qsort() but need to copy arrays again then. 
// Might be worth it, since O(n+m + nlog(n) + mlog(m) + n+m) < O(n*m). More meory heavy though.
    
int findSumPairsCount(FindSumPairs* obj, const int tot) { 
    int totalAmount = 0;
    for (size_t nums1Iterator = 0; nums1Iterator < obj->nums1Size; ++nums1Iterator) {
        const int difference = tot - obj->nums1[nums1Iterator];
        if (difference <= 0) continue;
        
        int tempAmount = 0;
        for (size_t nums2Iterator = 0; nums2Iterator < obj->nums2Size; ++nums2Iterator) {
            if (obj->nums2[nums2Iterator] == difference) ++tempAmount;
        }

        if (nums1Iterator < obj->nums1Size - 1 && obj->nums1[nums1Iterator] == obj->nums1[nums1Iterator+1]) {
            totalAmount += tempAmount;
            ++nums1Iterator;
        }
        totalAmount += tempAmount;
    } 
    return totalAmount;
}

void findSumPairsFree(FindSumPairs* obj) {
    free((*obj).nums1);
    free((*obj).nums2);
    free(obj);
}

/**
 * Your FindSumPairs struct will be instantiated and called as such:
 * FindSumPairs* obj = findSumPairsCreate(nums1, nums1Size, nums2, nums2Size);
 * findSumPairsAdd(obj, index, val);
 
 * int param_2 = findSumPairsCount(obj, tot);
 
 * findSumPairsFree(obj);
*/
