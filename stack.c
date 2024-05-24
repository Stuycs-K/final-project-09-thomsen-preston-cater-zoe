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

void deleteStack(Stack* stack) {
    while (!isEmpty(stack)) {
        pop(stack);
    }
    free(stack);
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

void pushNSpacesDown(Stack* stack, double data, int n) {
  if (n == 0) {
    push(stack, data);
    return;
  }

  StackNode* current = stack->top;
  for (int i = 0; i < n - 1; i++) {
    if (current == NULL) {
      fprintf(stderr, "Stack does not have %d elements\n", n);
      exit(EXIT_FAILURE);
    }
    current = current->next;
  }

  StackNode* newNode = createStackNode(data);
  newNode->next = current->next;
  current->next = newNode;
}

double pop(Stack* stack) {
  StackNode* temp = stack->top;
  double poppedData = temp->data;
  stack->top = temp->next;
  free(temp);
  return poppedData;
}

double peek(Stack* stack) {
  return stack->top->data;
}

int countElements(Stack* stack) {
    int count = 0;
    StackNode* current = stack->top;
    while (current != NULL) {
        count++;
        current = current->next;
    }
    return count;
}

int isEmpty(Stack* stack) {
    return stack->top == NULL;
}
