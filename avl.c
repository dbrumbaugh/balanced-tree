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


void avl_rebalance(bst* tree, bstnode* rebalance_node, int direction)
{
    bstnode* pivot = BRANCH(direction, rebalance_node);
    printf("pivot node is %d (%d)\n", pivot->value, pivot->balance_factor);

    // special case for deletion when the pivot is already balanced
    if (pivot->balance_factor == EVEN) {
        printf("special case 3!\n");

        // single rotation
        if (direction == LEFT)  {
            printf("rotating right!\n");
            avl_rotate_right(tree, rebalance_node);
            pivot->balance_factor = rebalance_node->balance_factor;
        }
        else {
            printf("rotating left!\n");
            avl_rotate_left(tree, rebalance_node);
            pivot->balance_factor = rebalance_node->balance_factor;
        }
    }

    else if (pivot->balance_factor == direction) {
        // single rotation
        if (direction == LEFT)  {
            printf("rotating right!\n");
            avl_rotate_right(tree, rebalance_node);
            rebalance_node->balance_factor = EVEN;
            pivot->balance_factor = EVEN;
        }
        else {
            printf("rotating left!\n");
            avl_rotate_left(tree, rebalance_node);
            rebalance_node->balance_factor = EVEN;
            pivot->balance_factor = EVEN;
        }
    }
    else {
        // double rotation
        if (direction == LEFT) {
            printf("Double rotation, left then right!\n");
            bstnode* b = rebalance_node->left;
            bstnode* x = b->right;
            avl_rotate_left(tree, b);
            avl_rotate_right(tree, rebalance_node);

            if (x->balance_factor == direction) {
                rebalance_node->balance_factor = REVERSE_DIRECTION(direction);
                b->balance_factor = 0;
            } else if (x->balance_factor == EVEN) {
                rebalance_node->balance_factor = 0;
                b->balance_factor = 0;
            } else if (x->balance_factor == REVERSE_DIRECTION(direction)) {
                rebalance_node->balance_factor = 0;
                b->balance_factor = direction;
            }

                x->balance_factor = 0;
        }
        else {
            printf("Double rotation, right then left\n");
            bstnode* b = rebalance_node->right;
            bstnode* x = b->left;
            avl_rotate_right(tree, b);
            avl_rotate_left(tree, rebalance_node);

            if (x->balance_factor == direction) {
                rebalance_node->balance_factor = REVERSE_DIRECTION(direction);
                b->balance_factor = 0;
            } else if (x->balance_factor == EVEN) {
                rebalance_node->balance_factor = 0;
                b->balance_factor = 0;
            } else if (x->balance_factor == REVERSE_DIRECTION(direction)) {
                rebalance_node->balance_factor = 0;
                b->balance_factor = direction;
            }

            x->balance_factor = 0;
        }
    }

    printf("end of rebalance\n");
}


bst* avl_create(void)
{
    return bst_create();
}


void avl_node_delete(bst* tree, bstnode* todelete, node** path)
{
    bst_node_delete(tree, todelete, path);

}


int avl_delete(bst* tree, int value)
{
    node* path_tracker = init_update_tracker();
    bstnode* todelete = bst_find_node_and_path(tree, value, &path_tracker, -1);

    if (!todelete) {
        revert_rank_updates(path_tracker, +1);
        return 0;
    }

    node* search_path_stop = path_tracker;
    avl_node_delete(tree, todelete, &path_tracker);

    node* tracker_head = path_tracker; 

    printf("Tracked nodes...\n");
    node* temp = path_tracker;
    while (temp) {
        printf("%d (%d)\n", temp->treenode->value,
                temp->treenode->balance_factor);
        temp = temp->next;
    }

    // process nodes swapped during the delete process
    while (path_tracker && path_tracker != search_path_stop) {
        //int delete_direction = (value < path_tracker->treenode->value) ? LEFT : RIGHT;
        int delete_direction = path_tracker->direction;
        _avl_delete_balancing(tree, path_tracker->treenode, delete_direction);


        path_tracker = path_tracker->next;
    }

    // We'll need to process balance updates for each node that was touched
    // by the delete, starting at the bottom.
    int original_balance = -1;

    printf("Tracked nodes...\n");
    temp = path_tracker;
    while (temp) {
        printf("%d (%d)\n", temp->treenode->value,
                temp->treenode->balance_factor);
        temp = temp->next;
    }

    
    printf("\n\nBeginning Rebalance for Delete of %d\n", value);
    while (path_tracker && original_balance != EVEN) {
        // process balance updates (and rotations!)
        original_balance = path_tracker->treenode->balance_factor;
        //int delete_direction = (value < path_tracker->treenode->value) ? LEFT : RIGHT;
        int delete_direction = path_tracker->direction;
        _avl_delete_balancing(tree, path_tracker->treenode, delete_direction);

        path_tracker = path_tracker->next;
    }

    destroy_update_tracker(tracker_head);

    return 1;
}


void _avl_delete_balancing(bst* tree, bstnode* rebalance_node, int delete_direction)
{
        // process balance updates (and rotations!)
        printf("Current rebalance node %d (%d)\n", rebalance_node->value,
                rebalance_node->balance_factor);

        printf("Delete direction is %d\n", delete_direction);

        // Update balance factors for rebalance point
        if (rebalance_node->balance_factor == delete_direction)
            rebalance_node->balance_factor = EVEN;

        else if (rebalance_node->balance_factor == EVEN)
            rebalance_node->balance_factor = REVERSE_DIRECTION(delete_direction);

        else if (rebalance_node->balance_factor == REVERSE_DIRECTION(delete_direction)) {
            printf("we must rebalance!\n");
            avl_rebalance(tree, rebalance_node, REVERSE_DIRECTION(delete_direction));
        }

        printf("Rebalance node final state: %d (%d)\n", rebalance_node->value,
                rebalance_node->balance_factor);
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

    node* path_tracker = init_update_tracker();
    bstnode* insert_location = bst_find_node_and_path(tree, value, &path_tracker, 1);

    if (insert_location) {
        revert_rank_updates(path_tracker, -1);
        destroy_update_tracker(path_tracker);
        return 0;
    }

    bst_node_insert(tree, newnode, path_tracker);

    // find the point at which we need to rebalance the tree.
    // This will be either the root, or the closest node to the insert
    // that is unbalanced.
    node* tracker_head = path_tracker; // store pointer to head so we can destroy it
    while (path_tracker && path_tracker->treenode->balance_factor == EVEN) {
        path_tracker = path_tracker->next;
    }

    bstnode* rebalance_node = (path_tracker) ? path_tracker->treenode : tree->head;

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
    if (rebalance_node->balance_factor == insert_direction) 
        avl_rebalance(tree, rebalance_node, insert_direction);
    
    /*
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
    } */


    destroy_update_tracker(tracker_head);
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
