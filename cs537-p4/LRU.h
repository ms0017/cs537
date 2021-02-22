#ifndef LRU_H
#define LRU_H



// Node with Doubly Linked List
//typedef struct Node {
//        struct Node *previous, *next;
//        unsigned long pageNum;
//}Node;

// Queue
//typedef struct Queue{
//        int count;
//        int numFrames;
//        Node *front, *back;
//}Queue;

// HashMap of Queue Nodes
//typedef struct HashMap{
//        int capacity;
//        Node** array;
//}HashMap;

#include "global.h"

long Dequeue_LRU(Queue* queue, HashMap* hash);

void Enqueue_LRU(Queue* queue, HashMap* hash, unsigned long pageNum);

void lruInit(Queue* queue, HashMap* hash, unsigned long pageNum);

void printNode(Node* node);

void printQueue(Queue* queue);


#endif

