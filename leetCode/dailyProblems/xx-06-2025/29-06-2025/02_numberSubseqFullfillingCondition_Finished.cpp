// Fixed MemoryLimitExceeded.
//
// Problem was that nums array was passed into binarySearch by copy and not reference.
// I also changed the exponent Array from static to a dynamic allocation in case this contributed too -
//        But I would change this back again and amend it such, that it can be expanded for future larger problems if needed.
//

#include <algorithm>

class Solution {
    private:
        const int BinarySearch(const std::vector<int>&, const int&, const int&);
        const int modPow(const int&);
        
        static const int MOD = 1e9 + 7;

    public:
        int numSubseq(vector<int>& nums, int target) {
            auto cmp = [](const int &a, const int &b) {return a < b;}; 
            
            size_t numsSize = nums.size();

            // Fill a vector with exponents to subtract
            std::vector<int> moddedExponent;
            moddedExponent.reserve(numsSize);
            moddedExponent[0] = 1;
            for (int i = 1; i < numsSize; ++i) {
                moddedExponent[i] = (moddedExponent[i-1] << 1) % MOD;
            }

            std::sort(nums.begin(), nums.end(), cmp);
            int output = 0;
            
            for (size_t i = 0; i < numsSize && nums[i]*2 <= target; ++i) {
                int posMax = BinarySearch(nums, i, target);
                while (nums[posMax] + nums[i] > target) --posMax;

                int exponent = posMax - i;

                output += moddedExponent[exponent];
                output %= MOD;
            }
            
            return output;
        }
};

const int Solution::BinarySearch(const std::vector<int> &nums, const int &l, const int &t) {
    int left = l;
    int right = nums.size()-1;

    while (left < right) {
        int mid = left + (right - left + 1) / 2;
        //std::cout << " -- mid: " << mid << " // value: " << nums[mid] << " // t-l: " << (t - nums[l]) << std::endl;
        
        if (nums[mid] > (t - nums[l])) right = mid - 1;
        else left = mid;
    } // For now can hppen that one too big is returned, could fix that with an if statement but needed in place.

    return left;
};

//std::cout << " | posMax: " << posMax << "\n";
//std::cout << "min: " << nums[i] << " | max: " << nums[posMax];


