#include <stdio.h>  // FILE, fgets, fopen, printf
#include <stdlib.h> // strol
#include <string.h> // strncmp, strlen, strchr, strncpy, strcspn
#include <stdbool.h> // bool, true, false
#include <ctype.h>  // isspace
#include <regex.h>
#define PCRE2_CODE_UNIT_WIDTH 8
#include <pcre2.h>
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

  char *pattern;

  if(hasPrefix(str, "Take ")){
    step.command = INPUT;
  } else if(hasPrefix(str, "Put ")){
    step.command = PUSH;
    pattern = "Put (?<ingredient>.+) into (the |(?<bowl>.+)th )?mixing bowl";
  } else if(hasPrefix(str, "Fold ")){
    step.command = POP;
  } else if(hasPrefix(str, "Add ")){
    step.command = ADD;
    pattern = "Add (?<ingredient>.+) to (the |(?<bowl>.+)th )?mixing bowl";
  } else if(hasPrefix(str, "Remove ")){
    step.command = SUBTRACT;
    pattern = "Remove (?<ingredient>.+) from (the |(?<bowl>.+)th )?mixing bowl";
  } else if(hasPrefix(str, "Combine ")){
    step.command = MULTIPLY;
  } else if(hasPrefix(str, "Divide ")){
    step.command = DIVIDE;
  } else if(hasPrefix(str, "Add dry ")){
    step.command = ADD_MANY;
  } else if(hasPrefix(str, "Liquefy contents ")){
    step.command = GLYPH_MANY;
    pattern = "Liquefy contents of (the |(?<bowl>.+)th )?mixing bowl";
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
    pattern = "Pour contents of (the |(?<bowl>.+)th )?mixing bowl into (the |(?<dish>.+)th )?baking dish";
  } else if(!strcmp(str, "Set aside")){
    step.command = END;
  } else if(hasPrefix(str, "Serve with ")){
    step.command = SUBROUTINE;
  } else if(hasPrefix(str, "Refrigerate ")){
    step.command = RETURN;
  } else{
    step.command = WHILE;
  }

  int errornumber;
  PCRE2_SIZE erroroffset;
  PCRE2_SPTR name_table;
  int namecount;
  int name_entry_size;

  pcre2_code *re = pcre2_compile((PCRE2_SPTR) pattern, PCRE2_ZERO_TERMINATED, 0, &errornumber, &erroroffset, NULL);
  if(re == NULL) printf("compilation error\n");

  pcre2_match_data *match_data = pcre2_match_data_create_from_pattern(re, NULL);
  int rc = pcre2_match(re, str, strlen(str), 0, PCRE2_ANCHORED | PCRE2_ENDANCHORED, match_data, NULL);
  PCRE2_SIZE *ovector = pcre2_get_ovector_pointer(match_data);
  if (rc < 0) printf("matching error %d\n", rc);

  pcre2_pattern_info(re, PCRE2_INFO_NAMECOUNT, &namecount);
  pcre2_pattern_info(re, PCRE2_INFO_NAMEENTRYSIZE, &name_entry_size);
  pcre2_pattern_info(re, PCRE2_INFO_NAMETABLE, &name_table);
  PCRE2_SPTR tabptr = name_table;

  for (int i = 0; i < namecount; i++) {
    int n = (tabptr[0] << 8) | tabptr[1]; // capture group number
    char *curr = str + ovector[2*n];
    str[ovector[2*n+1]]  ='\0';

    if(!strcmp("ingredient", tabptr + 2)){
      for(int i=0; i<ingred_count; i++) if(!strcmp(ings[i].name, curr)) step.ingredient = i;
    }
    if(!strcmp("bowl", tabptr + 2)){
      if(0 >= sscanf(curr, "%d", &step.bowl)) step.bowl = 0;
    }
    if(!strcmp("dish", tabptr + 2)){
      if(0 >= sscanf(curr, "%d", &step.bowl)) step.val = 0;
    }

    tabptr += name_entry_size;
  }

  pcre2_match_data_free(match_data);
  pcre2_code_free(re);

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
  for(int i=0; i<16; i++){
    readUntil(line, 256, '.', file);
    skipSpaces(file);
    recipe.steps[i] = strToStep(recipe.ingredients, recipe.ingred_count, line);
  }

  return recipe;
}
