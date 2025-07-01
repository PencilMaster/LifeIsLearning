// Very easy problem where a persons finger stays too long on a key sometimes and we have to figure out
// in how many ways she could have meant to type the message.
// -> Count all additional characters after each appearance + 1 in the end.
//
//

#include <string.h>
#include <assert.h>

int possibleStringCount(char* word) {
    // We can also just save strlen(word) and let for loop iterate that amount of times.
    assert(word[strlen(word)] == '\0' && "word string is not terminated by \\0");
    int count = 1;

    for (size_t i = 1; word[i] != '\0'; ++i) {
        if (word[i-1] != word[i]) continue;
        ++count;
    }

    return count;
}
