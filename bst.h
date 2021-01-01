/* bst.h
 * A simple binary search tree. This one is unbalanced, and is just to serve as
 * a starting point for working on the balanced tree.
 *
 * Supports the basic operations of search, insert, and get by index (rank).
 *
 * Does not support the insertion of duplicate items.
 *
 * Douglas Rumbaugh
 * 12/30/2020
 */

#include <assert.h>
#pragma once

#define AVL_SUPPORT


#define ASSERT_NOT_REACHED() (assert(0))

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


typedef struct BST {
    int length;
    bstnode* head;
} bst;

bst* bst_create();
bstnode* bstnode_create(int value);

int bst_insert(bst* tree, int value);
int bst_delete(bst* tree, int value);
bstnode* bst_search(bst* tree, int value);
bstnode* bst_index(bst* tree, int index);
int bst_get_index(bst* tree, int value);
void bst_clear(bst* tree);
void bst_destroy(bst* tree);
void bst_clear_destroy(bst* tree);

void _traverse_and_free(bstnode* head);
int _delete_node(bst* tree, bstnode* node);
void _traverse_and_count(bstnode* head, int* cnt);
int _count_children(bstnode* head);
