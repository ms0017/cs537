#ifndef GLOBAL_H
#define GLOBAL_H

// Node with Doubly Linked List
typedef struct Node{
        struct Node *previous, *next;
        unsigned long pageNum;
} Node;

// Queue
typedef struct Queue{
        int count;
        int numFrames;
        Node *front, *back;
} Queue;

typedef struct PPN_entry {
        int pid;
        int vpn;
        int ppn_num;
        int ref_bit;
} PPN_entry;


typedef struct PageTable {
        int empty_page;
        int size;
        int clock_ptr;
        PPN_entry** memory_table;
        Queue* free_entries;
} PageTable;

typedef enum {
        LRU_RP = 0,
        FIFO_RP = 1,
        CLOCK_RP = 2
} ReplacementPolicy;

// HashMap of Queue Nodes
typedef struct HashMap{
        int capacity;
        Node** array;
} HashMap;

typedef struct PID_VPN_struct {
	int PID;
	int VPN;
} PID_VPN_struct;


typedef struct Process{
	unsigned long PID;
	long priority;
	long last;
	long time;
	long last_VPN;
	struct Process* next;
	struct Process* previous;
}Process;

typedef struct Proc_Queue{
	long count;
	Process* front, *back;
}Proc_Queue;

// HashMap of Processes
typedef struct Proc_HashMap{
	int capacity;
	Process** array;
}Proc_HashMap;


typedef struct AlgorithmStruct {
	Queue* queue; // used for fifo and lru
	PageTable* memory;
	HashMap* hash;
	HashMap* hash_free_entries;
//	void* root;
} AlgorithmStruct;

void exit_with_message(char* input);

Node* makeNode(unsigned long pageNum);

Queue* makeQueue(int numFrames);

HashMap* makeHash(int capacity);

int FullQueue(Queue* queue);

int EmptyQueue(Queue* queue);

void printNode(Node* node);

void printQueue(Queue* queue);

Proc_Queue* makeProcQueue();

int pQueueIsEmpty(Proc_Queue* q);

void pEnqueue(Proc_Queue* q, Process* p);

Process* pDequeue(Proc_Queue* q);

Process* proc_Peek(Proc_Queue* q);

void updateLast(Process* proc, long index, long VPN);

Proc_HashMap* makeProcHash(int capacity);

Process* makeProcess(unsigned long PID, long index, Proc_HashMap* hash);

Process* pop(Process** head);

Process* push_proc(Process* head, Process* temp);

int pq_size(Process* head);

Process* push(Process* head, int data, int priority, Proc_HashMap* hash);

int peek(Process* head);

int isEmpty(Process* head);

unsigned long pageTableSize(int page_size, int mem_size);

int IsDigitsOnly(char* str);

PageTable* create_page_table(int size);

PPN_entry* create_ppn(int PID, int VPN, int ppn_num);

int main_mem_full(void* algorithm_struct_input);

double curr_mem_util(void* algorithm_struct_input);

#endif
