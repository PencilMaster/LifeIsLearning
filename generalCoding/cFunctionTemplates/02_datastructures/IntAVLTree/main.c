// We will write some tests here until I figure out googletests in python.
//
#include <stdio.h>
#include <stdlib.h>
#include "intAVLTree.h"

int main (int argc, char** argv) {
    //First we create an AVLTree. Here the typedef in .h and struct in .c could create problems.
    AVLTree* root = avlCreateTree(10, 10);
    printf("Key:value %d:%d \n", 10, avlGetValue(root, 10));

    for (size_t i = 0; i < 10; ++i) {
        avlInsert(root, i, i);
        printf("Key:value %zu:%d \n", i, avlGetValue(root,i));
    }
    
    avlFreeTree(root);
    root = NULL;


    return 0;
}
