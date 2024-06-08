#include <stdio.h>
#include <stdlib.h> // system
#include <string.h> // strcmp
#include "types.h"
#include "stack.h"
#include "execute.h"

void printPans(struct Stack *pan[], int count){
  for(int i=0; i<count; i++){
    while(pan[i]->top){
      struct Ingredient toPrint = pop(pan[i]);
      if(toPrint.state == LIQUID) printf("%c", (char) toPrint.count);
      else printf(" %ld", toPrint.count);
    }
  }
}

void copyStackOnto(struct Stack *to, struct Stack *from){
  struct Stack* tempStack = createStack();
  for(struct StackNode *ptr = from->top; ptr; ptr = ptr->next){
    push(tempStack, ptr->data);
  }
  for(struct StackNode *ptr = tempStack->top; ptr; ptr = ptr->next){
    push(to, ptr->data);
  }
  while(tempStack->top) pop(tempStack);
  deleteStack(tempStack);
}

void executeHelper(int recipe_count, struct Recipe recipes[], struct Recipe recipe, struct Stack *bowls[], struct Stack *pan[]){
  struct intStack *jumps = intCreateStack();

  for(int iptr=0; iptr<recipe.step_count; iptr++){
    struct Step *curr = recipe.steps + iptr;
    struct Ingredient *ing = &(recipe.ingredients[curr->ingredient]);
    struct Stack *bowl = bowls[curr->bowl-1]; // doesn't break on bowl==-1 but

    switch(curr->command){
      case INPUT:
        printf("Input requested.\n> ");
        scanf("%ld", &(ing->count));
        break;

      case PUSH:
        push(bowl, *ing);
        break;

      case POP:
        struct Ingredient tempPopped = pop(bowl);
        ing->state = tempPopped.state;
        ing->count = tempPopped.count;
        break;

      case ADD:
      case SUBTRACT:
      case MULTIPLY:
      case DIVIDE:
        struct Ingredient tempArithmetic = pop(bowl);
        if(curr->command == ADD)      tempArithmetic.count += ing->count;
        if(curr->command == SUBTRACT) tempArithmetic.count -= ing->count;
        if(curr->command == MULTIPLY) tempArithmetic.count *= ing->count;
        if(curr->command == DIVIDE)   tempArithmetic.count /= ing->count;
        push(bowl, tempArithmetic);
        break;

      case ADD_MANY:
        long sum = 0;
        for(int i=0; i<recipe.ingred_count; i++){
          struct Ingredient tempIngr = recipe.ingredients[i];
          if (DRY == tempIngr.state) sum += tempIngr.count; 
        }
        push(bowl, (struct Ingredient) {DRY, sum});
        break;

      case GLYPH_MANY:
        for(struct StackNode* ptr = bowl->top; ptr; ptr = ptr->next){
          ptr->data.state = LIQUID;          
        }
        break;

      case GLYPH:
        ing->state = LIQUID;
        break;

      case PUSHDOWN_CONST:
        pushNSpacesDown(bowl, curr->val);
        break;

      case PUSHDOWN:
        pushNSpacesDown(bowl, ing->count);
        break;

      case RANDOMIZE:
        randomizeStack(bowl);
        break;

      case CLEAN:
        while(bowl->top) pop(bowl);
        break;

      case PRINT:
        copyStackOnto(pan[curr->val-1], bowl);
        break;

      case WHILE:
        if(ing->count){
          intPush(jumps, iptr);
        }
        else{
          int count = 1;
          while(count){
            iptr++;
            if(recipe.steps[iptr].command == WHILE) count++;
            if(recipe.steps[iptr].command == END) count--;
          }
        }
        break;

      case END:
        if(curr->ingredient != -1) ing->count--;
        iptr = intPop(jumps) - 1;
        break;

      case BREAK:
        intPop(jumps);
        int count = 1;
        while(count){
          iptr++;
          if(recipe.steps[iptr].command == WHILE) count++;
          if(recipe.steps[iptr].command == END) count--;
        }
        break;

      case SUBROUTINE:
        for(int i=0; i<recipe_count; i++){
          if(!strcmp(recipes[i].title, curr->string)){
            //printf("%s\n", curr->string);
            struct Stack *newBowls[20];
            struct Stack *newPans[20];
            for(int i=0; i<20; i++){
              newBowls[i] = createStack();
              newPans[i] = createStack();
              copyStackOnto(newBowls[i], bowls[i]);
              copyStackOnto(newPans[i], pan[i]);
            }
            executeHelper(recipe_count, recipes, recipes[i], newBowls, newPans);
            copyStackOnto(bowls[0], newBowls[0]);
            for(int i=0; i<20; i++){
              //copyStackOnto(bowls[i], newBowls[i]);
              //copyStackOnto(pan[i], newPans[i]);
              deleteStack(newBowls[i]);
              deleteStack(newPans[i]);
            }
          }
        }
        break;

      case RETURN:
        printPans(pan, curr->val);
        iptr = recipe.step_count;
        break;

      case SYSTEM:
        char call[128] = "";
        char temp[128] = "";
        while(pan[0]->top){
          struct Ingredient toPrint = pop(pan[0]);
          if(toPrint.state == LIQUID) sprintf(temp, "%c", (char) toPrint.count);
          else sprintf(temp, " %ld", toPrint.count);
          strcat(call, temp);
        }
        system(call);
        break;

      default:
        printf("invalid command (what)\n");
        break;
    }
  }

  printf("\n---\n");

  printPans(pan, recipe.serves);

  // Cleanup
  intDeleteStack(jumps);

}

void execute(int recipe_count, struct Recipe recipes[]){
  printf("---executing:---\n");

  struct Stack *bowls[20];
  struct Stack *pan[20];
  for(int i=0; i<20; i++){
    bowls[i] = createStack();
    pan[i] = createStack();
  }

  executeHelper(recipe_count, recipes, recipes[0], bowls, pan);

  for(int i=0; i<20; i++){
    deleteStack(bowls[i]);
    deleteStack(pan[i]);
  }
}
