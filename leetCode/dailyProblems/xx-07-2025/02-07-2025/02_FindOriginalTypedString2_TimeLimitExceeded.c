// This code is correct but exceeds the time limit. 
// It uses Dynamic Programming and in-place adaption of the solution array for best memory usage.
//
//

#include <stdlib.h>
#include <limits.h>

static int MOD = 1e9 + 7;

int possibleStringCount(const char* word, const int k) {
    const int wordSize = strlen(word);
    if (wordSize < k) return 0;
    // 1. if strlen(word) < k she typed wrongly, return 0.
    // 2. count occurences in a character array and count original amount in an int.
    // 3. It is a choose (amount) problem. I think strlen(word)!

    // Notes: She wanted to type at least k characters. Possible combinations 

    int distinct = 1;
    for (size_t i = 1; i < wordSize; ++i) {
        if (word[i] != word[i-1]) ++distinct;
    }

    int curPos = 0;
    int count = 1;
    int *runLen = malloc(distinct * sizeof(*runLen));
    for (size_t i = 1; i < wordSize; ++i) {
        if (word[i] != word[i-1]) {
            runLen[curPos++] = count;
            count = 1;
            continue;
        }
        ++count;
    } runLen[curPos] = count;

    // -------- CODE ABOVE FILLS RUNLEN ARRAY CORRECTLY ---------

    int *dpArray = calloc(wordSize+1, sizeof(*dpArray));
    dpArray[0] = 1;
    int startPos = 0;
    int endPos = 0;
    // FILL DP ARRAY
    for (int runLenIterator = 0; runLenIterator < distinct; ++runLenIterator) {
        ++startPos;
        endPos += runLen[runLenIterator];
        for (int dpStepIterator = endPos; dpStepIterator >= startPos; --dpStepIterator) {
            int tempSum = 0;
            for (int dpSumIterator = dpStepIterator-1; dpSumIterator > dpStepIterator-1-runLen[runLenIterator]; --dpSumIterator) { 
                // TODO: Find out how to do efficient check of overflow for size_t
                tempSum += dpArray[dpSumIterator] % MOD;
                tempSum %= MOD;
                if (dpSumIterator <= startPos-1) break; // Previous sequence had to have at least startPos-1 elements
            }
            dpArray[dpStepIterator] = tempSum % MOD;
        }
    }
    // DP ARRAY FILLED, RUNLEN NOT NEEDED ANYMORE
    free(runLen);
    
    int result = 0;
    // FOR LOOP OVER DP ARRAY TO ADD ALL >= K LENGTH
    for (size_t resSumIterator = k; resSumIterator < wordSize+1; ++resSumIterator) {
        result += dpArray[resSumIterator] % MOD;
        result %= MOD;
    }
    // RESULT IS SAVED, DP NOT NEEDED ANYMORE
    free(dpArray);

    return result;
}

