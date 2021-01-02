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


    printf("Testing avl_delete...\n");
    assert(avl_delete(tree, 30) == 0);
    assert(tree->length == 5);

    assert(avl_delete(tree, 6) == 1);
    assert(tree->length == 4);
    assert(avl_search(tree, 6) == NULL);

    check_rank(tree->head, 0);

    assert(avl_index(tree, 4));

    printf("avl_delete testing passed...\n");

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
        avl_insert(tree, x);
    }
    printf("\nPassed\n.");


    printf("Verifying node ranks...\n");
    check_rank(tree->head, 0);

    printf("The following list of numbers should be in sorted order: ");
    inorder_traverse(tree->head);
    printf("\n");

    printf("The following list of numbers should be the same as the above: ");
    for (int i = 1; i <= tree->length; i++)
        printf("%d ", avl_index(tree, i)->value);

    printf("\n");

    avl_clear_destroy(tree);

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
    avl_insert(test, 48);
    bstnode* b = avl_search(test, 15);
    bstnode* a = b->parent;
    avl_rotate_left(test, a);    
    inorder_traverse(test->head);
    printf("\n");
    check_rank(test->head, 0);

    avl_insert(test, 31);

    // insert 25 and do a right-rotation about 31.
    avl_insert(test, 25);
    b = avl_search(test, 31);
    a = b->parent;
    avl_rotate_right(test, a);

    inorder_traverse(test->head);
    printf("\n");
    check_rank(test->head, 0);

    // insert 41 and do a left-rotation about 31
    avl_insert(test, 41);
    b = avl_search(test, 31);
    a = b->parent;
    avl_rotate_left(test, a);

    inorder_traverse(test->head);
    printf("\n");
    check_rank(test->head, 0);


    // insert 49
    printf("%d\n",avl_insert(test, 49));

    // insert 36 and do a left-rotation about 31
    avl_insert(test, 36);
    b = avl_search(test, 31);
    a = b->parent;
    avl_rotate_left(test, a);

    inorder_traverse(test->head);
    printf("\n");
    check_rank(test->head, 0);


    printf("%d\n", test->head->value);
    printf("%d\t%d\n", test->head->left->value, test->head->right->value);
    printf("%d\t%d\n", test->head->left->left->value, test->head->left->right->value);
    printf("%d\t%d\n", test->head->right->left->value, test->head->right->right->value);

    b = avl_search(test, 5);
    avl_rotate_left(test, b->parent);

    inorder_traverse(test->head);
    printf("\n");
    check_rank(test->head, 0);

    printf("%d\n", test->head->value);
    printf("%d\t%d\n", test->head->left->value, test->head->right->value);
    printf("%d\t%d\n", test->head->left->left->value, test->head->left->right->value);
    printf("%d\t%d\n", test->head->left->left->left->value, test->head->left->left->right->value);
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

    inorder_traverse(test->head);
    printf("\n");

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

            inorder_traverse(test->head);
            printf("\n");
            check_rank(test->head, 0);
        }    
    }

    return 0;
}


int main(int argc, char **argv)
{

    if (argc < 2 || (argc > 1 && !strcmp(argv[1], "standard")))
        standard_tests();
    else if (argc > 1 && !strcmp(argv[1], "example"))
        example_tree();
    else if (argc > 1 && !strcmp(argv[1], "rstress"))
        rotation_stress(1000);

    return 0;
}
