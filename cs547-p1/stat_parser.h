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

#ifndef STAT_PARSER
#define STAT_PARSER

char get_state(int pid);

unsigned long int get_user_time(int pid);

unsigned long int get_stime(int pid);

int get_memory_usage(int pid);

char* get_cmdline(int pid);

#endif
