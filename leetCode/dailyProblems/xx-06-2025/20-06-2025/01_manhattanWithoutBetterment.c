// The following Code is the first trial run I did. It only computes the maximum Manhattan Distance without changing k directions yet.
//
/*

#include <stdlib.h>

typedef struct Coordinate {
    int x;
    int y;
    int maxDistance;
    int maxBetterment;
};

int maxDistance(char* s, int k) {

    // We create a pair that keeps track of decrementing between two measured maxDistances.
    struct Coordinate point = {0, 0, 0};

    // This bool tracks whether maxDistance was changed in current iteration
    bool changed = false;
    // This int tracks possible betterments between new maxDistances
    int possibleBetterment = 0;

    for (char* iterator = s; *iterator != '\0'; ++iterator) {
        int oldDistance = abs(point.x) + abs(point.y);

        switch (*iterator) {
            case 'N':
                ++point.y;
                break;
            case 'W':
                --point.x;
                break;
            case 'S':
                --point.y;
                break;
            case 'E':
                ++point.x;
                break;
            default: 
                return 0;
        }

        int newDistance = abs(point.x) + abs(point.y);
        if (oldDistance > newDistance) ++possibleBetterment;
        point.maxDistance = (newDistance > point.maxDistance) ? (changed = true, newDistance) : (point.maxDistance);

        if (changed) point.maxBetterment = (point.maxBetterment > possibleBetterment) ? (possibleBetterment = 0, point.maxBetterment) : (possibleBetterment = 0, possibleBetterment);
    }

    // Now we will check for 
    return point.maxDistance;
}

*/
