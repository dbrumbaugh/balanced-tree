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

void bst_insert(bst* tree, int value)
{
    bstnode* newnode = malloc(sizeof(bstnode));
    if (!newnode){
        fprintf(stderr, "MEMORY ERROR in bst_insert. Mallocation failed.\n");
        exit(-1);
    }
    newnode->value = value;
    newnode->left = NULL;
    newnode->right = NULL;
    newnode->rank = 0;

    if (tree->length == 0) {
        tree->head = newnode;
        tree->length++;
    } 
    else {
        bstnode* current = tree->head;
        while (current){
            // We're going to just skip adding duplicates, rather than deal
            // with tracking multiple nodes with the same value. You'd only 
            // ever find one of them using a search anyway.
            if (value == current->value)
                return; 


            if (value < current->value) { 
                if (current->left) {
                    current = current->left;
                }
                else {
                   current->left = newnode;
                   tree->length++;
                   return;
                } 
            }
            else if (value > current->value) {
                if (current->right)
                    current = current->right;
                else {
                    current->right = newnode;
                    tree->length++;
                    return;
                }
            }
        }
    }
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
