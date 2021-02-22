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


#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include "run_command.h"


/*
 * Run a command via execvp and fork. Command contain each arg of command
 * as seperate arg of array and be NULL terminated  
 *
*  @param char** args of command to run 
**/
void run_command(char** args) {
	// Fork the current process
	pid_t pid = fork();

	if (pid == -1) {
		// Error
		fprintf(stderr, "Fork failed");
		exit(EXIT_FAILURE);
	} else if (pid == 0) {
		// In child process, execute execvp here
		int i = 0; 
		while (args[i] != NULL) {
				printf("%s ", args[i]);
				i++;
		}
		printf("\n");
	
		int exec_ret = execvp(args[0], args);
		
		if (exec_ret < 0) {
			fprintf(stderr, "Error calling execvp\n");
			exit(EXIT_FAILURE);
		}
	} else {
		// We are parent process
		int status = 0;
				
		do {
		// Wait for child to finish
		// It's pid is returned to the parent process
		int wait_ret = waitpid(pid, &status, 0);

		if (wait_ret == -1) {
			fprintf(stderr, "Wait for pid failed");
			exit(EXIT_FAILURE);
		}
		

		if (WIFEXITED(status)) {
    
                } else if (WIFSIGNALED(status)) {
                   printf("Killed with signal %d\n", WTERMSIG(status));
                } else if (WIFSTOPPED(status)) {
                   printf("Stopped with signal %d\n", WSTOPSIG(status));
                } else if (WIFCONTINUED(status)) {
                   printf("Continued\n");
                }

		} while (!WIFEXITED(status) && !WIFSIGNALED(status));
	}
}





