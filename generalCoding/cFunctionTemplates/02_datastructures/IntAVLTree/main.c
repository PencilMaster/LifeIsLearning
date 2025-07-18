// We will write some tests here until I figure out googletests in python.
//
#include <stdio.h>
#include <stdlib.h>
#include "intAVLTree.h"

int main (int argc, char** argv) {
    //First we create an AVLTree. Here the typedef in .h and struct in .c could create problems.
    AVLTree* root = avlCreateTree(1, 10);
    printf("%d", avlGetValue(root, 1));

    return 0;
}
