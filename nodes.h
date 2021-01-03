#pragma once

#define AVL_SUPPORT

typedef struct BSTNode {
    int value;
    int rank;
    struct BSTNode* left;
    struct BSTNode* right;
    struct BSTNode* parent;

#ifdef AVL_SUPPORT
    // To save on code copying, I'm going to just 
    // use the same node and tree objects for AVL
    // as well, so I need to add this here. It isn't
    // neccessary for standard BST operation, and won't
    // be used by any of the bst_ functions.
    int balance_factor;
#endif
} bstnode;

typedef struct Node {
    bstnode* treenode;
    struct Node* next;
    int direction;
} node;
