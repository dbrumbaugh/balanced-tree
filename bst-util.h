/*
 * bst-util.h
 * Some helper functions for testing bst funtionality.
 *
 * Douglas Rumbaugh
 * 01/01/2020
 *
 */

#pragma once

#include "bst.h"


void inorder_traverse(bstnode* head);
void subtree_traverse(bstnode* head, int* counter);
void check_rank(bstnode* head, int verbose);

