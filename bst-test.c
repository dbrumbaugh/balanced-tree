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
#include "bst-util.h"



int main1(int argc, char** argv)
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
    check_rank(tree->head, 0);

    assert(bst_index(tree, 1)->value == 0);
    assert(bst_index(tree, 2)->value == 1);
    assert(bst_index(tree, 3)->value == 5);
    assert(bst_index(tree, 4)->value == 6);
    assert(bst_index(tree, 5)->value == 15);

    printf("bst_index testing passed.\n");

    printf("Testing bst_insert w/ duplicate\n");
    assert(bst_insert(tree, 5) == 0);
    assert(tree->length == 5);
    assert(bst_index(tree, 1)->value == 0);
    assert(bst_index(tree, 2)->value == 1);
    assert(bst_index(tree, 3)->value == 5);
    assert(bst_index(tree, 4)->value == 6);
    assert(bst_index(tree, 5)->value == 15);
    printf("bst_insert duplicate passed.\n");


    assert(bst_search(tree, 5)->value == 5);
    assert(bst_search(tree, 20) == NULL);
    printf("bst_search testing passed.\n");


    printf("Testing bst_delete...\n");
    assert(bst_delete(tree, 30) == 0);
    assert(tree->length == 5);

    assert(bst_delete(tree, 6) == 1);
    assert(tree->length == 4);
    assert(bst_search(tree, 6) == NULL);

    check_rank(tree->head, 0);

    assert(bst_index(tree, 4));

    printf("bst_delete testing passed...\n");

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
    check_rank(tree->head, 0);

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

void testtree(void)
{
    bst* test = bst_create();
    int values[] = { 5, 6, 1, 0, 15, 48, 31, 25, 41, 43, 36, 0, 7, 95, 85  };

    for (int i=0; i<15; i++) {
            bst_insert(test, values[i]);
    }

    printf("traversing...\n");
    check_rank(test->head, 0);

    inorder_traverse(test->head);
    printf("\n");

    for (int i=0; i<15; i++) {
        int idx = bst_get_index(test, values[i]);
        assert(values[i] == bst_index(test, idx)->value);
    }


    assert(bst_get_index(test, 1000) == -1);


    for (int i=0; i<15; i++) {
        printf("deleting %d...\n", values[i]);
        bst_delete(test, values[i]);
        inorder_traverse(test->head);
        printf("\n");
        check_rank(test->head, 0);
        bst_insert(test, i);
    }
    
    bst_delete(test, 15);
    bst_delete(test, 48);
    bst_delete(test, 31);
    bst_delete(test, 25);
    inorder_traverse(test->head);
    printf("\n");

    bst_clear_destroy(test);
}


int main(int argc, char** argv)
{
    testtree();
}
