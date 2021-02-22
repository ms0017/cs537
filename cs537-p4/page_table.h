#ifndef PAGE_TABLE_H
#define PAGE_TABLE_H


// TODO: can change
struct node {
    int PID;
    int VPN;
    int ppn_num;
};

void add_node(void **root, struct node *node);

struct node *find_node(void **root, int PID , int VPN);

void delete_node(void **root, int PID, int VPN);

void free_node(void *ptr);

int compare(const void *a, const void *b);

struct node *make_node(int PID, int VPN, int value);



#endif

