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
#include "tracker.h"

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
    bstnode* newnode = bstnode_create(value);
    newnode->balance_factor = EVEN;

    if (tree->length == 0) {
        tree->head = newnode;
        tree->length++;
        return 1;
    }

    bstnode* current = tree->head;
    bstnode* rebalance_node = tree->head;

    node* update_tracker = init_update_tracker();

    while(current) {
        if (value == current->value) {
            //already in the tree
            revert_rank_updates(update_tracker, -1);
            destroy_update_tracker(update_tracker);
            return 0;
        }
        else if (value < current->value) {
            // go left
            current->rank++;
            update_tracker = track_update(update_tracker, current);

            if (current->left) {
                current = current->left;
                if (current->balance_factor) rebalance_node = current;
            }
            else {
                current->left = newnode;
                newnode->parent = current;
                tree->length++;
                destroy_update_tracker(update_tracker);
                printf("Inserting %d left of %d. Rebalance point is %d (%+d).\n",
                        value, current->value, rebalance_node->value, 
                        rebalance_node->balance_factor);
                break;
            }
        }
        else if (value > current->value) {
            // go right
            if (current->right) {
                current = current->right;
                if (current->balance_factor) rebalance_node = current;
            }
            else {
                // insert the node
                current->right = newnode;
                newnode->parent = current;
                tree->length++;
                destroy_update_tracker(update_tracker);
                printf("Inserting %d right of %d. Rebalance point is %d (%+d).\n",
                        value, current->value, rebalance_node->value,
                        rebalance_node->balance_factor);
                break;
            }
        }

    }

    int insert_direction = (value < rebalance_node->value) ? LEFT : RIGHT;
    bstnode* pivot = BRANCH(insert_direction, rebalance_node);
    bstnode* current2 = BRANCH(insert_direction, rebalance_node);

    while(current2->value != value) {
        if (value < current2->value) {
            current2->balance_factor = LEFT;
            current2 = current2->left;
        }
        else if (value > current2->value) {
            current2->balance_factor = RIGHT;
            current2 = current2->right;
        }
    }

    // The rebalance point was the root and the tree started even,
    // so this insert will have shifted the balance of the root from
    // zero to +/-1. Update that balance, and return.
    if (rebalance_node->balance_factor == 0) {
        rebalance_node->balance_factor = insert_direction;
        return 1;
    }

    // The new node went in on the opposite side of the rebalance point,
    // shifting it into a more balanced position.
    if (rebalance_node->balance_factor == REVERSE_DIRECTION(insert_direction)) {
        rebalance_node->balance_factor = EVEN;
        return 1;
    }

    // The new node went in on the unbalanced side of the rebalance point,
    // unbalancing it further (to +/- 2). So we need to do some rotations to
    // restore the balance.
    if (rebalance_node->balance_factor == insert_direction) {
        if (pivot->balance_factor == insert_direction) {
            // single rotation
            if (insert_direction == LEFT)  {
                printf("rotating right!\n");
                avl_rotate_right(tree, rebalance_node);
                rebalance_node->balance_factor = EVEN;
                rebalance_node->parent->balance_factor = EVEN;
            }
            else {
                printf("rotating left!\n");
                avl_rotate_left(tree, rebalance_node);
                rebalance_node->balance_factor = EVEN;
                rebalance_node->parent->balance_factor = EVEN;
            }
        }
        else {
            // double rotation
            if (insert_direction == LEFT) {
                printf("Double rotation, left then right!\n");
                bstnode* b = rebalance_node->left;
                bstnode* x = b->right;
                avl_rotate_left(tree, b);
                avl_rotate_right(tree, rebalance_node);

                if (x->balance_factor == insert_direction) {
                    rebalance_node->balance_factor = REVERSE_DIRECTION(insert_direction);
                    b->balance_factor = 0;
                } else if (x->balance_factor == EVEN) {
                    rebalance_node->balance_factor = 0;
                    b->balance_factor = 0;
                } else if (x->balance_factor == REVERSE_DIRECTION(insert_direction)) {
                    rebalance_node->balance_factor = 0;
                    b->balance_factor = insert_direction;
                }

                x->balance_factor = 0;
            }
            else {
                printf("Double rotation, right then left\n");
                bstnode* b = rebalance_node->right;
                bstnode* x = b->left;
                avl_rotate_right(tree, b);
                avl_rotate_left(tree, rebalance_node);

                if (x->balance_factor == insert_direction) {
                    rebalance_node->balance_factor = REVERSE_DIRECTION(insert_direction);
                    b->balance_factor = 0;
                } else if (x->balance_factor == EVEN) {
                    rebalance_node->balance_factor = 0;
                    b->balance_factor = 0;
                } else if (x->balance_factor == REVERSE_DIRECTION(insert_direction)) {
                    rebalance_node->balance_factor = 0;
                    b->balance_factor = insert_direction;
                }

                x->balance_factor = 0;
            }
        }
    }


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
