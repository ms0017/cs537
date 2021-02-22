//////////////////////////////////////////////////////////////////////////////
//
// Title: Programming Assignment 2 
// Course: CS 537, Fall 2020
//
// Authors: Kunal Waghray and Sheriff Issaka
// Emails: kwaghray@wisc.edu and issaka@wisc.edu
// NETIDs:   9079798139 (Sheriff), 9074418394 (Kunal)
// CS Login: issaka, kunal
// Lecturer's Name: Barton Miller
//
// Persons: NONE
// Online Sources: NONE
//
/////////////////////////////////////////////////////////////////////////////

#ifndef QUEUE_H
#define QUEUE_H

#include <semaphore.h>
#include "queue_stats.h"

typedef struct Queue {
        int first, last;
        int size;
        int enqueueCount, dequeueCount;
        char** buffer; // array of char*'s
        sem_t empty, full, mutex;
        Queue_Stats *qs;
} Queue;

Queue* CreateStringQueue(int size);

void EnqueueString(Queue *q, char *string);

char* DequeueString(Queue *q);

void PrintQueueStats(Queue *q);

#endif

