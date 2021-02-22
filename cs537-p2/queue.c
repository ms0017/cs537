//////////////////////////////////////////////////////////////////////////////
//
// Title: Programming Assignment 2 
// Course: CS 537, Fall 2020
//
// Authors: Kunal Waghray and Sheriff Issaka
// Emails: kwaghray@wisc.edu and issaka@wisc.edu
// CS Login: issaka, kunal
// Lecturer's Name: Barton Miller
//
// Persons: NONE
// Online Sources: NONE
//
/////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <strings.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include "queue.h"
#include <semaphore.h> 

/*
 * Helper method that prints error message and exits failure if semaphore returns error 
 *
 * @param return_val return value of semaphore
 * 
 */
void SemaphoreErrorMessage(int return_val) {
	if (return_val == -1) {
		fprintf(stderr, "Error calling system semaphore function\n");
		exit(EXIT_FAILURE);
	}
}

/* 
 * Allocate a new Queue Struct dynamically and initialize with buffer
 * (array of char pointers) of len size. Also include synchronization and state 
 * variables in the struct. 
 *
 * @param size of buffer of queue
 * @return pointer to queue struct
 *
 */
Queue* CreateStringQueue(int size) {
	if(size <= 0){
		printf("ERROR: size needs to be greater than or equal to 1\n");
		exit(1);
	}

	// Allocate queue struct, stats struct, and buffer 
	struct Queue* queue = (struct Queue*) malloc(sizeof(struct Queue));
	if(!queue){
		printf("FATAL ERROR: malloc failed\n");
		exit(1);
	}

	struct Queue_Stats* qs = (struct Queue_Stats*) malloc(sizeof(struct Queue_Stats));
	if(!qs){
		printf("FATAL ERROR: malloc failed\n");
		exit(1);
	}

	queue-> buffer = malloc(size * sizeof(char*));
	if (queue->buffer == NULL) {
		fprintf(stderr, "FATAL ERROR: malloc failed\n");
		exit(EXIT_FAILURE);
	}
	
	// Initialize values of queue variables	
	init(qs);
	queue-> size = size; 	
	queue-> enqueueCount = 0;
	queue-> dequeueCount = 0;
	queue-> qs = qs;
	sem_init(&queue->empty,	0, size);
	sem_init(&queue->full,  0, 0);
	sem_init(&queue->mutex, 0, 1);

	return queue;
	
}

/* 
 * Increment pointer in circular buffer
 *
 * @param v current place in buffer
 * @param q queue of which to determine next index in
 * @ret new index in buffer
 *
 */
static int ModIncr(int v, Queue *q)
{
	return (v + 1) % q->size;
}

/*
 * Enqueue given string address in queue. Blocks if queue full
 * until space available. 
 * 
 * @param q queue to enqueue char pointer into
 * @param char pointer to enqueue
 *
 */
void EnqueueString(Queue *q, char *string)
{
	struct timeval start_t, end_t;
	int start_ret, end_ret;
	int sem_return_val = 0;

	start_ret = gettimeofday(&start_t, NULL);
	
	// Block till queue has space and start of mutex
	sem_return_val = sem_wait(&q->empty);
	SemaphoreErrorMessage(sem_return_val);
	sem_return_val = sem_wait(&q->mutex);
 	SemaphoreErrorMessage(sem_return_val);
	
	// Add item to buffer
	q->buffer [ q->last ] = string;
	q->last = ModIncr( q->last, q );
 	
	// Update queue space and end mutex 
	sem_return_val = sem_post(&q->mutex);
	SemaphoreErrorMessage(sem_return_val);
	sem_return_val = sem_post(&q->full);
	SemaphoreErrorMessage(sem_return_val);
	
	// Update queue stats (enqueue count and enqueue time)
	incr_enqueueCount(q->qs);
	end_ret = gettimeofday(&end_t, NULL);

	if (start_ret == -1 || end_ret == -1) {
		fprintf(stderr, "Error calling gettimeofday()\n");
		exit(EXIT_FAILURE);
	}
	update_enqueueTime_t(q->qs, start_t.tv_sec, start_t.tv_usec, end_t.tv_sec, 
				end_t.tv_usec);
}

/*
 * Remove pointer to string in queue and return this value. Block if queue is empty until 
 * string placed in queue. 
 *
 * @param q queue from which to retrieve string
 * @ret pointer to string dequeued
 *
 */
char* DequeueString(Queue *q)
{
	struct timeval start_t, end_t;
	int start_ret, end_ret;
	int sem_return_val = 0;

	start_ret = gettimeofday(&start_t, NULL);
	
	// Block till queue has space and start of mutex 
	sem_return_val = sem_wait(&q->full);
	SemaphoreErrorMessage(sem_return_val);
	sem_return_val = sem_wait(&q->mutex);
	SemaphoreErrorMessage(sem_return_val);
	
	// Remove item from buffer
	char* ret = q->buffer[q->first];
	q->first = ModIncr(q->first,q);
	
	// Update queue space and end mutex
	sem_return_val = sem_post(&q->mutex);
	SemaphoreErrorMessage(sem_return_val);
	sem_return_val = sem_post(&q->empty);
	SemaphoreErrorMessage(sem_return_val);

	// Update queue stats (enqueue count and enqueue time) 
	incr_dequeueCount(q->qs);
	end_ret = gettimeofday(&end_t, NULL);
        
	if (start_ret == -1 || end_ret == -1) {
		fprintf(stderr, "Error calling gettimeofday)\n");
		exit(EXIT_FAILURE);
	}	

	update_dequeueTime_t(q->qs, start_t.tv_sec, start_t.tv_usec, end_t.tv_sec, 
			end_t.tv_usec);
	return ret;
}

/*
 * Print statistics of the queue specified including enqueueCount, 
 * dequeueCount, enqueueTime, and dequeueTime
 *
 * @param q queue of which to print statistics
 *
 */
void PrintQueueStats(Queue *q) {
	PrintQueueStatsQS(q->qs);
}

