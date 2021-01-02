/*
 * bst-util.c
 * Some helper functions for testing bst funtionality.
 *
 * Douglas Rumbaugh
 * 01/01/2020
 *
 */
#include <stdlib.h>
#include <stdio.h>

#include "bst-util.h"


void inorder_traverse(bstnode* head)
{
    if (head == NULL) return;

    inorder_traverse(head->left);
    printf("%d ", head->value);
    inorder_traverse(head->right);
}

void subtree_traverse(bstnode* head, int* counter)
{
    if (head == NULL) return;
    subtree_traverse(head->left, counter);
    (*counter)++;
    subtree_traverse(head->right, counter);
}
    

void check_rank(bstnode* head, int verbose)
{
    // for each node in the tree, we want to traverse the left subtree and count
    // up the number of nodes, and then compare the result to the rank of the node.
    if (head == NULL) return;

    check_rank(head->left, verbose);
    
    int calculated_rank = 1;
    subtree_traverse(head->left, &calculated_rank);
    if (verbose) {
        printf("For node %d\n", head->value);
        printf("Calculated Rank: %d\nStored Rank: %d\n", calculated_rank, head->rank);
    }
    assert(calculated_rank = head->rank);

    check_rank(head->right, verbose);
}
