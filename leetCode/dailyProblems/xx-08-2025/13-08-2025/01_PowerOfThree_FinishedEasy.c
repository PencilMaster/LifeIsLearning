// Added little solution that checks if something is a power of three
//

#include <stdbool.h>

bool isPowerOfThree(int n) {
    long counter = 1;
    while (counter < n) counter *= 3;
    if (counter == n) return true;
    return false;
}
