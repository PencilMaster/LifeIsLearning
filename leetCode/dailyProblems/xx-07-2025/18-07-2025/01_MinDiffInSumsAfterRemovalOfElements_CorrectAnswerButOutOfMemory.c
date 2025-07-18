// Here we have the first iterative dp version. 
// It is correct but creates a dp array which is too large for leetCode.
//
// Memory: O(2*n^2 + 2*n);
//
// Next approach is: Copy diagonal of the array when it gets too large
//
//

#include <stdlib.h>
#include <stdbool.h>

long long minimumDifference(int* nums, int numsSize) {
    const size_t n = numsSize/3;
    const long long doubleN = (n << 1);
    const long long dpSize = (n+1) * doubleN;
    int* dp = calloc(dpSize, sizeof(*dp));

    dp[0] = nums[0];
    //printf("dp[%zu] = %d \n", 0, dp[0]);
    for (size_t i = 1; i < n+1; ++i) {
        dp[i] = (dp[i-1] < nums[i] ? dp[i-1] : nums[i]);
        //printf("dp[%zu] = %d \n", i, dp[i]);
    }

    for (size_t horizontal = 1; horizontal < doubleN; ++horizontal) {
        for (size_t diagonal = 1; diagonal <= n+1; ++diagonal) {
            const size_t current = horizontal*(n+1) + diagonal-1;
            const bool operator = (horizontal > n-1 ? 0 : 1); // Subtraction == 0, Addition == 1
            int tempAdd;
            int tempSub;

            if (operator) tempAdd = dp[current - n - 1] + nums[horizontal + diagonal - 1];
            else tempSub = dp[current - n - 1] - nums[horizontal + diagonal - 1];

            //if diagonal == 1, then do operation on nums[horizontal] element
            if (diagonal == 1) {
                if (operator) dp[current] = tempAdd;
                else dp[current] = tempSub;
            }

            else if (operator) dp[current] = (tempAdd > dp[current - 1] ? dp[current-1] : tempAdd);
            else dp[current] = (tempSub > dp[current - 1] ? dp[current-1] : tempSub);

            //printf("dp[%zu] = %d \n", current, dp[current]);
        }
    } 

    int result = dp[n];
    free(dp);
    return result;
}
