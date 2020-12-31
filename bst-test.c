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
    printf("bst_insert of numbers passed\n");

    // FIXME: Come up with a better way to automate this test. Possibly generate
    // an array, and then use an "issorted" check upon it. This will do for now.
    printf("The following list of numbers should be in sorted order: ");
    inorder_traverse(tree->head);
    printf("\n");

    srand(time(NULL));
    int n = 100;
    printf("Inserting %d random numbers into the tree...\n", n);
    for (int i = 0; i < n; i++) {
        bst_insert(tree, rand() % 100);
    }
    printf("\n");

    printf("The following list of numbers should be in sorted order: ");
    inorder_traverse(tree->head);
    printf("\n");


    return 0;
}
