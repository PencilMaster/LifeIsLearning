// Had another problem as well not checking backwards with j. Fixed it
// but greedy algorithm is too slow for test cases. Going with little bit
// different approach next iteration. Should be way faster.
//

#include <assert.h>
#include <stdbool.h>

/**
 * Note: The returned array must be malloced, assume caller calls free().
 */
bool arrayContains(const int* array, const int* arraySize, const int* number) {
    assert(*arraySize >=0 && "Array can not be negative");

    for (int i = 0; i < *arraySize; ++i) {
        if (array[i] == *number) return true;
    }
    //printf("%d ", *number);
    return false;
}

//  Very annoying: Leetcode passes array sizes as an int and not size_t........
//  Now they want us to return an array containing indeces as int and not size_t.... This is absolutely not save
int* findKDistantIndices(int* nums, int numsSize, int key, int k, int* returnSize) {
    // We know that key is in nums
    // 1 <= k <= nums.length

    // Option 1: For each element in array int, we check neighbouring elements with distance k -- GREEDY
    // Option 2: We iterate over array and as soon as we find a key, 
    //      we add indices i with distance k to result array - if they are not already in there -- MORE EFFICIENT

    // Question: How do we know returnSize for malloc? -- We need to iterate over nums once to gather the data
    assert(numsSize >= 0 && "numsSize can not be negative");

    int* tempArray = malloc(numsSize * sizeof(*tempArray));
    int currArraySize = 0;

    for (int i = 0; i < numsSize; ++i) {
        for (int j = i - k; j < numsSize && j-i <= k; ++j) {
            if (j >= 0 && nums[j] == key && !arrayContains(tempArray, &currArraySize, &i)) {
                //printf("%d ", (int)i);
                tempArray[currArraySize++] = i;
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
