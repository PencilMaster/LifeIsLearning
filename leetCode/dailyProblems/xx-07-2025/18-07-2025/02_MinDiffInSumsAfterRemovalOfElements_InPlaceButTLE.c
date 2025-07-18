// I changed it to be in-place. It only uses a vector with size n+1 now, but still
// does the same amount of computations as it is a DP solution.
//
// Idea: Maybe able to do it more efficient with some form of prefix sum? Storing the current removed numbers in a sorted vector
//          And if nums[i] > then current n-th largest, we readd n-th largest and add nums[i] to removed. Until we get to nums[2n]
//          -- Need to do it differently but probably with prefix sum
//
#include <stdlib.h>
#include <stdbool.h>

long long minimumDifference(int* nums, int numsSize) {
    const size_t n = numsSize/3;
    const long long doubleN = (n << 1);
    int* dp = malloc((n+1) * sizeof(*dp));

    dp[0] = nums[0];
    //printf("dp[%zu] = %d \n", 0, dp[0]);
    for (size_t i = 1; i < n+1; ++i) {
        dp[i] = (dp[i-1] < nums[i] ? dp[i-1] : nums[i]);
        //printf("dp[%zu] = %d \n", i, dp[i]);
    }

    for (size_t horizontal = 1; horizontal < doubleN; ++horizontal) {
        const bool operator = (horizontal > n-1 ? 0 : 1); // Subtraction == 0, Addition == 1
        for (size_t diagonal = 1; diagonal <= n+1; ++diagonal) {
            //const size_t current = horizontal*(n+1) + diagonal-1;
            const size_t current = diagonal-1;
            int tempAdd;
            int tempSub;

            if (operator) tempAdd = dp[current] + nums[horizontal + diagonal - 1];
            else tempSub = dp[current] - nums[horizontal + diagonal - 1];

            //if diagonal == 1, then do operation on nums[horizontal] element
            if (diagonal == 1) {
                if (operator) dp[current] = tempAdd;
                else dp[current] = tempSub;
                continue;
            }

            else if (operator) dp[current] = (tempAdd > dp[current - 1] ? dp[current-1] : tempAdd);
            else dp[current] = (tempSub > dp[current - 1] ? dp[current-1] : tempSub);

            //printf("dp[%zu] = %d \n", current, dp[current]);
        }
    } 

    return dp[n];
}
