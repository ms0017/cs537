#ifndef STATISTICS_H
#define STATISTICS_H

#include "global.h"

double cumulative_average(double old_average, double new_val, int n, int duration);

//double memory_utilization(PageTable* page_table);

int main_mem_full(void* algorithm_struct_input);

//unsigned long occupied_pages(PageTable* page_table);

unsigned long occupied_pages(void* algorithm_struct_input);

#endif


