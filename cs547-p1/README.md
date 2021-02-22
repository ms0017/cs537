# Project Description Link
http://pages.cs.wisc.edu/~bart/537/programs/program1.html


# Example run of the files 

\>make <br/>
\>cmd_process -p 1 -s -S -v <br/>
1: S utime=1846 1007 42388 [/sbin/init]  <br/>
\>make clean <br/>

Note that we print in format pid: state utime=# systime virtualmem [cmdline]
And that flag specifications are in project description link 

# cmd_process.c

This is main driver of the program. It sets state variables to record what to print based what is specified in command line Then, it prints out the desired result. 

# stat_parser.c

This module extracts relavant strings from /stat and /statm for cmd_process.c. Specifically, it extracts state, user time, stime, memory usage, and cmdline argument used to start the program. 

# get_process_list.c

This module contains methods used by cmd_process.c. If there is no -p option, it looks through /proc to find processes belonging to the user. Additionally, it has methods to check whether a process is currently running and whether a string is only digits (used by cmd_process). 











