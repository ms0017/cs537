# Add your dependencies below as needed

537make: graph.o run_command.o reader.o data_structures.o time_diff.o
	 gcc -o 537make graph.o run_command.o data_structures.o reader.o time_diff.o

#reader.o: reader.c reader.h
#	gcc -c -Wall -Wextra reader.c
run_command.o : run_command.c run_command.h 
	gcc -c -ggdb -Wall -Wextra run_command.c

graph.o: graph.c graph.h run_command.h reader.h time_diff.h 
	gcc -c -ggdb -Wall -Wextra graph.c

data_structures.o: data_structures.c data_structures.h 
	gcc -c -ggdb -Wall -Wextra data_structures.c
   
reader.o: reader.c reader.h
	gcc -c -ggdb -Wall -Wextra reader.c

time_diff.o: time_diff.c time_diff.h
	gcc -c -ggdb -Wall -Wextra time_diff.c

clean: 
	rm 537make *.o 
