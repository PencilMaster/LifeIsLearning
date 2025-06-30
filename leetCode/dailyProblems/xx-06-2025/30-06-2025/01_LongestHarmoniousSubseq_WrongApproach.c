// In this first solution, I returned the length of the longest subarray, where |nums[left] - nums[right]| == 1
//
//

int findLHS(int* nums, int numsSize) {
    // For each nums entry, check from the right for the first one that fits until left = right
    int maxLength = 0;

    //I did it for max difference between first element and last element. This is wrong -> maybe a sliding window approach with updating min, max over array could work?
    for (int left = 0; left < numsSize; ++left) {
        for (int right = numsSize-1; right > left; --right) {
            if ((nums[left] - nums[right] > 0) ? ((nums[left] - nums[right]) == 1) : ((nums[right] - nums[left]) == 1)) {
                maxLength = (right-left+1 > maxLength) ? right-left+1 : maxLength;
                break;
            }
        }
    }

    return maxLength;
}
