// This code has memory initialisation and access issues. Was written pretty quickly and switched to the C++ approach afterwards. Will finish after a good nights sleep though.
//

#include <stdlib.h>
#include <string.h>

/**
 * Note: The returned array must be malloced, assume caller calls free().
 */
char** divideString(char* s, int k, char fill, int* returnSize) {
    size_t strLength = strlen(s);

    // Iterate over whole string and at the end att fill for modulo size
    size_t outerArray = 0;
    size_t innerArray = 0;
    *returnSize = (strLength + k-1) / k;
    char** returnString = malloc(((strLength + k-1) / k) * sizeof(char*));
    returnString[0] = malloc(k * sizeof(char));
    
    for (size_t i = 0; i < strLength; ++i) {
        returnString[outerArray][innerArray++] = s[i];
        if (innerArray == k) {
            ++outerArray;
            returnString[outerArray] = malloc((k+1) * sizeof(char));
            innerArray = 0;
        }
    }

    while (innerArray < k) {
        returnString[outerArray][innerArray++] = fill;
    }

    return returnString;
}
