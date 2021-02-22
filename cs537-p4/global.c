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
#include <string.h>
#include "global.h"

void exit_with_message(char* input) {
	fprintf(stderr, "%s\n", input);
	exit(EXIT_FAILURE);
}

// Creates new Node
Node* makeNode(unsigned long pageNum){
        Node* node = (Node*)malloc(sizeof(Node));
	if(node == NULL){
		exit_with_message("MALLOC FAILED");
	}

        node->pageNum = pageNum;
        node->previous = node->next = NULL;
        return node;
}

// Creates new Queue
Queue* makeQueue(int numFrames){
        Queue* queue = (Queue*)malloc(sizeof(Queue));
	if(queue == NULL){
		exit_with_message("MALLOC FAILED");
	}

        queue-> count = 0;
        queue->front = NULL;
       	queue->back = NULL;
        queue->numFrames = numFrames;
        return queue;
}

// Create HashMap
HashMap* makeHash(int capacity){
        HashMap* newHash = (HashMap*)malloc(sizeof(HashMap));
        if(newHash == NULL){
		exit_with_message("MALLOC FAILED");
	}

	newHash->capacity = capacity;
        newHash->array = (Node**)malloc(newHash->capacity * sizeof(Node*));
        if(newHash->array == NULL){
		exit_with_message("MALLOC FAILED");
	}

	// clear array
        for(int i = 0; i < newHash->capacity; i++){
                newHash->array[i] = NULL;
        }

        return newHash;
}

// Check if Queue is full
int FullQueue(Queue* queue){
        return queue->count == queue->numFrames;
}

// Check if queue is empty
int EmptyQueue(Queue* queue){
        return queue->back == NULL;
}

void printNode(Node* node){
        if(node == NULL){
                return;
        }
        printf("[%ld]", node->pageNum);
        printNode(node->next);
}

void printReverse(Node* node) {
	if(node == NULL) return;
	printf("[%ld]", node -> pageNum);
	printReverse(node->previous);
}

void printQueue(Queue* queue){
        printf("****Queue****\n");
       
      	if (queue ->front != NULL && (queue -> front -> previous != NULL )) {
		printf("THAT's AND ISSUES front of queue prev not null or back not null\n");
		exit(EXIT_FAILURE);
	}

	if (queue -> front == NULL && queue -> back != NULL) {
		printf("BAck not null front is\n");
		exit(EXIT_FAILURE);
	}

	printNode(queue->front);
	printf("\n");
	printReverse(queue->back);
        printf("\n****end of queue****\n");
}

Proc_Queue* makeProcQueue(){
	Proc_Queue* pqueue = (Proc_Queue*)malloc(sizeof(Proc_Queue));
	if(pqueue == NULL){
		exit_with_message("MALLOC FAILED");
	}
	pqueue->front = pqueue->back = NULL;
	pqueue->count = 0;
	return pqueue;
}

int pQueueIsEmpty(Proc_Queue* q){
	return q->back == NULL;
}

void pEnqueue(Proc_Queue* q, Process* p){
	p->next = q->front;
	

	if(pQueueIsEmpty(q)){
		q->back = q->front = p;
	}else{
		q->front->previous = p;
		q->front = p;
	}
	q->count++;
}

Process* pDequeue(Proc_Queue* q){

	if (q -> front == NULL) {
		fprintf(stderr, "Dequeue from empty queue\n");
		exit(EXIT_FAILURE);
	}

	if(q->front == q->back){
		q->front = NULL;
	}
	
	Process* remove = q->back;
	q->back = q->back->previous;
	
	if(q->back){
		q->back->next = NULL;
	}
	
	q->count--;

	remove -> next = NULL;
	remove -> previous = NULL;

	return remove;
}

Process* proc_Peek(Proc_Queue* q){
	return q->back;
}

void updateLast(Process* proc, long index, long VPN){
	proc->last = index;
	proc->last_VPN = VPN;
}
// Create HashMap
Proc_HashMap* makeProcHash(int capacity){
	Proc_HashMap* newHash = (Proc_HashMap*)malloc(sizeof(Proc_HashMap));
	if(newHash == NULL){
		exit_with_message("MALLOC FAILED");
	}
	newHash->capacity = capacity;
	newHash->array = (Process**)malloc(newHash->capacity * sizeof(Process*));
	if(newHash->array == NULL){
		exit_with_message("MALLOC FAILED");
	}
	// clear array
	for(int i = 0; i < newHash->capacity; i++){
		newHash->array[i] = NULL;
	}

	return newHash;
}


Process* makeProcess(unsigned long PID, long index, Proc_HashMap* hash){
	Process* proc = (Process*)malloc(sizeof(Process));
	if(proc == NULL){
		exit_with_message("MALLOC FAILED");
	}
	proc->PID = PID;
	proc-> priority = index;
	proc->time = 0;
	proc -> next = NULL;
	proc -> previous = NULL;
	hash->array[PID] = proc;
	proc -> last_VPN = -1;

	return proc;
}

Process* pop(Process** head){
	Process* remove = *head;
	(*head) = (*head)->next;
	return remove;
}

Process* push_proc(Process* head, Process* temp){
	Process* start = head;
	if (head == NULL) {
		head = temp;
	} else if(head->priority > temp -> priority){
		temp->next = head;
		head = temp;
	}else{
		while(start->next != NULL && start->next->priority < temp -> priority){
			start = start->next;
		}
		temp->next = start->next;
		start->next = temp;
	
	}
	return head;
}

int pq_size(Process* head) {
	int count = 0; 
	
	while (head != NULL) {
		head = head -> next;
		count++;
	}
	
	return count;
}

Process* push(Process* head, int data, int priority, Proc_HashMap* hash){
	Process* start = head;
	Process* temp = makeProcess(data, priority, hash);
	//if head has less priority to new data
	if (head == NULL) {
		head = temp;
	} else if(head->priority > priority){
		temp->next = head;
		head = temp;
	}else{
		while(start->next != NULL && start->next->priority < priority){
			start = start->next;
		}
		temp->next = start->next;
		start->next = temp;
	
	}
	return head;
}
int peek(Process* head){
	return head->PID;
}

int isEmpty(Process* head){
	return head == NULL;
}

unsigned long pageTableSize(int page_size, int mem_size) {
	if (((mem_size * 1048576) / page_size) < 0 || mem_size <= 0 || page_size <= 0) {
		exit_with_message("Invalid page_size or memory size");
	}

	return (mem_size * 1048576) / page_size;
}

int IsDigitsOnly(char* str) {
	int len = strlen(str);

	for (int i = 0; i < len; i++) {
		char c = str[i];
		if (c < '0' || c > '9') {
			return 0;
		}
	}
	return 1;
}

PageTable* create_page_table(int size) {
	PageTable* page_table = malloc(sizeof(PageTable));
	if(page_table == NULL){
		exit_with_message("MALLOC FAILED");
	}
	page_table -> empty_page = 0; 
	page_table -> size = size;
	page_table -> clock_ptr = 0;
	page_table -> memory_table = malloc(sizeof(PPN_entry*) * size);
	if(page_table->memory_table == NULL){
		exit_with_message("MALLOC FAILED");
	}

	page_table -> free_entries = makeQueue(size);
	return page_table;
}	

PPN_entry* create_ppn(int PID, int VPN, int ppn_num) {
	PPN_entry* ppn_entry = malloc(sizeof(PPN_entry));
	if(ppn_entry == NULL){
		exit_with_message("MALLOC FAILED");
	}

	ppn_entry -> pid = PID;
	ppn_entry -> vpn = VPN;
	ppn_entry -> ppn_num = ppn_num;
	ppn_entry -> ref_bit = 1;
	
	return ppn_entry;
}

