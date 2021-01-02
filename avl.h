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
#include "bst.h"
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

bst* avl_create(void);

int avl_insert(bst* tree, int value);
int avl_delete(bst* tree, int value);
bstnode* avl_search(bst* tree, int value);
bstnode* avl_index(bst* tree, int index);
int avl_get_index(bst* tree, int value);

void avl_rotate_left(bst* tree, bstnode* center);
void avl_rotate_right(bst* tree, bstnode* center);

void avl_clear(bst* tree);
void avl_destroy(bst* tree);
void avl_clear_destroy(bst* tree);
