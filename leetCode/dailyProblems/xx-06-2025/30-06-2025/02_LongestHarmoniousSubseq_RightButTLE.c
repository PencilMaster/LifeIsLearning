// This solution works but exceeds the time limit.
//
// Since order of elements does not matter, we can sort and sliding window. 
// This will be more efficient and the next approach.
//


inline int calcDiff(const int *a, const int *b) {
    return *a - *b;
}

int findLHS(int* nums, int numsSize) {
    int maxLength = 0;

    // This works but exceeds time limit in last test case.
    for (int left = 0; left < numsSize-maxLength; ++left) {
        int min = nums[left], max = nums[left];
        int upper = 0, lower = 0;
        
        for (int right = left+1; right < numsSize; ++right) {
            int curDif = calcDiff(nums+left, nums+right);
            if (0 == curDif) continue;

            if (-1 == curDif) max = nums[right];
            else ++upper;

            if (1 == curDif) min = nums[right];
            else ++lower;
        }
        if (calcDiff(nums+left, &min) == 1) maxLength = (numsSize - left - lower > maxLength ? numsSize-left-lower : maxLength);
        if (calcDiff(nums+left, &max) == -1) maxLength = (numsSize - left - upper > maxLength ? numsSize-left-upper : maxLength);
    }

    return maxLength;
}
