// For now I implemented charCount and the static MOD variable with 
// which we will ensure that the returned result stays inside given length limits.
//
// Also right now I am implementing the extraction and implementation of sorted Array of occurrences Array
//

#include <stdlib.h>
#include <limit.h>

void fillAmount(int *, int *);
bool visited(int *, int *, int *, int *);

static int MOD = 1e9 + 7;

int possibleStringCount(char* word, int k) {
    int wordSize = strlen(word);
    if (wordSize < k) return 0;
    // 1. if strlen(word) < k she typed wrongly, return 0.
    // 2. count occurences in a character array and count original amount in an int.
    // 3. It is a choose (amount) problem. I think strlen(word)!

    // Notes: She wanted to type at least k characters. Possible combinations 

    int distinct = 1;
    int* charArray = malloc(26 * sizeof(*charArray));
    for (size_t i = 1; i < wordSize; ++i) {
        if (word[i] == word[i-1]) ++charArray[word[i] - 'a'];
        else ++distinct;
    }

    int* amountArray = malloc(distinct * sizeof(*amountArray));
    fillAmount(amountArray, distinct);


    // choose k - distinct out of charArray -> += choose k-distinct+1 -> ... -> until choose charArray
    free(charArray);
    return 0;
}

void fillAmount(int *amountArray, int *amountSize) {
    // Extract lowest value out of charArray;
    int lowVal = INT_MAX;
    int visitedSize = 0;
    for (size_t i = 0; i < *size; ++i)
        if (amountArray[i] != 0) ++visitedSize;

    int* visitedArray = malloc(visitedSize * sizeof(*visitedArray));
    for (size_t i = 0; i < *size; ++i) {
        if (amountArray[i] == 0 || visited(amountArray, amountSize, visitedArray, &visitedSize)) continue;
        else if (amountArray[i] < lowVal) lowVal = amountArray[i];
    }
    return;
}

bool visited(int *amountArray, int *amountSize, int *visitedArray, int *visitedSize) {

}
