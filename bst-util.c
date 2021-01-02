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


void subtree_node_counts(bstnode* head, int verbose)
{
    if (head == NULL) return;

    subtree_node_counts(head->left, verbose);
    int left_nodes = 0;
    int right_nodes = 0;

    subtree_traverse(head->left, &left_nodes);
    subtree_traverse(head->right, &right_nodes);

    if (verbose) {
        printf("For node %d...\n", head->value);
        printf("\tleft subtree:\t%d\n", left_nodes);
        printf("\tright subtree:\t%d\n", right_nodes);
    }

    assert(abs(left_nodes - right_nodes) <= 1);

    subtree_node_counts(head->right, verbose);
}

int calculate_tree_height(bstnode* head)
{
    if (head == NULL) return 0;

    int left = calculate_tree_height(head->left);
    int right = calculate_tree_height(head->right);

    /*
    printf("(calculate_height) For node %d...\n", head->value);
    printf("\tLeft Height:\t%d\n", left);
    printf("\tRight Height:\t%d\n", right);
    */

    return 1 + MAX(left, right); // the height is the max height of its subtrees
}



void check_strict_balance(bstnode* head, int verbose)
{
    if (head == NULL) return;

    check_strict_balance(head->left, verbose);

    int left_height = calculate_tree_height(head->left);
    int right_height = calculate_tree_height(head->right);

    if (verbose) {
        printf("(check balance) For node %d...\n", head->value);
        printf("\tleft tree:\t%d\n", left_height);
        printf("\tright tree:\t%d\n", right_height);
        printf("\tbalance:\t%d\n", right_height - left_height);
    }

    assert(abs(left_height - right_height) <= 1);

    check_strict_balance(head->right, verbose);

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
