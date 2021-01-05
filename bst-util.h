/*
 * bst-util.h
 * Some helper functions for testing bst funtionality.
 *
 * Douglas Rumbaugh
 * 01/01/2020
 *
 */

#pragma once

#include "bst.h"
#define MAX(x, y) ((x) > (y) ? (x) : (y))
#define MIN(x, y) ((x) < (y) ? (x) : (y))

void inorder_traverse(bstnode* head);
void subtree_traverse(bstnode* head, int* counter);
void check_rank(bstnode* head, int verbose);
int calculate_tree_height(bstnode* head);
void subtree_node_counts(bstnode* head, int verbose);
void check_strict_balance(bstnode* head, int verbose);
void check_bst_ordering(bst* tree);
void check_bst_indexing(bst* tree);
