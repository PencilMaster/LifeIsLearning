#ifndef INTAVLTREE_H
#define INTAVLTREE_H

#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct AVLKey {
    int key;
    int value;
}   AVLKey;

typedef struct AVLNode {
    AVLKey keyStruct;
    
    int bf;
    int height;

    struct AVLNode* parent;
    struct AVLNode* left;
    struct AVLNode* right;
}   AVLNode;

typedef struct AVLNode AVLTree;


AVLTree* avlCreateTree(const int key, const int value); 
AVLNode* avlCreateNode(const AVLKey avlkey, const AVLNode* parent); 

void avlInsert(const AVLNode* node, const int key, const int value);
void avlUpdatePosition(AVLNode* node);

int avlGetHeight(AVLNode* node);
int avlGetValue(AVLNode* node, int key);
int avlDeleteKey(AVLNode* node, const int key);
int avlDeleteNode(AVLNode* node);

void avlFreeNode(AVLNode* node);
AVLNode* avlFindNode(const AVLNode* node, const int key);

void avlBalanceNN(AVLNode* node);
void avlBalancePP(AVLNode* node);
void avlBalanceNP(AVLNode* node);
void avlBalancePN(AVLNode* node);

#endif
