// Different solution, taken by someone else and I will write my own 
// segmented Tree algorithm in cTemplates directoy for it.

#include <stdlib.h>
#include <stdbool.h>
#include <memory.h>

#define LEFT(a)          (((a) << 1) + 1)           // Index of left child in segment tree
#define RIGHT(a)         (((a) << 1) + 2)           // Index of right child in segment tree
#define MAX(a, b)        (((a) > (b)) ? (a) : (b))
#define TREE_SIZE(B)     (4 * (B) * sizeof(int))
#define USED             (0)                        // Value for used/empty basket

void buildTree(int* tree, int* baskets, int idx, int left, int right) {
    if (left == right) {
        tree[idx] = baskets[left];
        return;
    }

    int mid = (left + right) >> 1;
    buildTree(tree, baskets, LEFT(idx), left, mid);         // Build left subtree
    buildTree(tree, baskets, RIGHT(idx), mid + 1, right);   // Build right subtree
    tree[idx] = MAX(tree[LEFT(idx)], tree[RIGHT(idx)]);     // Store max of both children
}

bool ableToPick(int* tree, int idx, int val, int left, int right) {
    if (tree[idx] < val) return false;      // No basket in range can hold the fruit
    if (left == right) {                    // Found suitable basket (leaf)
        tree[idx] = USED;                   // Mark basket as used
        return true;
    }

    int mid = (left + right) >> 1;
    bool picked = false;
    // Left child can't fit fruit, Try right
    if (tree[LEFT(idx)] < val) picked = ableToPick(tree, RIGHT(idx), val, mid + 1, right);
    else picked = ableToPick(tree, LEFT(idx), val, left, mid); // Try left

    tree[idx] = MAX(tree[LEFT(idx)], tree[RIGHT(idx)]); // Update current node after pick
    return picked;
}

int numOfUnplacedFruits(int* fruits, int fsz, int* baskets, int bsz) {
    int* tree = malloc(TREE_SIZE(bsz));
    memset(tree, 0, TREE_SIZE(bsz));
    buildTree(tree, baskets, 0, 0, bsz-1);

    int remained = fsz;
    for (int i = 0; i < fsz; ++i) {
        if (ableToPick(tree, 0, fruits[i], 0, bsz-1)) --remained;
    }
    free(tree);
    return remained;
}
