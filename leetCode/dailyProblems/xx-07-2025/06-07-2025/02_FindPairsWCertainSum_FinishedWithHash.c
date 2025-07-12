// Implemented second iteration with a hashTable and hashFunction.
//
// For this I used a prime number as HASH_SIZE, S.t. less clustering occurs.
// Ammended the solution of another person, using memset to set memory to zero and make checking during allocation easier.
// Also, since we know the HASH_SIZE, immediately initialising it with the length makes freeing easier.
//
// Would love to make it thread safe with a static lock variable but leetCode is single threaded and it would add overhead here.
//

#include <stdlib.h>

// Do a little hash node and hash table.
// Got inspiration from a solution, after my sol gets TLE at test number 23/26

#define HASH_SIZE 123209 // Use a prime number here

typedef struct HashNode {
    int key;
    int count;
    struct HashNode* next;
} HashNode;

typedef struct {
    int* nums1;
    size_t nums1Size;

    int* nums2;
    size_t nums2Size;

    HashNode* hashTable[HASH_SIZE]; // Gets freed automatically when obj is freed. -- BUT is usually stored in stack. here in heap since obj = malloc() creates it.
} FindSumPairs;

/*  --------------------
 *  Hash-Functions below
 *  --------------------
 */ 

int hashFunction(const int key) { // this is passed positive
    const unsigned int index = ((unsigned int)key) % HASH_SIZE; // To make sure that no negative index can be returned
    return (int)index;
}

void hashInsert(HashNode** hashTable, const int key, const int value) {
    const int index = hashFunction(key);
    HashNode* curr = hashTable[index];

    while (curr) { // Memset() in initialisation set filled hashTable with pointers containing value 0;
        if (curr->key == key) {
            curr->count += value;
            return;
        }
        curr = curr->next;
    }

    HashNode* newNode = malloc(sizeof(*newNode));
    newNode->key = key;
    newNode->count = value;
    newNode->next = hashTable[index]; // opinters are Copy-by-Value
    hashTable[index] = newNode;
}

int hashGet(HashNode** hashTable, const int key) {
    const int index = hashFunction(key);
    const HashNode* curr = hashTable[index];

    while (curr) { // Memset() in initialisation set filled hashTable with pointers containing value 0;
        if (curr->key == key) {
            return curr->count;
        }
        curr = curr->next;
    }
    return 0;
}

void hashFree(HashNode** hashTable) {
    for (size_t i = 0; i < HASH_SIZE; ++i) {
        HashNode* curr = hashTable[i];
        while (curr) {
            HashNode* temp = curr;
            curr = curr->next;
            free(temp);
        }
        hashTable[i] = NULL;
    }
}

int cmpfunction(const void* A, const void* B) { // In case we use qsort()
    int* a = (int*) A;
    int* b = (int*) B;
    return (*a > *b) - (*a < *b);
}

FindSumPairs* findSumPairsCreate(const int* const nums1, const int nums1Size, const int* const nums2, const int nums2Size) {
    FindSumPairs* obj = malloc(sizeof(*obj));

    (*obj).nums1 = malloc(nums1Size * sizeof(*nums1));
    (*obj).nums1Size = (size_t) nums1Size;
    (*obj).nums2 = malloc(nums2Size * sizeof(*nums2));
    (*obj).nums2Size = (size_t) nums2Size;

    memcpy((*obj).nums1, nums1, nums1Size * sizeof(*nums1));
    memcpy((*obj).nums2, nums2, nums2Size * sizeof(*nums2));
    memset((*obj).hashTable, 0, sizeof((*obj).hashTable));

    qsort(obj->nums1, obj->nums1Size, sizeof(*(obj->nums1)), cmpfunction);

    for (size_t i = 0; i < nums2Size; ++i) {
        hashInsert(obj->hashTable, nums2[i], 1);
    }
    
    return obj;
}

void findSumPairsAdd(FindSumPairs* obj, const int index, const int val) {
    // Would love to introduce a locking variable which locks the Nodes for thread safety.
        const int oldVal = obj->nums2[index];
        hashInsert(obj->hashTable, oldVal, -1);

        obj->nums2[index] += val;

        const int newVal = obj->nums2[index];
        hashInsert(obj->hashTable, newVal, 1);
}

// Might want to use qsort() but need to copy arrays again then. 
// Might be worth it, since O(n+m + nlog(n) + mlog(m) + n+m) < O(n*m). More meory heavy though.
    
int findSumPairsCount(FindSumPairs* obj, const int tot) { 
    int totalAmount = 0;
    for (size_t nums1Iterator = 0; nums1Iterator < obj->nums1Size; ++nums1Iterator) {
        const int difference = tot - obj->nums1[nums1Iterator];
        totalAmount += hashGet(obj->hashTable, difference);
    } 
    return totalAmount;
}

void findSumPairsFree(FindSumPairs* obj) {
    free((*obj).nums1);
    free((*obj).nums2);
    hashFree((*obj).hashTable);
    free(obj);
}

/**
 * Your FindSumPairs struct will be instantiated and called as such:
 * FindSumPairs* obj = findSumPairsCreate(nums1, nums1Size, nums2, nums2Size);
 * findSumPairsAdd(obj, index, val);
 
 * int param_2 = findSumPairsCount(obj, tot);
 
 * findSumPairsFree(obj);
*/
