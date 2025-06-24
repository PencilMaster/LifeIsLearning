// This solution implements toBaseTen without using std::pow(). The only problem 
// here is that executing the code exceeds the memory limit allowed by the leetCode task parameters.
//

#include <string>
#include <algorithm>
#include <cassert>
#include <cmath>
#include <limits.h>


class Solution {
public:
    // kMirrored check function
    bool isPalindrome(const std::string s) {
        std::string::const_iterator itFront = cbegin(s);
        std::string::const_iterator itBack = cend(s) - 1;
        while (itFront <= itBack) {
            if (*itFront != *itBack) {
                return false;
            }
            ++itFront;
            --itBack;
        }
        return true;
    }

    void updateKNumberEnd(std::string &kNumberEnd, int k) {
        if (kNumberEnd == "") {
            kNumberEnd = "1";
            return;
        }

        int leading = 1;
        for (std::string::iterator itEnd = kNumberEnd.end(); itEnd >= kNumberEnd.begin(); --itEnd) {
            if (*itEnd == static_cast<char>(k-1)) {
                *itEnd = 0;
            }
            else {
                *itEnd = static_cast<char>((*itEnd - '0')+1);
                return;
            }
        }
        kNumberEnd = to_string(leading).append(kNumberEnd);
    }

    const bool toBaseTen(const std::string &kNumber, long long &baseTenLong, const int &k) {
        // long mulpitplier = 1; Not needed if we go from left to right
        for (char c : kNumber) {
            int temp = c - '0';
            if (temp >= k || (baseTenLong > (LLONG_MAX - temp) / k)) return false; // 1. Checks validity of char in base 2. Checks that no overflow would occur
            else baseTenLong = baseTenLong * k + temp;
        }
        return true;
    }

    long long kMirror(int k, int n) {
        // Just to make sure that invariant is followed.
        assert(n > 0 && k > 1 && "At least one input parameter is too low");

        // 0. Initialise state variables
        int counter = 0;
        long long result = 0;
        std::string kNumberEnd = "";
        int kNumberFront = 1;
        
        // 1. k - base string creator
        while (counter < n) {
            const std::string kNumber = kNumberEnd.append(to_string(kNumberFront));
            
            // 2. k - Mirror check
            if (kNumber.length() == 1) {
                result += kNumberFront;
                ++counter;
            }
            else {
                // 2 Cases -- Case 1: even and Iterators go past each other, Case 2: odd and Iterators hit each other. Both doable with begin > end.
                bool kMirrored = isPalindrome(kNumber);

                // 3. Convert to base 10 and check it's mirroring
                if (kMirrored) {
                    long long baseTenLong = 0;
                    if (toBaseTen(kNumber, baseTenLong, k) && isPalindrome(to_string(baseTenLong))) {
                        result += baseTenLong;
                        ++counter;
                    }
                }
            }

            if (++kNumberFront >= k) {
                updateKNumberEnd(kNumberEnd, k); // We need an update function to update kNumberEnd
                kNumberFront = 1;
            }
        }

        return result;
    }
};
