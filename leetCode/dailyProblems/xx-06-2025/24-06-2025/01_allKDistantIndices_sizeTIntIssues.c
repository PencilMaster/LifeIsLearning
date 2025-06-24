//  LeetCode wants us to return an array containing indices of type "int".
//  They also give us the length of their array as an "int"..
//  It is way safer to use size_t. They do not do that. Instead casting from the implementation safe size_t
//  to int generates overflows in my program, since size_t is unsigned long for me.
//
//  I will change all size_t to int in next implementation and see if it runs.
//

#include <assert.h>
#include <stdbool.h>

/**
 * Note: The returned array must be malloced, assume caller calls free().
 */
bool arrayContains(const int* array, const int* arraySize, const size_t* number) {
    assert(*arraySize >=0 && "Array can not be negative");

    for (size_t i = 0; i < (size_t)*arraySize; ++i) {
        if (array[i] == (int)*number) return true;
    }
    //printf("%d ", (int)number);
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

    for (size_t i = 0; i < (size_t)numsSize; ++i) {
        for (size_t j = i; j < (size_t)numsSize && j-i <= k; ++j) {
            if (nums[j] == key && !arrayContains(tempArray, &currArraySize, &i)) {
                //printf("%d ", (int)i);
                tempArray[currArraySize++] = (int)i;
            }
        }
    }

    *returnSize = currArraySize;
    int* resultArray = malloc(*returnSize * sizeof(*resultArray));

    for (size_t i = 0; i < (size_t)*returnSize; ++i)
        resultArray[i] = tempArray[i];
    free(tempArray);

    return resultArray;
}
