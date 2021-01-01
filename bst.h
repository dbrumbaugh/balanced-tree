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


#define ASSERT_NOT_REACHED() (assert(0))

typedef struct BSTNode {
    int value;
    int rank;
    struct BSTNode* left;
    struct BSTNode* right;
    struct BSTNode* parent;
} bstnode;


typedef struct Node {
    bstnode* treenode;
    struct Node* next;
} node;

typedef struct BST {
    int length;
    bstnode* head;
} bst;


int bst_insert(bst* tree, int value);
int bst_delete(bst* tree, int value);
bstnode* bst_search(bst* tree, int value);
bstnode* bst_index(bst* tree, int index);
bst* bst_create();
void bst_clear(bst* tree);
void bst_destroy(bst* tree);
void bst_clear_destroy(bst* tree);

void _traverse_and_free(bstnode* head);
void _revert_rank_updates(node* head, int direction);
int _delete_node(bst* tree, bstnode* node);
