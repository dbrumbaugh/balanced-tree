/*
 * bst.c
 * A simple binary search tree. This one is unbalanced, and is just to
 * serve as a starting point for working on the balanced tree.
 *
 * Supports the basic operations of search, insert, and rank.
 *
 * Douglas Rumbaugh
 * 12/30/2020
 */

#include <stdio.h>
#include <stdlib.h>
#include "bst.h"

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

    if (tree->length == 0) {
        tree->head = newnode;
        tree->length++;
        return 1;
    } 
    else {
        // Okay, this is a bit janky, but I need to track all of the nodes
        // whose ranks have been updated during insert so that, should an
        // insert not occur (due to a dupe entry), I can roll back those rank
        // changes.  Given that the length of an insert path is usually going
        // to be around lg n for a balanced tree, and even linear isn't "too"
        // bad, I'm allocating these nodes on the stack. If this structure is
        // going to be used for large amounts of data, it might be better to
        // allocate on the heap instead. But this saves me from needing to run
        // through and destroy the list at the end of each insert call.
        node* rank_updated = alloca(sizeof(node));
        if (!rank_updated) {
            fprintf(stderr, "MEMORY ERROR in bst_insert. Allocation failed.\n");
            exit(-1);
        }
        rank_updated->treenode = NULL;
        rank_updated->next = NULL;
        
        bstnode* current = tree->head;
        while (current){
            // We're going to just skip adding duplicates, rather than deal
            // with tracking multiple nodes with the same value. You'd only 
            // ever find one of them using a search anyway.
            if (value == current->value){
                // before returning, we need to run through and revert any
                // rank updates made by the algorithm during the attempted
                // insert.
                node* head = rank_updated;
                while(head && head->treenode) {
                    head->treenode->rank--;
                    head = head->next;
                }

                // no need to free this list, as it is stored on the stack
                // anyway.

                return 0;

            }


            if (value < current->value) { 
                // as we are going to insert the new node to the left of this
                // one, we will be growing it's left subtree by 1, and so we
                // need to update the rank.
                current->rank++;

                // Additionally, we need to track the fact that we have updated
                // it, so that we can revert the update if needed.
                if (rank_updated->treenode) {
                    node* new = alloca(sizeof(node));
                    if (!new) {
                        fprintf(stderr, "MEMORY ERROR in bst_insert. Allocation failed.\n");
                        exit(-1);
                    }
                    new->treenode = current;

                    // insert at the head of the list
                    new->next = rank_updated;
                    rank_updated = new;
                }
                else {
                    rank_updated->treenode = current;
                }


                if (current->left) {
                    current = current->left;
                }
                else {
                   current->left = newnode;
                   tree->length++;
                   return 1;
                } 
            }
            else if (value > current->value) {
                if (current->right)
                    current = current->right;
                else {
                    current->right = newnode;
                    tree->length++;
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
