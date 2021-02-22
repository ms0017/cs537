#ifndef GRAPH_H
#define GRAPH_H

// Create a linked list
typedef struct ListNode {
        int index; // index in the array
	// TODO: instead should I store pointer to targetdata?
	// 	then need to find where it is which is O(n) 
	//
	// But with index I should be able to access in O(1) anyways...
        //char* targetName;
        //char** commands;
        struct ListNode* next;
} ListNode;

typedef struct LinkedList {
        int size; // TODO: add code for this
        ListNode *head;
} LinkedList;

typedef struct Graph {
        int numTargets;
        LinkedList* adjacencyLists;
} Graph;

// LinkedList* getList(Graph *g, int val) // don't need public

// void add(LinkedList* l, int val); // don't need public

void addEdge(Graph *g, int start, int end);

Graph* initGraph(int numTargets);

void printGraph(Graph* graph);


#endif
