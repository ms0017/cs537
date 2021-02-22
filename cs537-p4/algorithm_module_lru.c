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


#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <search.h>
#include "page_table.h"
#include "FIFO.h"
#include "LRU.h"
#include "CLOCK.h"
#include "algorithm_module.h"
#include "global.h"

void* create_replacement_structs(int size) {
	AlgorithmStruct* algorithm_struct = malloc(sizeof(AlgorithmStruct));
	if (algorithm_struct == NULL) {
		exit_with_message("malloc failed");
	}
	
	
	algorithm_struct -> queue = makeQueue(size);
	algorithm_struct -> memory = create_page_table(size);
	algorithm_struct -> hash = makeHash(size);
	algorithm_struct -> hash_free_entries = makeHash(size);

	return (void*) algorithm_struct;
}

void free_replacement_struct(int size, void* root, void* algorithm_struct_input) {
	AlgorithmStruct* algorithm_struct = (AlgorithmStruct*) algorithm_struct_input;
	Queue* queue = algorithm_struct -> queue;
	HashMap* hash = algorithm_struct -> hash;  
	HashMap* hash_free_entries = algorithm_struct -> hash_free_entries;   
	PageTable* memory = algorithm_struct -> memory;
 	
	while (!EmptyQueue(queue)) Dequeue_LRU(queue, hash);
	while (!EmptyQueue(memory -> free_entries)) Dequeue_LRU(memory->free_entries, hash_free_entries);

	// Free memory table
	for (int i = 0; i < size; i++) {
		if (memory -> memory_table[i] != NULL) {
			free(memory->memory_table[i]);
			memory->memory_table[i] = NULL;
		} 
	}

	for (int i = 0; i < size; i++) {
		if (hash -> array[i] != NULL) {
			free(hash->array[i]);
			hash->array[i] = NULL;
		}
	}
	
	for (int i = 0; i < size; i++) {
		if (hash_free_entries->array[i] != NULL) {
			free(hash_free_entries->array[i]);
			hash_free_entries->array[i] = NULL;
		}
	}
	
	tdestroy(root, free_node);
	free(memory -> free_entries);	
	free(queue);
	free(hash -> array);
	free(hash_free_entries -> array);	
	free(hash);
	free(hash_free_entries);
	free(memory -> memory_table);
	free(memory);

	free(algorithm_struct);
}


int entry_to_evict(void* algorithm_struct_input) {
 	AlgorithmStruct* algorithm_struct = (AlgorithmStruct*) algorithm_struct_input;
	Queue* lru_queue = algorithm_struct -> queue;
	HashMap* hash = algorithm_struct -> hash;

	return Dequeue_LRU(lru_queue, hash);
}

void page_replacement_add(int PID, int VPN, void* root, void* algorithm_struct_input) {
	
	AlgorithmStruct* algorithm_struct = (AlgorithmStruct*) algorithm_struct_input;
	
	PageTable* memory = algorithm_struct -> memory;
	HashMap* hash_free_entries = algorithm_struct -> hash_free_entries;

	if (!main_mem_full(algorithm_struct)) {
		// Add to page_table and hashmap
		memory -> memory_table[memory -> empty_page] = create_ppn(PID, VPN, memory -> empty_page);
		add_node(root, make_node(PID, VPN,  memory -> empty_page));

		// Increment free pages	
		memory -> empty_page = memory -> empty_page + 1;
	} else if (!EmptyQueue(memory -> free_entries)) {
		int ppn_entry = Dequeue_LRU(memory -> free_entries, hash_free_entries);

		// set current entry
		memory -> memory_table[ppn_entry] = create_ppn(PID, VPN, ppn_entry); // memory
		add_node(root, make_node(PID, VPN, ppn_entry)); // hashtable	
	} else { 
		// Find entry to replace
		int ppn_entry;
		ppn_entry = entry_to_evict(algorithm_struct_input);

		// get info of entry to replace
		int PID_replace = memory -> memory_table [ppn_entry] -> pid;
		int VPN_replace = memory -> memory_table [ppn_entry] -> vpn;
		
		delete_node(root, PID_replace, VPN_replace); // hashtable
		free(memory -> memory_table[ppn_entry]);
		memory -> memory_table[ppn_entry] = NULL; // memory

		// set current entry
		memory -> memory_table[ppn_entry] = create_ppn(PID, VPN, ppn_entry); // memory
		add_node(root, make_node(PID, VPN, ppn_entry)); // hashtable

	}
}

void page_referenced(int ppn_num, void* algorithm_struct_input) {
	AlgorithmStruct* algorithm_struct = (AlgorithmStruct*) algorithm_struct_input;
	Queue* lru_queue = algorithm_struct -> queue; /// ****
	HashMap* hash = algorithm_struct -> hash;      /// 
	
	lruInit(lru_queue, hash, ppn_num);

}

void procc_complete(int PID_complete, void* root, void* algorithm_struct_input) {
	AlgorithmStruct* algorithm_struct = (AlgorithmStruct*) algorithm_struct_input;
	PageTable* memory = algorithm_struct -> memory;

	for (int i = 0; i < memory -> empty_page; i++) {
		if (memory -> memory_table[i] != NULL && memory -> memory_table[i] -> pid == PID_complete) {
			add_free_entry(i, root, algorithm_struct);
		}
	}


	}

void add_free_entry(int ppn_num, void* root, void* algorithm_struct_input) {
	AlgorithmStruct* algorithm_struct = (AlgorithmStruct*) algorithm_struct_input;
	HashMap* hash = algorithm_struct -> hash;      /// 
	PageTable* memory = algorithm_struct -> memory;
	Queue* lru_queue = algorithm_struct -> queue; /// ****
	HashMap* hash_free_entries = algorithm_struct -> hash_free_entries;
	// get info of entry to remove
	int PID_replace = memory -> memory_table [ppn_num] -> pid;
	int VPN_replace = memory -> memory_table [ppn_num] -> vpn;
		
	// remove previous entry 
		
	delete_node(root, PID_replace, VPN_replace); // hashtable
	
	remove_middle_element(lru_queue, hash, ppn_num);

	free(memory -> memory_table[ppn_num]);
	memory -> memory_table[ppn_num] = NULL; // memory
	 
	// Add to free entries 
	lruInit(memory -> free_entries, hash_free_entries, ppn_num);
}


