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
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
/*
 * Checks the last modified dates of files
 * @param char* target, target file 
 * @param char* dependency, name of dependence
 * @return int, the boolean for time after check
 *
 */
int last_mod(char* target, char* dependency){
	struct stat a;
	struct stat b;
	time_t time1;
	time_t time2;
	// Checks file times
	if(!(stat(target, &a))){
		time1 = (a.st_mtime);
	}

	if(!(stat(dependency, &b))){
		time2 = (b.st_mtime);
	}

	return time1 < time2;

}
