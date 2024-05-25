#include <stdio.h>  // FILE, fgets, fopen, printf
#include <stdlib.h> // strol
#include <string.h> // strncmp, strlen, strchr, strncpy, strcspn
#include <stdbool.h> // bool, true, false
#include <ctype.h>  // isspace
#include <regex.h>
#include "parser.h" // Ingredient, Recipe

bool hasPrefix(const char *str, const char *pre){
  return strncmp(pre, str, strlen(pre)) == 0;
}

void printIngredient(struct Ingredient ing){
    printf("%ld\t%s\t%s\n", ing.count, (char*[]){"DRY", "WET", "???"}[ing.state], ing.name);
}
void printStep(struct Ingredient *ings, struct Step step){
  printf("%-12s\tbowl_%d", bob[step.command], step.bowl);
  printf("\t%-12s", step.ingredient == -1 ? "" : ings[step.ingredient].name);
  printf("\t%d", step.val);
  if(step.string) printf("\t%s", step.string);
  printf("\n");
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

struct Step strToStep(struct Ingredient* ings, int ingred_count, char *str){
  struct Step step;
  step.command = -1;
  step.ingredient = -1;
  step.bowl = -1;
  step.val = -1;
  strcpy(step.string, "");

  char *regexString;
  short flags;

  if(hasPrefix(str, "Take ")){
    step.command = INPUT;
  } else if(hasPrefix(str, "Put ")){
    step.command = PUSH;
    regexString = "Put (.+) into (the |.+th )?mi()xing bowl";
    flags = 0110;
  } else if(hasPrefix(str, "Fold ")){
    step.command = POP;
    regexString = "Fold (.+) into (the |.+th )?mi()xing bowl";
    flags = 0110;
  } else if(hasPrefix(str, "Add ")){
    step.command = ADD;
    regexString = "Add (.+) to (the |.+th )?mixing bowl()";
    flags = 0110;
  } else if(hasPrefix(str, "Remove ")){
    step.command = SUBTRACT;
    regexString = "Remove (.+) from (the |.+th )?mixing bowl()";
    flags = 0110;
  } else if(hasPrefix(str, "Combine ")){
    step.command = MULTIPLY;
    regexString = "Combine (.+) into (the |.+th )?mixing bowl()";
    flags = 0110;
  } else if(hasPrefix(str, "Divide ")){
    step.command = DIVIDE;
    regexString = "Divide (.+) into (the |.+th )?mixing bowl()";
    flags = 0110;
  } else if(hasPrefix(str, "Add dry ")){
    step.command = ADD_MANY;
  } else if(hasPrefix(str, "Liquefy contents ")){
    step.command = GLYPH_MANY;
    regexString = "Liquefy con()tents of (the |.+th )?mixi()ng bowl";
    flags = 0010;
  } else if(hasPrefix(str, "Liquefy ")){
    step.command = GLYPH;
  } else if(hasPrefix(str, "Stir ")){
    step.command = PUSHDOWN;
  } else if(hasPrefix(str, "Stir ")){ // not working
    step.command = PUSHDOWN_CONST;
  } else if(hasPrefix(str, "Mix ")){
    step.command = RANDOMIZE;
  } else if(hasPrefix(str, "Clean ")){
    step.command = CLEAN;
  } else if(hasPrefix(str, "Pour ")){
    step.command = PRINT;
    regexString = "Po()ur contents of (the |.+th )?mixing bowl into (the |.+th )?baking dish";
    flags = 0011;
  } else if(!strcmp(str, "Set aside")){
    step.command = END;
  } else if(hasPrefix(str, "Serve with ")){
    step.command = SUBROUTINE;
  } else if(hasPrefix(str, "Refrigerate ")){
    step.command = RETURN;
  } else{
    step.command = WHILE;
  }

  regex_t regexCompiled;
  regmatch_t groupArray[4];
  char *fields[4];
  regcomp(&regexCompiled, regexString, REG_EXTENDED);
  if(!regexec(&regexCompiled, str, 4, groupArray, 0)){
    for(int i=0; i<3; i++){
      fields[i] = str + groupArray[i+1].rm_so;
      str[groupArray[i+1].rm_eo] = '\0';
    }
    if((flags >> 6 & 0007) == 1){
      for(int i=0; i<ingred_count; i++) if(!strcmp(ings[i].name, fields[0])) step.ingredient = i;
    }
    if((flags >> 3 & 0007) == 1){
      if(0 >= sscanf(fields[1], "%dth ", &step.bowl)) step.bowl = 0;
    }
    if((flags >> 0 & 0007) == 1){
      if(0 >= sscanf(fields[2], "%dth ", &step.val)) step.val = 0;
    }
  }
  regfree(&regexCompiled);

  return step;
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
  for(int i=0; i<14; i++){
    readUntil(line, 256, '.', file);
    skipSpaces(file);
    recipe.steps[i] = strToStep(recipe.ingredients, recipe.ingred_count, line);
  }

  return recipe;
}
