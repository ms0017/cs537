//////////////////////////////////////////////////////////////////////////////
//
// Title: Programming Assignment 2 
// Course: CS 537, Fall 2020
//
// Authors: Kunal Waghray and Sheriff Issaka
// Emails: kwaghray@wisc.edu and issaka@wisc.edu
// Lecturer's Name: Barton Miller
// CS Login: issaka, kunal
//
// Persons: NONE
// Online Sources: NONE
//
/////////////////////////////////////////////////////////////////////////////


#include <time.h>
#include <stdio.h>
#include "queue_stats.h"

 /*
 * 
 * Initializes queue values
 * @param qs  a pointer to the queue to be processed
 * 
 */
void init(Queue_Stats* qs) {
	qs-> enqueueTime = 0;
	qs-> dequeueTime = 0;
	qs-> dequeueCount = 0;
	qs-> enqueueCount = 0;
}


 /*
 * 
 * This method updates enqueue time cummulatively,
 * time being the amount of time elapsed per enqueue
 * 
 * @param qs  queue to be processed
 * @param start  clock start time
 * @param start_usec  clock start in ml seconds
 * @param end  clock end time
 * @param end_usec  clock end time in ml seconds
 */
void update_enqueueTime_t(Queue_Stats *qs, time_t start, suseconds_t start_usec, 
			time_t end, suseconds_t end_usec) {
        // cummulating enqueue times
	qs-> enqueueTime += (double) (end - start) + (double) (end_usec - start_usec) / 1000000; //difftime(end, start);
}

 /*
 * 
 * This method updates dequeue time cumulatively,
 * time being the amount of time elapsed per dequeue
 * 
 * @param qs  queue to be processed
 * @param start  clock start time
 * @param start_usec  clock start in ml seconds
 * @param end  clock end time
 * @param end_usec  clock end time in ml seconds
 */
void update_dequeueTime_t(Queue_Stats *qs, time_t start, suseconds_t start_usec, 
	       		time_t end, suseconds_t end_usec) {
        // cummulating enqueue times
	qs-> dequeueTime += (double) (end - start) + (double) (end_usec - start_usec) / 1000000; //difftime(end ,start);
}


 /*
 * 
 * This method counts the total number of dequeues
 *
 * @param qs  a pointer to the queue to be processed
 * 
 */
void incr_dequeueCount(Queue_Stats *qs) {
  	// updating count
	qs-> dequeueCount = qs-> dequeueCount + 1;
}


 /*
 * 
 * This method counts the total number of enqueues
 *
 * @param qs  a pointer to the queue to be processed
 * 
 */
void incr_enqueueCount(Queue_Stats *qs) {
    	// updating count
	qs-> enqueueCount = qs-> enqueueCount + 1;
}

 /*
 * 
 * Prints statistics for each file
 * 
 * @param qs  a pointer to the queue to be processed
 * 
 */
void PrintQueueStatsQS(Queue_Stats* qs) {
	fprintf(stderr, "******** Queue Stats ***********\n");
	fprintf(stderr, "Enqueue Time:  %lf\n",  qs-> enqueueTime);
	fprintf(stderr, "Dequeue Time:  %lf\n",  qs-> dequeueTime);
	fprintf(stderr, "Dequeue Count: %d\n", qs-> dequeueCount);
	fprintf(stderr, "Enqueue Count: %d\n", qs-> enqueueCount);
} 


