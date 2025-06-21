// This is the Code I have which passes 790 / 828 test cases. But there are fundamental issues inside.
// We can just do it ireatively. The whole iteration part I tried to implement at the end, could be done during
// the first walkthrough, which already makes it work in O(n) -- Note this for future reference
//  
// Learnings: 1. Struct with constructor - could have also typedefed or defined it as a macro, s.t. we don't have to call it explicitly;

/*
#include <stdlib.h>
typedef struct {
    int x;
    int y;
    char* iterator;
} MaxPoint;

typedef struct {
    int x;
    int y;
    int maxDistance;
    int maxBetterment;
    int resultingDistance;
    MaxPoint maxPoint;
} Coordinate;

Coordinate Coordinate_default() {
    Coordinate c;
    c.x = 0;
    c.y = 0;
    c.maxDistance = 0;
    c.maxBetterment = 0;
    c.resultingDistance = 0;
    c.maxPoint.x = c.x;
    c.maxPoint.y = c.y;
    c.maxPoint.iterator = NULL;
    return c;
}

void adjustMaxPoint(MaxPoint* m, char* iterator, int x, int y) {
    m->iterator = iterator;
    m->x = x;
    m->y = y;
    return;
}

int maxDistance(char* s, int k) {

    // We create a pair that keeps track of decrementing between two measured maxDistances.
    Coordinate point = Coordinate_default();

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
        point.maxDistance = (newDistance >= point.maxDistance) ? (changed = true, newDistance) : (point.maxDistance);

        if (changed) {
            adjustMaxPoint(&(point.maxPoint), iterator, point.x, point.y);
            point.maxBetterment += possibleBetterment;
            possibleBetterment = 0;
            changed = false;
        }
    }

    // Now we will adjust the max Manhattan Distance with betterments
    int possibleChangeCount = k;
    int timesTwo = (possibleChangeCount > point.maxBetterment) ? (possibleChangeCount -= point.maxBetterment, point.maxBetterment) : (possibleChangeCount);
    point.resultingDistance += point.maxDistance + 2 * timesTwo;
    if (timesTwo == k) return point.resultingDistance;

    //Iterate over last movements after last maxDistance, adjust all possible betterments
    int oldDistance = point.maxDistance;
    int currDistFromMax = 0;
    int maxDistFromMax = 0;
    // Since "bool changed" is always false at this point in the code, we reuse it
    for (char* iterator = point.maxPoint.iterator+1; *iterator != '\0' && possibleChangeCount > 0; ++iterator) {
        switch (*iterator) {
            case 'N':
                ++point.maxPoint.y;
                break;
            case 'W':
                --point.maxPoint.x;
                break;
            case 'S':
                --point.maxPoint.y;
                break;
            case 'E':
                ++point.maxPoint.x;
                break;
            default: 
                return 0;
        }
        
        int newDistance = abs(point.maxPoint.x) + abs(point.maxPoint.y);
        if (!changed && oldDistance > newDistance) {
            --possibleChangeCount;
            ++point.maxPoint.iterator;
            ++point.maxDistance;
            ++point.resultingDistance;
            if (!possibleChangeCount) changed = true;
        }
        else if (!changed || oldDistance < newDistance) {
            ++point.maxPoint.iterator;
            ++point.maxDistance;
            ++point.resultingDistance;
        }
        else {
            if (oldDistance > newDistance) --currDistFromMax;
            else if (oldDistance < newDistance) ++currDistFromMax;
            
            if (currDistFromMax > maxDistFromMax) {
                maxDistFromMax = currDistFromMax;
                point.maxPoint.iterator = iterator;
            }
        }
        oldDistance = newDistance; // oldDistance becomes newDistance in preparation for next loop
    }

    point.maxDistance += maxDistFromMax;
    point.resultingDistance += maxDistFromMax;

    return point.resultingDistance;
}
*/
