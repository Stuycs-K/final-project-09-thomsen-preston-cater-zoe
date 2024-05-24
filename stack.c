#include <stdio.h>  // FILE, fgets, fopen, printf
#include <stdlib.h> // strol
#include <string.h> // strncmp, strlen, strchr, strncpy, strcspn
#include "stack.h" // Implementation of stacks into C

struct StackNode { //This is the struct for each node of the stack, which references the next node in the stack
    double data;
    struct StackNode* next;
};

struct Stack { //This is to refer to each individual stack as a whole, all it needs is the top node.
    StackNode* top;
};

Stack* createStack() {
    Stack* stack = (Stack*)malloc(sizeof(Stack));
    stack->top = NULL;
    return stack;
}

StackNode* createStackNode(double data) {
    StackNode* newNode = (StackNode*)malloc(sizeof(StackNode));
    newNode->data = data;
    newNode->next = NULL;
    return newNode;
}

int push(Stack* stack, double ingredient) {
  StackNode* newNode = createStackNode(ingredient);
  newNode->next = stack->top;
  stack->top = newNode;
}

int pop(Stack* stack, double ingredient) {

}

char* peek(Stack* stack, double ingredient) {

}

int isEmpty(Stack* stack) {
    return stack->top == NULL;
}
