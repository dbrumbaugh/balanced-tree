/*
 * avl.h
 *
 * An implementation of the AVL Balanced Binary Search Tree
 *
 * Douglas Rumbaugh
 * 12/30/2020
 *
 */

#include <assert.h>
#pragma once

#define BAL_NULL  -10
#define BAL_PLUS  +1
#define BAL_MINUS -1
#define BAL_EVEN  0

#define BRANCH(bal, node) ((bal == BAL_PLUS) ? node->right : node->left)

// FIXME: Perhaps this should account for BAL_EVEN as well and leave it
//        unchanged? It shouldn't happen in the module as written, but 
//        might be worth just accounting for it anyway.
#define REVERSE_BALANCE(bal) ((bal == BAL_PLUS) ? BAL_MINUS : BAL_PLUS)

#define ASSERT_NOT_REACHED() (assert(0))


typedef struct AVLNode {
    int value;
    int rank;
    struct AVLNode* left;
    struct AVLNode* right;
    int balance_factor;
} avlnode;


typedef struct Node {
    avlnode* treenode;
    struct Node* next;
} node;


typedef struct AVLTree {
    avlnode* root;
    int length;
} avltree;


avltree* avl_create(void);

int avl_insert(avltree* tree, int value);
avlnode* avl_search(avltree* tree, int value);
avlnode* avl_index(avltree* tree, int index);


void avl_clear(avltree* tree);
void avl_destroy(avltree* tree);
void avl_clear_destroy(avltree* tree);

void _avl_traverse_and_free(avlnode* head);

