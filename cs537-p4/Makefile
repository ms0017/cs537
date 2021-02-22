reader_test: page_algorithm.o page_table.o FIFO.o LRU.o global.o CLOCK.o reader.o statistics.o parse_trace.o
	gcc -o reader_test page_algorithm.o page_table.o FIFO.o LRU.o global.o CLOCK.o reader.o statistics.o parse_trace.o

537pfsim-clock: reader.o algorithm_module_clock.o page_table.o FIFO.o LRU.o global.o CLOCK.o statistics.o parse_trace.o
	gcc -o 537pfsim-clock reader.o algorithm_module_clock.o page_table.o FIFO.o LRU.o global.o CLOCK.o statistics.o parse_trace.o

537pfsim-fifo: reader.o algorithm_module_fifo.o page_table.o FIFO.o LRU.o global.o CLOCK.o statistics.o parse_trace.o
	gcc -o 537pfsim-fifo reader.o algorithm_module_fifo.o page_table.o FIFO.o LRU.o global.o CLOCK.o statistics.o parse_trace.o

537pfsim-lru: reader.o algorithm_module_lru.o page_table.o FIFO.o LRU.o global.o CLOCK.o statistics.o parse_trace.o
	gcc -o 537pfsim-lru reader.o algorithm_module_lru.o page_table.o FIFO.o LRU.o global.o CLOCK.o statistics.o parse_trace.o

page_algo_test: page_algorithm.o page_table.o FIFO.o LRU.o global.o CLOCK.o parse_trace.o
	gcc -o page_algo_test page_algorithm.o page_table.o FIFO.o LRU.o global.o CLOCK.o parse_trace.o

lru_test: LRU.o global.o FIFO.o
	gcc -o lru_test LRU.o global.o FIFO.o

algorithm_module_fifo.o: algorithm_module_fifo.c algorithm_module.h page_table.h FIFO.h LRU.h CLOCK.h global.h 
	gcc -c -ggdb -Wall -Wextra algorithm_module_fifo.c

algorithm_module_lru.o: algorithm_module_lru.c algorithm_module.h page_table.h FIFO.h LRU.h CLOCK.h global.h 
	gcc -c -ggdb -Wall -Wextra algorithm_module_lru.c

algorithm_module_clock.o: algorithm_module_clock.c algorithm_module.h page_table.h FIFO.h LRU.h CLOCK.h global.h 
	gcc -c -ggdb -Wall -Wextra algorithm_module_clock.c

reader.o: reader.c page_table.h FIFO.h LRU.h global.h CLOCK.h statistics.h parse_trace.h
	gcc -c -ggdb -Wall -Wextra reader.c

page_algorithm.o: page_algorithm.c page_algorithm.h page_table.h FIFO.h LRU.h global.h CLOCK.h
	gcc -c -ggdb -Wall -Wextra page_algorithm.c		

page_table.o: page_table.c page_table.h global.h 
	gcc -c -ggdb -Wall -Wextra page_table.c

FIFO.o: FIFO.c FIFO.h global.h
	gcc -c -ggdb -Wall -Wextra FIFO.c

LRU.o: LRU.c LRU.h global.h
	gcc -c -ggdb -Wall -Wextra LRU.c

CLOCK.o: CLOCK.c CLOCK.h global.h
	gcc -c -ggdb -Wall -Wextra CLOCK.c

global.o: global.c global.h
	gcc -c -ggdb -Wall -Wextra global.c

statistics.o: statistics.h statistics.c global.h
	gcc -c -ggdb -Wall -Wextra statistics.c

parse_trace.o: parse_trace.c parse_trace.h global.h
	gcc -c -ggdb -Wall -Wextra parse_trace.c


clean: 
	rm reader_test page_algo_test *.o
# 537pfsim-fifo: 
# 537pfsim-lru: 
# 537pfsim-clock: 

