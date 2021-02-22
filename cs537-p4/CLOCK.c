//////////////////////////////////////////////////////////////////////////////
//
// Title: Programming Assignment 4
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



#include "CLOCK.h" 

void updateChance(int index, PageTable* memory){
	memory->memory_table[index]->ref_bit = 1;

}

int replacePage_CLOCK(PageTable* memory){
	while(1){
		if(memory->memory_table[memory->clock_ptr]->ref_bit == 0){
			return memory->clock_ptr;
		}
		//exhausted chance
		memory->memory_table[memory->clock_ptr]->ref_bit = 0;
		memory->clock_ptr = (memory->clock_ptr+1) % memory->size;
	}
}

