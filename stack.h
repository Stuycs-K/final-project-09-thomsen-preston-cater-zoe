#ifndef STACK_H
#define STACK_H

struct StackNode { //This is the struct for each node of the stack, which references the next node in the stack
    double data;
    struct StackNode* next;
};

struct Stack { //This is to refer to each individual stack as a whole, all it needs is the top node.
    StackNode* top;
};

Stack* createStack();
int push(Stack* stack, char* ingredient);
void pushNSpacesDown(Stack* stack, double data, int n); //Unique to the chef language I believe
double pop(Stack* stack); //Returns the data of node that is popped.
double peek(Stack* stack);
int countElements(Stack* stack);
int isEmpty(Stack* stack);

#endif // STACK_H
