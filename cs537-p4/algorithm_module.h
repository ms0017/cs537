#ifndef ALGORITHM_MODULE_H
#define ALGORITHM_MODULE_H

void* create_replacement_structs(int size);

void free_replacement_struct(int size, void* root, void* algorithm_struct_input);

int entry_to_evict(void* algorithm_struct);

int main_mem_full(void* algorithm_struct);

double curr_mem_util(void* algorithm_struct);

void page_replacement_add(int VPN, int PID, void* root, void* algorithm_struct);

void page_referenced(int ppn_num, void* algorithm_struct);

void procc_complete(int PID_complete, void* root, void* algorithm_struct);

void add_free_entry(int ppn_num, void* root, void* algorithm_struct);

#endif
