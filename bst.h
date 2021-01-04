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

#pragma once

#include <assert.h>
#include "nodes.h"
#include "tracker.h"

#define AVL_SUPPORT

#define RIGHT  +1
#define LEFT -1
#define EVEN  0

#define BRANCH(bal, node) ((bal == RIGHT) ? node->right : node->left)

// FIXME: Perhaps this should account for EVEN as well and leave it
//        unchanged? It shouldn't happen in the module as written, but 
//        might be worth just accounting for it anyway.
#define REVERSE_DIRECTION(bal) ((bal == RIGHT) ? LEFT : RIGHT)

#define ASSERT_NOT_REACHED() (assert(0))

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
bstnode* bst_find_node_and_path(bst* tree, int value, node** path_tracker, int rank_update);
int bst_get_index(bst* tree, int value);
void bst_clear(bst* tree);
void bst_destroy(bst* tree);
void bst_clear_destroy(bst* tree);

void _traverse_and_free(bstnode* head);
int bst_node_delete(bst* tree, bstnode* del_node, node** path_tracker);
void bst_node_insert(bst* tree, bstnode* newnode, node* path_tracker);
void _traverse_and_count(bstnode* head, int* cnt);
int _count_children(bstnode* head);
