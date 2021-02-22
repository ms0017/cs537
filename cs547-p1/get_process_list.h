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

#ifndef GET_PROCESS_LIST
#define GET_PROCESS_LIST

unsigned int get_uid(char* process);
int IsDigitsOnly(char* str);
int* user_created_procc (char* directory_path, int self_uid, int procc_size);
int num_user_created_procc(char* directory_path, int self_uid);
int isRunningProcess(int pid);

#endif
