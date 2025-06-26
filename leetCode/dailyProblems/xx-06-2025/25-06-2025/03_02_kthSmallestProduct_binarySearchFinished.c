// Debugging the second part cost me some time. Reviewing binary Search variant, where a second one is done inside the first one. 
// Epecially returning all lower calc than the target, or actually returning the targets position - 
// if it were in the order of calculations - confused me. Since it most of the time is not a product that is present,
// but at some point it becomes present when we get closer to the actual value we are looking for. Both cases are handled in one.
//
//

#include <stdlib.h>


const long long computeKPosition(const int* nums1, const int nums1Size, const int* nums2, const int nums2Size, const long long target) {
    long long count = 0;


    for (int i = 0; i < nums1Size; ++i) {
        if (nums1[i] == 0) {
            if (target >= 0) count += nums2Size;
            continue;
        }

        else {
            long long left = 0, right = nums2Size;
            while (left < right) {
                long long mid = left + (right - left) / 2;
                if (1LL * nums1[i] * nums2[mid] <= target) {
                    if (nums1[i] > 0)
                        left = mid + 1;
                    else right = mid;
                }
                else {
                    if (nums1[i] > 0)
                        right = mid;
                    else left = mid + 1;
                }
            }
            count += (nums1[i] > 0) ? left : nums2Size - left;
        }
    }
    return count;
}

long long kthSmallestProduct(const int* nums1, const int nums1Size, const int* nums2, const int nums2Size, const long long k) {
    long long count = 0;

    // 1. define target between 10^10 and -10^10 --- Because -1.e10 <= nums1[i], nums[j] <= 1.e10
    long long left = -1e10;
    long long right = 1e10;

    while (left < right) {
        long long mid = left + ((right - left) / 2);
        if (computeKPosition(nums1, nums1Size, nums2, nums2Size, mid) < k) {
            left = mid + 1;
        }
        else {
            right = mid;
        }
    }
    return left;
}



// Iteratively we will compute the lowest products bottom-up.
    // Assume [x_n , ... , 0 , ... , x_1] and [y_n , ... , 0 , ... , y_1] from lowest to highest int.
    // 1. if x < 0 and y > 0 exist, we iterate over all of them, starting with x_n
        // 1.1. num1 = x_n * y_1; ++count; 
        // 1.2. 

    // We have O(k)

    // This can be less efficient I think, so let's just do binary se

