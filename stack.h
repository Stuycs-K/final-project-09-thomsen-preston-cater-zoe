#ifndef STACK_H
#define STACK_H

#include "types.h" //For the struct ingredient

struct StackNode { //This is the struct for each node of the stack, which references the next node in the stack
    struct Ingredient data;
    struct StackNode* next;
};

struct Stack { //This is to refer to each individual stack as a whole, all it needs is the top node.
    struct StackNode* top;
};

struct intStackNode {
    int data;
    struct intStackNode* next;
};

struct intStack {
    struct intStackNode* top;
};


//Int stack stuff
struct intStack* intCreateStack();
int intcountElements(struct intStack* stack);
void intDeleteStack(struct intStack* stack);
struct intStackNode* intCreateStackNode(int ingredient);
void intPush(struct intStack* stack, int ingredient);
int intPop(struct intStack* stack);
int intPeek(struct intStack* stack);

//Actual chef stack stuff
struct Stack* createStack();
void deleteStack(struct Stack* stack);
void push(struct Stack* stack, struct Ingredient ingredient);
void pushNSpacesDown(struct Stack* stack, int n); //Unique to the chef language I believe
struct Ingredient pop(struct Stack* stack); //Returns the data of node that is popped.
struct Ingredient peek(struct Stack* stack);
int countElements(struct Stack* stack);
void randomizeStack(struct Stack* stack);

#endif // STACK_H
