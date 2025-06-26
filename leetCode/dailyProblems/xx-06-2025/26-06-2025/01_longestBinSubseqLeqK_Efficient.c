// Todays problem was a bit tricky to debug in leetCode, but it worked out pretty well.
//
// Explanation:
//
// Intuition + Approach:
//     1. Create bitrepresentation of k (kBit).
//     2. We have 2 cases for length of string s and kBit (kLength).
//        a. strlen(s) < kLength.
//        b. strlen(s) >= kLength.
//     Case a: return strlen(s) since value(s) is for sure < k.
//     Case b: continue.
//     3. 2-Steps.
//        a. Count all '0' chars in strlen(s) - kLength part (save in count).
//        b. Compare s[strlen(s) - kLength, ... , strlen(s)-1] with kBit[0, ... , kLength-1].
//            Case (s[i] == '0' && k[i-strlen(s) - kLength] == '1') -> return count += strlen(s)-i;
//            Case (s[i] == '0' && k[i-strlen(s) - kLength] == '0') -> ++count;
//            Case (s[i] == '1' && k[i-strlen(s) - kLength] == '0') -> "delete" the '1' and return strlen(s) - iteratorEnd - 1; Since resulting string with deleted '1' will be for sure smaller than k.
//            Case (s[i] == '1' && k[i-strlen(s) - kLength] == '1') -> ++ count;
//
// Complexity (kLength = size of kBitString)
//
//     Time complexity: O(kLength + strlen(s))
//
//     Space complexity: O(kLength)

#include <limits.h>
#include <stdlib.h>
#include <stdio.h>

int longestSubsequence(const char* s, const int k) {

    // Find out bitLength for k;
    size_t calcKLength = 0; // Don't need space for '\0' character since we don't actually compute bin of k
    int upperBound = 1;
    while (calcKLength <= INT_MAX/2 && k >= upperBound) { // k=5, up= 2, c=1, up=4, c=2, up=8, c=3
        upperBound *= 2;
        ++calcKLength;
    }
    calcKLength = (k ? calcKLength : 1);
    const size_t kLength = calcKLength; // kLength is without '\0' included

    if (strlen(s) < kLength) return strlen(s);

    // Maybe not needed, we can already set all 1s to the left of strlen(s) - kLength-1 in s to zero. Endpart is in question. BitMask maybe?
        
    char* kBit = malloc((calcKLength+1) * sizeof(*kBit));
    int copyK = k;
    kBit[calcKLength] = '\0';
    while (calcKLength > 0) { 
        //printf("copyK = %d \n", copyK); //DEBUGGING
        if (copyK%2) 
            kBit[--calcKLength] = '1';
        else 
            kBit[--calcKLength] = '0';
        copyK /= 2;
    }

    //DEBUGGING dnoe in leetCode, sadly no gdb here
    // printf("k in Bit rep: = %s \n", kBit);
    // printf("kLength = %d \n", kLength);
    // printf("strlen = %d \n", strlen(s));
    
    size_t zeroPartLength = strlen(s) - kLength; 
    int longestSubseqLength = 0;
    for (size_t sIterator = 0; sIterator < zeroPartLength; ++sIterator) 
        if (s[sIterator] == '0') ++longestSubseqLength;

    // DEBUGGING
    // printf("zeroPartLength = %d \n", zeroPartLength);

    if (k == 0) {
        free(kBit);
        return longestSubseqLength;
    }

    for (size_t iteratorEnd = zeroPartLength; iteratorEnd < strlen(s); ++iteratorEnd) {
        if (s[iteratorEnd] == '0') {
            if (kBit[iteratorEnd - zeroPartLength] == '1') {
                // DEBUGGING
                // printf("iteratorEndPosition = %d \n", iteratorEnd);
                // printf("(int)(strlen(s) - iteratorEnd) = %d \n", (int)(strlen(s) - iteratorEnd));
                longestSubseqLength += (int)(strlen(s) - iteratorEnd); // We know per task that s and iterator won't overflow in conversion from size_t to int, but otherwise this needs to be checked!
                free(kBit);
                return longestSubseqLength; // We can can do calculation here in return well, but storing result and possibly handling a free() problem before returning seems more secure.
            }
            ++longestSubseqLength; // s='0', k='0'
            continue;
        }
        else if (kBit[iteratorEnd - zeroPartLength] == '0') { // s= '1', k= '0'
            longestSubseqLength += (int)(strlen(s) - iteratorEnd - 1);
            free(kBit);
            return longestSubseqLength;
        }
        else ++longestSubseqLength; // Both are '1'
    }

    free(kBit);
    return longestSubseqLength;
}

// Super nice solution from someone else, with my comment.
//
// Super elegant. we only count the 1s we can take from the right of s, for the whole length of k.
// Think: if (s[i] == '1') Can we still accomodate this one or will it get bigger than k then? -> If possible, ans++;
//
// #include <string.h>
//
// int longestSubsequence(char* s, int k) {
//     int n = strlen(s), ans = 0;
//     long long val = 0, pow = 1;
//
//     for (int i = n - 1; i >= 0; i--) {
//         if (s[i] == '0') ans++;
//         else if (pow <= k && val + pow <= k) {
//
//             val += pow;
//             ans++;
//         }
//         if (pow <= k) pow <<= 1;
//     }
//     return ans;
// }
