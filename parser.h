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
  char ingredient[256];
  int bowl;
  char string[256];
  int val;
};

struct Recipe {
  char title[256];
  int ingred_count;
  struct Ingredient ingredients[64];
};

void printIngredient(struct Ingredient ing);

struct Recipe parse(const char *fname);
  
#endif // PARSER_H
