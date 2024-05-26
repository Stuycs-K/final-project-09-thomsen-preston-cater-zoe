#ifndef PARSER_H
#define PARSER_H

enum State {
  DRY,
  LIQUID,
  UNKNOWN
};

struct Ingredient {
  char name[256];
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
  RETURN
};

struct Step {
  enum Command command;
  int ingredient;
  int bowl;
  int val;
  char string[256];
};

struct Recipe {
  char title[256];
  int ingred_count;
  struct Ingredient ingredients[64];
  int step_count;
  struct Step steps[128];
  int serves;
};

void printIngredient(struct Ingredient ing);
void printStepHeaders();
void printStep(struct Ingredient *ings, struct Step step);

struct Recipe parse(const char *fname);
  
#endif // PARSER_H
