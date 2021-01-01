/*
 * avl.c
 *
 * An implementation of the AVL Balanced Binary Search Tree
 *
 * Douglas Rumbaugh
 * 12/30/2020
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include "avl.h"

bst* avl_create(void)
{
    return bst_create();
}


int avl_delete(bst* tree, int value)
{
    return 0;
}


int avl_insert(bst* tree, int value)
{
    bstnode* newnode = bstnode_create(value);

    return 0;
}


bstnode* avl_search(bst* tree, int value)
{
    return bst_search(tree, value);
}


bstnode* avl_index(bst* tree, int rank)
{
    return bst_index(tree, rank);
}


int avl_get_index(bst* tree, int value)
{
    return bst_get_index(tree, value);
}


void avl_clear(bst* tree)
{
    bst_clear(tree);
}


void avl_destroy(bst* tree)
{
    bst_destroy(tree);
}


void avl_clear_destroy(bst* tree)
{
    bst_clear_destroy(tree);
}
