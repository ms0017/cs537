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
// Online Sources: NONE 
//
/////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include "FIFO.h"
// Dequeue
unsigned long Dequeue_FIFO(Queue* queue, HashMap* hash){
	if(EmptyQueue(queue)){
		fprintf(stderr, "Tried to remove from empty FIFO queue\n");
		exit(EXIT_FAILURE);
	}

	// if single node in queue
	if(queue->front == queue->back){
		queue->front = NULL;
	}
	
	// Change back
	Node* remove = queue->back;
	queue->back = queue->back->previous;
	// Back node shouldn't point to anything infront	
	if(queue->back){
		queue->back->next = NULL;
	}
	unsigned long ret = remove->pageNum; 
	free(remove);
	queue->count--;

		
	hash -> array[ret] = NULL;

	return ret;
}

void remove_middle_element(Queue* queue, HashMap* hash, unsigned long pageNum) {
	Node* page = hash -> array[pageNum];

	if (page == NULL) { 
		printf("Tried to remove element from middle that is not in queue\n");
		exit(EXIT_FAILURE);
	}
	

	int removed = 0; 

	if (page == queue -> front) {
		queue -> front = page -> next;
		if (queue -> front != NULL) queue -> front -> previous = NULL;
		removed = 1;
	}

	if (page == queue -> back) {	
		queue -> back = page -> previous;
		if (queue -> back != NULL) queue -> back -> next = NULL;
		removed = 1;
	} 

	if (removed == 0) {
		Node* previous_node = page -> previous;
		Node* next_node = page -> next;

		if (previous_node != NULL) previous_node -> next = next_node;
		if (next_node != NULL)	next_node -> previous = previous_node;
	}

	page -> next = NULL;
	page -> previous = NULL;
	
	free(page);	
	hash -> array[pageNum] = NULL;
	queue -> count--;
}

// Enqueue
void Enqueue_FIFO(Queue* queue, HashMap* hash, unsigned long pageNum){
	
	if(FullQueue(queue)){
		hash->array[queue->back->pageNum] = NULL;
		Dequeue_FIFO(queue, hash);
		printQueue(queue);
		printf("Dequeued to make space size of queue is %d\n", queue -> numFrames);
		exit(EXIT_FAILURE);
	}

	// Make new node to add 
	Node* newNode = makeNode(pageNum);
	// Make front
	newNode->next = queue->front;
	
	if(EmptyQueue(queue)){
		queue->back = queue->front = newNode;
	}else{
		queue->front->previous = newNode;
		queue->front = newNode;
	}
	
	//Add page entry to HashMap
	hash->array[pageNum] = newNode;
	queue->count++;

}
int hashContains(HashMap* hash, unsigned long pageNum){
	return hash->array[pageNum] != NULL;
}

void FIFO(Queue* queue, HashMap* hash, unsigned long pageNum){
	if(!hashContains(hash, pageNum)){
		Enqueue_FIFO(queue, hash, pageNum);	
       }       
}


