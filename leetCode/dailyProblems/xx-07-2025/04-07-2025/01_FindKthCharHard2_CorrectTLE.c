// This is a hard version of the previous task. Currently the code is correct but not efficient enough, it gives a time limit exceeded error.
//
//

#include <stdlib.h>

char kthCharacter(long long k, int* operations, int operationsSize) {
    // assert(operationsSize > 0 && "OperationsSize needs to be positive");

    // Greedy
    // Build vector modulo 26 for values (do if (== 'z') -> = 'a')

    char* word = malloc(k * sizeof(*word));
    word[0] = 'a';

    long long length = 1, wordLength = 1;
    int* curOperation = operations;

    for (long long wordIterator = 1; wordIterator < k; ++wordIterator, --length) {
        if (length == 0) { // Here we update length && operation for next block
            length = wordIterator; // temp
            wordLength = wordIterator;
            ++curOperation;
        }

        if (*curOperation) {
            if (word[wordLength-length] == 'z') word[wordIterator] = 'a';
            else word[wordIterator] = word[wordLength-length]+1;
        }
        else {
            word[wordIterator] = word[wordLength-length];
        }
    }
    char result = word[k-1];
    free(word);
    return result;
}
