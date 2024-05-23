#ifndef PARSER_H
#define PARSER_H

#include <stdbool.h>

struct Ingredient {
  char name[256];
  bool dry;
  long count;
};

struct Recipe {
  char title[256];
  int ingred_count;
  struct Ingredient ingredients[64];
};

void printIngredient(struct Ingredient ing);

struct Recipe parse(const char *fname);
  
#endif // PARSER_H
