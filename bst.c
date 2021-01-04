/* bst.c
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bst.h"

void _traverse_and_count(bstnode* head, int* cnt)
{
    if (head == NULL) return;
    _traverse_and_count(head->left, cnt);
    (*cnt)++;
    _traverse_and_count(head->right, cnt);
}


int _count_children(bstnode* head)
{
    int cnt = 0;
    _traverse_and_count(head, &cnt);
    return cnt;
}

void bst_rotate_left(bst* tree, bstnode* center)
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


void bst_rotate_right(bst* tree, bstnode* center)
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


int bst_node_delete(bst* tree, bstnode* del_node, node** path_tracker)
{
    // if there's only one element in the tree, we'll just handle that
    // as a special case.
    if (tree->length == 1) {
        tree->head = NULL;
        tree->length--;
        free(del_node);
        return 1;
    }

    // we want to move the node to be deleted down the tree
    // until it has no right children
    while(del_node->right)
        bst_rotate_left(tree, del_node);

    // once it has no right children, we can "snip" it out
    // of the tree.
    if (del_node->left) {
        del_node->left->parent = del_node->parent;
    }

    int left_child = del_node->value < del_node->parent->value;
    if (del_node->value < del_node->parent->value) {
        del_node->parent->left = del_node->left;
        del_node->parent->rank = del_node->rank;
    } else {
        del_node->parent->right = del_node->left;
    }

    free(del_node);
    tree->length--;

    return 1;
}


bstnode* bstnode_create(int value)
{
    bstnode* newnode = malloc(sizeof(bstnode));
    if (!newnode){
        fprintf(stderr, "MEMORY ERROR in bst_insert. Mallocation failed.\n");
        exit(-1);
    }

    memset(newnode, 0, sizeof(bstnode));

    newnode->value = value;
    newnode->rank = 1;

    return newnode;
}


int bst_get_index(bst* tree, int value) 
{
    bstnode* current = tree->head;
    int index = 0;

    while (current)  {
        if (current->value == value) {
            return index + current->rank;
        }
        if (value < current->value){
            current = current->left;
        }
        else if (value > current->value) {
            index += current->rank;
            current = current->right;
        }        
    }

    return -1;
}


bstnode* bst_find_node_and_path(bst* tree, int value, node** path_tracker,
        int rank_update)
{
    bstnode* current = tree->head;
    bstnode* requested_node = NULL;

    while (current) {
        if (current->value == value) {
            requested_node = current;
            current = NULL;
        } 

        else if (value < current->value) {
            current->rank = current->rank + rank_update;
            track_update(path_tracker, current, LEFT);
            current = current->left;
        }

        else {
            track_update(path_tracker, current, RIGHT);
            current = current->right;
        }
    }

    return requested_node;
}


int bst_delete(bst* tree, int value)
{
    node* path_tracker = init_update_tracker();
    bstnode* todelete = bst_find_node_and_path(tree, value, &path_tracker, -1);

    if (!todelete) {
        revert_rank_updates(path_tracker, +1);
        return 0;
    }

    bst_node_delete(tree, todelete, &path_tracker);
    destroy_update_tracker(path_tracker);

    return 1;
}


void bst_node_insert(bst* tree, bstnode* newnode, node* path_tracker)
{
    bstnode* insert_location = path_tracker->treenode;
    if (path_tracker->direction == LEFT)
        insert_location->left = newnode;
    else // path->tracker->direction == RIGHT
        insert_location->right = newnode;

   newnode->parent = insert_location;
   tree->length++;
}



int bst_insert(bst* tree, int value)
{
    bstnode* newnode = bstnode_create(value);

    // if the tree doesn't have a root node
    // then just add this one as root and end.
    if (tree->length == 0) {
        tree->head = newnode;
        tree->length++;
        return 1;
    }

    node* path_tracker = init_update_tracker();

    bstnode* insert_location = bst_find_node_and_path(tree, value, &path_tracker, 1);

    // if a node of the specified value doesn't exist, insert it
    if (!insert_location) {
        bst_node_insert(tree, newnode, path_tracker);
        destroy_update_tracker(path_tracker);
        return 1;
    }

    // otherwise, roll back rank updates and don't insert anything
    revert_rank_updates(path_tracker, -1);
    destroy_update_tracker(path_tracker);
    return 0;
}


bstnode* bst_index(bst* tree, int index)
{
    int old_index = index;
    if (index > tree->length || index <= 0) return NULL;    
    bstnode* current = tree->head;
    while (current) {
        if (index == current->rank) return current;

        if (index < current->rank) current = current->left;
        else {
            index = index - current->rank;
            current = current->right;
        }
    }

    return NULL;
}


bstnode* bst_search(bst* tree, int value)
{
    bstnode* current = tree->head;
    while (current) {
        if (current->value == value)
            return current;

        current = current->value > value ? current->left : current->right;
    }

    return NULL;
}


bst* bst_create()
{
    bst* tree = malloc(sizeof(bst));
    if (!tree) {
        fprintf(stderr, "MEMORY ERROR in bst_create. Mallocation failed.\n");
        exit(-1);
    }

    memset(tree, 0, sizeof(bst));

    return tree;
}


void bst_clear(bst* tree)
{
    _traverse_and_free(tree->head);
}


void _traverse_and_free(bstnode* head)
{
    if (head == NULL) return;
    _traverse_and_free(head->left);
    _traverse_and_free(head->right);
    free(head);
}


void bst_destroy(bst* tree)
{
    free(tree);
}


void bst_clear_destroy(bst* tree)
{
    bst_clear(tree);
    bst_destroy(tree);
}
