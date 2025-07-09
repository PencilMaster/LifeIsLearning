
// I am currently implementing an AVL Tree in this file.
// Work in Progress : 1/11 functions done.
// -- Will split this into a .h file and a .c file.

// TODO: CREATE HEADER WITH FUNCTION DECLARATIONS!

#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

/* 
 * Node Struct contains <key:value>: {AVLKey keyStruct}
 * Node Struct contains ints: {int balancefactor(bf), int height}
 * Node Struct contains ptrs: {Node* parent, Node* left, Node* right}
 * */

typedef struct AVLKey {
    int key;
    int value;
} AVLKey;

typedef struct AVLNode {
    AVLKey keyStruct;
  
    int bf;
    int height;

    AVLNode* parent;
    AVLNode* left;
    AVLNode* right;
} AVLNode;

/* We will have the following functions:
 *
 * 1. AVLNode* avlCreateNode(const AVLKey avlkey, const AVLNode* node); -- DONE
 * 2. void avlInsert(AVLNode* node, int key, int value);
 * 3. void avlUpdate(AVLNode* node, int key, int value);
 * 4. int avlFind(AVLNode* node, int key);                              -- DONE 
 * 5. AVLNode* avlFindNode(AVLNode* node, int key);                     -- DONE
 *
 *  --- Delete functions ---
 * 1. void avlDelete(AVLNode* node, int key);
 * 2. void avlFreeNode(AVLNode* node);
 *
 *  --- Balance functions ---
 * 1. void avlRestoreBalance(AVLNode* node); // If currentNode->bf changes. This function is called.
 * 2. void avlBalanceNN(AVLNode* node);
 * 3. void avlBalanceNP(AVLNode* node);
 * 4. void avlBalancePP(AVLNode* node);
 * 5. void avlBalancePN(AVLNode* node);
 * 
 * */

/* Expected Input: (AVLKey, AVLNode*);
 * 
 * Behaviour: 
 * 1. Returns a pointer to a new AVLNode struct with NO children.
 * 2. Sets its avlkey and parent to function arguments.
 *
 * */
AVLNode* avlCreateNode(const AVLKey avlkey, const AVLNode* parent) {
    AVLNode* newNode = malloc(sizeof(*newNode));
    if (newNode == NULL) {
    // Implement logging here or throw an exception;
    return NULL;
    }:w

    newNode->keyStruct->key = avlkey->key;
    newNode->keyStruct->value = avlkey->value;

    newNode->bf = 0;
    newNode->height = 0;

    newNode->parent = parent;
    newNode->left = NULL;
    newNode->right = NULL;

    return newNode;
}

//  If key cannot be found, we return value "-1"
int avlFind(AVLNode* node, int key) {
    AVLNode* temp = avlFindNode(node, key);
    if (temp == NULL) return -1;
    return temp->keyStruct->value;
}

AVLNode* avlFindNode(const AVLNode* node, int key) {
    AVLNode* temp = node;
    int difference = temp->keyStruct->key - key;
    int loopStatus = 1;

    while (loopStatus) {
        loopStatus = 0;
        while (difference > 0 && temp->left != NULL) {
            temp = temp->left;
            difference = temp->keyStruct->key - key;
            loopStatus = 1;
        } 
        while (difference < 0 && temp->right != NULL) {
            temp = temp->right;
            difference = temp->keyStruct->key - key;
            loopStatus = 2;
        } 
    }

    if (!(temp->keyStruct->key - key)) return temp;
    else return NULL;
}

void avlInsert(AVLNode* node, int key, int value) {

    return;
}



