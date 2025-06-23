// Hard and long problem. Since memory handling is easier in C++, I am writing it in C++ first.
//
// Current problem in Code:
// 1. std::pow(k, i) returns a long double, which becomes way longer 
//    than a LLONG_MAX and casting it to a long long creates an overflow.
//    Even with a static_cast<long long>(std::pow(k, i)), this is not fixable.


#include <string>
#include <algorithm>
#include <cassert>
#include <cmath>


class Solution {
public:
    // kMirrored check function
    bool kMirroredCheck(const std::string s) {
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
                bool kMirrored = kMirroredCheck(kNumber);

                // 3. Convert to base 10 and check it's mirroring
                if (kMirrored) {
                    long long baseTenLong = 0;
                    std::string::const_iterator itTenBack = kNumber.end();
                    for (size_t i = 0; i < kNumber.length(); ++i) {
                        baseTenLong += std::pow(k, i) * (*(--itTenBack) - '0'); // Error because pow creates a long double that is way longer than LLONG_MAX and casting it creates an overflow.
                    }
                    if (kMirroredCheck(to_string(baseTenLong))) {
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
