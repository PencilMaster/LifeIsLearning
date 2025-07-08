// This code contains an implementation of a hash table with a hashStruct, hashFunction, hashInsert, hashGet and hashFree method
//

#define HASH_SIZE 123209 // Use a prime number here

typedef struct HashNode {
    int key;
    int count;
    struct HashNode* next;
} HashNode;

/*  Here another struct needs to be implemented, containing HashNode* hashTable[HASH_SIZE]; element
 *  --------------------
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
