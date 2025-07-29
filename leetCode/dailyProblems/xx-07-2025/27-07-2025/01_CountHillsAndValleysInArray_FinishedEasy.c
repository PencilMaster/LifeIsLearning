// last solved exercise during vacation in Italy.
//

#include <stdbool.h>

int countHillValley(int* nums, int numsSize) {
    bool greater = 0;
    bool smaller = 0;
    int result = 0;
    for (int i = 1; i < numsSize; ++i) {
        if (nums[i] > nums[i-1]) {
            if (smaller) ++result;
            greater = true;
            smaller = false;
        }
        else if (nums[i] < nums[i-1]) {
            if (greater) ++result;
            smaller = true;
            greater = false;
        }
    }
    return result;
}
