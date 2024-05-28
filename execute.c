#include <stdio.h>
#include "types.h"
#include "execute.h"

void execute(struct Recipe recipe){

  printf("---executing:---\n");

  for(int iptr=0; iptr<recipe.step_count; iptr++){
    struct Step *curr = recipe.steps + iptr;

    switch(curr->command){
      case INPUT:
        printf("inputting");
        break;

      case PUSH:
        printf("pushing");
        break;

      case ADD:
        printf("adding");
        break;

      case SUBTRACT:
        printf("subtracting");
        break;

      case GLYPH_MANY:
        printf("glyphing many");
        break;

      case PRINT:
        printf("printing");
        break;

      default:
        printf("invalid command (what)");
        break;
    }

    printf("\n");
  }

}
