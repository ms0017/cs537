  
////////////////////////////////////////////////
/// 
/// Project 3 CS 537
/// Kunal Waghray 
// email kwaghya@wisc.edu 
// cs login kunal 
/// Sheriff Issaka 
/// email issaka@wisc.edu 
/// cs login issaka 
///
//////////////////////////////////////////////


#include <stdio.h>
#include <stdlib.h>
#include "data_structures.h"

/*
 * Create stack and malloc desired memory
 *
 * @paramcapacity, capacity of stack
**/
Stack* CreateStack(int capacity) {
	Stack* s = (Stack*)  malloc(sizeof(Stack));
	
	s -> arr = (TargetData**) malloc(sizeof(TargetData*) * capacity);	
	s -> capacity = capacity;
	s -> index = -1; 

	return s;
}

/*
 * Return true if stack empty
 * @param Stack* to check if empty
 *
**/
int isEmpty(Stack* s) {
	return (s -> index == -1);  
}
/*
 * Push address of targetData to the stack
 * 
 * @param Stack* to push to  
 * @param Target* data address to add to stack
**/
void push(Stack* stack, TargetData* targetData) {
	stack -> index++;
	stack -> arr[stack->index] = targetData;
}

/*
 * Pop item from the stack
 * @param Stack
**/
TargetData* pop(Stack* stack) {
	if (!isEmpty(stack)) {
		TargetData* item = stack -> arr[ stack -> index];
		stack -> index--;
		return item;
	} else {
		return NULL;
	}
}

/*
 * Free malloc'd memory from stack
 * @param stack to free
 */
void freeStack(Stack* stack){
	free(stack -> arr);
	free(stack);
}

