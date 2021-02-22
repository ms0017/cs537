CC = gcc
CFLAGS = -Wall -pedantic
OBJECTS = 537ps.o get_process_list.o stat_parser.o

537ps: $(OBJECTS)
	$(CC) $(CFLAGS) -o 537ps $(OBJECTS) 

537ps.o: 537ps.c stat_parser.h get_process_list.h
	$(CC) $(CFLAGS) -c -ggdb 537ps.c

stat_parser.o: stat_parser.c stat_parser.h
	$(CC) $(CFLAGS) -c -ggdb stat_parser.c

get_process_list.o: get_process_list.c get_process_list.h
	$(CC) $(CFLAGS) -c -ggdb get_process_list.c

clean: 
	rm -f $(OBJECTS) 537ps


