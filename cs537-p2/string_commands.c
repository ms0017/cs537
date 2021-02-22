//////////////////////////////////////////////////////////////////////////////
//
// Title: Programming Assignment 2 
// Course: CS 537, Fall 2020
//
// Authors: Kunal Waghray and Sheriff Issaka
// Emails: kwaghray@wisc.edu and issaka@wisc.edu
// Lecturer's Name: Barton Miller
// CS Login: issaka, kunal
// Persons: NONE
// Online Sources: NONE
//
/////////////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "string_commands.h"

/*
 * Replace space characters in string with 
 * the asterisk character
 *
 * @param string address of string which will modify
 */
void replace_space_with_asterisk(char* string) {
	int i = 0;
  	char c = '*';
	for (i = 0; string[i] != '\0'; i++) {
		if (isspace(string[i])) {
			string[i] = c;
		}
	}

}

/*
 * Replace all lower case characters with upper case characters
 * in the string
 *
 * @param string address of stirng which will modify
 *
 */
void replace_lower_to_upper(char *string) {
	for (int i = 0; string[i] != '\0'; i++) {
		if (islower(string[i])) { 
			string[i] = toupper(string[i]);
		}
	
	}
}


