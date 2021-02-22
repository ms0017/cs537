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
#include <ctype.h>
#include <string.h>
 
// IGNORES 1st CHAR
/*
 * @param char *s , string to be 
 * Checks if a string is empty
 */
int emptyString(char *s) {
	int i = 0; 
	// Read till end of string
	while (s[i] != '\0') {
		 // Check if it is not space
		 if (!isspace(s[i]) && i > 0){
			return 0;
		 }

		 i++;
	}
	   
	return 1;
}
/*
 *@param char* str, string to be modifiedr 
 * cha** ret, array to be returned after split
 * Splits strings by spaces
 *
 */
void split_spaces(char * str, char** ret){
	if(ret == NULL){
		printf("error \n");
		exit(1);
	}
	// Make a copy of string to enable modification
	char newStr[strlen(str)];
	strcpy(newStr, str);
	// Set token of deliminiter
	char* token = strtok(newStr, " ");
	int count = 0;
	while(token != NULL){
		// Split by spaces into array
		ret[count] = malloc(sizeof(char)*strlen(token));
		sprintf(ret[count++], "%s", token); 
		token = strtok(NULL, " ");
	}
	ret[count] = NULL;
}
/*
 * @param char* file_name
 *  Reads contents of file
 *
 **/

char*** reader(char* file_name){
	// Opens files depending of specification
	FILE *fp;
	if(file_name != NULL){
		fp = fopen(file_name, "r");
		if(fp == NULL){
			fprintf(stderr, "File not found\n");
			exit(EXIT_FAILURE);
		}
	}else {
		fp = fopen("makefile", "r");
	
		if (fp == NULL) {
			fp = fopen("Makefile", "r");
			if(fp == NULL){
				fprintf(stderr, "File not found\n");
				exit(EXIT_FAILURE);
				}
		}
	}
	
	int const buf_size = 4096;
        int pointerCount = 0;
	int line_num = 1; 
	int resizeBuf = 100;
	// Allocating spaces for an array contiaing lines
	char **arrPointers = (char**) malloc(resizeBuf * sizeof(char*));
	if (arrPointers == NULL) {
		fprintf(stderr, "malloc error\n:");
		exit(EXIT_FAILURE);
	}


	while(1) {
                // Mallocing space for each string
                char *str = (char*)malloc(buf_size*sizeof(char));
                int c;
                if (str == NULL) {
                        fprintf(stderr, "Error, unsuccessfull malloc\n");
                        exit(EXIT_FAILURE);
                }

                int i = 0;
                int valid_line = 1;
		int brk_invalid = 0;
		int colon_seen = 0; // false 
		int is_target = 1; // true
		int is_comment = 0; // false
                // Keep reading characters until see EOF, newline, or buffer
                // does not have space for the given line
		// Keep space for null terminator and : if needed
                while ((c = fgetc(fp)) != EOF && c != '\n' && i < buf_size - 2) {
			if (i == 0 && c == '\t') {
				is_target = 0;
				continue;
		
		       } else if (i ==0 && is_target != 0) {	
				is_target = 1;
				colon_seen = 0;
				str[i++] = '1';
				str[i++] = ' ';
			}
			
			if (c == '\0') {
				fprintf(stderr, "%d: Null Error:  Cannot have null in line\n", line_num);
				exit(EXIT_FAILURE);
			}

			if(c == '#'){
				is_comment = 1; 
				// since no inline comments, know comment line 	
				while ((c = fgetc(fp)) != EOF && c != '\n') {}
				break;	
			} else if (is_target && c == ':') {
				colon_seen = 1; // true
				str[i] = ':'; 
				i++;
				str[i] = ' '; 
				i++;
				continue;
			}
			
			if (!brk_invalid) {
                        	str[i] = c;
                        	i++;
			}else{
				break;
			}
		}
		// Checking values after the while loop breaks
                if(i >= buf_size -2){
			fprintf(stderr, "%d: Buffer Overflow: line too large\n", line_num);
			exit(EXIT_FAILURE);
		// Since only inline comments
		} else if (is_comment) { 
			valid_line = 0; 
		} else if (c == EOF && i > 0) {
                        str[i] = '\0';
                        valid_line = 1; // true
                } else if (c == EOF) {
                        // No string processed before EOF seen
                        valid_line = 0;
		} else if (c == '\n') {
			if(i > 0){
                        	str[i] = '\0'; // terminate the string
                        	valid_line = 1; // true
			}else{
				valid_line = 0;
			}

                // Not enough space for given line
                // so flush it out and print error message
                } else {
                        fprintf(stderr, "%d: Buffer Overflow: line too large\n", line_num);
                        // Flush out the remaining line
                        while ((c = fgetc(fp)) != EOF && c != '\n') {}
                        valid_line = 0; // false
                }
		// Checking for missing tab
		if (valid_line && !emptyString(str) && (is_target && i > 1 && str[2] == ' ')) {
		 	fprintf(stderr, "%d: Missing Tab: Line must start with tab or be target\n", line_num);
			exit(EXIT_FAILURE);
		// Checking for missing colon		
		} else if (valid_line && !emptyString(str) && (is_target && !colon_seen)) {
			fprintf(stderr, "%d: Missing Colon: Target line missing colon\n", line_num);
                	exit(EXIT_FAILURE);
		} else if (valid_line && !emptyString(str)) {
			// Resize arrayPointer buffer if needed
			if (pointerCount > resizeBuf - 1) {
				resizeBuf = resizeBuf * 2;
				arrPointers = realloc(arrPointers,resizeBuf * sizeof(char*));
			}
			arrPointers[pointerCount] = str;
			pointerCount++;		
		} else {
                        // Free last unused str alloc
                        free(str);
                }

                // Enqueing NULL if end of file
                if (c == EOF) {
                        //EnqueueString(queue_munch1, NULL);
                        break;
                }
		
		line_num++;

        }
	// Resizing arrPoiters if needed
	if (pointerCount > resizeBuf - 1) {
		resizeBuf = resizeBuf * 2;
		arrPointers = realloc(arrPointers,resizeBuf * sizeof(char*));
		}
	arrPointers[pointerCount] = NULL;

	// Mallocing space for the array to return
	char ***retThree = (char***)malloc(500*sizeof(char**));
	if(retThree == NULL) {
		printf("failed to malloc\n");
		exit(1);
	}
	
	// Count number spaces in string
	// and adding the strings to the array to return
	for (int i = 0; i < (pointerCount); i++){
		int spaceCount = 0;
	
		for(int j = 0; j < (int)strlen(arrPointers[i]); j++){
			if(isspace(arrPointers[i][j])){
				spaceCount++;
			}
		}
		// Space for array to be used for splits
		spaceCount += 2;
		char **ret = (char**)malloc(spaceCount*sizeof(char*));
		if(ret == NULL){
			fprintf(stderr,"malloc error\n");
			exit(1);
		}
		split_spaces(arrPointers[i], ret);
		retThree[i] = ret;
	}
		
	int i = 0; 
	// Free alloced spaces
	while (arrPointers[i] != NULL) {
		free(arrPointers[i]); 
		i++;
	}
	free(arrPointers);
	fclose(fp);
	return retThree;
}

