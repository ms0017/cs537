//////////////////////////////////////////////////////////////////////////////
//
// Title: Programming Assignment 2 
// Course: CS 537, Fall 2020
//
// Authors: Kunal Waghray and Sheriff Issaka
// Emails: kwaghray@wisc.edu and issaka@wisc.edu
// CS Login: issaka kunal
// Lecturer's Name: Barton Miller
//
// Persons: NONE
// Online Sources: NONE
//
/////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "queue.h"
#include <pthread.h>
#include "string_commands.h"


 
 /*
 *
 * Reads input reads from standard input, one line at at time
 * @param queue  queue to be worked on
 * 
 */
void *ReaderThread(void* queue) {
	int const buf_size = 4096;
	Queue* queue_munch1 = queue;

  	// Reading files
	while(1){
     		// Mallocing space for each string
		char *str = (char*)malloc(buf_size*sizeof(char));
		int c; 
		if (str == NULL) {
			fprintf(stderr, "Error, unsuccessfull malloc\n");
			exit(EXIT_FAILURE);
		}
		
		int i = 0; 
		int valid_line = 1; 
     		
		// Keep reading characters until see EOF, newline, or buffer
		// does not have space for the given line 
		while ((c = fgetc(stdin)) != EOF && c != '\n' && i < buf_size - 1) {
			str[i] = c;
			i++;
		}

		if (c == EOF && i > 0) {
			// Have to print string terminated with the 
			// EOF 
			str[i] = '\0';
			valid_line = 1; // true
		} else if (c == EOF) {
			// No string processed before EOF seen
			valid_line = 0;
		} else if (c == '\n') {
			str[i] = '\0'; // terminate the string
			valid_line = 1; // true
      		
		// Not enough space for given line 
		// so flush it out and print error message 
		} else {
			fprintf(stderr, "Error: buffer overflow\n");
			// Flush out the remaining line 
			while ((c = fgetc(stdin)) != EOF && c != '\n') {}
			valid_line = 0; // false
		}

    		// Enqueing if the line is valid
		if (valid_line) {
			EnqueueString(queue_munch1, str);
		} else {
     			// Free last unused str alloc
			free(str);
		}
	  	
		// Enqueing NULL if end of file
		if (c == EOF) {
			EnqueueString(queue_munch1, NULL);
			break;
		}

	}

	return NULL;
}

/*
 * Define structure used for passing in
 * queues to function params
 *
 */
typedef struct QueueParams {
	Queue* queue1;
	Queue* queue2;
} QueueParams;



 /*
 * 
 * Replaces spaces within each lines with asterisks
 * @param queue_param_m1_m2  two queues to be processed
 * 
 */
void *Munch1(void* queue_param_m1_m2) {
	char* result;

	// Retrieve queues from parameter passed in 	
	QueueParams* queue_param = queue_param_m1_m2;
	Queue* queue_munch1 = queue_param->queue1;
	Queue* queue_munch2 = queue_param->queue2;
 
  	// munching	
	while (1) {
		result = DequeueString(queue_munch1);	
	
		// If reached end of file in reader (see NULL in queue) stop, 
		// else replace spaces with asterisk and enqueue in 
		// the next queue
		if (result == NULL) {
			EnqueueString(queue_munch2, result);
			break;		
		} else {
			replace_space_with_asterisk(result);
			EnqueueString(queue_munch2, result);
		}
	}

	return NULL;
}


 /*
 * 
 * Converts lower cases of each line to upper cases
 * @param queue_param_m2_mwriter  two queues to be processed
 * 
 */
void *Munch2(void* queue_param_m2_writer) {
	char* result;
	QueueParams* queue_param = queue_param_m2_writer;

  	// Retrieve queues from parameter passed in 
	Queue* queue_munch2 = queue_param->queue1;
	Queue* queue_writer = queue_param->queue2;

  	// munching
	while (1) {
  	 	// derqueing
		result = DequeueString(queue_munch2);
	
     		// checking end of file and enqueing
		if (result == NULL) {
			EnqueueString(queue_writer, result);
			break;
		} else {
     			// changing from lower to upper case and enqueing 
			replace_lower_to_upper(result);
			EnqueueString(queue_writer, result);
		}
	}
	
	return NULL;
}


 /*
 * 
 * Writes files to standard output, one line at a time
 * @param queue  queue to be processed
 * 
 */
void *Writer(void* queue) {
	Queue* queue_writer = queue;
	char* result;

   	 while (1) {	
      		// Dequeing 
		result = DequeueString(queue_writer);
	  	
		// If EOF was reached by reader stop (see NULL in queue), 
		// else print out string from queue
		if (result == NULL) {
			break;
		} else {
			fprintf(stdout, "%s\n", result);
			free(result);
		}
	}

  	// Notifying end of file
	fprintf(stdout, "E0F: PROCESSED %d LINES \n",
		       	queue_writer -> qs -> dequeueCount - 1);
	return NULL;
}

 /*
 * 
 *  Program driver
 * 
 */
int main()
{
  	// Create three queues to be used by the Reader, Munch1, Munch2, 
	// and Writer thread      
	Queue* queue_munch1 = CreateStringQueue(10);
	Queue* queue_munch2 = CreateStringQueue(10);
	Queue* queue_writer = CreateStringQueue(10);
        
  	// Create struct that will be used to pass parameters to 
	// Munch1 and Munch2 (struct has two queues in it). This is 
	// done because thread only accepts one param 
	QueueParams *queue_param_m1_m2     = (QueueParams*) malloc(sizeof(QueueParams));
	if (queue_param_m1_m2 == NULL) {
		fprintf(stderr, "Error, unsucessful malloc\n");
		exit(EXIT_FAILURE);
	}
	
	QueueParams *queue_param_m2_writer = (QueueParams*) malloc(sizeof(QueueParams));
	if (queue_param_m2_writer ==NULL) {
		fprintf(stderr, "Error, unsuccessful malloc\n");
		exit(EXIT_FAILURE);
	}

  	queue_param_m1_m2->queue1 = queue_munch1;
	queue_param_m1_m2->queue2 = queue_munch2;
	queue_param_m2_writer->queue1 = queue_munch2;
	queue_param_m2_writer->queue2 = queue_writer;

	// Initialize values used to start threads and check return values
	pthread_t *ptr;
	int pthread_return_val = 0; 
	ptr = malloc(sizeof(pthread_t)*4);
	if (ptr == NULL) {
		fprintf(stderr, "Error, unsuccesful malloc\n");
		exit(EXIT_FAILURE);
	}

  	
	// Create threads for Reader, Munch1, Munch2, and Writer
	// and check return value of pthread_create
	pthread_return_val = pthread_create(&ptr[0], NULL, ReaderThread, (void*) queue_munch1);
 	if (pthread_return_val != 0) {
		fprintf(stderr, "pthread_create failure\n");
		exit(EXIT_FAILURE);
	}

	pthread_return_val = pthread_create(&ptr[1], NULL, Munch1,       (void*) queue_param_m1_m2); //queue_m1_m2);		if () {
   	if (pthread_return_val != 0) {
		fprintf(stderr, "pthread_create failure\n");
		exit(EXIT_FAILURE);
	}
	pthread_return_val = pthread_create(&ptr[2], NULL, Munch2,       (void*) queue_param_m2_writer); // queue_m2_writer);
  	if (pthread_return_val != 0) {
		fprintf(stderr, "pthread_create failure\n");
		exit(EXIT_FAILURE);
	}

	pthread_return_val = pthread_create(&ptr[3], NULL, Writer,       (void*) queue_writer);
  	if (pthread_return_val != 0) {
		fprintf(stderr, "pthread_create failure\n");
		exit(EXIT_FAILURE);
	}

 	// Join the four threads created and check return value
	for (int i = 0; i < 4; i++) {
		pthread_return_val = pthread_join(ptr[i], NULL);
		if (pthread_return_val != 0) {
			fprintf(stderr, "pthread_join failure\n");
			exit(EXIT_FAILURE);
		}
	}

	// Print stats for each queue
	fprintf(stderr, "******** Reader to Munch 1 *****\n");
	PrintQueueStats(queue_munch1);
	fprintf(stderr, "******** Munch 1 to Munch 2 *****\n");
	PrintQueueStats(queue_munch2);
	fprintf(stderr, "******** Munch2 to Writer ******\n");	
	PrintQueueStats(queue_writer);

  	// Free malloced space
	free(queue_param_m1_m2);
	free(queue_param_m2_writer);

	free(queue_munch1-> qs);
	free(queue_munch2-> qs);
	free(queue_writer-> qs);
	
	free(queue_munch1->buffer);
	free(queue_munch2->buffer);
	free(queue_writer->buffer);

	free(queue_munch1);
	free(queue_munch2);
	free(queue_writer);

	free(ptr);

	return 0;
}

