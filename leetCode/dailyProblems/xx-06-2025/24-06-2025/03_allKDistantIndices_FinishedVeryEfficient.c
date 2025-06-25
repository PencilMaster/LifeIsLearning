// It is finished. Changes:
//
// 1. Contains function() -> pop_back() -- we only need to check last element
// 2. Iteration only over [i-k, ... , i , ... , i+k] where nums[i] == key.
//        2.a.  This iteration is done right to left
//        2.b.  Each index is compared to biggest stored index in result Array
//              2.b.i.  When index is found, only add missing [found+1, ... , i+k] indexes
//              2.b.ii. when it's not found, add all indexes in range. Watch out for 0 and arraySize
//
//


#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

/**
 * Note: The returned array must be malloced, assume caller calls free().
 */
const int pop_back(const int* array, const int* arraySize) {
    assert(*arraySize >=0 && "Array can not be negative");
    return *arraySize > 0 ? array[*arraySize-1] : -1;
}

//  Very annoying: Leetcode passes array sizes as an int and not size_t........
//  Now they want us to return an array containing indeces as int and not size_t. This is not save but we'll do it for now
int* findKDistantIndices(const int* nums, const int numsSize, const int key, const int k, int* returnSize) {

    assert(numsSize >= 0 && "numsSize can not be negative");

    int* tempArray = malloc(numsSize * sizeof(*tempArray));
    int currArraySize = 0;

    for (int i = 0; i < numsSize; ++i) {
        if (nums[i] == key) {

            // Check from the right and stop as soon as one is found.
            // Then save last index not yet contained and save all needed ones in-order.
            int notContained = -1;
            for (int j = i+k; j >= i-k; --j) {
                if (j >= 0 && j < numsSize && j == pop_back(tempArray, &currArraySize)) {
                    notContained = j+1;
                    break;
                }
                notContained = (i-k > 0) ? i-k : 0;
            }
            if (notContained < 0 || notContained > i+k) continue;
            else {
                for (int j = notContained; notContained < numsSize && notContained <= i+k; ++notContained) {
                    tempArray[currArraySize++] = notContained;
                }
            }
        }
    }

    *returnSize = currArraySize;
    int* resultArray = malloc(*returnSize * sizeof(*resultArray));

    for (int i = 0; i < *returnSize; ++i)
        resultArray[i] = tempArray[i];
    free(tempArray);

    return resultArray;
}

int main (int args, char** argv) {
    size_t numsSize = 100;
    int* nums = malloc(numsSize * sizeof(*nums));

    for (size_t i = 0; i < numsSize; ++i) {
        nums[i] = rand() % 10;
    }
    
    int key = 6;
    int returnSize;
    int* kDistantIndices = findKDistantIndices(nums, (int)numsSize, key, 3, &returnSize);
    
    
    printf("Main executed just fine, amount of indices in k-distance are %d", returnSize);
    free(kDistantIndices);
    return 0;
}

// TODO: let numsSize = argv[][]; (check again where arguments are exactly saved in argv array)
//       let key = argv[][];

    // Thoughts:

    // We know that key is in nums
    // 1 <= k <= nums.length

    // Option 1: For each element in array int, we check neighbouring elements with distance k -- GREEDY
    // Option 2: We iterate over array and as soon as we find a key, 
    //      we add indices i with distance k to result array - if they are not already in there -- MORE EFFICIENT

    // Question: How do we know returnSize for malloc? -- We need to iterate over nums once to gather the data
