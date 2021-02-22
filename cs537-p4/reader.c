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


#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include "global.h"
#include "page_table.h"
#include "FIFO.h"
#include "LRU.h"
#include "CLOCK.h"
#include "statistics.h"
#include "parse_trace.h"
#include "algorithm_module.h"
/*
 *
 * This function does a second pass over the file
 * @params: int page_size, siz of page
 * @params: int mem_size, size of memory
 * @params: FILE* fp, pointer to file
 * @params: HashMap* unused_hash, hash for processing
 *
 * */
void run(int page_size, int mem_size, Process* pqueue, FILE* fp, Proc_HashMap* unused_hash){
	long time = 0;
	long mem_reference_count = 0;
	long page_fault_count = 0; 
	unsigned long total_unblocked = 0;
	unsigned long total_pages_occupied = 0; 
	int memory_reference_ppn = -1;
	unsigned long page_table_size = pageTableSize(page_size, mem_size);
	// Structure containing necessary fields for processing
	AlgorithmStruct* algorithm_struct = (AlgorithmStruct*) create_replacement_structs(page_table_size);
	// root for tree search
	void* root;					
	root = NULL;
	if(fp == NULL){
		printf("file error\n");
		exit(EXIT_FAILURE);
	}
	Proc_Queue* blocked = makeProcQueue();
	// when there are processes to be executed
	while(!pQueueIsEmpty(blocked) || !isEmpty(pqueue)){
		// There are four options
		// (1) Page Fault
		// (2) IO Complete
		// (3) IO Start
		// (4) Memory reference

		memory_reference_ppn = -1;
		if(!pQueueIsEmpty(blocked) && proc_Peek(blocked)->time == time){
			
			// (2) IO Complete		
			Process* newProc = pDequeue(blocked);	
			PID_VPN_struct* pid_vpn_struct = get_PID_and_VPN(newProc -> priority, fp);
			long PID = pid_vpn_struct -> PID;
			long VPN = pid_vpn_struct -> VPN;
			free(pid_vpn_struct);

			page_replacement_add(PID, VPN, &root, (void*) algorithm_struct);
			
			// Add process to ready queue
			pqueue = push_proc(pqueue, newProc);	

			// (3) IO START
			if(!pQueueIsEmpty(blocked)){
				proc_Peek(blocked)->time = time + 2000000;
			}
		} 

		// continue reading in priority queue
		if(!isEmpty(pqueue)){
			Process* process = pop(&pqueue);
			fseek(fp, process -> priority, SEEK_SET);
			
			PID_VPN_struct* pid_vpn_struct = get_PID_and_VPN(process -> priority, fp);
			long int PID = pid_vpn_struct -> PID;	
			long int VPN = pid_vpn_struct -> VPN;
			free(pid_vpn_struct);
				
			// (1) Page Fault
			if(find_node(&root, PID, VPN) == NULL) {
				if(pQueueIsEmpty(blocked)) {
					process->time = time + 2000000 + 1;	
					pEnqueue(blocked, process);	
				} else {
					process->time = -1;
					pEnqueue(blocked, process);
				}
				page_fault_count++;	
			
			// (4) Memory Reference 
			} else {
				++mem_reference_count;
				memory_reference_ppn = find_node(&root, PID, VPN) -> ppn_num;
				page_referenced(memory_reference_ppn, (void*) algorithm_struct);
				

				// Find the next element for the process 
				fseek(fp, process -> priority, SEEK_SET);
			
				if (process -> priority < process -> last) {
					process -> priority = get_next_index(process -> PID, fp, process -> priority);		
					pqueue = push_proc(pqueue, process);
				} else {
					procc_complete(process -> PID, &root, algorithm_struct);
					free(process);
					process = NULL;
				}

			}
		}



		// cummulate duration of processes
		int duration = 1;	
		if (isEmpty(pqueue) && !pQueueIsEmpty(blocked)) {
			int new_time = proc_Peek(blocked)->time;
		        duration = new_time - time; 	
		}
		
		total_unblocked      += pq_size(pqueue)*duration; 	
		total_pages_occupied += occupied_pages(algorithm_struct)* duration;
		time = time + duration; 
	}
	

	// calculate average durations
	double av_run_proc = (double) total_unblocked / time;
	double av_mem_util = (double) total_pages_occupied / (time * page_table_size); 
	// print out stats
	printf("################## STATISTICS ########################\n");
	printf("Average Memory Utilization: %lf\n", av_mem_util);
	printf("Average Runable Processes: %lf\n", av_run_proc);//avg_runnable_proc);
	printf("Total Memory References %ld\n", mem_reference_count);
	printf("Total Page Ins: %ld\n", page_fault_count);
	printf("Running Time: %ld\n", time);
	// free up allocated spaces
	free_replacement_struct(page_table_size, root, algorithm_struct);
	free(blocked);
	free(pqueue);
	free(unused_hash->array);
	free(unused_hash);
	fclose(fp);
}

/*
 * Does a first pass over the processes
 * @params: int page_size, size of page
 * @params: int mem_size, memory size
 * @params: char* file, file to read
 */
void readFile(int page_size, int mem_size, char* file){
	Proc_HashMap* hash = makeProcHash(5000);
	FILE *fp;
	fp = fopen(file, "r");
	
	if(fp == NULL){
		printf("file error\n");
		exit(EXIT_FAILURE);
	}
	char* line = NULL;
	size_t len = 0;
	ssize_t read = 0;
	int firstFlag = 1;
	Process* pc;
	while((read = getline(&line, &len, fp)) != -1){
		char str[++read]; 
		char delim[] = " ";
		strcpy(str, line);
		char* ptr = strtok(str, delim);
		long PID = atoi(ptr);
		long index = ftell(fp)-read + 1;;
		
		if(firstFlag){
			pc = makeProcess(PID, index, hash);
			--firstFlag;
		}
		if(hash->array[PID] == NULL){
			pc = push(pc, PID, index, hash);			
		}else{
			updateLast(hash->array[PID], index, -1);
		}
	}

	if(line){free(line);}	

	run(page_size, mem_size, pc,  fp, hash);
}

/*
 * checks if number is a power of two
 * @params: int x, number to check
 * @return: boolean if int
 */
int isPowTwo(int x){
	if(x == 0){
		return 0;
	}
	while(x!=1){
		if(x % 2 != 0){
			return 0;
		}
		x /= 2;
	}
	return 1;
}


 /*
 * Drives the program
 *
 * @param argc  counts number of values 
 * @param argv  store the user specified values
 * 
 */
int main (int argc, char **argv){
	int c;
	int p = 4096;
	int m = 1;
	// read args
	while ((c = getopt(argc, argv, ":p:m:")) != -1) {
		switch(c){
		case 'p':
   			if (IsDigitsOnly(optarg)) { 
    				p = atoi(optarg);
             			if(!isPowTwo(p)){
					exit_with_message("p must be power of 2\n");
				}
				break;
			}

			exit_with_message("Page size must be all digits");
			break;
		case 'm':
      			if(IsDigitsOnly(optarg)){
  				m = atoi(optarg);
				break;
			}

			exit_with_message("Mem size must be all digits");
			break;
		default:
			printf("INVALID OPTION\n");
			exit(1);	
		}
	}
	
	readFile(p, m, argv[argc-1]);
	return 0;
}


