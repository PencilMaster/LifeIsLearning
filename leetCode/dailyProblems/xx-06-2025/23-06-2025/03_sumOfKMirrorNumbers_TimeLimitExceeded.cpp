// Big step forward --> Problems were:
//
// 1. In updateKNumberOld I decremented (k-1)-'0' by accident instead of transforming the char
//    saved in *itEnd to its ASCII value, with *itEnd-'0'.
//    -> This lead to an overflow in toBaseTen, as for ex. LLONG_MAX + (-47) would occur with k=2.
//
//

#include <string>
#include <algorithm>
#include <cassert>
#include <cmath>
#include <limits.h>


class Solution {
public:
    // kMirrored check function
    bool isPalindrome(const std::string &s) {
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

    void updateKNumberOld(std::string &kNumberOld, const int &k) {
        if (kNumberOld == "") {
            kNumberOld = "1";
            return;
        }

        int leading = 1;
        for (std::string::iterator itEnd = kNumberOld.end() - 1; itEnd >= kNumberOld.begin(); --itEnd) {
            if (*itEnd - '0' == (k-1)) {
                *itEnd = '0';
            }
            else {
                *itEnd = '0' + ((*itEnd - '0')+1);
                leading = 0; // REDUNDANT BUT SAFETY
                return;
            }
        }
        if (leading) kNumberOld = to_string(leading) + kNumberOld; // IF CHECK IS REDUNDANT BUT SAFETY
    }

    const bool toBaseTen(const std::string &kNumber, long long &baseTenLong, const int &k) {
        // long mulpitplier = 1; Not needed if we go from left to right
        for (const char &c : kNumber) {
            int temp = c - '0';
            if (temp < 0 || temp >= k || (baseTenLong > (LLONG_MAX - temp) / k)) return false; // 0.-1. Checks validity of char in base 2. Checks that no overflow would occur
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
        std::string kNumberOld = "";
        int kNumberNew = 1;
        
        // 1. k - base string creator
        while (counter < n) {
            const std::string kNumber = kNumberOld + to_string(kNumberNew);
            
            // 2. k - Mirror check
            if (kNumber.length() == 1) {
                result += kNumberNew;
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

            if (++kNumberNew >= k) {
                updateKNumberOld(kNumberOld, k); // We need an update function to update kNumberOld
                kNumberNew = 1;
            }
        }

        return result;
    }
};
