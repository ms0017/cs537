//////////////////////////////////////////////////////////////////////////////
//
// Title: Programming Assignment 1 (Custom ps)
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
#include <string.h>

/*  
 * Gets single-char state information of process. 
 *
 * @param pid of process to get state information of 
 * @return single-char state information of process
 *
 */
char get_state(int pid) {
	char path[50]; 	
	sprintf(path, "/proc/%i/stat", pid );

	FILE *stat_file = NULL;
        int c;
	int spaces = 0;

	if ((stat_file = fopen( path, "r" )) == NULL) {
		fprintf(stderr, "Error reading file for pid %d\n", pid);
		exit(EXIT_FAILURE);
	}
	
	// State if third item in proc file
	while((c = fgetc(stat_file)) != EOF) {
		if (spaces == 2) {
			fclose(stat_file);
			return (char) c;
		}

		if ((char) c == ' ') spaces++;
	}

	// Should return state in while loop	
	fclose(stat_file);
	fprintf(stderr, "Could not read state from pid %d\n", pid);
	exit(EXIT_FAILURE);
}

/*
 * Helper method for get_user_time and get_stime functions. Returns 
 * the num_element'th value in the stat file. The value should be a 
 * long integer.  
 *
 * @param num_element is the index of the space-seperated value to return 
 * @param path is the path to the stat file containing the value
 * @param pid is the pid of the process retrieving info from
 *
 * @return user_time of stime of pid 
 *
 */
static unsigned long int get_statfile_value(int num_element, char* path, int pid) {
	FILE *stat_file = NULL;

	if ((stat_file = fopen( path, "r" )) == NULL) {
		fprintf(stderr, "Error reading file for pid %d\n", pid);
		exit(EXIT_FAILURE);
	}

	unsigned long int result = 0;
	int num_spaces = num_element - 1;
	int c;
	int spaces = 0;
	
	// Iterate through char by char, reading in value
	// once enough spaces seen	
	while ((c = fgetc(stat_file)) != EOF) {
		if ( spaces == num_spaces && ( c < '0' || c > '9' ) ) {  
			break;
		} else if ( spaces == num_spaces ) {
			result = result * 10;
			result += (c - '0');
		} 

		if ((char) c == ' ') spaces++;
	}

	fclose(stat_file);
	return result;
}

/*
 * Get user time of process 
 *
 * @param pid of process to get user time of 
 * @return user time of process
 */
unsigned long int get_user_time(int pid) {
	char path[50];
	sprintf(path, "/proc/%i/stat", pid);

	// User time is the 14th element 
	return get_statfile_value(14, path, pid);	
}

/*
 * Get stime of process
 *
 * @param pid of process to get user time of 
 * @return stime of process
 */
unsigned long int get_stime(int pid) {
	char path[50];
	sprintf(path, "/proc/%i/stat", pid);
	
	// stime is the 15th element 
	return get_statfile_value(15, path, pid);		
}

/*
 * Get memory usage of process
 *
 * @param pid of process to get memory usage of 
 * @return memory usage of process
 *
 */
int get_memory_usage(int pid) {
	char path[50];
	sprintf(path, "/proc/%i/statm", pid);
	
	FILE *statm_file = NULL;
	int number;

	if ((statm_file = fopen(path, "r")) == NULL) {
		fprintf(stderr, "Error reading file for pid %d\n", pid);
		exit(EXIT_FAILURE);
	}

	fscanf(statm_file, "%d", &number);
	fclose(statm_file);

	return number;
}

/*
 * Get commnand-line that started this program 
 *
 * @param pid of process to get command-line of
 * @param str where command-line is stored in 
 *
 */
char* get_cmdline(int pid) {
	char path[50]; 	
	sprintf(path, "/proc/%i/cmdline", pid );
	

	FILE *stat_file = NULL;
        if ((stat_file = fopen( path, "r" )) == NULL) {
		fprintf(stderr, "Error reading file for pid %d\n", pid);
		exit(EXIT_FAILURE);
	}
	
	char* line = NULL;
	ssize_t nread;
 	size_t len;
	ssize_t length = 0;	
	
	while ((nread = getline(&line, &len, stat_file)) != -1) {
		// nread changes, so store length of input
		length = nread;		
	}
       	
	if (length == 0) {
		free(line);
		fclose(stat_file);
		return  calloc(1, 1);
	}
	
	// Replace everything but the last null termination
	for (int i = 0; i < length - 1; i++) {
		line[i] = (line[i] == '\0') ? ' ' : (char) line[i];
	}


	fclose(stat_file);
	return line;
}
