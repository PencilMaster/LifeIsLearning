// This code exceeds the time limit. Need to check termination of while loop again to make sure it terminates.
// -- It worked already before but first I only decremented the top and did not adjust according to the median.
//
// SECOND APPROACH would be memoization with DP but the current program seems very close to the solution.
// 

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int median(int* charCount) {
    int medianCount = 0;
    int nonZeros = 0;
    for (size_t i = 0; i < 26; ++i) {
        if (charCount[i] != 0 && ++nonZeros) medianCount += charCount[i];
    }
    return (int)(medianCount / nonZeros);
}

size_t posInArray(int* charCount, char* comp) {
    size_t position = 0;
    if (!strcmp(comp, "max"))
        for (size_t i = 1; i < 26; ++i) {
            if (charCount[i] > charCount[position]) position = i;
        }
    else if (!strcmp(comp, "min"))
        for (size_t i = 1; i < 26; ++i) {
            if (charCount[i] != 0 && charCount[i] < charCount[position]) position = i;
        }
    else {} //Would write to console or a log-file that a non-allowed comparison operator was given

    return position;
}


int minimumDeletions(char* word, int k) {
    int* charCount = calloc(26, sizeof(int));

    // We fill the integer array with amount of times a character appears
    size_t wordLength = strlen(word);
    if (wordLength == 1) return 0; // Resolve easy case immediately

    for (size_t wordIterator = 0; wordIterator < wordLength; ++wordIterator) {
        ++charCount[word[wordIterator]-97];
    }

    // This step needs fixing. Need smart decrement either from top or bottom
    // My thought process going in was truly flawed. Feels like I need another approach. 
    // Seems like a good problem to solve mathematically first and then implement an iterative minimisation formula
    
    int count = 0;
    int max = charCount[posInArray(charCount, "max")];
    int min = charCount[posInArray(charCount, "min")];
    while (max - min > k) {
        if (max - min <= median(charCount)) {
            --charCount[posInArray(charCount, "min")];
            min = charCount[posInArray(charCount, "min")];
        }
        else {
            --charCount[posInArray(charCount, "max")];
            max = charCount[posInArray(charCount, "max")];
        }
        ++count;
    }

    return count;
}



// To print out charCount array for testing
// for (size_t wordIterator = 0; wordIterator < 26; ++wordIterator) {
//     printf("%d", charCount[wordIterator]);
// } // THIS WORKS PERFECT

// printf("\n sizeof(charCount) : %d", (sizeof(charCount) / sizeof(int))); Tried to calculate array size but it does (pointer size / int size) which is 8 / 4 = 2

// printf("\n%s : %d", "max", posInArray(charCount, "max"));
// printf("\n%s : %d", "min", posInArray(charCount, "min"));
