#ifndef FIFO_H
#define FIFO_H


#include <stdio.h>
#include <stdlib.h>
#include "global.h"

unsigned long Dequeue_FIFO(Queue* queue, HashMap* hash);

void remove_middle_element(Queue* queue, HashMap* hash, unsigned long pageNum);

void Enqueue_FIFO(Queue* queue, HashMap* hash, unsigned long pageNum);

int hashContains(HashMap* hash, unsigned long pageNum);

void FIFO(Queue* queue, HashMap* hash, unsigned long pageNum);

void printNode(Node* node);

void printQueue(Queue* queue);




#endif


