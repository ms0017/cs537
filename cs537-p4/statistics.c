//////////////////////////////////////////////////////////////////////////////
//
// Title: Programming Assignment 4
// Course: CS 537, Fall 2020
//
// Authors: Kunal Waghray and Sheriff Issaka
// Emails: kwaghray@wisc.edu and issaka@wisc.edu
// Lecturer's Name: Barton Miller
//
// Persons: NONE
// Online Sources: NONE
//
/////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include "statistics.h"

double cumulative_average(double old_average, double new_val, int n, int duration) {
	return (double) (old_average * n + new_val*duration) / (double) (n + duration + 1);
}

int main_mem_full(void* algorithm_struct_input) {
        AlgorithmStruct* algorithm_struct = (AlgorithmStruct*) algorithm_struct_input;
        PageTable* memory = algorithm_struct -> memory;

        return memory -> empty_page >= memory -> size;
}

unsigned long occupied_pages(void* algorithm_struct_input) {
	AlgorithmStruct* algorithm_struct = (AlgorithmStruct*) algorithm_struct_input; 
	PageTable* page_table = algorithm_struct -> memory;
	

	if (page_table -> empty_page - page_table -> free_entries -> count < 0) {
		printf("Pages Occupied less than 0\n");
		exit(EXIT_FAILURE);	
	}

	return page_table -> empty_page - page_table -> free_entries -> count;
}

