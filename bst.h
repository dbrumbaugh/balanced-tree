/* bst.h
 * A simple binary search tree. This one is unbalanced, and is just to serve as
 * a starting point for working on the balanced tree.
 *
 * Supports the basic operations of search, insert, and rank.
 *
 * Douglas Rumbaugh
 * 12/30/2020
 */

typedef struct BSTNode {
    int value;
    int rank;
    struct BSTNode* left;
    struct BSTNode* right;
} bstnode;

typedef struct BST {
    int length;
    bstnode* head;
} bst;


void bst_insert(bst* tree, int value);
bstnode* bst_search(bst* tree, int value);
bst* bst_create();
void bst_clear(bst* tree);
void bst_destroy(bst* tree);
void bst_clear_destroy(bst* tree);

void _traverse_and_free(bstnode* head);
