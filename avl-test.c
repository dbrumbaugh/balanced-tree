/* 
 * avl-test.c
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
#include <string.h>

#include "avl.h"
#include "bst-util.h"


int standard_tests()
{

    // test the creation of an empty tree. The length should be zero,
    // and the head of the tree should be NULL.
    bst* tree = avl_create();
    assert(!tree->head);       // the head of the empty avl should be NULL
    assert(tree->length == 0); // the length of an empty avl should be 0
    printf("avl_create passed!\n");

    // searching an empty tree should return NULL
    assert(avl_search(tree, 8) == NULL);
    assert(avl_search(tree, 0) == NULL);
    printf("avl_search on empty passed!\n");


    // insert some elements into the tree
    avl_insert(tree, 5);
    avl_insert(tree, 6);
    avl_insert(tree, 1);
    avl_insert(tree, 0);
    avl_insert(tree, 15);

    printf("%d\n", tree->length);
    assert(tree->length == 5);
    printf("avl_insert of numbers passed.\n");

    printf("Verifying node ranks...\n");
    check_rank(tree->head, 0);

    assert(avl_index(tree, 1)->value == 0);
    assert(avl_index(tree, 2)->value == 1);
    assert(avl_index(tree, 3)->value == 5);
    assert(avl_index(tree, 4)->value == 6);
    assert(avl_index(tree, 5)->value == 15);

    printf("avl_index testing passed.\n");

    printf("Testing avl_insert w/ duplicate\n");
    assert(avl_insert(tree, 5) == 0);
    assert(tree->length == 5);
    assert(avl_index(tree, 1)->value == 0);
    assert(avl_index(tree, 2)->value == 1);
    assert(avl_index(tree, 3)->value == 5);
    assert(avl_index(tree, 4)->value == 6);
    assert(avl_index(tree, 5)->value == 15);
    printf("avl_insert duplicate passed.\n");


    assert(avl_search(tree, 5)->value == 5);
    assert(avl_search(tree, 20) == NULL);
    printf("avl_search testing passed.\n");


    printf("Testing avl_delete_slow...\n");
    assert(avl_delete_slow(&tree, 30) == 0);
    assert(tree->length == 5);

    assert(avl_delete_slow(&tree, 6) == 1);
    assert(tree->length == 4);
    assert(avl_search(tree, 6) == NULL);

    check_rank(tree->head, 0);

    assert(avl_index(tree, 4));

    check_strict_balance(tree->head, 0);

    printf("avl_delete_slow testing passed...\n");

    printf("Verifying BST ordering...\n");
    check_bst_ordering(tree);
    printf("passed!\n");

    srand(time(NULL));
    int n = 1000;
    printf("Inserting %d random numbers into the tree...\n", n);
    for (int i = 0; i < n; i++) {
        int x = rand() % n;
        avl_insert(tree, x);
        check_strict_balance(tree->head, 0);
    }
    printf("\nPassed\n.");

    printf("Verifying BST ordering...\n");
    check_bst_ordering(tree);
    printf("passed!\n");


    printf("Verifying node ranks...\n");
    check_rank(tree->head, 0);
    printf("Passed!\n");

    printf("Validating balance...\n");
    check_strict_balance(tree->head, 0);
    printf("passed!\n");

    printf("Verifying BST Indexing.\n");
    check_bst_indexing(tree);
    printf("passed!\n");


    srand(time(NULL));
    int m = 1000;
    printf("Deleting %d random numbers from the tree...\n", m);
    for (int i = 0; i < m; i++) {
        int x = rand() % n;
        bstnode* node = bst_search(tree, x);
        if (!node || (!node->right && !node->left))
            avl_delete_slow(&tree, x);
    }

    check_strict_balance(tree->head, 0);
    printf("\nPassed\n.");


    avl_clear_destroy(tree);

    return 0;
}


int double_rot() 
{
    bst* test = avl_create();
    avl_insert(test, 5);
    avl_insert(test, 6);
    avl_insert(test, 1);
    avl_insert(test, 0);
    avl_insert(test, 15);
    avl_insert(test, 527);
    avl_insert(test, 144);
    avl_insert(test, 159);

    
    avl_insert(test, 98);
    avl_insert(test, 395);

    check_strict_balance(test->head, 1);

    avl_clear_destroy(test);
    return 0;
}


int example_tree()
{
    bst* test = avl_create();
    avl_insert(test, 5);
    avl_insert(test, 6);
    avl_insert(test, 1);
    avl_insert(test, 0);
    avl_insert(test, 15);

    inorder_traverse(test->head); 
    printf("\n");

    // insert 48, and then do a left-rotation about 15
    printf("should rotate left\n");
    avl_insert(test, 48);
    bstnode* b = avl_search(test, 15);
    bstnode* a = b->parent;

    //avl_rotate_left(test, a);    
    

    inorder_traverse(test->head);
    printf("\n");
    check_rank(test->head, 0);

    printf("shouldn't rotate\n");
    avl_insert(test, 31);

    // insert 25 and do a right-rotation about 31.
    printf("should rotate right\n");
    avl_insert(test, 25);
    b = avl_search(test, 31);
    a = b->parent;

    //avl_rotate_right(test, a);

    inorder_traverse(test->head);
    printf("\n");
    check_rank(test->head, 0);

    // insert 41 and do a left-rotation about 31
    printf("should rotate left\n");
    avl_insert(test, 41);
    b = avl_search(test, 31);
    a = b->parent;

    //avl_rotate_left(test, a);

    inorder_traverse(test->head);
    printf("\n");
    check_rank(test->head, 0);


    // insert 49
    
    printf("shouldn't rotate\n");
    avl_insert(test, 49);

    /*
    // insert 36 and do a left-rotation about 31
    printf("should rotate left.\n");
    avl_insert(test, 36);
    b = avl_search(test, 31);
    a = b->parent;
    //avl_rotate_left(test, a);

    inorder_traverse(test->head);
    printf("\n");
    check_rank(test->head, 0);
    */

    printf("checking balance...\n");
    check_strict_balance(test->head, 0);
    printf("done...\n");


    printf("delete testing...\n");


    avl_delete_slow(&test, 1);
    check_strict_balance(test->head, 0);


    avl_delete_slow(&test, 36);
    avl_delete_slow(&test, 6);
    avl_delete_slow(&test, 41);
    avl_delete_slow(&test, 49);

    check_strict_balance(test->head, 0);
    /*
    printf("delete 42\n");
    avl_delete_slow(test, 42);
    check_rank(test->head, 0);
    inorder_traverse(test->head);

    printf("delete 36\n");
    avl_delete_slow(test, 36);
    check_rank(test->head, 0);
    inorder_traverse(test->head);

    printf("delete 1000\n");
    avl_delete_slow(test, 1000);
    check_rank(test->head, 0);
    inorder_traverse(test->head);

    printf("delete 0\n");
    avl_delete_slow(test, 0);
    check_rank(test->head, 0);
    inorder_traverse(test->head);
    */


    avl_clear_destroy(test);
    return 0;
}


int rotation_stress(int n)
{
    // stress test left and right rotation by
    // inserting a bunch of random elements,
    // then rotating a bunch of random elements
    // and ensuring that the BST structure and
    // ranks (left subtree counts) are properly
    // maintained.
    //
    bst* test = avl_create();

    srand(time(NULL));
    printf("Inserting %d random numbers into the tree...\n", n);
    for (int i = 0; i < n; i++) {
        int x = rand() % n;
        avl_insert(test, x);
    }

    check_bst_ordering(test);

    printf("Passed\n");
    
    // select several random nodes from the tree (by index).  if the selected
    // node is in the tree rotate about that node and verify BST ordering as
    // well as ranks.
    for (int j=0; j<200; j++) {
        int x = rand() % test->length;
        bstnode* rot_node = avl_index(test, x);
        if (rot_node) {
            int dir = rand() % 2;
            if (dir) {
                printf("rotating left...\n");
                avl_rotate_left(test, rot_node);
                printf("done rotating...\n");
            } 
            else {
                printf("rotating right..\n");
                avl_rotate_right(test, rot_node);
                printf("done rotating...\n");
            }

            check_bst_ordering(test);
            check_bst_indexing(test);
            check_rank(test->head, 0);
        }    
    }

    printf("Passed\n");

    return 0;
}


int main(int argc, char **argv)
{

    if (argc < 2 || (argc > 1 && !strcmp(argv[1], "standard")))
        standard_tests();
    else if (argc > 1 && !strcmp(argv[1], "example"))
        example_tree();
    else if (argc > 1 && !strcmp(argv[1], "rstress"))
        rotation_stress(10000);
    else if (argc > 1 && !strcmp(argv[1], "rot"))
        double_rot();

    return 0;
}
