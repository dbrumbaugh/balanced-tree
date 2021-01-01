#include "bst.h"

typedef struct Node {
    bstnode* treenode;
    struct Node* next;
} node;

node* init_update_tracker();
void revert_rank_updates(node* head, int direction);
node* track_update(node* updated_nodes, bstnode* tracked_node);
void destroy_update_tracker(node* update_tracker);
