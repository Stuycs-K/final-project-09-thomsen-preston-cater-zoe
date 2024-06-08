#include <stdio.h> // printf
#include "types.h"

void printIngredient(struct Ingredient ing, char *name){
  printf("%ld\t%s\t%s\n", ing.count, (char*[]){"DRY", "WET", "???"}[ing.state], name);
}

void printStepHeaders(){
  printf("%-12s", "command name");
  printf("\tbowl#");
  printf("\t%-16.16s", "ingredient");
  printf("\t%s", "val");
  printf("\t%s", "string");
  printf("\n");
}

static char *commandNames[] = {"INPUT", "PUSH", "POP", "ADD", "ADD_MANY", "SUBTRACT", "MULTIPLY", "DIVIDE", "GLYPH", "GLYPH_MANY", "PUSHDOWN", "PUSHDOWN_CONST", "RANDOMIZE", "CLEAN", "PRINT", "WHILE", "END", "BREAK", "SUBROUTINE", "RETURN", "SYSTEM"};

void printStep(char names[][128], struct Step step){
  printf("%-12s", commandNames[step.command]);

  if(step.bowl == -1) printf("\t");
  else printf("\tbowl%d", step.bowl);

  printf("\t%-16.16s", step.ingredient == -1 ? "" : names[step.ingredient]);

  if(step.val == -1) printf("\t");
  else printf("\t%d", step.val);

  if(step.string) printf("\t%s", step.string);

  printf("\n");
}

