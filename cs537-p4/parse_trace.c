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
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "global.h"
#include "parse_trace.h"


char* trimStr(char* str){
	char* ptr;
	if(!str){
		return NULL;
	}
	if(!*str){
		return str;
	}
	for(ptr = str + strlen(str) -1; (ptr >= str) && isspace(*ptr); --ptr);
	ptr[1] = '\0';
	return str;
}

PID_VPN_struct* get_PID_and_VPN(long int offset, FILE* fp) {
	if(fseek(fp, offset, SEEK_SET) == -1){
		exit_with_message("fseek() failed");
	
	}

	char* line = NULL;
	size_t len = 0;
	ssize_t read = 0;
	if((read = getline(&line, &len, fp)) == -1){
		printf("EOF\n");
		exit(EXIT_FAILURE);
	}
	char str[++read]; 
	char delim[] = " ";
	strcpy(str, trimStr(line));
	char* ptr = strtok(str, delim);
	char* vp = strtok(NULL, delim);

	PID_VPN_struct* return_val = malloc(sizeof(PID_VPN_struct));
	if(return_val == NULL){
		exit_with_message("MALLOC FAILED");
	}

	return_val -> PID = atoi(ptr);
	return_val -> VPN = atoi(vp);
	
	if (line) free(line);

	return return_val;

}

long int get_next_index (int proc_PID, FILE* fp, long int curr_offset) {
	char* line = NULL;
	size_t len = 0;
	ssize_t read = 0;
	int firstLine = 0; 

	if(fseek(fp, curr_offset, SEEK_SET)  == -1){
		exit_with_message("fseek() failed");
	}
	

	while ((read = getline(&line, &len, fp)) != -1) {
		if(firstLine == 0){
			firstLine = 1;
			continue;
		}

		char str[++read]; 
		char delim[] = " ";
		strcpy(str, line);
		char* ptr = strtok(str, delim);
		long PID = atoi(ptr);
		long ftell_val;
		if ((ftell_val = ftell(fp)) == -1) {
			exit_with_message("ftell() failed");
		}
		long index = ftell_val-read + 1;
		
		if (PID == proc_PID) {
			if (line) free(line);
			return index;
		}
	}
	
	if (line) free(line);

	return -1;
}

