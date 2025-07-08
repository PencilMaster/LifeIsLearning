// Here I implemented a minHeap during a leetCode exercise.
// Copy of my code without the exercise elements.
//

#include <stdlib.h>
#include <stdbool.h>

int* minHeapCreate(const size_t HEAP_SIZE) {
    int* minHeap = calloc(HEAP_SIZE, sizeof(*minHeap)); // check for problem during initialisation
    return minHeap; 
}

void minHeapFree(int* minHeap) {
    free(minHeap);
    return;
}
    
const bool minHeapContains(const int* minHeap) {
    return *minHeap;
}

const int minHeapPeek(const int* minHeap) {
    return minHeap[0];
}

void heapSwap(int* const a, int* const b) {
    int temp = *a;
    *a = *b;
    *b = temp;
    return;
}


// Restore down until left child == 0;
void minHeapRestoreDown(int* minHeap, const int currHeapSize, const int pos) {
    int left = pos*2+1;
    int right = pos*2+2;
    int minimum = pos;
    int children = ((currHeapSize - right < 0) ? -1 : ((currHeapSize - right == 0) ? 0 : 1)); // when right is OOB then children == 0, when left is OOB then children < 0
    switch(children) {
        case -1: return;

        case 0: {
            if (minHeap[left] < minHeap[pos]) {
                minimum = left;
            }
            break;
        }
        case 1: {
            if (minHeap[left] < minHeap[pos]) {
                minimum = left;
            }
            if (minHeap[right] < minHeap[minimum]) {
                minimum = right;
            }
            break;
        }
        default: return;
    }
    if (minimum == pos) return;
    heapSwap(minHeap+(minimum), minHeap+pos);
    minHeapRestoreDown(minHeap, currHeapSize, minimum);
    return;
}

// Restore up until pos == 0 or no switch;
void minHeapRestoreUp(int* minHeap, const int pos) {
    if (pos > 0 && minHeap[(pos-1)/2] > minHeap[pos]) {
        heapSwap(minHeap+((pos-1)/2), minHeap+pos);
        minHeapRestoreUp(minHeap, (pos-1)/2);
    }
    return;
}

// Add at the end and call minHeapRestoreUp()
void minHeapInsert(int* minHeap, const int currHeapSize, const int value) {
    minHeap[currHeapSize] = value;
    minHeapRestoreUp(minHeap, currHeapSize);
    return;
}

// pop min element and restore heap condition
int minHeapPop(int* minHeap, const int currHeapSize) {
    int minInt = minHeap[0];
    minHeap[0] = minHeap[currHeapSize-1];
    minHeap[currHeapSize-1] = 0;

    if (currHeapSize <= 2) return minInt;
    
    minHeapRestoreDown(minHeap, currHeapSize-1, 0);
    return minInt;
}

// ================================== LEETCODE SPECIFIC ANSWER BELOW ===========================
// int cmpEvents0(const void* a, const void* b) {
//     const int* const * const eA = a;
//     const int* const * const eB = b;
//     return (*eA[0] > *eB[0]) - (*eA[0] < *eB[0]);
// }
// 
// int cmpEvents1(const void* a, const void* b) {
//     const int* const * const eA = a;
//     const int* const * const eB = b;
//     return ((*eA)[1] > (*eB)[1]) - ((*eA)[1] < (*eB)[1]);
// }
// 
// int maxEvents(int** events, const int eventsSize, int* eventsColSize) {
//     if (eventsSize == 1) return 1;
//     qsort(events, eventsSize, sizeof(*events), cmpEvents0);
// 
//     int currLength = 1;
//     int currStart = 0;
//     for (size_t eventsIt = 0; eventsIt < eventsSize-1;) {
//         if (events[eventsIt][0] == events[++eventsIt][0]) ++currLength;
//         else {
//             // Sort first part
//             if (currLength > 1) qsort(events+currStart, currLength, sizeof(*events), cmpEvents1);
//             // Finished sort
//             currLength = 1;
//             currStart = eventsIt;
//         }
//     }
//     if (currLength > 1) qsort(events+currStart, eventsSize-currStart, sizeof(*events), cmpEvents1);
// 
//     // Min Heap work starts:
//     int* minHeap = minHeapCreate(eventsSize);
//     int currHeapSize = 0;
//     int currDay = events[0][0];
//     int result = 0;
//     size_t eventsIt = 0;
// 
//     while (eventsIt < eventsSize || currHeapSize) {
//         // Add all events from today
//         while (eventsIt < eventsSize && currDay == events[eventsIt][0]) 
//             minHeapInsert(minHeap, currHeapSize++, events[eventsIt++][1]);
// 
//         // Remove all Events that are too old
//         while (currHeapSize && minHeapPeek(minHeap) < currDay)
//             minHeapPop(minHeap, currHeapSize--);
// 
//         // We attend one event today
//         if (currHeapSize) {
//             minHeapPop(minHeap, currHeapSize--);
//             ++result;
//             ++currDay;
//         } else if (eventsIt < eventsSize) { // If heap is empty then we jump to next event start day
//             currDay = events[eventsIt][0];
//         }
//     }
// 
//     minHeapFree(minHeap);
//     return result;
// }
