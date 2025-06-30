// Finally realised after trying some concepts, that binary search over solution space,
// Seems like the most efficient and easy to write, way to solve this problem.
// 
// computeKPosition is not done yet. Will finish on 26th after that daily.

const long long computeKPosition(const int* nums1, const int nums1Size, const int* nums2, const int nums2Size, const long long mid) {
    int count = 0;
    for (int i = 0; i < nums1Size; ++i) {
        if (nums1[i] == 0) {
            if (target >= 0) count += nums2Size;
            continue;
        }

        if (nums1[i] > 0) {

        }
    }
}

long long kthSmallestProduct(const int* nums1, const int nums1Size, const int* nums2, const int nums2Size, const long long k) {
    long long count = 0;

    // 1. define target between 10^10 and -10^10 --- Because -1.e10 <= nums1[i], nums[j] <= 1.e10
    long long left = 1.e-10;
    long long right = 1.e10;

    while (left < right) {
        long long mid = left + (right - left) / 2;
        if (computeKPosition(nums1, &nums1Size, nums2, &nums2Size, mid) < k) {
            left = mid + 1;
        }
        else {
            right = mid;
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

