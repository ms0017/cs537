CC = gcc
CFLAGS = -Wall -pedantic -Wextra
OBJECTS = prodcom.o queue.o queue_stats.o string_commands.o

#                    queue_stats
#                  / 
#          - queue  
#         /   
# prodcom 
#         \    
#          - string_commands
#              


prodcom: $(OBJECTS)
	$(CC) $(CFLAGS) -pthread -o prodcom $(OBJECTS)

prodcom.o: prodcom.c queue.h string_commands.h
	$(CC) $(CFLAGS) -c -ggdb  prodcom.c

string_commands.o: string_commands.c string_commands.h
	$(CC) $(CFLAGS) -c -ggdb string_commands.c

queue.o: queue.c queue.h queue_stats.h 
	$(CC) $(CFLAGS) -c -ggdb queue.c

queue_stats.o: queue_stats.c queue_stats.h
	$(CC) $(FLAGS) -c -ggdb queue_stats.c

clean:
	rm -f $(OBJECTS) prodcom
