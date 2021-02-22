# Project Description Link
http://pages.cs.wisc.edu/~bart/537/programs/program2.html


# Example run of the files 

\>make <br/>
\>prodcom < text.txt <br/>
LINE*1
LINE*2
E0F: PROCESSED 2 LINES
***** Stats for Munch 1 *****
******** Queue Stats ***********
Enqueue Time:  0.000022
Dequeue Time:  0.001432
Dequeue Count: 3
Enqueue Count: 3
***** Stats for Munch 2 *****
******** Queue Stats ***********
Enqueue Time:  0.000022
Dequeue Time:  0.001442
Dequeue Count: 3
Enqueue Count: 3
***** Stats for Writer ******
******** Queue Stats ***********
Enqueue Time:  0.000022
Dequeue Time:  0.001448
Dequeue Count: 3
Enqueue Count: 3

\>make clean <br/>

# prodcom.c

This is main driver of the program. It reads lines from standard input. Processes the file but replaces all spaces with an asterisk and changes all lower case characters to uppercase

# queue.c

This module makes queues and gives it features such as enqueueing and dequeueing. It uses semaphores as a synchronization mechanism to ensure proper code execution. 

# queue_stats.c

This module keeps statistics on each queue, such as total enqueues and dequeues and so provides a method to print out the statistics of each queues.

#strin_commands.c
This module contains methods to convert spaces to asterisks and to convert lower case characters to upper case characters

