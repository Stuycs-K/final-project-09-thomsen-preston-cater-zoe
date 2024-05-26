#include <stdio.h>  // FILE, fgets, fopen, printf
#include <stdlib.h> // strol
#include <string.h> // strncmp, strlen, strchr, strncpy, strcspn
#include <stdbool.h> // bool, true, false
#include <ctype.h>  // isspace
#define PCRE2_CODE_UNIT_WIDTH 8
#include <pcre2.h>
#include "parser.h" // Ingredient, Recipe

bool hasPrefix(const char *str, const char *pre){
  return strncmp(pre, str, strlen(pre)) == 0;
}

void printIngredient(struct Ingredient ing){
    printf("%ld\t%s\t%s\n", ing.count, (char*[]){"DRY", "WET", "???"}[ing.state], ing.name);
}

void printStepHeaders(){
  printf("%-12s", "command name");
  printf("\tbowl#");
  printf("\t%-16s", "ingredient name");
  printf("\t%s", "val");
  printf("\t%s", "string");
  printf("\n");
}

static char *commandNames[] = {"INPUT", "PUSH", "POP", "ADD", "ADD_MANY", "SUBTRACT", "MULTIPLY", "DIVIDE", "GLYPH", "GLYPH_MANY", "PUSHDOWN", "PUSHDOWN_CONST", "RANDOMIZE", "CLEAN", "PRINT", "WHILE", "END", "BREAK", "SUBROUTINE", "RETURN"};
void printStep(struct Ingredient *ings, struct Step step){
  printf("%-12s", commandNames[step.command]);

  if(step.bowl == -1) printf("\t");
  else printf("\tbowl%d", step.bowl);

  printf("\t%-16s", step.ingredient == -1 ? "" : ings[step.ingredient].name);

  if(step.val == -1) printf("\t");
  else printf("\t%d", step.val);

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

struct CommandParse {
  enum Command command;
  char *pattern;
  pcre2_code *regex;
  pcre2_match_data *matches;
};

struct CommandParse parses[20] = {
  {INPUT, "Take (?<ingredient>.+) from (the )?refrigerator"},
  {PUSH, "Put (?<ingredient>.+) into (the |(?<bowl>.+)th )?mixing bowl"},
  {POP, "Fold (?<ingredient>.+) into (the |(?<bowl>.+)th )?mixing bowl"},
  {ADD_MANY, "Add dry ingredients( to (the |(?<bowl>.+)th )?mixing bowl)?"},
  {ADD, "Add (?<ingredient>.+)( to (the |(?<bowl>.+)th )?mixing bowl)?"},
  {SUBTRACT, "Remove (?<ingredient>.+)( from (the |(?<bowl>.+)th )?mixing bowl)?"},
  {MULTIPLY, "Combine (?<ingredient>.+)( into (the |(?<bowl>.+)th )?mixing bowl)?"},
  {DIVIDE, "Divide (?<ingredient>.+)( into (the |(?<bowl>.+)th )?mixing bowl)?"},
  {GLYPH_MANY, "Liquefy contents of (the |(?<bowl>.+)th )?mixing bowl"},
  {GLYPH, "Liquefy (?<ingredient>.+)"},
  {PUSHDOWN_CONST, "Stir( (the |(?<bowl>.+)th )?mixing bowl)? for (?<minutes>.+) minutes.+"},
  {PUSHDOWN, "Stir (?<ingredient>.+) into (the |(?<bowl>.+)th )?mixing bowl"},
  {RANDOMIZE, "Mix( (the |(?<bowl>.+)th )?mixing bowl)? well"},
  {CLEAN, "Clean (the |(?<bowl>.+)th )?mixing bowl"},
  {PRINT, "Pour contents of (the |(?<bowl>.+)th )?mixing bowl into (the |(?<dish>.+)th )?baking dish"},
  {SUBROUTINE, "Serve with (?<recipe>.+)"},
  {RETURN, "Refrigerate( for (?<hours>.+) hours)?"},
  {WHILE, "(?<verb>.+) the (?<ingredient>.+)"},
  {END, "(.+) the (?<ingredient>.+) until (?<verb>.+)"},
  {BREAK, "Set( aside)"}
};

void setupParses(){
  int errornumber;
  PCRE2_SIZE erroroffset;
  for(int i=0; i<20; i++){
    parses[i].regex = pcre2_compile((PCRE2_SPTR) parses[i].pattern, PCRE2_ZERO_TERMINATED, PCRE2_UNGREEDY, &errornumber, &erroroffset, NULL);
    if(parses[i].regex == NULL){
      PCRE2_UCHAR buffer[256];
      pcre2_get_error_message(errornumber, buffer, sizeof(buffer));
      printf("PCRE2 compilation failed at offset %d: %s\n", (int)erroroffset, buffer);
    }
    parses[i].matches = pcre2_match_data_create_from_pattern(parses[i].regex, NULL);
  }
}

void cleanupParses(){
  for(int i=0; i<20; i++){
    pcre2_code_free(parses[i].regex);
    pcre2_match_data_free(parses[i].matches);
  }
}

struct Step strToStep(struct Ingredient* ings, int ingred_count, char *str){
  struct Step step = {
    .command = -1,
    .ingredient = -1,
    .bowl = -1,
    .val = -1,
    .string = ""
  };

  int matched;
  int rc;
  for(matched=0; matched<20; matched++){
    rc = pcre2_match(parses[matched].regex, str, strlen(str), 0, PCRE2_ANCHORED | PCRE2_ENDANCHORED, parses[matched].matches, NULL);
    if(rc > 0) break;
  }
  if(rc < 0) printf("No match found.");

  step.command = parses[matched].command;
  PCRE2_SIZE *ovector = pcre2_get_ovector_pointer(parses[matched].matches);

  PCRE2_SPTR name_table;
  int namecount;
  int name_entry_size;
  pcre2_pattern_info(parses[matched].regex, PCRE2_INFO_NAMECOUNT, &namecount);
  pcre2_pattern_info(parses[matched].regex, PCRE2_INFO_NAMEENTRYSIZE, &name_entry_size);
  pcre2_pattern_info(parses[matched].regex, PCRE2_INFO_NAMETABLE, &name_table);
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
      if(0 >= sscanf(curr, "%d", &step.val)) step.val = 0;
    }
    if(!strcmp("minutes", tabptr + 2)){
      if(0 >= sscanf(curr, "%d", &step.val)) step.val = 0;
    }
    if(!strcmp("hours", tabptr + 2)){
      if(0 >= sscanf(curr, "%d", &step.val)) step.val = 0;
    }
    if(!strcmp("recipe", tabptr + 2)){
      strcpy(step.string, curr);
    }
    if(!strcmp("verb", tabptr + 2)){
      strcpy(step.string, curr);
    }

    tabptr += name_entry_size;
  }

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
  setupParses();
  for(int i=0; i<16; i++){
    readUntil(line, 256, '.', file);
    skipSpaces(file);
    recipe.steps[i] = strToStep(recipe.ingredients, recipe.ingred_count, line);
  }
  cleanupParses();

  return recipe;
}
