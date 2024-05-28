#ifndef STACK_H
#define STACK_H

struct StackNode { //This is the struct for each node of the stack, which references the next node in the stack
    double data;
    struct StackNode* next;
};

struct Stack { //This is to refer to each individual stack as a whole, all it needs is the top node.
    struct StackNode* top;
};

struct Stack* createStack();
int push(struct Stack* stack, double ingredient);
void pushNSpacesDown(struct Stack* stack, double ingredient, int n); //Unique to the chef language I believe
double pop(struct Stack* stack); //Returns the data of node that is popped.
double peek(struct Stack* stack);
int countElements(struct Stack* stack);
void randomizeStack(struct Stack* stack);

#endif // STACK_H
