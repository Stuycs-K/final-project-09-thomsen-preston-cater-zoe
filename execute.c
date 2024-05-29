#include <stdio.h>
#include "types.h"
#include "execute.h"

void execute(struct Recipe recipe){

  printf("---executing:---\n");

  for(int iptr=0; iptr<recipe.step_count; iptr++){
    struct Step *curr = recipe.steps + iptr;
    struct Ingredient *ing = &(recipe.ingredients[curr->ingredient]);

    switch(curr->command){
      case INPUT:
        printf("inputting\n");
        printf("Input requested.\n> ");
        scanf("%ld", &(ing->count));
        break;

      case PUSH:
        printf("pushing\n");
        break;

      case POP:
        printf("popping\n");
        break;

      case ADD:
        printf("adding\n");
        break;

      case ADD_MANY:
        printf("adding many\n");
        break;

      case SUBTRACT:
        printf("subtracting\n");
        break;

      case MULTIPLY:
        printf("multiplying\n");
        break;

      case DIVIDE:
        printf("dividing\n");
        break;

      case GLYPH_MANY:
        printf("glyphing many\n");
        break;

      case GLYPH:
        printf("glyphing one\n");
        ing->state = LIQUID;
        break;

      case RANDOMIZE:
        printf("randomizing\n");
        break;

      case CLEAN:
        printf("cleaning\n");
        break;

      case PRINT:
        printf("printing\n");
        break;

      case WHILE:
        printf("whiling\n");
        break;

      case END:
        printf("ending\n");
        break;

      case BREAK:
        printf("breaking\n");
        break;

      case SUBROUTINE:
        printf("subroutining\n");
        break;

      case RETURN:
        printf("returning\n");
        break;

      default:
        printf("invalid command (what)\n");
        break;
    }
  }

}
