// Optimised solution with three pointers, not needing a count variable.
//

char* makeFancyString(char* s) {
    char* first = s;
    char* mid = s+1;
    char* last = s+2;

    if (*first == '\0' || *mid == '\0' || *last == '\0') return s;

    for (;*last != '\0'; ++last) {
        if (*last == *mid && *mid == *first) continue;
        else *(first < mid? first += 2, first : (mid += 2, mid)) = *last;

    }

    *(first < mid? first += 2, first : (mid += 2, mid)) = '\0';
    return s;
}

// aaaabbbbaaaaa
// aaaabbbbaaaaa
