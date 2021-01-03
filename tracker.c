#include "tracker.h"
#include <stdlib.h>
#include <stdio.h>


node* init_update_tracker()
{
    node* head = malloc(sizeof(node));
    
    if (!head) {
        fprintf(stderr, "MEMORY ERROR in init_update_tracker. Mallocation failed.\n");
        exit(-1);
    }

    head->next = NULL;
    head->treenode = NULL;

    return head;
}


void revert_rank_updates(node* head, int direction) {
    // these nodes are stored on the stack, so they'll be cleaned up
    // when the calling function returns. No need to free them here.
    while(head && head->treenode) {
        if (head->direction == LEFT)
            head->treenode->rank = head->treenode->rank + direction;
        head = head->next;
    }
}

void track_update(node** updated_nodes, bstnode* tracked_node, int direction)
{
    if ((*updated_nodes)->treenode) {
        node* new = malloc(sizeof(node));
        if (!new) {
            fprintf(stderr, "MEMORY ERROR in track_update. Mallocation failed.\n");
            exit(-1);
        }

        new->treenode = tracked_node;
        new->next = *updated_nodes;
        new->direction = direction;

        *updated_nodes = new;
    }
    else
    {
        (*updated_nodes)->treenode = tracked_node;
        (*updated_nodes)->next = NULL;
        (*updated_nodes)->direction = direction;
    }
}


void destroy_update_tracker(node* update_tracker)
{
    node* current = update_tracker;
    node* next = current->next;

    while (current) {
        next = current->next;
        free(current);
        current = next;
    }
}
