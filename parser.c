#include <stdio.h>  // FILE, fgets, fopen, printf
#include <stdlib.h> // strol
#include <string.h> // strncmp, strlen, strchr, strncpy, strcspn
#include <stdbool.h> // bool, true, false
#include <ctype.h>  // isspace
#include "parser.h" // Ingredient, Recipe

bool hasPrefix(const char *str, const char *pre){
  return strncmp(pre, str, strlen(pre)) == 0;
}

void printIngredient(struct Ingredient ing){
    printf("%ld\t%s\t%s\n", ing.count, (char*[]){"DRY", "WET", "???"}[ing.state], ing.name);
}

char *readUntil(char *str, int n, char c, FILE *stream){
  char *curr = str;
  while ((*curr = getc(stream)) != c && *curr != EOF) curr++;
  *curr = '\0';
  return str;
}

void skipSpaces(FILE *stream){
  char curr;
  while(isspace(curr = getc(stream)) && curr != EOF);
  ungetc(curr, stream);
}

char *trimSpaces(char *curr){
  while(curr && isspace(*curr)) curr++;
  return curr;
}

// Read a single-line ingredient string as an Ingredient
struct Ingredient strToIng(char *str){
  struct Ingredient ing;
  
  char* text;
  ing.count = strtol(str, &text, 10);
  text = trimSpaces(text);
  char* postmeasure = trimSpaces(strchr(text, ' '));

  if(hasPrefix(text, "heaped ") || hasPrefix(text, "level ")){
    ing.state = DRY;
    strncpy(ing.name, trimSpaces(strchr(postmeasure, ' ')), 256);
  }
  else if(hasPrefix(text, "ml ")   || hasPrefix(text, "l ") ||
          hasPrefix(text, "dash ") || hasPrefix(text, "dashes ")){
    ing.state = LIQUID;
    strncpy(ing.name, postmeasure, 256);
  }
  else if(hasPrefix(text, "g ")          || hasPrefix(text, "kg ") ||
          hasPrefix(text, "pinch ")      || hasPrefix(text, "pinches ")){
    ing.state = DRY;
    strncpy(ing.name, postmeasure, 256);
  }
  else if(hasPrefix(text, "cup ")        || hasPrefix(text, "cups ") ||
          hasPrefix(text, "teaspoon ")   || hasPrefix(text, "teaspoons ") ||
          hasPrefix(text, "tablespoon ") || hasPrefix(text, "tablespoons ")){
    ing.state = UNKNOWN;
    strncpy(ing.name, postmeasure, 256);
  }
  else{
    ing.state = UNKNOWN;
    strncpy(ing.name, text, 256);
  }

  return ing;
}

// Like fgets, but removes the trailing newline
char *fgets2(char *restrict s, int n, FILE *restrict stream){
  fgets(s, n, stream);
  s[strcspn(s, "\n")] = '\0';
  return s;
}

// Takes a filename and parses the file as a recipe
struct Recipe parse(const char *fname){
  FILE *file = fopen(fname, "r");
  struct Recipe recipe;
  char line[256];

  // Get title
  fgets2(recipe.title, 256, file);

  // Skip everything after the title and before the ingredients
  while(strcmp(fgets2(line, 256, file), "Ingredients."));

  // Read ingredients one line at a time until the next blank line
  recipe.ingred_count = 0;
  while(strcmp(fgets2(line, 256, file), "")){
    recipe.ingredients[recipe.ingred_count++] = strToIng(line);
  }

  // Skip everything after the ingredients and before the method
  while(strcmp(fgets2(line, 256, file), "Method."));

  // Read method one sentence at a time until
  for(int i=0; i<6; i++){
    readUntil(line, 256, '.', file);
    skipSpaces(file);
    printf("%s\n", line);
  }

  return recipe;
}
