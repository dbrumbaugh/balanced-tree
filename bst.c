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

int _delete_node(bst* tree, bstnode* del_node)
{
    int head_node = del_node->parent == NULL;
    int left_child = !head_node && del_node->value < del_node->parent->value;

    if (!del_node->right) {
        if (head_node) {
            tree->head = del_node->left;
        }
        else if (left_child) {
            del_node->parent->left = del_node->left;
        }
        else {
            del_node->parent->right = del_node->left;
        }

        if (del_node->left)
            del_node->left->parent = del_node->parent;

        free(del_node);
        return 1;
    }

    bstnode* r = del_node->right;

    if (!r->left) {
        r->left = del_node->left;
        r->rank = del_node->rank;

        if (head_node)
            tree->head = r;
        else if (left_child)
            del_node->parent->left = r;
        else
            del_node->parent->right = r;

        r->parent = del_node->parent;
        free(del_node);
        return 1;
    }

    bstnode* s = r->left;
    while (s->left) {
        r = s;
        s = r->left;
    }


    s->left = del_node->left;
    if (del_node->left)
        del_node->left->parent = s;

    r->left = s->right;
    if (s->right)
        s->right->parent = r;
    r->rank = _count_children(s->right) + 1;

    s->right = del_node->right;
    if (del_node->right)
        del_node->right->parent = s;

    if (head_node) {
        tree->head = s;
    }
    else if (left_child) 
        del_node->parent->left = s;
    else
        del_node->parent->right = s;

    s->parent = del_node->parent;
    s->rank = del_node->rank;

    free(del_node);
    return 1;
}


node* _init_update_tracker()
{
    node* head = malloc(sizeof(node));
    
    if (!head) {
        fprintf(stderr, "MEMORY ERROR in init_update_tracker. Mallocation failed.\n");
        exit(-1);
    }

    head->next = NULL;
    head->treenode = NULL;

    return head;
}


void _revert_rank_updates(node* head, int direction) {
    // these nodes are stored on the stack, so they'll be cleaned up
    // when the calling function returns. No need to free them here.
    while(head && head->treenode) {
        head->treenode->rank = head->treenode->rank + direction;
        head = head->next;
    }
}


node* _track_update(node* updated_nodes, bstnode* tracked_node)
{
    if (updated_nodes->treenode) {
        node* new = malloc(sizeof(node));
        if (!new) {
            fprintf(stderr, "MEMORY ERROR in track_update. Mallocation failed.\n");
            exit(-1);
        }

        new->treenode = tracked_node;
        new->next = updated_nodes;
        return new;
    }
    else
    {
        updated_nodes->treenode = tracked_node;
        updated_nodes->next = NULL;
        return updated_nodes;
    }
}


void _destroy_update_tracker(node* update_tracker)
{
    node* current = update_tracker;
    node* next = current->next;

    while (current) {
        next = current->next;
        free(current);
        current = next;
    }
}



int bst_delete(bst* tree, int value)
{
    node* rank_tracker = _init_update_tracker();

    bstnode* current = tree->head;
    bstnode* todelete = NULL;
    while (current) {
        if (current->value == value) {
            todelete = current;
            current = NULL;
        } 

        else if (current->value > value) {
            current->rank--;
            rank_tracker = _track_update(rank_tracker, current);
            current = current->left;
        }

        else {
            current = current->right;
        }
    }

    if (!todelete) {
        printf("reverting...\n");
        _revert_rank_updates(rank_tracker, +1);
        return 0;
    }

    _delete_node(tree, todelete);
    tree->length--;
    _destroy_update_tracker(rank_tracker);
    return 1;
}

int bst_insert(bst* tree, int value)
{
    bstnode* newnode = malloc(sizeof(bstnode));
    if (!newnode){
        fprintf(stderr, "MEMORY ERROR in bst_insert. Mallocation failed.\n");
        exit(-1);
    }
    newnode->value = value;
    newnode->left = NULL;
    newnode->right = NULL;
    newnode->rank = 1;
    newnode->parent = NULL;

    if (tree->length == 0) {
        tree->head = newnode;
        tree->length++;
        return 1;
    } 
    else {
        node* update_tracker = _init_update_tracker();
        
        bstnode* current = tree->head;
        while (current){
            // We're going to just skip adding duplicates, rather than deal
            // with tracking multiple nodes with the same value. You'd only 
            // ever find one of them using a search anyway.
            if (value == current->value){
                // before returning, we need to run through and revert any
                // rank updates made by the algorithm during the attempted
                // insert.
                _revert_rank_updates(update_tracker, -1);
                _destroy_update_tracker(update_tracker);

                return 0;
            }

            if (value < current->value) { 
                // as we are going to insert the new node to the left of this
                // one, we will be growing it's left subtree by 1, and so we
                // need to update the rank.
                current->rank++;

                // Additionally, we need to track the fact that we have updated
                // it, so that we can revert the update if needed.
                update_tracker = _track_update(update_tracker, current);

                if (current->left) {
                    current = current->left;
                }
                else {
                   current->left = newnode;
                   newnode->parent = current;
                   tree->length++;
                   _destroy_update_tracker(update_tracker);
                   return 1;
                } 
            }
            else if (value > current->value) {
                if (current->right)
                    current = current->right;
                else {
                    current->right = newnode;
                    newnode->parent = current;
                    tree->length++;
                    _destroy_update_tracker(update_tracker);
                    return 1;   
                }
            }       
        }
    }

    ASSERT_NOT_REACHED();
}


bstnode* bst_index(bst* tree, int index)
{
    int old_index = index;
    if (index > tree->length || index <= 0) return NULL;    
    bstnode* current = tree->head;
    while (current) {
        /*printf("At node %d\n", current->value);
        printf("\tcurrent rank: %d\n", current->rank);
        printf("\tcurrent index: %d\n", index); */
        if (index == current->rank) return current;

        if (index < current->rank) current = current->left;
        else {
            index = index - current->rank;
            current = current->right;
        }
    }

    //printf("Not found for index %d\n", old_index);
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
    tree->head = NULL;
    tree->length = 0;

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
