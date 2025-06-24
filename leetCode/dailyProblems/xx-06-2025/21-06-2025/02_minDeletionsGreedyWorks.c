//  This Code works... I reframed my mind. Since the frequency counting was already right,
//  I needed to fix the minimisation steps. Greedy = try out all possible values the median could take
//  and allow a deviation of values up to "+ k"
//


#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <limits.h>

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
        ++charCount[word[wordIterator] - 'a'];
    }

    int minDel = INT_MAX;
    int max = charCount[posInArray(charCount, "max")];
    int min = charCount[posInArray(charCount, "min")];
    if (max == min) return 0;
    for (int i = 0; i < max+1; ++i) {
        int deletes = 0;
        for (int j = 0; j < 26; ++j) {

            if (charCount[j] < i) {
                deletes += charCount[j];
            }
            else if (charCount[j] > i + k) {
                deletes += charCount[j] - (i + k);
            }   
        
        }
        if (deletes < minDel) minDel = deletes;
    }

    return minDel;
}



// To print out charCount array for testing
// for (size_t wordIterator = 0; wordIterator < 26; ++wordIterator) {
//     printf("%d", charCount[wordIterator]);
// } // THIS WORKS PERFECT

// printf("\n sizeof(charCount) : %d", (sizeof(charCount) / sizeof(int))); Tried to calculate array size but it does (pointer size / int size) which is 8 / 4 = 2

// printf("\n%s : %d", "max", posInArray(charCount, "max"));
// printf("\n%s : %d", "min", posInArray(charCount, "min"));
