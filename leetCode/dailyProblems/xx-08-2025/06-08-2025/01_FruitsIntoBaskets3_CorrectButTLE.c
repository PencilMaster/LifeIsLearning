#include <stddef.h>

int numOfUnplacedFruits(int* fruits, int fruitsSize, int* baskets, int basketsSize) {
    assert(fruitsSize == basketsSize && "fruits and baskets do not have the same size");
    // Greedym from left to right through fruits, we skip any smaller basket sizes
    // If greater equal, then ++count, basket = 0 and continue; else just continue to next fruit

    int count = 0;
    size_t fruitsIt = 0;

    for (int* fruitsAccess = fruits; fruitsIt < fruitsSize; ++fruitsIt, ++fruitsAccess) {
        int* basketsAccess = baskets;
        size_t basketsIt = 0;
        while (basketsIt < basketsSize && *fruitsAccess > *basketsAccess) {++basketsIt; ++basketsAccess;}
        if (basketsIt >= basketsSize) continue;
        else {
            *basketsAccess = 0;
            ++count;
        }
    }

    return fruitsSize - count;
}
