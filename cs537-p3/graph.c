//////////////////////////////////////////////////////////////////////////////
//
// Title: Programming Assignment 3
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "graph.h"
#include "run_command.h"
#include "data_structures.h"
#include "reader.h"
#include "time_diff.h"

/**
 * Remove ':' from target name string. Return the string back if succesful 
 * else return NULL if ':' not found. 
 *
 * @param char* to remove ':' from 
 * @return char* with ':' removed  
 *
 */
char* parseTargetName(char* string) {

	int colon_seen = 0; // false 
	int end_index = 0; 
	int length = 0;

	// Iterate over string from end to start, set index based on where
	// colon appears
	for (int i = strlen(string) - 1; i >= 0; i--) {
		if (string[i] == ':') {
			colon_seen = 1; // true
		} else if (colon_seen && (string[i] != ' ' || string[i] != '\t')) {
			end_index = i;
			break;
		}
		
	}
	
	// If found colon, null terminate string right after this, else return NULL
	if (end_index > 0) {
		length = end_index + 1;

		// Modify string to remove elements after colon
		string[length] = '\0';
		
		// return targetName;
		return string;
	} else {
		return NULL;
	}
}

/**
 * Remove ':' from target name if appears in 1th index. 
 * Return back string if ':' found in 1st or 2nd index. Else return NULL. 
 *
 * @param result array of strings that represents a target
 * @return char* of string if colon found, else return NULL
 *
 */
char* getTargetName(char** result) {
	char* ret = parseTargetName(result[1]);
	
	int size = 0; 
	while (result[size] != NULL) {
		size++; 
	}

	if (ret != NULL) {
		return ret; // return values not used 
	// Because we add space after :, splitting will ensure
	// this index only contains ':' if exists after a number of spaces
	} else if (size >= 3 && result[2][0] == ':') { 
		return result[1];
	} else {
		return NULL;
	}
}




/**
 * Recursively run targetData in post-order (i.e. run all dependencies before
 * running current target). 
 *
 * @param targetData with information on target to run
 * @param allTargets array of all targetData information
 * @param numTargets the number of targets we have
 * @return int 1 if had to run current target (i.e. out of date), else return 0
 *
 */
int run(TargetData* targetData, TargetData** allTargets, int numTargets) {
	int run_curr = 0; // false 
		
	if (targetData -> numDependencies == 0) {
		run_curr = 1; 
	}
	
	// Target does not exist, so must run command
	FILE* currFile = fopen(targetData -> targetName, "r");
	if (currFile == NULL) {
		run_curr = 1; 
	} else {
		fclose(currFile);
	}

	// Check each dependency and compile if needed
	for (int i = 0; i < targetData -> numDependencies; i++) {
		int is_target = 0; // false

		for (int k = 0; k < numTargets; k++) {
			if (strcmp(targetData -> dependencies[i], allTargets[k] -> targetName) == 0) {
			       is_target = 1;
       							       
			       // Run child process. If dependency run, must run current
			       if (run(allTargets[k], allTargets, numTargets)) {
			      		run_curr = 1; 
			       }

			       break;	
			}	
		}
		
		// If dependency is not a target, then is a file 
		// Run if this file was modified after current
		if (!is_target) {
			// If is neither file nor target, throw error
			FILE* targetFile = fopen(targetData -> dependencies[i], "r");
			if (targetFile == NULL) {
				fprintf(stderr, "Missing rule to build %s for %s", targetData -> dependencies[i], targetData -> targetName);
				exit(EXIT_FAILURE);
			}
			
			if (last_mod(targetData -> targetName, targetData -> dependencies[i])) {
				run_curr = 1;
			}

			if (targetFile != NULL) fclose(targetFile);
		}
	}
	
	// Run current target if needed 
	if (run_curr) {
		for (int j = 0; j < targetData -> numCommands; j++) { 
			run_command(targetData -> commands[j]);
		}

		return 1; // ran command
	} else {
		return 0; 
	}
}

/*
 * Returns 1 if have a cycle in dependency graph, else return 0. Use algorithm for checking
 * for cycles in direct graph with BFS (uses Kahn's algo for Topological sort). 
 *
 * @param numTargets the number of targets in our graph
 * @param allTargets an array with all the target information
 */
int haveCycle(int numTargets, TargetData** allTargets) {
	// Initialize to 0 
	for (int i = 0; i < numTargets; i++) {
		allTargets[i] -> inDegree = 0;
	}

	// Set the in-degree of all nodes
	for (int g = 0; g < numTargets; g++) {
		TargetData* targetData = allTargets[g];	
		for (int i = 0; i < targetData -> numDependencies; i++) {
			for (int k = 0; k < numTargets; k++) {
				if (strcmp(targetData -> dependencies[i], allTargets[k] -> targetName) == 0) {
			       		allTargets[k] -> inDegree = allTargets[k] -> inDegree + 1;	
				}
			}
		}
	}
	
	// Put all veriticies with in-degree 0 in stack
	Stack* stack = CreateStack(numTargets);
	for (int i = 0; i < numTargets; i++) {
		if (allTargets[i] -> inDegree == 0) {
			push(stack, allTargets[i]);
		}
	}	
	
	int numVisited = 0; 

	while (!isEmpty(stack)) {
		TargetData* targetData = pop(stack);
		
		// Iterate through neighbors of node with in-degree 0 (i.e. we can visit this now) 
		// and decrease in-degree by 1
		for (int i = 0; i < targetData -> numDependencies; i++) {
			for (int k = 0; k < numTargets; k++) {
				if (strcmp(targetData -> dependencies[i], allTargets[k] -> targetName) == 0) {
			       		allTargets[k] -> inDegree = allTargets[k] -> inDegree - 1;	
					
					// Note: this won't go -1 since if it did depend on current, 
					// it would have indegree > 0 previously 
					// Add all notes with in-degree 0 to stack
					if (allTargets[k] -> inDegree == 0) {
						push(stack, allTargets[k]);
					}
					
				}
			}
		}
		
		numVisited++;
	}

	// Throw error if were not able to visit every node 
	if (numVisited != numTargets) {
		for (int i = 0; i < numTargets; i++) {
			
			// Print nodes that were not able to visit (as involved in a cycle) 			
			if (allTargets[i] -> inDegree > 0) { 
				fprintf(stderr, "Cycle with target: %s with dependencies: ", allTargets[i] -> targetName);
				
				for (int j = 0; j < allTargets[i] -> numDependencies; j++) {
					fprintf(stderr, "%s ", allTargets[i] -> dependencies[j]);
				}

				fprintf(stderr, "\n");
			}
		}
		
		freeStack(stack);
		return 1; // true (have a cycle) 
	} else {
		freeStack(stack);
		return 0; // false (no cycle) 
	}
}

int main(int argc, char **argv) {
	char* file_name = NULL;
	char* target_name = NULL;
	int set_file_name = 0;
	int set_target = 0;
	if(argc == 4){
		// See Usage 1
		if(strcmp(argv[1],"-f") == 0){
			file_name = argv[2];
			set_file_name = 1;
			target_name = argv[3];
			set_target = 1;
		// See Usage 2
		}else if(strcmp(argv[2], "-f") == 0){
			file_name = argv[3];
			set_file_name = 1;
			target_name = argv[1];
			set_target = 1;
		}else{
			// Usage 1 
			printf("Usage: 537Make -f <file name> <target name>\n");
			// Usage 2
			printf("Usage: 537Make <target name> -f <file name>\n");
			exit(EXIT_FAILURE);
		}
	// See Usage 3
	}else if(argc == 2){
		set_target = 1;
		target_name = argv[1];
	}else if(argc == 3 || argc > 4){
		printf("***Please use of one of these use cases***\n");
		printf("Usage: 537make\n");
		// Usage 3
		printf("Usage: 537make <target name>\n");
		printf("Usage: 537make -f <file name> <target name>\n");
		printf("Usage: 537make <target name> -f <file name>\n");
		exit(EXIT_FAILURE);
	}
	char *** results;
	if(set_file_name){
		results = reader(file_name);
	}else{
		results = reader(NULL);
	}
	int numToParse =  0; // 11; TEST 2
	while(results[numToParse] != NULL){
		++numToParse;
	}

	int numTargets = 0; 


	// We initialize an array or targets with their data in 4 passes
	// (1) Determine number targets
	// (2) Set number of commands for each target
	// (3) Set the commands of each target
	// (4) Set number of dep & dependencies of each target
	// We determine the number of commands/dependencies before initializing commands/dependencies
	// so we know how much space to malloc for the arrays that contain this data

	// First pass, determine number of targets 
	// and intiialize graph
	for (int i = 0; i < numToParse; i++) {
		char** result = results[i];
		char* target = getTargetName(result);
		
		if (result[0][0] == '1' && target != NULL) {
			numTargets++;
		} else if (result[0][0] == '1') {
			fprintf(stderr, "Target line missing colon\n");
			exit(EXIT_FAILURE);
		}
	}
	
	TargetData** allTargets = malloc(sizeof(TargetData*) * numTargets);
        if (allTargets == NULL) {
		fprintf(stderr, "Malloc failed");
		exit(EXIT_FAILURE);
	} 
	

	// Second pass, set numCommands
	int numTargetCommands = 0;
	int targetNum = -1; // uninitialized to start
	for (int i = 0; i < numToParse; i++) {
		char** result = results[i];
		char* target = NULL;
		if (result[0][0] == '1') target = result[1];


		if (target != NULL && targetNum != -1) {	
			// Set data of previous targetData
			allTargets[targetNum] -> numCommands = numTargetCommands;
		}
		
		if (target != NULL) {
			// Create new target 
			targetNum++;
			allTargets[targetNum] = malloc(sizeof(TargetData)); // Note: careful TargetData not *TargetData
			if (allTargets[targetNum] == NULL) {
				fprintf(stderr, "Malloc failed\n");
				exit(EXIT_FAILURE);
			}

			allTargets[targetNum] -> targetName = target; 

			// Set counters
			numTargetCommands = 0; // reset	
		 } else {
			numTargetCommands++;
		 }
	}
	
	// Create target for last entry seen
	if (targetNum != -1) {
		allTargets[targetNum] -> numCommands = numTargetCommands;
	}	


	// Third pass, populate commands 
	targetNum = -1;
	int commandNum = 0; 

	for (int i = 0; i < numToParse; i++) {
		char** result = results[i];
		char* target = NULL;
		if (result[0][0] == '1') target = result[1];


		if (target != NULL) {		
			targetNum++;
			commandNum = 0;
			allTargets[targetNum] -> commands = malloc(sizeof(char*) * (allTargets[targetNum] -> numCommands)); 
			if (allTargets[targetNum] -> commands == NULL) {
				fprintf(stderr, "Malloc failed\n");
				exit(EXIT_FAILURE);
			}
		} else {
			allTargets[targetNum] -> commands [commandNum] = result;
			commandNum++;
		}
	}
	
		

	// Fourth pass, populate dependencies
	targetNum = -1;
	for (int i = 0; i < numToParse; i++) {

		char** result = results[i];
		//char* target = getTargetName(result);
		char* target = NULL;
		if (result[0][0] == '1') target = result[1];


		// Add all dependencies to the node 
		if (target != NULL) {
			targetNum++;
		
			// First pass: count the number of entries in the array
			int j = 2;
			while (result[j] != NULL) {
				j++;
			}
		
			if (j >= 3 && result[2][0] == ':') j--;

			// A char* for every string dependency
			// Do not want space for index 0, so size = j - 1
			allTargets[targetNum] -> numDependencies = j - 2;
			allTargets[targetNum] -> dependencies = (char**) malloc(sizeof(char*) * (allTargets[targetNum] -> numDependencies));
			if (allTargets[targetNum] -> dependencies == NULL) {
				fprintf(stderr, "Malloc error\n");
				exit(EXIT_FAILURE);
			}

			int take   = 2;
		        int insert = 0;	
			while (result[take] != NULL && insert < allTargets[targetNum] -> numDependencies) {
				if (take == 2 && result[take][0] == ':') {take++; continue;}
				allTargets[targetNum] -> dependencies[insert] = result[take]; // start at index 0 in new array, but get from index 1 in original 
				insert++;
				take++;
			}
		}
	}
	

	// Check if dependency is either file or target
  	for (int i = 0; i < numTargets; i++) {
		for (int j = 0; j < allTargets[i] -> numDependencies; j++) { //allTargets[i] -> numDependencies; j++) {
			
			int isFile = 0; // false
			int isTarget = 0; // false   
			
			// Check if is file 
			FILE* file = fopen(allTargets[i] -> dependencies[j], "r"); 	
			
			if (file != NULL) {
				isFile = 1; // true
				fclose(file);
			} 
			
			// Check if it is a target 
			for (int k = 0; k < numTargets; k++) {
				if (strcmp(allTargets[i] -> dependencies[j], allTargets[k] -> targetName) == 0) {
					isTarget = 1; // true 
				}
			} 

			if (!isFile && !isTarget) {
				fprintf(stderr, "Dependence is neither a file nor a target. Error from dependence %s of target %s\n", 
						allTargets[i] -> dependencies[j], allTargets[i] -> targetName);
				exit(EXIT_FAILURE);
			} 
							
		}
	}
	

  	// Run 
	// Throw an error if cycle in targets, else run make 
	if( haveCycle(numTargets, allTargets) ) {
		printf("HAVE A CYCLE\n");
		exit(EXIT_FAILURE);
	} else {
		// Run the first target
		if (!set_target) {
			run(allTargets[0], allTargets, numTargets);
		// Run target specified 
		} else {
			int valid_target = 0; 
			for (int i = 0; i < numTargets; i++) {
				if (strcmp(allTargets[i] -> targetName, target_name) == 0) {
				        run(allTargets[i], allTargets, numTargets);	
					valid_target = 1; 
					break;
				}
			} 
			
			if (!valid_target) {
				fprintf(stderr, "Failure: No rule to make target %s", target_name);
				exit(EXIT_FAILURE);
			}
		}	
	}



	// Free input data 
	int i = 0; 
	while (results[i] != NULL) {
		int j = 0; 
		while (results[i][j] != NULL) {
				free(results[i][j]);
				j++;
		} 
		
		free(results[i]);	
		i++;
	}
	free(results);
	
	// Free Target Structs
	for (int i = 0; i < numTargets; i++) {
		free(allTargets[i] -> commands);
		free(allTargets[i] -> dependencies);
		free(allTargets[i]);
	}
	
	free(allTargets);

}
