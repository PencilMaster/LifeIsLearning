// I finished debugging my C approach. Honestly doing it again, 
// I would loop over groups (0, ... ,returnSize-1) in outer loop and do inner for loops:
//    for (int i = 0; i < k; ++i) returnString[group][i]((group * k) + i < strLength) ? (s[(group * k) + i]) : (fill);
//

/**
 * Note: The returned array must be malloced, assume caller calls free().
 */
char** divideString(char* s, int k, char fill, int* returnSize) {
    size_t strLength = strlen(s);

    // Iterate over whole string and at the end att fill for modulo size
    int outerArray = 0;
    int innerArray = 0;
    *returnSize = (strLength + k-1) / k;

    char** returnString = malloc((*returnSize) * sizeof(*returnString));
    returnString[0] = malloc((k+1) * sizeof(**returnString));
    
    for (size_t i = 0; i < strLength; ++i) { 
        returnString[outerArray][innerArray++] = s[i];

        if (innerArray == k) {
            returnString[outerArray++][innerArray] = '\0';
            if (i == strLength-1) break;
            returnString[outerArray] = malloc((k+1) * sizeof(**returnString));
            innerArray = 0;
        }
    }

    bool filled = false;
    while (innerArray < k) {
        returnString[outerArray][innerArray++] = fill;
        filled = true;
    }
    if (filled) returnString[outerArray][k] = '\0';

    return returnString;
}
