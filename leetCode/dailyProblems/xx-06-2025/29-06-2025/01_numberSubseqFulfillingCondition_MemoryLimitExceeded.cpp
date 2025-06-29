// Trying to get it done fast on Sunday evening led me here.
// It works, but uses too much memory for now. My guess is filling the Exponent-Vector with all entries is overkill.
// Will sleep now and ammend it after tomorrows leetCode.
//
//

#include <algorithm>
#include <iostream> //debugging
#include <vector>

class Solution {
    private:
        const int BinarySearch(const std::vector<int> nums, const int &left, const int &target);
        const int modPow(const int&);
        void fillModdedExponent();
        
        static const int MOD = 1e9 + 7;
        std::vector<int> moddedExponent;

    public:
        int numSubseq(vector<int>& nums, int target) {
            auto cmp = [](const int &a, const int &b) {return a < b;}; 
            
            size_t numsSize = nums.size();
            this->moddedExponent.resize(numsSize);
            fillModdedExponent();

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

const int Solution::BinarySearch(const std::vector<int> nums, const int &l, const int &t) {
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

const int Solution::modPow(const int &exponent) {
    int possibilities = 1;
    for (int i = 0; i < exponent/10; i += 10) {
        possibilities <<= 10;
        possibilities %= MOD;
    }

    for (int i = exponent % 10; i > 0; --i) {
        possibilities <<= 1;
        possibilities %= MOD;
    }

    return possibilities;
};

void Solution::fillModdedExponent() {
    const int moddedSize = moddedExponent.size();
    moddedExponent[0] = 1;
    for (int i = 1; i < moddedSize; ++i) {
        moddedExponent[i] = (moddedExponent[i-1] << 1) % MOD;
    }
 };

                //std::cout << " | posMax: " << posMax << "\n";
                                //std::cout << "min: " << nums[i] << " | max: " << nums[posMax] ;


