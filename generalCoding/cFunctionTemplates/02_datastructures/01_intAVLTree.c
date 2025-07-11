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
 * 1. AVLNode* avlCreateNode(const AVLKey avlkey, const AVLNode* node);     -- DONE
 * 2. void avlInsert(AVLNode* node, int key, int value);                    -- DONE
 * 5. void avlUpdatePosition(AVLNode* node);                                -- DONE
 *
 *  --- Utility functions ---
 * 1. int avlGetHeight(AVLNode* node); // if node == NULL return 0          -- DONE
 * 2. int avlGetValue(AVLNode* node, int key);                              -- DONE
 * 
 *  --- Delete functions ---
 * 1. void avlDelete(AVLNode* node, int key);
 * 2. void avlFreeNode(AVLNode* node);
 *
 *  --- Balance functions ---
 * 1. void avlRestoreBalance(AVLNode* node); // If currentNode->bf changes. This function is called.
 * 2. void avlBalanceNN(AVLNode* node);                                 -- DONE
 * 3. void avlBalanceNP(AVLNode* node);                                 -- DONE
 * 4. void avlBalancePP(AVLNode* node);                                 -- DONE
 * 5. void avlBalancePN(AVLNode* node);                                 -- DONE
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

// TODO:
    // 1. NEED TO UPDATE HEIGHTS AND BALANCE OF PARENTS IF HEIGHT CHANGED.
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
        assert(temp->left != NULL && "FUNCTION: AVLINSERT ---- STOPPED ---- DIFFERENCE > 0 ---- TEMP->LEFT == NULL");
        temp->left = avlCreateNode((struct AVLKey){key, value}, temp);
    }
    else if (difference < 0) {
        assert(temp->right != NULL && "FUNCTION: AVLINSERT ---- STOPPED ---- DIFFERENCE < 0 ---- TEMP->RIGHT == NULL");
        temp->right = avlCreateNode((struct AVLKey){key, value}, temp);
    }
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

// =============================================== FIND FUNCTIONS ===============================================


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

//  If key cannot be found, we return value "-1"
int avlGetValue(AVLNode* node, int key) {
    AVLNode* temp = avlFindNode(node, key);
    if (temp == NULL) return -1;
    return temp->keyStruct->value;
}


// =============================================== BALANCE FUNCTIONS ===============================================

void avlBalanceNN(AVLNode* node) {
    assert(node && node->bf == -2 && "FUNCTION: AVLBALANCENN ---- STOPPED ---- BALANCE FACTOR IS NOT -2 ");    
    assert(node && node->right->bf == -1 && "FUNCTION: AVLBALANCENN ---- STOPPED ---- NODE->RIGHT BALANCE FACTOR NOT NEGATIVE");    

    AVLNode* temp = node->right;
    node->right = temp->left;
    if (temp->right != NULL) node->right->parent = node;
    temp->left = node;
    temp->parent = node->parent;
    node->parent = temp;

    //CAREFUL, CHECK IF  BF ACTUALLY ALWAYS GETS PUT TO 0, THIS HAPPENS WHEN NODE->RIGHT->BF WAS -1, WHICH IT SHOULD BE

    //TODO: adjust height and balance factor
    node->bf = 0;
    --(node->height);

    //Temp height does not change
    temp->bf = temp->left->height - temp->right->height;
    
    return;
    //Since temp height does not change, we do not need to call balance on another node after this function
}

void avlBalancePP(AVLNode* node) {
    assert(node && node->bf == 2 && "FUNCTION: AVLBALANCENN ---- STOPPED ---- BALANCE FACTOR IS NOT 2 ");    
    assert(node && node->left->bf == 1 && "FUNCTION: AVLBALANCENN ---- STOPPED ---- NODE->LEFT BALANCE FACTOR NOT POSITIVE");    

    AVLNode* temp = node->left;
    node->left = temp->right;
    if (temp->left != NULL) node->left->parent = node;
    temp->right = node;
    temp->parent = node->parent;
    node->parent = temp;

    //CAREFUL, CHECK IF  BF ACTUALLY ALWAYS GETS PUT TO 0, THIS HAPPENS WHEN NODE->LEFT->BF WAS 1, WHICH IT SHOULD BE

    //TODO: adjust height and balance factor
    node->bf = 0;
    --(node->height);

    //Temp height does not change
    temp->bf = temp->left->height - temp->right->height;
    
    return;
    //Since temp height does not change, we do not need to call balance on another node after this function
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
    // RL always changes this way
    ++(RL->height);
    RL->bf = 0;

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
    // LR always changes this way
    ++(LR->height);
    LR->bf = 0;

    return;
}
