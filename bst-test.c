/* 
 * bst-test.c
 * A simple test suite for the basic binary search tree.
 *
 * Douglas Rumbaugh
 * 12/30/2020
 *
 */

#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <time.h>
#include "bst.h"

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
    

void check_rank(bstnode* head)
{
    // for each node in the tree, we want to traverse the left subtree and count
    // up the number of nodes, and then compare the result to the rank of the node.
    if (head == NULL) return;

    check_rank(head->left);

    int calculated_rank = 1;
    subtree_traverse(head->left, &calculated_rank);
    printf("For node %d\n", head->value);
    printf("Calculated Rank: %d\nStored Rank: %d\n", calculated_rank, head->rank);

    check_rank(head->right);
}


int main(int argc, char** argv)
{

    // test the creation of an empty tree. The length should be zero,
    // and the head of the tree should be NULL.
    bst* tree = bst_create();
    assert(!tree->head);       // the head of the empty bst should be NULL
    assert(tree->length == 0); // the length of an empty bst should be 0
    printf("bst_create passed!\n");

    // searching an empty tree should return NULL
    assert(bst_search(tree, 8) == NULL);
    assert(bst_search(tree, 0) == NULL);
    printf("bst_search on empty passed!\n");


    // insert some elements into the tree
    bst_insert(tree, 5);
    bst_insert(tree, 6);
    bst_insert(tree, 1);
    bst_insert(tree, 0);
    bst_insert(tree, 15);

    assert(tree->length == 5);
    printf("bst_insert of numbers passed.\n");

    printf("Verifying node ranks...\n");
    check_rank(tree->head);

    assert(bst_index(tree, 1)->value == 0);
    assert(bst_index(tree, 2)->value == 1);
    assert(bst_index(tree, 3)->value == 5);
    assert(bst_index(tree, 4)->value == 6);
    assert(bst_index(tree, 5)->value == 15);

    printf("bst_index testing passed.\n");


    assert(bst_search(tree, 5)->value == 5);
    assert(bst_search(tree, 20) == NULL);
    printf("bst_search testing passed.\n");

    // FIXME: Come up with a better way to automate this test. Possibly generate
    // an array, and then use an "issorted" check upon it. This will do for now.
    printf("The following list of numbers should be in sorted order: ");
    inorder_traverse(tree->head);
    printf("\n");

    srand(time(NULL));
    int n = 1000;
    printf("Inserting %d random numbers into the tree...\n", n);
    for (int i = 0; i < n; i++) {
        int x = rand() % 100;
        printf("%d ", x);
        bst_insert(tree, x);
    }
    printf("\nPassed\n.");


    printf("Verifying node ranks...\n");
    check_rank(tree->head);

    printf("The following list of numbers should be in sorted order: ");
    inorder_traverse(tree->head);
    printf("\n");

    printf("The following list of numbers should be the same as the above: ");
    for (int i = 1; i <= tree->length; i++)
        printf("%d ", bst_index(tree, i)->value);

    printf("\n");

    bst_clear_destroy(tree);

    return 0;
}
