#ifndef PAGE_ALGORITHM_H
#define PAGE_ALGORITHM_H

#include "page_table.h"
#include "FIFO.h"
#include "LRU.h"
#include "CLOCK.h"
#include "global.h"

PageTable* create_page_table(int size);

PPN_entry* create_ppn(int PID, int VPN, int ppn_num);

int PID_and_VPN(int PID, int VPN);

int entry_to_evict(ReplacementPolicy replacement_policy, Queue* lru_queue, Queue* fifo_queue, HashMap* hash, PageTable* memory); 

int main_mem_full(PageTable* memory);

double curr_mem_util(PageTable* memory);

PPN_entry* page_replacement_add(int PID, int VPN, PageTable* memory,
                                ReplacementPolicy replacement_policy, void* root,
                                Queue* lru_queue, Queue* fifo_queue, HashMap* hash, HashMap* hash_free_entries);

void page_referenced(ReplacementPolicy replacement_policy, Queue* lru_queue, Queue* fifo_queue,
                        HashMap* hash, int ppn_num, PageTable* memory);

void add_free_entry(PageTable* memory, int ppn_num, HashMap* hash, ReplacementPolicy replacement_policy,
                        void* root, Queue* lru_queue, Queue* fifo_queue, HashMap* hash_free_entries);


void procc_complete(PageTable* memory, int PID_complete, HashMap* hash, ReplacementPolicy replacement_policy,
		                         void* root, Queue* lru_queue, Queue* fifo_queue, HashMap* hash_free_entries);

#endif
