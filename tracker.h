#pragma once
#include "bst.h"

node* init_update_tracker();
void revert_rank_updates(node* head, int direction);
void track_update(node** updated_nodes, bstnode* tracked_node, int direction);
void destroy_update_tracker(node* update_tracker);
