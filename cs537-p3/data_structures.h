#ifndef STACK_H
#define STACK_H

typedef struct TargetData {
        char* targetName;
        int numCommands;
        char*** commands; // Note: this should be fine
        char** dependencies; // file and target dependencies, look through and see if name
                             // then when run, look through can check if
                             // target.o ==
        int numDependencies;
        int inDegree; // used for determinging if cycle
} TargetData;


typedef struct Stack {
        int index;
        TargetData** arr;
        int capacity;
} Stack;

Stack* CreateStack(int capacity);
int isEmpty(Stack* s);
void push(Stack* stack, TargetData* targetData);
TargetData* pop(Stack* stack);
void freeStack(Stack* stack);


#endif // STACK_H
