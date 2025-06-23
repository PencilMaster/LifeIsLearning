// With this approach we pass 91 / 97 tests. It is almost up to par. We need to inhibit the 
// generation of a couple of palindromes with base 10, that can not be palindromes in base k.
// Without generating and checking them -> math.
//

#include <algorithm>
#include <string>

//#include <iostream> DEBUGGING

class Solution {
public:

    // If parity is false -> odd, then cut right most digit and flip it. Append it.
    long long createPalindrome(const int &number, const bool &parity) {
        const std::string left = to_string(number);
        std::string right = left;
        if (!parity) right.pop_back();
        reverse(right.begin(), right.end());
        return std::stoll(left + right);
    }

    const std::string toBaseK(const long long &baseTenPal, const int &k) {
        long long toDivide = baseTenPal;
        int maxVal = k;
        std::string result = "";

        while (toDivide > 0) {
            result = to_string(toDivide % k) + result;
            toDivide /= k;
        }

        return result;
    }

    const bool isPalindrome(const std::string str) {
        for (size_t l = 0, r = str.size()-1; l < r; ++l, --r) 
            if (str[l] != str[r]) return false;
        return true;
    }

    long long kMirror(int k, int n) {
         // Just to make sure that invariant is followed.
        assert(n > 0 && k > 1 && "At least one input parameter is too low");

        int counter = 0;
        long long result = 0;

        int palLength = 1;
        bool parity = false; //Odd palindrome length = false, even = true;
        while (counter < n) {
            // We want to create string left, reverse it for string right.
            // Then left + right = palindrome. How to only create palindromes in order.
            for (int i = 1 * palLength; i < palLength * 10; ++i) {
                const long long baseTenPal = createPalindrome(i, parity);
                if (isPalindrome(toBaseK(baseTenPal, k))) {
                    result += baseTenPal;
                    if (++counter >= n) return result;
                }
            }
            
            if (parity) palLength *= 10;
            parity = !parity;

        }
        return result;
    }
};
