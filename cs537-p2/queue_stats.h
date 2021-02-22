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

#ifndef QUEUE_STATS
#define QUEUE_STATS

#include <sys/time.h>

typedef struct Queue_Stats {
        double enqueueTime; 
        double dequeueTime;
        int dequeueCount;
        int enqueueCount;
} Queue_Stats;

void init(Queue_Stats* qs);

void update_enqueueTime(Queue_Stats* qs, clock_t start, clock_t end);

void update_dequeueTime(Queue_Stats* qs, clock_t start, clock_t end);

void update_enqueueTime_t(Queue_Stats *qs, time_t start, suseconds_t start_usec,
                        time_t end, suseconds_t end_usec);

void update_dequeueTime_t(Queue_Stats *qs, time_t start, suseconds_t start_usec,
                        time_t end, suseconds_t end_usec);

void incr_dequeueCount(Queue_Stats* qs);

void incr_enqueueCount(Queue_Stats* qs);

void PrintQueueStatsQS(Queue_Stats* qs);

#endif
