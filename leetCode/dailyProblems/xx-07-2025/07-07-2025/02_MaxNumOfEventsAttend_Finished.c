// There was one ++eventIt; too much. It cost me multiple hours by now.
// Create testcases from now on and use gdb if needed.
//
//

#include <stdlib.h>
#include <stdbool.h>

int* minHeapCreate(size_t HEAP_SIZE) {
    int* minHeap = calloc(HEAP_SIZE, sizeof(*minHeap)); // check for problem during initialisation
    return minHeap; 
}

void minHeapFree(int* minHeap) {
    free(minHeap);
    return;
}
    
const bool minHeapContains(int* minHeap) {
    return *minHeap;
}

const int minHeapPeek(int* minHeap) {
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
                //if (minHeap[left] == 0) printf("DANGER - LEFT: %d", minHeap[left]);
                minimum = left;
            }
            break;
        }
        case 1: {
            if (minHeap[left] < minHeap[pos]) {
                //if (minHeap[left] == 0) printf("DANGER - LEFT: %d", minHeap[left]);
                minimum = left;
            }
            if (minHeap[right] < minHeap[minimum]) {
                //if (minHeap[right] == 0) printf("DANGER - RIGHT: %d", minHeap[right]);
                minimum = right;
            }
            break;
        }
        default: return;
    }
    //if (minHeap[pos] == 0) printf("DANGER - POS: %d", minHeap[pos]);
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
    //printf("insert: %d, minHeap[0]: %d\n", value, minHeap[0]);
    return;
}

// pop min element and restore heap condition
int minHeapPop(int* minHeap, const int currHeapSize) {
    int minInt = minHeap[0];
    minHeap[0] = minHeap[currHeapSize-1];
    minHeap[currHeapSize-1] = 0;

    //if (minInt == 0) printf("DANGER - WE ARE POPPIN: %d", minInt);
    if (currHeapSize <= 2) return minInt;
    
    minHeapRestoreDown(minHeap, currHeapSize-1, 0);
    return minInt;
}

int cmpEvents0(const void* a, const void* b) {
    const int* const * eA = a;
    const int* const * eB = b;
    return (*eA[0] > *eB[0]) - (*eA[0] < *eB[0]);
}

int cmpEvents1(const void* a, const void* b) {
    const int* const * eA = a;
    const int* const * eB = b;
    return ((*eA)[1] > (*eB)[1]) - ((*eA)[1] < (*eB)[1]);
}

int maxEvents(int** events, const int eventsSize, int* eventsColSize) {
    if (eventsSize == 1) return 1;
    qsort(events, eventsSize, sizeof(*events), cmpEvents0);

    printf("[");
    for (size_t it = 0; it < eventsSize-1; ++it) {
        printf("[%d,%d],", events[it][0], events[it][1]);
    }
    printf("[%d,%d]]\n", events[eventsSize-1][0], events[eventsSize-1][1]);

    int currLength = 1;
    int currStart = 0;
    for (size_t eventsIt = 0; eventsIt < eventsSize-1;) {
        if (events[eventsIt][0] == events[++eventsIt][0]) ++currLength;
        else {
            // Sort first part
            if (currLength > 1) qsort(events+currStart, currLength, sizeof(*events), cmpEvents1);
            // Finished sort
            currLength = 1;
            currStart = eventsIt;
        }
    }
    if (currLength > 1) qsort(events+currStart, eventsSize-currStart, sizeof(*events), cmpEvents1);

    // Min Heap work starts:
    int* minHeap = minHeapCreate(eventsSize);
    int currHeapSize = 0;
    int currDay = events[0][0];
    int result = 0;
    size_t eventsIt = 0;

    while (eventsIt < eventsSize || currHeapSize) {
        // Add all events from today
        while (eventsIt < eventsSize && currDay == events[eventsIt][0]) 
            minHeapInsert(minHeap, currHeapSize++, events[eventsIt++][1]);

        // Remove all Events that are too old
        while (currHeapSize && minHeapPeek(minHeap) < currDay)
            minHeapPop(minHeap, currHeapSize--);

        // We attend one event today
        if (currHeapSize) {
            minHeapPop(minHeap, currHeapSize--);
            ++result;
            ++currDay;
        } else if (eventsIt < eventsSize) { // If heap is empty then we jump to next event start day
            currDay = events[eventsIt][0];
        }
    }



    printf("[");
    for (size_t it = 0; it < eventsSize-1; ++it) {
        printf("[%d,%d],", events[it][0], events[it][1]);
    }
    printf("[%d,%d]]", events[eventsSize-1][0], events[eventsSize-1][1]);

    minHeapFree(minHeap);
    return result;
}
