#include <stdio.h>  // FILE, fgets, fopen, printf
#include <stdlib.h> // strol
#include <string.h> // strncmp, strlen, strchr, strncpy, strcspn
#include <time.h> // for the randomization function
#include "stack.h" // Implementation of stacks into C

//THIS IS ALL FOR THE INT STACKS THAT ARE ONLY FOR LOOPING AND ITERATION

struct intStack* intCreateStack() {
  struct intStack* stack = malloc(sizeof(struct intStack));
  stack->top = NULL;
  return stack;
}

int intcountElements(struct intStack* stack) {
  int count = 0;
  struct intStackNode* current = stack->top;
  return count;
}

void intDeleteStack(struct intStack* stack) {
  while (intcountElements(stack) != 0) {
    intPop(stack);
  }
  free(stack);
}

struct intStackNode* intCreateStackNode(int ingredient) {
  struct intStackNode* newNode = malloc(sizeof(struct StackNode));
  newNode->data = ingredient;
  newNode->next = NULL;
  return newNode;
}

void intPush(struct intStack* stack, int ingredient) {
  struct intStackNode* newNode = intCreateStackNode(ingredient);
  newNode->next = stack->top;
  stack->top = newNode;
}

int intPop(struct intStack* stack) {
  struct intStackNode* temp = stack->top;
  int poppedData = temp->data;
  stack->top = temp->next;
  free(temp);
  return poppedData;
}

int intPeek(struct intStack* stack) {
  return stack->top->data;
}

//THIS IS ALL FOR REGULAR STACKS

struct Stack* createStack() {
  struct Stack* stack = malloc(sizeof(struct Stack));
  stack->top = NULL;
  return stack;
}

int countElements(struct Stack* stack) {
  int count = 0;
  struct StackNode* current = stack->top;
  return count;
}

void deleteStack(struct Stack* stack) {
  while (stack->top) {
    pop(stack);
  }
  free(stack);
}

struct StackNode* createStackNode(struct Ingredient ingredient) {
  struct StackNode* newNode = malloc(sizeof(struct StackNode));
  newNode->data = ingredient;
  newNode->next = NULL;
  return newNode;
}

void push(struct Stack* stack, struct Ingredient ingredient) {
  struct StackNode* newNode = createStackNode(ingredient);
  newNode->next = stack->top;
  stack->top = newNode;
}

void pushNSpacesDown(struct Stack* stack, int n) {
  if (n == 0) {
    return;
  }

  struct StackNode* topNode = stack->top;
  stack->top = topNode->next;
  topNode->next = NULL;

  struct StackNode* current = stack->top;
  for (int i = 0; i < n - 1; i++) {
      if (current == NULL) {
        fprintf(stderr, "Stack does not have %d elements\n", n);
        exit(EXIT_FAILURE);
      }
      current = current->next;
  }

  topNode->next = current->next;
  current->next = topNode;
}

struct Ingredient pop(struct Stack* stack) {
  struct StackNode* temp = stack->top;
  struct Ingredient poppedData = temp->data;
  stack->top = temp->next;
  free(temp);
  return poppedData;
}

struct Ingredient peek(struct Stack* stack) {
  return stack->top->data;
}

void getStackElements(struct Stack* stack, int count, struct Ingredient elements[100]) { // This function makes the stack into an array which can be rearranged (more easily)

  struct StackNode* current = stack->top;
  for (int i = 0; i < count; i++) {
    elements[i] = current->data;
    current = current->next;
  }
}

/*
void randomizeStack(struct Stack* stack) {
  int count = countElements(stack);
  if (count < 2) {
    return; // No need to randomize if the stack has less than 2 elements
  }

  // Pop all elements into an array
  struct Stack elements[] = malloc(count * sizeof(double));
  if (!elements) {
    fprintf(stderr, "Memory allocation failed\n");
    exit(EXIT_FAILURE);
  }
  for (int i = 0; i < count; i++) {
    elements[i] = pop(stack);
  }

  // Shuffle the array
  srand(time(NULL));
  for (int i = count - 1; i > 0; i--) {
    int j = rand() % (i + 1);
    double temp = elements[i];
    elements[i] = elements[j];
    elements[j] = temp;
  }

  // Push the shuffled elements back onto the stack
  for (int i = 0; i < count; i++) {
    push(stack, elements[i]);
  }

  free(elements);
}
*/
