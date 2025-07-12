// I am currently implementing an AVL Tree in this file.
// Work in Progress : 1/11 functions done.
// -- Will split this into a .h file and a .c file.

// TODO - CREATE HEADER WITH FUNCTION DECLARATIONS!
// TODO - CREATE PYTHON TESTSCRIPT FOR EACH FUNCTION!

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
 *  --- Mutate keystruct ---
 * 1. AVLNode* avlCreateTree(const AVLKey avlkey, const AVLNode* node);     -- CHANGE ARGUMENTS TO MORE USERFRIENDLY
 * 2. AVLNode* avlCreateNode(const AVLKey avlkey, const AVLNode* node);     -- DONE
 * 3. void avlInsert(AVLNode* node, int key, int value);                    -- DONE
 * 4. void avlUpdatePosition(AVLNode* node);                                -- DONE
 *
 *  --- Utility functions ---
 * 1. int avlGetHeight(AVLNode* node); // if node == NULL return 0          -- DONE
 * 2. int avlGetValue(AVLNode* node, int key);                              -- DONE
 * 
 *  --- Delete functions ---
 * 1. int avlDeleteKey(AVLNode* node, const int key);                       -- DONE
 * 2. int avlDeleteNode(AVLNode* node, int key);                            -- DONE
 * 3. void avlFreeNode(AVLNode* node);                                      -- DONE
 *
 *  --- Find functions ---
 *  1. avlFindNode(const AVLNode* node, const int key)                      -- DONE
 *
 *  --- Balance functions ---
 * 2. void avlBalanceNN(AVLNode* node);                                     -- DONE
 * 3. void avlBalanceNP(AVLNode* node);                                     -- DONE
 * 4. void avlBalancePP(AVLNode* node);                                     -- DONE
 * 5. void avlBalancePN(AVLNode* node);                                     -- DONE
 * 
 * */

// =============================================== MUTATE KEYSTRUCT ===============================================
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
        assert(newNode != NULL && "FUNCTION: AVLCREATENODE ---- STOPPED ---- MALLOC FAILED") {
        // Implement logging here or throw an exception;
        return NULL;
    }

    newNode->keyStruct->key = avlkey->key;
    newNode->keyStruct->value = avlkey->value;

    newNode->bf = 0;
    newNode->height = 1;

    newNode->parent = parent;
    newNode->left = NULL;
    newNode->right = NULL;

    return newNode;
}

void avlInsert(const AVLNode* node, const int key, const int value) {
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

    if (!(temp->keyStruct->key - key)) temp->keyStruct->value = value;
    else if (difference > 0) {
        assert(temp->left -= NULL && "FUNCTION: AVLINSERT ---- STOPPED ---- DIFFERENCE > 0 ---- TEMP->LEFT != NULL");
        temp->left = avlCreateNode((struct AVLKey){key, value}, temp);
    }
    else if (difference < 0) {
        assert(temp->right == NULL && "FUNCTION: AVLINSERT ---- STOPPED ---- DIFFERENCE < 0 ---- TEMP->RIGHT != NULL");
        temp->right = avlCreateNode((struct AVLKey){key, value}, temp);
    }

    do {
        avlUpdatePosition(temp);
        if (temp->bf == 2) {
            if (temp->left->bf < 0) avlBalancePN(temp);
            else avlBalancePP(temp);
        }
        else if (temp->bf == -2) {
            if (temp->left->bf < 0) avlBalanceNN(temp);
            else avlBalanceNP(temp);
        }
        temp = temp->parent;
    } while (temp);
    
    return;
}

// Maybe write an inline version of this. WARNING: Use inline functions only in file where it is defined (translation unit).
void avlUpdatePosition(AVLNode* node) {
    if (!node) return;
    int lHeight = avlGetHeight(node->left);
    int rHeight = avlGetHeight(node->right);

    node->height = (lHeight > rHeight ? lHeight : rHeight);
    node->bf = (lHeight - rHeight);
    return;
}

// =============================================== UTILITY FUNCTIONS ===============================================

int avlGetHeight(AVLNode* node) {
    if(!node) return 0;
    return node->height;
}

//  If key cannot be found, we return value "-1"
//  TODO - WRITE A LOG MESSAGE IF THE KEY CANNOT BE FOUND, INTO A LOG-FILE
int avlGetValue(AVLNode* node, int key) {
    AVLNode* temp = avlFindNode(node, key);
    assert(temp != NULL && "FUNCTION: AVLGETVALUE ---- STOPPED ---- key cannot be found");
    return temp->keyStruct->value;
}

// =============================================== DELETE FUNCTIONS ===============================================

int avlDeleteKey(AVLNode* node, const int key) {
    return avlDeleteNode((avlFindNode(node, key));
}

int avlDeleteNode(AVLNode* node) {
    assert(node && "Key that is to be deleted, does not exist in tree");
    int returnVal = node->keyStruct->value;

    bool state = (node->bf > 0 ? 1 : 0);
    AVLNode* iterator;
    if (state) iterator = node->left;
    else iterator = node->right;

    while (state && iterator->right != NULL) iterator = iterator->right;
    while (!state && iterator->left != NULL) iterator = iterator->left;

    AVLNode* tempP = iterator->parent;
    AVLNode* tempC = (state ? iterator->left : iterator->right);

    iterator->right = node->right;
    iterator->left = node->left;
    iterator->parent = node->parent;

    if (state) tempP->right = tempC;
    else tempP->left = tempC;
    
    do { 
        avlUpdatePosition(tempP);
        if (tempP->bf == 2) {
            if (tempP->left->bf < 0) avlBalancePN(tempP);
            else avlBalancePP(tempP);
        }
        else if (tempP->bf == -2) {
            if (tempP->left->bf < 0) avlBalanceNN(tempP);
            else avlBalanceNP(tempP);
        }
        tempP = tempP->parent;
    } while (tempP);

    avlFreeNode(node);

    return returnVal;
}

void avlFreeNode(AVLNode* node) {
    //if (node->parent && node->keyStruct->key > node->parent->keyStruct->key) {
    //    node->parent->right = NULL;
    //}
    //else if (node->parent && node->keyStruct->key < node->parent->keyStruct->key) {
    //    node->parent->left = NULL;
    //}

    free(node);
    return;
}

// =============================================== FIND FUNCTIONS ===============================================


AVLNode* avlFindNode(const AVLNode* node, const int key) {
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

// =============================================== BALANCE FUNCTIONS ===============================================

void avlBalanceNN(AVLNode* node) {
    assert(node && node->bf == -2 && "FUNCTION: AVLBALANCENN ---- STOPPED ---- BALANCE FACTOR IS NOT -2 ");    
    assert(node && !(node->right->bf > 0) && "FUNCTION: AVLBALANCENN ---- STOPPED ---- NODE->RIGHT BALANCE FACTOR NOT 0 OR NEGATIVE");    

    AVLNode* temp = node->right;
    node->right = temp->left;
    if (node->right != NULL) node->right->parent = node;
    temp->left = node;
    temp->parent = node->parent;
    node->parent = temp;

    avlUpdatePosition(node);
    avlUpdatePosition(temp);

    return;
}

void avlBalancePP(AVLNode* node) {
    assert(node && node->bf == 2 && "FUNCTION: AVLBALANCENN ---- STOPPED ---- BALANCE FACTOR IS NOT 2 ");    
    assert(node && !(node->right->bf < 0) && "FUNCTION: AVLBALANCENN ---- STOPPED ---- NODE->LEFT BALANCE FACTOR NOT 0 OR POSITIVE");    

    AVLNode* temp = node->left;
    node->left = temp->right;
    if (node->left != NULL) node->left->parent = node;
    temp->right = node;
    temp->parent = node->parent;
    node->parent = temp;

    avlUpdatePosition(node);
    avlUpdatePosition(temp);

    return;
}

void avlBalanceNP(AVLNode* node) {
    assert(node->bf == -2 && "FUNCTION: AVLBALANCENN ---- STOPPED ---- BALANCE FACTOR IS NOT -2 ");    
    assert(node->right->bf == 1 && "FUNCTION: AVLBALANCENN ---- STOPPED ---- NODE->RIGHT BALANCE FACTOR NOT POSITIVE");    

    AVLNode* R = node->right;
    AVLNode* RL = R->left;
        
    // First, small rotation to the right
    R->left = RL->right;
    if (R->left) R->left->parent = R;

    RL->right = R;
    R->parent = RL;

    // Second, small rotation to the left
    node->right = RL->left;
    if (node->right) node->right->parent = node;

    RL->left = node;
    RL->parent = node->parent;
    node->parent = RL;

    // Adjust height and balance factor
    avlUpdatePosition(node);
    avlUpdatePosition(R);
    avlUpdatePosition(RL);
    
    return;
}

void avlBalancePN(AVLNode* node) {
    assert(node->bf == 2 && "FUNCTION: AVLBALANCENN ---- STOPPED ---- BALANCE FACTOR IS NOT 2 ");    
    assert(node->left->bf == -1 && "FUNCTION: AVLBALANCENN ---- STOPPED ---- NODE->RIGHT BALANCE FACTOR NOT NEGATIVE");    

    AVLNode* L = node->left;
    AVLNode* LR = L->right;
        
    // First, small rotation to the left 
    L->right = LR->left;
    if (L->right) L->right->parent = L;

    LR->left = L;
    L->parent = LR;

    // Second, small rotation to the left
    node->left = LR->right;
    if (node->left) node->left->parent = node;

    LR->right = node;
    LR->parent = node->parent;
    node->parent = LR;

    // Adjust height and balance factor
    avlUpdatePosition(node);
    avlUpdatePosition(L);
    avlUpdatePosition(LR);

    return;
}
