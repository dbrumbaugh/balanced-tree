/*
 * avl.c
 *
 * An implementation of the AVL Balanced Binary Search Tree
 *
 * Douglas Rumbaugh
 * 01/01/2021
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include "avl.h"

void avl_rotate_left(bst* tree, bstnode* center)
{
    // if the rotation center has no node on the right, there isn't anything
    // to rotate! So just pass
    if (!center->right) return;

    bstnode* pivot = center->right;
    
    // named subtrees, for convenience
    bstnode* alpha = center->left;
    bstnode* beta = pivot->left;
    bstnode* gamma = pivot->right;

    // Check if the center of rotation is the root of the
    // tree. If so, we'll need to make the pivot the new
    // root, so just take care of that now.
    int head_node = !center->parent;
    if (head_node) tree->head = pivot;

    pivot->parent = center->parent;
    pivot->left = center;
    center->parent = pivot;

    center->right = beta;
    if (beta) {
        beta->parent = center;
    }

    // Because the pivot's left subtree become's centers's right,
    // we can fix its rank by just adding centers's rank to
    // it, which accounts for all the elements in a
    // and its left subtree already. 
    //
    // centers's left subtree isn't touched, and so a's rank
    // should remain the same.
    pivot->rank += center->rank; 

    if (pivot->parent && pivot->parent->left && pivot->parent->left->value == center->value)
        pivot->parent->left = pivot;
    else if (pivot->parent && pivot->parent->right && pivot->parent->right->value == center->value)
        pivot->parent->right = pivot;
}


void avl_rotate_right(bst* tree, bstnode* center)
{
    // if the rotation center has no node on the left, there isn't anything
    // to rotate! So just pass
    if (!center->left) return;

    bstnode* pivot = center->left;

    // named subtrees, for convenience
    bstnode* alpha = center->right;
    bstnode* beta = pivot->right;
    bstnode* gamma = pivot->left;

    // Check if the center of rotation is the root of the
    // tree. If so, we'll need to make the pivot the new
    // root, so just take care of that now.
    int head_node = !center->parent;
    if (head_node) tree->head = pivot;

    pivot->parent = center->parent;
    pivot->right = center;
    center->parent = pivot;

    center->left = beta;
    if (beta) {
        beta->parent = center;
    }

    // Center loses pivot's left subtree, so deduct this from its rank.
    center->rank -= pivot->rank;

    if (pivot->parent && pivot->parent->left && pivot->parent->left->value == center->value)
        pivot->parent->left = pivot;
    else if (pivot->parent && pivot->parent->right && pivot->parent->right->value == center->value)
        pivot->parent->right = pivot;
    
}


bst* avl_create(void)
{
    return bst_create();
}


int avl_delete(bst* tree, int value)
{
    //TODO: This isn't final, obviously. Just want to
    //      validate that rotations are working okay
    //      before diving too deeply into the weeds on
    //      getting this to work.
    return bst_delete(tree, value);
}


int avl_insert(bst* tree, int value)
{
    //TODO: This isn't final, obviously. Just want to
    //      validate that rotations are working okay
    //      before diving too deeply into the weeds on
    //      getting this to work.

    return bst_insert(tree, value);

    /*
    bstnode* newnode = bstnode_create(value);
    newnode->balance_factor = BAL_EVEN;

    return 0;
    */
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
