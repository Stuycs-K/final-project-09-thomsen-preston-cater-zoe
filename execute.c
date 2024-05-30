#include <stdio.h>
#include "types.h"
#include "stack.h"
#include "execute.h"

void execute(struct Recipe recipe){

  printf("---executing:---\n");

  struct Stack* bowl1 = createStack();
  struct Stack* pan1 = createStack();

  for(int iptr=0; iptr<recipe.step_count; iptr++){
    struct Step *curr = recipe.steps + iptr;
    struct Ingredient *ing = &(recipe.ingredients[curr->ingredient]);

    switch(curr->command){
      case INPUT:
        // printf("inputting\n");
        printf("Input requested.\n> ");
        scanf("%ld", &(ing->count));
        break;

      case PUSH:
        // printf("pushing\n");
        push(bowl1, *ing);
        break;

      case POP:
        // printf("popping\n");
        break;

      case ADD:
        // printf("adding\n");
        break;

      case ADD_MANY:
        // printf("adding many\n");
        break;

      case SUBTRACT:
        // printf("subtracting\n");
        break;

      case MULTIPLY:
        // printf("multiplying\n");
        break;

      case DIVIDE:
        // printf("dividing\n");
        break;

      case GLYPH_MANY:
        // printf("glyphing many\n");
        for(struct StackNode* ptr = bowl1->top; ptr; ptr = ptr->next){
          ptr->data.state = LIQUID;          
        }
        break;

      case GLYPH:
        // printf("glyphing one\n");
        ing->state = LIQUID;
        break;

      case RANDOMIZE:
        // printf("randomizing\n");
        break;

      case CLEAN:
        // printf("cleaning\n");
        break;

      case PRINT:
        // printf("printing\n");
        // reads from bowl1 into temp and then from temp into pan1
        // this preserves the order
        struct Stack* temp = createStack();
        for(struct StackNode *ptr = bowl1->top; ptr; ptr = ptr->next){
          push(temp, ptr->data);
        }
        for(struct StackNode *ptr = temp->top; ptr; ptr = ptr->next){
          push(pan1, ptr->data);
        }
        while(temp->top) pop(temp);
        deleteStack(temp);
        break;

      case WHILE:
        // printf("whiling\n");
        break;

      case END:
        // printf("ending\n");
        break;

      case BREAK:
        // printf("breaking\n");
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

  while(pan1->top){
    struct Ingredient toPrint = pop(pan1);
    if(toPrint.state == LIQUID) printf("%c", (char) toPrint.count);
    else printf(" %ld ", toPrint.count);
  }
  deleteStack(pan1);
  while(bowl1->top) pop(bowl1);
  deleteStack(bowl1);

}
