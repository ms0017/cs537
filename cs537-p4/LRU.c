//////////////////////////////////////////////////////////////////////////////
//
// Title: Programming Assignment 4
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


#include <stdio.h>
#include <stdlib.h>
#include "LRU.h"
#include "FIFO.h"

// Dequeue
long Dequeue_LRU(Queue* queue, HashMap* hash){
	if(EmptyQueue(queue)){
		fprintf(stderr, "Tried to remove item from empty LRU queue\n");
		exit(EXIT_FAILURE);
	}
	
	int return_val = -1;

	// if single node in queue
	if(queue->front == queue->back){
		return_val = queue -> front -> pageNum;
		
				
		free(queue->front);
		queue->front = NULL;
	        queue->back = NULL;	
	} else {
		// Change back
		Node* remove = queue->back;
		queue->back = queue->back->previous;
		// Back node shouldn't point to anything infront	
		if(queue->back){
			queue->back->next = NULL;
		}
		
       		return_val = remove -> pageNum;
		free(remove);
	}
	
	hash -> array[return_val] = NULL;

	queue->count--;	
	return return_val;
}

// Enqueue
void Enqueue_LRU(Queue* queue, HashMap* hash, unsigned long pageNum){
	
	if(FullQueue(queue)){
		// Remove back element from hash
		hash->array[queue->back->pageNum] = NULL;
		// Now remove from queue also
		Dequeue_LRU(queue, hash);
		printf("REACHED CAPACITY OF LRU\n");
		printQueue(queue);
		exit(EXIT_FAILURE);
	}
	// Make new node to add 
	Node* newNode = makeNode(pageNum);
	newNode->next = queue->front;
	if(EmptyQueue(queue)){
		queue->back = newNode;
		queue->front = newNode;
	}else{
		queue->front->previous = newNode;
		queue->front = newNode;
	}
	
	//Add page entry to HashMap
	hash->array[pageNum] = newNode;
	queue->count++;

}

void lruInit(Queue* queue, HashMap* hash, unsigned long pageNum){
	Node* page = hash->array[pageNum];
	// if cache miss
	if(page == NULL){
		Enqueue_LRU(queue, hash, pageNum);
	// bring page to front if not front (change forward pointer)
	}else if(page != queue->front){
		page->previous->next = page->next;
		// if page has valid next, set to previous (change back pointer)
		if(page->next){
			page->next->previous = page->previous;
		}
		// if page is back, change back
		if(page == queue->back){
			queue->back = page->previous; //// TODO: shouldn't this be queue -> next if it exists?
			queue->back->next = NULL;
		}
		// put new page in front of queue
		page->next = queue->front;
		page->previous = NULL;
		page->next->previous = page;	
		queue->front = page;
	}
}

