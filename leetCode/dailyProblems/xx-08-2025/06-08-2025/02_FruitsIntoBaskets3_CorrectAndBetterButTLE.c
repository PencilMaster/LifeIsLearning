// It remembers which ones for sure won't fit into any basket, 
// but it still fails when we have fruits == baskets and both are extremely large

#include <stddef.h>
#include <limits.h>

int numOfUnplacedFruits(int* fruits, int fruitsSize, int* baskets, int basketsSize) {
    assert(fruitsSize == basketsSize && "fruits and baskets do not have the same size");
    // Greedym from left to right through fruits, we skip any smaller basket sizes
    // If greater equal, then ++count, basket = 0 and continue; else just continue to next fruit

    int count = 0;
    size_t fruitsIt = 0;

    int currentMin = INT_MAX;

    for (int* fruitsAccess = fruits; fruitsIt < fruitsSize; ++fruitsIt, ++fruitsAccess) {
        if (*fruitsAccess >= currentMin) continue;
        int* basketsAccess = baskets;
        size_t basketsIt = 0;
        while (basketsIt < basketsSize && *fruitsAccess > *basketsAccess) {++basketsIt; ++basketsAccess;}
        if (basketsIt >= basketsSize) {
            currentMin = (*fruitsAccess < currentMin ? *fruitsAccess : currentMin);
            continue;
            }
        else {
            *basketsAccess = 0;
            ++count;
        }
    }

    return fruitsSize - count;
}
