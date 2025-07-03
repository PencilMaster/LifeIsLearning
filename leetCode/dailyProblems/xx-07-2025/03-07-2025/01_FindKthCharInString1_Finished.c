// This was an easy problem.
//
// 1. Malloc a char array with size k
// 2. Set first entry = 'a'
// 3. Build Array up until k-th entry (word[k-1])
//    Here the rule is, we increment the existing characters ASCII by 1 and add at the end ('z' -> 'a')
// 4. return word[k-1]
//

#include <stdlib.h>

char kthCharacter(int k) {
    // Greedy
    // Build vector modulo 26 for values (do if (== 'z') -> = 'a')

    char* word = malloc(k * sizeof(*word));
    word[0] = 'a';
    int length = 1;
    for (int wordIterator = 1; wordIterator < k; ++wordIterator, --length) {
        if (length == 0) length = wordIterator;

        if (word[wordIterator-length] == 'z') word[wordIterator] = 'a';
        else word[wordIterator] = word[wordIterator-length]+1;
    }

    return word[k-1];
}
