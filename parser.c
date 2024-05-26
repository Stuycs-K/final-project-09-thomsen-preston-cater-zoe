#include <stdio.h>  // FILE, fgets, fopen
#include <stdlib.h> // strol
#include <string.h> // strncmp, strlen, strchr, strncpy, strcspn
#include <ctype.h>  // isspace
#define PCRE2_CODE_UNIT_WIDTH 8
#include <pcre2.h>
#include "types.h" // Ingredient, Recipe
#include "parser.h"

// --- Utility functions

// Checks if a string begins with a given prefix
int hasPrefix(const char *str, const char *pre){
  return strncmp(pre, str, strlen(pre)) == 0;
}

// Like fgets, but reads until a character is found
char *readUntil(char *str, int n, char c, FILE *stream){
  char *curr = str;
  while (n-- > 0 && (*curr = getc(stream)) != c && *curr != EOF) curr++;
  *curr = '\0';
  return str;
}

// Like fgets, but removes the trailing newline
char *fgets2(char *restrict s, int n, FILE *restrict stream){
  fgets(s, n, stream);
  s[strcspn(s, "\n")] = '\0';
  return s;
}

// Remove spaces from the start of a string
char *trimSpaces(char *curr){
  while(curr && isspace(*curr)) curr++;
  return curr;
}

// -- Ingredient parsing functions

// Read a single-line ingredient string as an Ingredient
struct Ingredient strToIng(char *str){
  struct Ingredient ing;
  
  // Parse ingredient count
  char* text;
  ing.count = strtol(str, &text, 10);
  text = trimSpaces(text);
  char* postcount = trimSpaces(strchr(text, ' '));

  // Parse ingredient state and name
  if(hasPrefix(text, "heaped ") || hasPrefix(text, "level ")){
    ing.state = DRY;
    strncpy(ing.name, trimSpaces(strchr(postcount, ' ')), 256);
  }
  else if(hasPrefix(text, "ml ")   || hasPrefix(text, "l ") ||
          hasPrefix(text, "dash ") || hasPrefix(text, "dashes ")){
    ing.state = LIQUID;
    strncpy(ing.name, postcount, 256);
  }
  else if(hasPrefix(text, "g ")          || hasPrefix(text, "kg ") ||
          hasPrefix(text, "pinch ")      || hasPrefix(text, "pinches ")){
    ing.state = DRY;
    strncpy(ing.name, postcount, 256);
  }
  else if(hasPrefix(text, "cup ")        || hasPrefix(text, "cups ") ||
          hasPrefix(text, "teaspoon ")   || hasPrefix(text, "teaspoons ") ||
          hasPrefix(text, "tablespoon ") || hasPrefix(text, "tablespoons ")){
    ing.state = UNKNOWN;
    strncpy(ing.name, postcount, 256);
  }
  else{
    ing.state = UNKNOWN;
    strncpy(ing.name, text, 256);
  }

  return ing;
}

// --- Step parsing functions

// Helper struct to store commands and their associated patterns
struct CommandParse {
  enum Command command;
  char *pattern;
  pcre2_code *regex;
  pcre2_match_data *matches;
};

// Ordered array of command-pattern pairs: earlier indices have higher precedence
#define PATTERN_COUNT 20
struct CommandParse parses[PATTERN_COUNT] = {
  {INPUT, "Take (?<ingredient>.+) from (the )?refrigerator"},
  {PUSH, "Put (?<ingredient>.+) into (the |(?<bowl>.+)(st|nd|rd|th) )?mixing bowl"},
  {POP, "Fold (?<ingredient>.+) into (the |(?<bowl>.+)(st|nd|rd|th) )?mixing bowl"},
  {ADD_MANY, "Add dry ingredients( to (the |(?<bowl>.+)(st|nd|rd|th) )?mixing bowl)?"},
  {ADD, "Add (?<ingredient>.+)( to (the |(?<bowl>.+)(st|nd|rd|th) )?mixing bowl)?"},
  {SUBTRACT, "Remove (?<ingredient>.+)( from (the |(?<bowl>.+)(st|nd|rd|th) )?mixing bowl)?"},
  {MULTIPLY, "Combine (?<ingredient>.+)( into (the |(?<bowl>.+)(st|nd|rd|th) )?mixing bowl)?"},
  {DIVIDE, "Divide (?<ingredient>.+)( into (the |(?<bowl>.+)(st|nd|rd|th) )?mixing bowl)?"},
  {GLYPH_MANY, "Liquefy contents of (the |(?<bowl>.+)(st|nd|rd|th) )?mixing bowl"},
  {GLYPH, "Liquefy (?<ingredient>.+)"},
  {PUSHDOWN_CONST, "Stir( (the |(?<bowl>.+)(st|nd|rd|th) )?mixing bowl)? for (?<minutes>.+) minutes?.+"},
  {PUSHDOWN, "Stir (?<ingredient>.+) into (the |(?<bowl>.+)(st|nd|rd|th) )?mixing bowl"},
  {RANDOMIZE, "Mix( (the |(?<bowl>.+)(st|nd|rd|th) )?mixing bowl)? well"},
  {CLEAN, "Clean (the |(?<bowl>.+)(st|nd|rd|th) )?mixing bowl"},
  {PRINT, "Pour contents of (the |(?<bowl>.+)(st|nd|rd|th) )?mixing bowl into (the |(?<dish>.+)(st|nd|rd|th) )?baking dish"},
  {SUBROUTINE, "Serve with (?<recipe>.+)"},
  {RETURN, "Refrigerate( for (?<hours>.+) hours)?"},
  {WHILE, "(?<verb>.+) the (?<ingredient>.+)"},
  {END, "(.+) (the (?<ingredient>.+) )?until (?<verb>.+)"},
  {BREAK, "Set( aside)"}
};

// Run once: compile all command regexes and prepare them to be run
void setupParses(){
  int errornumber;
  PCRE2_SIZE erroroffset;
  for(int i=0; i<PATTERN_COUNT; i++){
    parses[i].regex = pcre2_compile((PCRE2_SPTR) parses[i].pattern, PCRE2_ZERO_TERMINATED, PCRE2_UNGREEDY, &errornumber, &erroroffset, NULL);
    if(parses[i].regex == NULL){
      PCRE2_UCHAR buffer[256];
      pcre2_get_error_message(errornumber, buffer, sizeof(buffer));
      printf("PCRE2 compilation failed at offset %d: %s\n", (int)erroroffset, buffer);
    }
    parses[i].matches = pcre2_match_data_create_from_pattern(parses[i].regex, NULL);
  }
}

// Run once: clean up all command regexes
void cleanupParses(){
  for(int i=0; i<20; i++){
    pcre2_code_free(parses[i].regex);
    pcre2_match_data_free(parses[i].matches);
  }
}

// Read a single string as a step
struct Step strToStep(struct Ingredient* ings, int ingred_count, char *str){
  // Default values
  struct Step step = {
    .command = -1,
    .ingredient = -1,
    .bowl = -1,
    .val = -1,
    .string = ""
  };

  // Find matching regex
  int matched;
  int rc;
  for(matched=0; matched<20; matched++){
    rc = pcre2_match(parses[matched].regex, str, strlen(str), 0, PCRE2_ANCHORED | PCRE2_ENDANCHORED, parses[matched].matches, NULL);
    if(rc > 0) break;
  }
  if(rc < 0) printf("No match found."); // Really should throw an error here (malformed command)

  // Store previously-calculated matches and associated command name
  step.command = parses[matched].command;
  PCRE2_SIZE *ovector = pcre2_get_ovector_pointer(parses[matched].matches);

  // Get named capture group details
  PCRE2_SPTR name_table;
  int namecount;
  int name_entry_size;
  pcre2_pattern_info(parses[matched].regex, PCRE2_INFO_NAMECOUNT, &namecount);
  pcre2_pattern_info(parses[matched].regex, PCRE2_INFO_NAMEENTRYSIZE, &name_entry_size);
  pcre2_pattern_info(parses[matched].regex, PCRE2_INFO_NAMETABLE, &name_table);
  PCRE2_SPTR tabptr = name_table;

  // Iterate through named capture groups and perform the appropriate actions
  for (int i = 0; i < namecount; i++) {
    int n = (tabptr[0] << 8) | tabptr[1]; // capture group number
    char *curr = str + ovector[2*n];
    str[ovector[2*n+1]]  ='\0';

    // Ingredient: find index of ingredient in ingredient list and save
    if(!strcmp("ingredient", tabptr + 2)){
      for(int i=0; i<ingred_count; i++) if(!strcmp(ings[i].name, curr)) step.ingredient = i;
    }
    // Bowl: parse number, otherwise default to 1
    else if(!strcmp("bowl", tabptr + 2)){
      if(0 >= sscanf(curr, "%d", &step.bowl)) step.bowl = 1;
    }
    // Dish: parse number, otherwise default to 1 (only used in print)
    else if(!strcmp("dish", tabptr + 2)){
      if(0 >= sscanf(curr, "%d", &step.val)) step.val = 1;
    }
    // Minutes: parse number (only used in pushdown_const)
    else if(!strcmp("minutes", tabptr + 2)){
      sscanf(curr, "%d", &step.val); // really should throw an error
    }
    // Hours: parse number, otherwise default to 0 (only used in return)
    else if(!strcmp("hours", tabptr + 2)){
      if(0 >= sscanf(curr, "%d", &step.val)) step.val = 0;
    }
    // Recipe: save subroutine name (only used in subroutine)
    else if(!strcmp("recipe", tabptr + 2)){
      strcpy(step.string, curr);
    }
    // Verb: save verb (only used in while and end)
    else if(!strcmp("verb", tabptr + 2)){
      strcpy(step.string, curr);
    }

    // Continue to next named capture group
    tabptr += name_entry_size;
  }

  return step;
}

// Takes a filename and parses the file as a recipe
struct Recipe parse(const char *fname){
  FILE *file = fopen(fname, "r");
  struct Recipe recipe;
  char line[256];

  // Get title
  fgets2(recipe.title, 256, file);
  if(recipe.title[strlen(recipe.title)-1] == '.')
    recipe.title[strlen(recipe.title)-1] = '\0';

  // Skip everything after the title and before the ingredients
  while(strcmp(fgets2(line, 256, file), "Ingredients."));

  // Read ingredients one line at a time until the next blank line
  recipe.ingred_count = 0;
  while(strcmp(fgets2(line, 256, file), "")){
    recipe.ingredients[recipe.ingred_count++] = strToIng(line);
  }

  // Skip everything after the ingredients and before the method
  while(strcmp(fgets2(line, 256, file), "Method."));

  // Read method one sentence at a time
  setupParses();
  recipe.step_count = 0;
  char c;
  // read until whitespace followed by newline peeked
  while('\n' != ungetc(getc(file), file)){
    // read next step
    readUntil(line, 256, '.', file);
    // parse line
    recipe.steps[recipe.step_count++] = strToStep(recipe.ingredients, recipe.ingred_count, line);
    // delete next character if it's a space or newline
    if(!isspace(c = getc(file))) ungetc(c, file);
  }
  cleanupParses();

  // Skip all blank lines
  while(!strcmp(fgets2(line, 256, file), ""));

  // Parse "serves" line
  sscanf(line, "Serves %d.", &recipe.serves);

  // Work needed: parse subroutines

  return recipe;
}
