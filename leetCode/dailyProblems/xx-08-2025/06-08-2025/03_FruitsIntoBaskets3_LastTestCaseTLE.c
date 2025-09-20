// Here only the testcase which has inverted fruits and baskets sequence, gets a TLE
//

#include <stddef.h>
#include <limits.h>

int numOfUnplacedFruits(int* fruits, int fruitsSize, int* baskets, int basketsSize) {
    assert(fruitsSize == basketsSize && "fruits and baskets do not have the same size");
    // Greedym from left to right through fruits, we skip any smaller basket sizes
    // If greater equal, then ++count, basket = 0 and continue; else just continue to next fruit

    int count = 0;
    size_t fruitsIt = 0;

    int currentMin = INT_MAX;
    unsigned int similar = 1;

    for (int* fruitsAccess = fruits; fruitsIt < fruitsSize; ++fruitsIt, ++fruitsAccess) {
        if (*fruitsAccess >= currentMin) continue;

        if (fruitsIt < (fruitsSize - 1) && *fruitsAccess == *(fruitsAccess+1)) {
            ++similar;
            continue;
        } 

        int* basketsAccess = baskets;
        size_t basketsIt = 0;
        unsigned int found = 0;

        while (basketsIt < basketsSize && found < similar) {
            if (*basketsAccess >= *fruitsAccess) {
                *basketsAccess = 0;
                ++found;
            }
            ++basketsIt; ++basketsAccess;
        }

        count += found;
        similar = 1;

        if (basketsIt >= basketsSize) {
            currentMin = (*fruitsAccess < currentMin ? *fruitsAccess : currentMin);
            continue;
        }
    }

    return fruitsSize - count;
}
