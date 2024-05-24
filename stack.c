#include <stdio.h>  // FILE, fgets, fopen, printf
#include <stdlib.h> // strol
#include <string.h> // strncmp, strlen, strchr, strncpy, strcspn
#include <time.h> // for the randomization function
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

StackNode* createStackNode(double ingredient) {
  StackNode* newNode = (StackNode*)malloc(sizeof(StackNode));
  newNode->data = ingredient;
  newNode->next = NULL;
  return newNode;
}

int push(Stack* stack, double ingredient) {
  StackNode* newNode = createStackNode(ingredient);
  newNode->next = stack->top;
  stack->top = newNode;
}

void pushNSpacesDown(Stack* stack, double ingredient, int n) {
  if (n == 0) {
    push(stack, ingredient);
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

  StackNode* newNode = createStackNode(ingredient);
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
  return count;
}

double* getStackElements(Stack* stack, int count) { // This function makes the stack into an array which can be rearranged
  double* elements = (double*)malloc(countElements(stack));

  StackNode* current = stack->top;
  for (int i = 0; i < count; i++) {
    elements[i] = current->data;
    current = current->next;
  }
  return elements;
}

void randomizeStack(Stack* stack) {
  int count = countElements(stack);
  if (count < 2) {
    return; // No need to randomize if there are less than 2 (because then there's only one)
  }

  double* elements = getStackElements(stack, count);

  // Shuffle the elements array
  srand(time(NULL));
  for (int i = count - 1; i > 0; i--) {
    int j = rand() % (i + 1);
    double temp = elements[i];
    elements[i] = elements[j];
    elements[j] = temp;
  }

  // Rebuild the stack with the shuffled elements
  deleteStack(stack);
  stack = createStack();
  for (int i = 0; i < count; i++) {
    push(stack, elements[i]);
  }

  free(elements);
}
