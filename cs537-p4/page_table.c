//////////////////////////////////////////////////////////////////////////////
//
// Title: Programming Assignment 4
// Course: CS 537, Fall 2020
//
// Authors: Kunal Waghray and Sheriff Issaka
// Emails: kwaghray@wisc.edu and issaka@wisc.edu
// CS Login: issaka, kunal
// Lecturer's Name: Barton Miller
//
// Persons: NONE
// Online Sources: Used code from Robert Williams as reference https://github.com/increscent/c_examples/blob/main/tsearch/main.c
//
/////////////////////////////////////////////////////////////////////////////

#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <search.h>
#include "page_table.h"
#include "global.h"

// This is the compare function for the binary tree
// Returns -1 if a <  b
// Returns 1  if a >  b
// Returns 0  if a == b
int compare(const void *a, const void *b)
{
    struct node *node_a = (struct node *)a;
    struct node *node_b = (struct node *)b;

    if (node_a->PID < node_b->PID) {
        return -1;
    } else if (node_a -> PID == node_b -> PID && node_a -> VPN < node_b -> VPN) {
        return -1;
    } else if (node_a->PID != node_b->PID || node_a->VPN != node_b->VPN) {
        return 1;
    } else {
        return 0;
    }
}

// Make node for binary tree
struct node *make_node(int PID, int VPN, int ppn_num)
{
    struct node *node;

    if ((node = malloc(sizeof(struct node))) == NULL)
        exit_with_message("Failed to malloc");

    node->PID = PID;
    node->VPN = VPN;
    node->ppn_num = ppn_num;

    return node;
}

// Add the node to the binary tree
void add_node(void **root, struct node *node)
{
    void *result;
    struct node *existing;

    if ((result = tsearch(node, root, compare)) == NULL) {
        // Failed to add the node
        exit_with_message("Insufficient memory");
    } else {
        // Check if an node with the same key already existed
        existing = *(struct node **)result;

        if (existing != node) {
            free(node);
        }
    }
}

// Find a node in the binary tree
// Returns NULL if no node is found
struct node *find_node(void **root, int PID, int VPN)
{
    void *result;
    struct node *node;
    struct node search_node;

    search_node.PID = PID;
    search_node.VPN = VPN;

    if ((result = tfind(&search_node, root, compare)) == NULL) {
        // No node found

        node = NULL;
    } else {
        // Node found
        node = *(struct node**)result;
    }

    return node;
}


// Delete a node in the binary tree
// If it is not found, do nothing
void delete_node(void **root, int PID, int VPN)
{
    struct node *node;

    if ((node = find_node(root, PID, VPN)) == NULL) {
        // Nothing to delete
    } else {
        tdelete(node, root, compare);
        // It's important to free the only after deleting it
        free(node);
    }
}

// Free node when destroying the tree
void free_node(void *ptr)
{
    struct node *node = ptr;
    free(node);
}

