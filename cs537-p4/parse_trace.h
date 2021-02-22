#ifndef PARSE_TRACE_H
#define PARSE_TRACE_H


char* trimStr(char* str);

long get_PID (long int offset, FILE* fp);

long get_VPN (long int offset, FILE* fp);

PID_VPN_struct* get_PID_and_VPN(long int offset, FILE* fp);

long int get_next_index (int proc_PID, FILE* fp, long int curr_offset);


#endif
