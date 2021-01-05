/*
 * avl.h
 *
 * An implementation of the AVL Balanced Binary Search Tree
 *
 * Douglas Rumbaugh
 * 12/30/2020
 *
 */
#pragma once

#include "nodes.h"
#include <assert.h>
#include "bst.h"
#include "tracker.h"


bst* avl_create(void);

int avl_insert(bst* tree, int value);
int avl_delete(bst* tree, int value);
int avl_delete_slow(bst** tree, int value);
bstnode* avl_search(bst* tree, int value);
bstnode* avl_index(bst* tree, int index);
int avl_get_index(bst* tree, int value);

void avl_rotate_left(bst* tree, bstnode* center);
void avl_rotate_right(bst* tree, bstnode* center);

void avl_clear(bst* tree);
void avl_destroy(bst* tree);
void avl_clear_destroy(bst* tree);

void _avl_delete_balancing(bst* tree, bstnode* rebalance_node, int delete_direction);
