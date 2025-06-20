// This is the code I wrote for the specification. it pased all cases:
// 
// Only annoying thing: libraries do not need to be included on leetcode.

#include <stdlib.h>
#include <stdbool.h>

char* generateTag(char* caption) {
    // 1. We walk over cation and copy it out into another string
    // 2. We rewrite caption for the length of tempCaption and add a '\0' afterwards

    char* tempCaption = (char *)malloc(sizeof(char) * 101);
    bool big = false;

    size_t it = 0;
    while (caption[it] == ' ') {++it;}
    
    tempCaption[0] = (char) 35;
    size_t tempIterator = 1;
    for (; tempIterator < 100 && caption[it] != '\0'; ++tempIterator, ++it) {
        while (caption[it] != '\0' && caption[it] == ' ') {
            big = true;
            ++it;
        }
        if (caption[it] == '\0') break;
        else if (!big && caption[it] != ' ') {
            if (caption[it] >= 97) tempCaption[tempIterator] = caption[it];
            else tempCaption[tempIterator] = caption[it] + 32;
            continue;
        }
        else if (big && caption[it] != ' ') {
            if (caption[it] <= 90) tempCaption[tempIterator] = caption[it];
            else tempCaption[tempIterator] = caption[it] - 32;
            big = false;
        }
    }
    tempCaption[tempIterator] = '\0';

    return tempCaption;
}
