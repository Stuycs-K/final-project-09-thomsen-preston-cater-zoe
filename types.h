#ifndef TYPES_H
#define TYPES_H

enum State {
  DRY,
  LIQUID,
  UNKNOWN
};

struct Ingredient {
  enum State state;
  long count;
};

enum Command {
  INPUT,
  PUSH,
  POP,
  ADD,
  ADD_MANY,
  SUBTRACT,
  MULTIPLY,
  DIVIDE,
  GLYPH,
  GLYPH_MANY,
  PUSHDOWN,
  PUSHDOWN_CONST,
  RANDOMIZE,
  CLEAN,
  PRINT,
  WHILE,
  END,
  BREAK,
  SUBROUTINE,
  RETURN,
  SYSTEM
};

struct Step {
  enum Command command;
  int ingredient;
  int bowl;
  int val;
  char string[128];
};

struct Recipe {
  char title[128];
  int ingred_count;
  struct Ingredient ingredients[64];
  int step_count;
  struct Step steps[128];
  int serves;
};

void printIngredient(struct Ingredient ing, char *name);
void printStepHeaders();
void printStep(char names[][128], struct Step step);

#endif // TYPES_H
