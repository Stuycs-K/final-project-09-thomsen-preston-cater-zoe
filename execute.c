#include <stdio.h>
#include <string.h> // strcmp
#include "types.h"
#include "stack.h"
#include "execute.h"

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

void executeHelper(int recipe_count, struct Recipe recipes[], struct Recipe recipe, struct Stack *bowls[], struct Stack *pan[], int firstStep){
  struct intStack *jumps = intCreateStack();

  for(int iptr=firstStep; iptr<recipe.step_count; iptr++){
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
          while(recipe.steps[iptr].command != END) iptr++;
        }
        break;

      case END:
        if(curr->ingredient != -1) ing->count--;
        iptr = intPop(jumps) - 1;
        break;

      case BREAK:
        while(recipe.steps[iptr].command != END) iptr++;
        break;

      case SUBROUTINE:
        for(int i=0; i<recipe_count; i++){
          if(!strcmp(recipes[i].title, curr->string)){
            //printf("%s\n", curr->string);
            struct Stack *newBowls[] = {createStack(), createStack()};
            struct Stack *newPans[] = {createStack(), createStack()};
            for(int i=0; i<2; i++){
              copyStackOnto(newBowls[i], bowls[i]);
              copyStackOnto(newPans[i], pan[i]);
            }
            executeHelper(recipe_count, recipes, recipes[i], newBowls, newPans, 0);
            for(int i=0; i<2; i++){
              copyStackOnto(bowls[i], newBowls[i]);
              copyStackOnto(pan[i], newPans[i]);
              deleteStack(newBowls[i]);
              deleteStack(newPans[i]);
            }
          }
        }
        break;

      case RETURN:
        for(int i=0; i<curr->val; i++){
          while(pan[i]->top){
            struct Ingredient toPrint = pop(pan[i]);
            if(toPrint.state == LIQUID) printf("%c", (char) toPrint.count);
            else printf("%ld", toPrint.count);
          }
        }
        iptr = recipe.step_count;
        break;

      default:
        printf("invalid command (what)\n");
        break;
    }
  }

  printf("\n---\n");

  for(int i=0; i<recipe.serves; i++){
    while(pan[i]->top){
      struct Ingredient toPrint = pop(pan[i]);
      if(toPrint.state == LIQUID) printf("%c", (char) toPrint.count);
      else printf("%ld", toPrint.count);
    }
  }

  // Cleanup
  intDeleteStack(jumps);

}

void execute(int recipe_count, struct Recipe recipes[]){
  printf("---executing:---\n");

  struct Stack *bowls[] = {createStack(), createStack()};
  struct Stack *pan[] = {createStack(), createStack()};

  executeHelper(recipe_count, recipes, recipes[0], bowls, pan, 0);

  for(int i=0; i<2; i++) deleteStack(bowls[i]);
  for(int i=0; i<2; i++) deleteStack(pan[i]);
}
