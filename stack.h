#ifndef STACK_H
#define STACK_H

struct StackNode { //This is the struct for each node of the stack, which references the next node in the stack
    double data;
    struct StackNode* next;
};

struct Stack { //This is to refer to each individual stack as a whole, all it needs is the top node.
    StackNode* top;
};

int push(Stack* stack, char* ingredient);
int pop(Stack* stack, char* ingredient);
char* peek(Stack* stack, char* ingredient);
int isFull(Stack* stack);

#endif // STACK_H
