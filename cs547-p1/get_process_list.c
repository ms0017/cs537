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

#include <dirent.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

/*
 * Get user id of process (or self) 
 *
 * @param process with pid of process to get uid of, or self 
 * @return user id of specfied process 
 *
 */
unsigned int get_uid(char* process) {
	FILE* file = NULL;

	char filename[50];
	sprintf( filename, "/proc/%s/status", process );	

	file = fopen( filename, "r" );
	char* line = NULL;
	size_t len = 0;
	ssize_t read;
	
	if (file == NULL) {
		fprintf( stderr, "Error retrieving uid of %s", process );
		exit(EXIT_FAILURE);
	}
	
	while ((read = getline(&line, &len, file)) != -1) {
		// First find line wthat starts with Uid 
		if(strncmp(line, "Uid:", strlen("Uid:")) == 0) {
			// Now finextract the real uid from this line 
			// (the first integerin this line)
			int int1, int2, int3;
			char str1[20];

			sscanf(line, "%s %d %d %d", str1, &int1, &int2, &int3);
			
			free(line);
			fclose(file);	
			return int1;
		}

	}

	free(line);
	fclose(file);
	return 1;
}

/*
 * Returns 0 (false) if str passed in is 
 * not only digits, else returns 1 (true) 
 *
 * @param str to verify 
 * @return 1 if only digits, else 0
 */
int IsDigitsOnly(char* str){
    int len = strlen(str);

    for (int i = 0; i < len; i++) {
	char c = str[i];
        if (c < '0' || c > '9')
            return 0; // false
    }
    return 1; // true
}

/*
 * Returns true (1) if running process exists with the pid given, else false (0)
 *
 * @param pid of process to check
 * @return true of false (1 or 0) if running process exists
 *
 */
int isRunningProcess(int pid) {
	DIR *directory;
	struct dirent *entry;
	if ((directory = opendir("/proc/")) == NULL) {
		fprintf(stderr, "Error opening proc directory");
		exit(EXIT_FAILURE);
	}
	
	while ((entry = readdir(directory)) != NULL) {
		if (IsDigitsOnly(entry->d_name) && atoi(entry->d_name) == pid) {
			closedir(directory);
			return 1; // true
		}
	}
	
	closedir(directory);
	return 0; // false
}



/*
 * Helper function of num_pids. Returns number of entries cretaed by 
 * current user. 
 *
 * @param directory_path 
 * @param self_uid 
 * @return number of entires created by current user 
 *
 */
int num_user_created_procc(char* directory_path, int self_uid) {
	DIR *directory;
	struct dirent *entry;
 	
	// Open directory to get user created processes 	
	if ((directory = opendir(directory_path)) == NULL) {
		fprintf(stderr, "Error opening directory to get user created processes");
		exit(EXIT_FAILURE);	
	}
	
	// Count number of user created processes 	
	int counter = 0;
	while ((entry = readdir(directory)) != NULL) {
		if (IsDigitsOnly(entry->d_name) && get_uid(entry->d_name) == self_uid) {
			counter++;
		}
	}

	closedir(directory);
	return counter;
}

/*
 * Return integer array of all processes created by the current user
 *
 * @param directory_path of 
 * @param self_uid user id of current process
 * @param procc_size number of processes 
 *
 */
int* user_created_procc (char* directory_path, int self_uid, int procc_size) {
	DIR *directory;
	struct dirent *entry;
	
	// Store processes created by the user in procc
	int *procc = malloc(procc_size * sizeof(int));

	if (procc == NULL) {
		fprintf(stderr, "Memory alloc failed");
		exit(EXIT_FAILURE);
	}

	if ((directory = opendir(directory_path)) == NULL) {
		fprintf(stderr, "Error opening directory to get user created processes");
		exit(EXIT_FAILURE);
	}

	int counter = 0;
	while ((entry = readdir(directory)) != NULL) {
		if (IsDigitsOnly(entry->d_name) && get_uid(entry->d_name) == self_uid) {
			procc[counter] = atoi(entry->d_name);
			counter++;	
		}
	}
	
	closedir(directory);
	return procc;	
}




