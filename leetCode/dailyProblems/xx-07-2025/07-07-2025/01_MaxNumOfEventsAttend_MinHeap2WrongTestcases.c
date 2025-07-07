// Here, I implemented a minimum Heap in c and store all enddates of events in the min heap.
// They are added in order of their start date, such that none are forgotten.
// They are also evaluated such that not two events chosen overlap.
//
// In testcase 44/45 we get 99999 instead of 99888. I could not detect any fault when analysing the heap itself.
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
    return *minHeap;
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
    int children = ((currHeapSize - right < 0) ? -1 : ((currHeapSize - right == 0) ? 0 : 1)); // when right is OOB then children == 0, when left is OOB then children < 0
    switch(children) {
        case -1: return;

        case 0: {
            if (minHeap[left] < minHeap[pos]) {
                heapSwap(minHeap+(left), minHeap+pos);
                minHeapRestoreDown(minHeap, currHeapSize, left);
                return;
            }
            return;
        }
        case 1: {
            if (minHeap[left] < minHeap[right]) {
                if (minHeap[left] < minHeap[pos]) {
                    heapSwap(minHeap+(left), minHeap+pos);
                    minHeapRestoreDown(minHeap, currHeapSize, left);
                    return;
                }
                return;
            }
            if (minHeap[right] < minHeap[pos]) {
                heapSwap(minHeap+(right), minHeap+pos);
                minHeapRestoreDown(minHeap, currHeapSize, right);
                return;
            }
            return;
        }
        default: return;
    }
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
    minHeap[0] = 0;
    heapSwap(minHeap, minHeap + currHeapSize-1);
    minHeapRestoreDown(minHeap, currHeapSize-1, 0);
    //printf("pop: %d, minHeap[0]: %d\n", minInt, minHeap[0]);
    return minInt;
}

int cmpEvents0(const void* a, const void* b) {
    int** eA = (int**) a;
    int** eB = (int**) b;
    return (**eA > **eB) - (**eA < **eB);
}

int cmpEvents1(const void* a, const void* b) {
    int** eA = (int**) a;
    int** eB = (int**) b;
    return (*((*eA)+1) > *((*eB)+1)) - (*((*eA)+1) < *((*eB)+1));
}

int maxEvents(int** events, const int eventsSize, int* eventsColSize) {
    if (eventsSize == 1) return 1;
    qsort(events, eventsSize, sizeof(*events), cmpEvents0);

    int currLength = 1;
    int currStart = 0;
    for (size_t eventsIt = 0; eventsIt < eventsSize-1;) {
        if (events[eventsIt][0] == events[++eventsIt][0]) ++currLength;
        else {
            // Sort first part
            qsort(events+currStart, eventsIt-currStart, sizeof(*events), cmpEvents1);
            // Finished sort
            currLength = 1;
            currStart = eventsIt;
            ++eventsIt;
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



    // printf("[");
    // for (size_t it = 0; it < eventsSize-1; ++it) {
    //     printf("[%d,%d],", events[it][0], events[it][1]);
    // }
    // printf("[%d,%d]]", events[eventsSize-1][0], events[eventsSize-1][1]);

    minHeapFree(minHeap);
    return result;
}
