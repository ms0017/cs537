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

#include <math.h>
#include "stat_parser.h"
#include "get_process_list.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>


/*
 * Prints output for each flag
 *
 * @param pid of the process that wants to print 
 * @param p_flag  p flag value of corresponding pid
 * @param s_flag  s flag value of corresponding pid
 * @param U_flag  U flag value of corresponding pid
 * @param S_flag  S flag value of corresponding pid
 * @param v_flag  v flag value of corresponding pid
 * @param c_flag  c flag value of corresponding pid
 * 
 *
 */

void output_given_flags(int pid, bool p_flag, bool s_flag, bool U_flag, bool S_flag, bool v_flag, bool c_flag){	
		printf("%d: ", pid);
   
  		 // checking boolean value of s  flag 
		if (s_flag){
			printf("%c ", get_state(pid));
		}
   
   		// checking boolean value of U  flag 
		if (U_flag){
			printf("utime=%ld " , get_user_time(pid));
		}
   
   		// checking boolean value of S  flag 
		if (S_flag){
			printf("%ld ", get_stime(pid));
			
		}
   
  		 // checking boolean value of v  flag 
		if (v_flag){
			printf("%d ",get_memory_usage(pid));

		}
   
   		// checking boolean value of c  flag 
		if (c_flag){
			char* array = get_cmdline(pid);
			printf("[%s] ", array);
			free(array);
		}
		printf("\n");
}
	
 
 /*
 * Drives the program
 *
 * @param argc  counts number of values 
 * @param argv  store the user specified values
 * 
 */
int main (int argc, char **argv){
	//initializing values to store flags and ints to be used below
	int c, pid;
	bool p_flag, s_flag, U_flag, S_flag, v_flag, c_flag;
	p_flag = s_flag = S_flag = v_flag = false;
	U_flag = c_flag = true;
	
 
 	//checking user input values and setting appropriate flags
	while ((c = getopt(argc, argv, "-p:s::U::S::v::c::")) != -1) {
 
 		// checking each optional case
		switch(c){
			case 'p':
      				// checking for right p input
				if(IsDigitsOnly(optarg)){
					pid = atoi(optarg);
					p_flag = true;
				
					// If process does not exist, then 
					// terminate the program	
					if (!isRunningProcess(pid)) {
						exit(EXIT_SUCCESS);
					}

					break;
				}
				printf("PID MUST BE ALL DIGIT \n");
				exit(1);

       			// checking s case 
			case 's':
				s_flag = false;
				if (optarg != 0 && strcmp(optarg,"-") != 0){
					printf("INVALID INPUT FOR s \n");
					exit(1);
				}else if(optarg != 0 && strcmp(optarg, "-") == 0){
					break;
				}
				s_flag = true;
				break;
        		
			// checking U case
			case 'U':
				U_flag = true;
				if (optarg!= 0 && strcmp(optarg, "-") != 0){
					printf("INVALID INPUT FOR U \n");
					exit(1);
				}
				else if (optarg!= 0 && strcmp(optarg, "-") == 0){
					U_flag = false;
					break;
				}

				break;
        
			// checking S case
			case 'S':
				S_flag = false;	
				if (optarg != 0 && strcmp(optarg,"-") != 0){
					printf("INVALID INPUT FOR S \n");
					exit(1);

				}else if(optarg != 0 && strcmp(optarg, "-") == 0){
					break;
				}
				S_flag = true;
				break;
			
			// checking v case
			case 'v':
				v_flag = false;
				if (optarg != 0 && strcmp(optarg,"-") != 0){
					printf("INVALID INPUT FOR v \n");
					exit(1);
				}else if(optarg != 0 && strcmp(optarg, "-") == 0){
					break;
				}
				v_flag = true;
				break;

			// checking c case
			case 'c':
				c_flag = true;
				if (optarg!= 0 && strcmp(optarg, "-") != 0){
					printf("INVALID INPUT FOR c \n");
					exit(1);
				}
				else if (optarg!= 0 && strcmp(optarg, "-") == 0){
					c_flag = false;
					break;
				}

				break;
        		// checking unknown options
			case '?':
				exit(1);

			// checking default case
			default:
				printf("INVALID OPTION\n");
				exit(1);	
		}
	}
	
 	// calling method to print outputs if p values is specified

	if (p_flag){
 		output_given_flags(pid, p_flag, s_flag, U_flag, S_flag, v_flag, c_flag);
	} else{
		// printing all processes of current user if p flag isn't set
		char str[500];
  		 // getting id of current process
		sprintf(str, "%d", getpid());
  		 // calling method to return user id
		int uid = get_uid(str);
		int size = num_user_created_procc("/proc/", uid);
		int* user_created = user_created_procc("/proc/", uid, size);
   
   		// calling method to print values for each user process
		for (int i = 0; i < size; i++) {
			output_given_flags(user_created[i], p_flag, s_flag, U_flag, S_flag, v_flag, c_flag);	
		}

		free(user_created);
	}

}


