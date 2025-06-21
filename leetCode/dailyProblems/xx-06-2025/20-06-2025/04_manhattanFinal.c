// This is the final code.
//
// It works flawlessly. I re-wrote it in 15 minutes after lunch. This is not that hard.
//
// Learning:
// -- Write down problem and fracture it into subproblems.
// -- Are subproblems case distinctions we can solve in each step iteratively (/ first greedy)?
//
// Code:

#include <stdlib.h>
#include <stdbool.h>

struct Coordinate {
    int x;
    int y;
    int maxDistance;
} Coordinate_default = {0, 0, 0};


int maxDistance(char* s, int k) {

    // We create a pair that keeps track of decrementing between two measured maxDistances.
    struct Coordinate point = Coordinate_default;

    // This bool tracks whether maxDistance was changed in current iteration
    bool changed = false;
    // This int tracks possible betterments between new maxDistances
    int allowedChanges = k;
    int currentLevel = 0; // tracks currentLevel when adjustments are not allowed anymore
    int tempMaxLevel = 0; // tracks max distance in not allowed space

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
        if (allowedChanges && oldDistance > newDistance) {
            ++point.maxDistance;
            --allowedChanges;
        }
        else if (oldDistance > newDistance) {
            --currentLevel;
        }
        else if (allowedChanges && oldDistance < newDistance) {
            ++point.maxDistance;
        }
        else if (oldDistance < newDistance) {
            if (++currentLevel > tempMaxLevel) tempMaxLevel = currentLevel;
        }
    }
    point.maxDistance += tempMaxLevel;
    return point.maxDistance;
}

