#include <stdio.h>
#include "types.h"
#include "stack.h"
#include "execute.h"

void execute(struct Recipe recipe){

  printf("---executing:---\n");

  struct Stack *bowls[] = {createStack(), createStack()};
  struct Stack *pan[] = {createStack(), createStack()};
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
        // printf("pushing\n");
        push(bowl, *ing);
        break;

      case POP:
        // printf("popping\n");
        struct Ingredient tempPopped = pop(bowl);
        ing->state = tempPopped.state;
        ing->count = tempPopped.count;
        break;

      case ADD:
      case SUBTRACT:
      case MULTIPLY:
      case DIVIDE:
        // printf("subtracting\n");
        struct Ingredient tempArithmetic = pop(bowl);
        if(curr->command == ADD)      tempArithmetic.count += ing->count;
        if(curr->command == SUBTRACT) tempArithmetic.count -= ing->count;
        if(curr->command == MULTIPLY) tempArithmetic.count *= ing->count;
        if(curr->command == DIVIDE)   tempArithmetic.count /= ing->count;
        push(bowl, tempArithmetic);
        break;

      case ADD_MANY:
        // printf("adding many\n");
        long sum = 0;
        for(int i=0; i<recipe.ingred_count; i++){
          struct Ingredient tempIngr = recipe.ingredients[i];
          if (DRY == tempIngr.state) sum += tempIngr.count; 
        }
        push(bowl, (struct Ingredient) {DRY, sum});
        break;

      case GLYPH_MANY:
        // printf("glyphing many\n");
        for(struct StackNode* ptr = bowl->top; ptr; ptr = ptr->next){
          ptr->data.state = LIQUID;          
        }
        break;

      case GLYPH:
        // printf("glyphing one\n");
        ing->state = LIQUID;
        break;

      case RANDOMIZE:
        // printf("randomizing\n");
        // randomizeStack(bowl);
        break;

      case CLEAN:
        // printf("cleaning\n");
        while(bowl->top) pop(bowl);
        break;

      case PRINT:
        // printf("printing\n");
        // reads from bowl1 into temp and then from temp into pan1
        // this preserves the order
        struct Stack* tempStack = createStack();
        for(struct StackNode *ptr = bowl->top; ptr; ptr = ptr->next){
          push(tempStack, ptr->data);
        }
        for(struct StackNode *ptr = tempStack->top; ptr; ptr = ptr->next){
          push(pan[curr->val-1], ptr->data);
        }
        while(tempStack->top) pop(tempStack);
        deleteStack(tempStack);
        break;

      case WHILE:
        // printf("whiling\n");
        if(ing->count){
          intPush(jumps, iptr);
        }
        else{
          while(recipe.steps[iptr].command != END) iptr++;
        }
        break;

      case END:
        // printf("ending\n");
        if(curr->ingredient != -1) ing->count--;
        iptr = intPop(jumps) - 1;
        break;

      case BREAK:
        // printf("breaking\n");
        while(recipe.steps[iptr].command != END) iptr++;
        break;

      case SUBROUTINE:
        // printf("subroutining\n");
        break;

      case RETURN:
        // printf("returning\n");
        break;

      default:
        printf("invalid command (what)\n");
        break;
    }
  }

  printf("---\n");

  for(int i=0; i<2; i++){
    while(pan[i]->top){
      struct Ingredient toPrint = pop(pan[i]);
      if(toPrint.state == LIQUID) printf("%c", (char) toPrint.count);
      else printf("%ld", toPrint.count);
    }
    deleteStack(pan[i]);
  }
  for(int i=0; i<2; i++){
    while(bowls[i]->top) pop(bowls[i]);
    deleteStack(bowls[i]);
  }

  intDeleteStack(jumps);

}
