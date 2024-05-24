#ifndef PARSER_H
#define PARSER_H

#include <stdbool.h>

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
  struct Step steps[64];
};

void printIngredient(struct Ingredient ing);

struct Recipe parse(const char *fname);
  
#endif // PARSER_H
