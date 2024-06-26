#include <stdio.h>  // FILE, fgets, fopen, fclose
#include <stdlib.h> // strol
#include <string.h> // strncmp, strlen, strcpy, strcspn
#define PCRE2_CODE_UNIT_WIDTH 8
#include <pcre2.h>
#include <ctype.h>  // isspace
#include "types.h" // Ingredient, Recipe, 128
#include "utils.h" // hasPrefix, readUntil, fgets2, trimSpaces
#include "parser.h"

// Helper struct to store commands and their associated patterns
struct CommandParse {
  enum Command command;
  char *pattern;
  pcre2_code *regex;
  pcre2_match_data *matches;
};

// Ordered array of command-pattern pairs: earlier indices have higher precedence
// First pattern is only for ingredients
#define PATTERN_COUNT 22
struct CommandParse parses[PATTERN_COUNT] = {
  {-1, "\\s*(\\d+)?\\s+(?:(?:(heaped|level)\\s+)?(g|kg|pinch|ml|l|dash|cup|teaspoon|tablespoon)(?:e?s)?\\s+)?(.+)"},
  {INPUT, "Take (?<ingredient>.+) from (the )?refrigerator"},
  {PUSH, "Put (?<ingredient>.+) into (the )?((?<bowl>\\d+)(st|nd|rd|th) )?mixing bowl"},
  {POP, "Fold (?<ingredient>.+) into (the )?((?<bowl>\\d+)(st|nd|rd|th) )?mixing bowl"},
  {ADD_MANY, "Add dry ingredients( to (the )?((?<bowl>\\d+)(st|nd|rd|th) )?mixing bowl)?"},
  {ADD, "Add (?<ingredient>.+)( to (the )?((?<bowl>\\d+)(st|nd|rd|th) )?mixing bowl)?"},
  {SUBTRACT, "Remove (?<ingredient>.+)( from (the )?((?<bowl>\\d+)(st|nd|rd|th) )?mixing bowl)?"},
  {MULTIPLY, "Combine (?<ingredient>.+)( into (the )?((?<bowl>\\d+)(st|nd|rd|th) )?mixing bowl)?"},
  {DIVIDE, "Divide (?<ingredient>.+)( into (the )?((?<bowl>\\d+)(st|nd|rd|th) )?mixing bowl)?"},
  {GLYPH_MANY, "Liquefy contents of (the )?((?<bowl>\\d+)(st|nd|rd|th) )?mixing bowl"},
  {GLYPH, "Liquefy (?<ingredient>.+)"},
  {PUSHDOWN_CONST, "Stir( (the )?((?<bowl>\\d+)(st|nd|rd|th) )?mixing bowl)? for (?<minutes>.+) minutes?"},
  {PUSHDOWN, "Stir (?<ingredient>.+) into (the )?((?<bowl>\\d+)(st|nd|rd|th) )?mixing bowl"},
  {RANDOMIZE, "Mix( (the )?((?<bowl>\\d+)(st|nd|rd|th) )?mixing bowl)? well"},
  {CLEAN, "Clean (the )?((?<bowl>\\d+)(st|nd|rd|th) )?mixing bowl"},
  {PRINT, "Pour contents of (the )?((?<bowl>\\d+)(st|nd|rd|th) )?mixing bowl into (the )?((?<dish>.+)(st|nd|rd|th) )?baking dish"},
  {SUBROUTINE, "Serve with (?<recipe>.+)"},
  {RETURN, "Refrigerate( for (?<hours>.+) hours?)?"},
  {SYSTEM, "Broil contents of (the )?((?<dish>\\d+)(st|nd|rd|th) )?baking dish"},
  {END, "(.+) (the (?<ingredient>.+) )?until (?<verb>.+)"},
  {WHILE, "(?<verb>.+) the (?<ingredient>.+)"},
  {BREAK, "Set( aside)"}
};

// Run once: compile all command regexes and prepare them to be run
void setupParses(){
  int errornumber;
  PCRE2_SIZE erroroffset;
  for(int i=0; i<PATTERN_COUNT; i++){
    uint32_t options = i ? PCRE2_UNGREEDY : 0;
    parses[i].regex = pcre2_compile((PCRE2_SPTR) parses[i].pattern, PCRE2_ZERO_TERMINATED, options, &errornumber, &erroroffset, NULL);
    if(parses[i].regex == NULL){
      PCRE2_UCHAR buffer[256] = "PCRE2 compilation failed: ";
      pcre2_get_error_message(errornumber, buffer + strlen(buffer), sizeof(buffer) - strlen(buffer));
      error(buffer);
    }
    parses[i].matches = pcre2_match_data_create_from_pattern(parses[i].regex, NULL);
  }
}

// Run once: clean up all command regexes
void cleanupParses(){
  for(int i=0; i<PATTERN_COUNT; i++){
    pcre2_code_free(parses[i].regex);
    pcre2_match_data_free(parses[i].matches);
  }
}

// -- Ingredient parsing functions

// Read a single-line ingredient string as an Ingredient
struct Ingredient strToIng(char *str, char **name){
  struct Ingredient ing;
  
  int rc = pcre2_match(parses[0].regex, str, strlen(str), 0, PCRE2_ANCHORED | PCRE2_ENDANCHORED, parses[0].matches, NULL);
  if (rc < 0) error("match error");
  PCRE2_SIZE *ovector = pcre2_get_ovector_pointer(parses[0].matches);

  char *field[5];
  for (int i = 1; i < rc; i++) {
    field[i] = str + ovector[2*i];
    str[ovector[2*i+1]] = '\0';
  }

  // How many of an ingredient
  ing.count = strtol(field[1], NULL, 10);

  // The state of an ingredient
  if(!strcmp(field[2], "heaped") || !strcmp(field[2], "level") ||
     !strcmp(field[3], "g") || !strcmp(field[3], "kg") || !strcmp(field[3], "cup")){
    ing.state = DRY;
  }
  else if(!strcmp(field[3], "ml") || !strcmp(field[3], "l") || !strcmp(field[3], "dash")){
    ing.state = LIQUID;
  }
  else{
    ing.state = UNKNOWN;
  }

  // The name of an ingredient
  *name = strdup(field[4]);

  return ing;
}

// --- Step parsing functions

// Read a single string as a step
struct Step strToStep(char **names, int ingred_count, char *str){
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
  // skip first pattern, it's only for ingredients
  for(matched=1; matched<PATTERN_COUNT; matched++){
    rc = pcre2_match(parses[matched].regex, str, strlen(str), 0, PCRE2_ANCHORED | PCRE2_ENDANCHORED, parses[matched].matches, NULL);
    if(rc > 0) break;
  }
  if(rc < 0) error("No match found.");

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
      for(int i=0; i<ingred_count; i++) if(!strcmp(names[i], curr)) step.ingredient = i;
      if(step.ingredient == -1 && step.command != END){
        char buffer[256];
        snprintf(buffer, sizeof(buffer), "Invalid ingredient name: %s", curr);
        error(buffer);
      }
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
struct Recipe parse(FILE *file, char **names){
  struct Recipe recipe;
  char line[256];

  setupParses();

  // Get title
  fgets2(recipe.title, 128, file);
  if(recipe.title[strlen(recipe.title)-1] == '.')
    recipe.title[strlen(recipe.title)-1] = '\0';

  // Skip everything after the title and before the ingredients
  while(strcmp(fgets2(line, 256, file), "Ingredients."));

  // Read ingredients one line at a time until the next blank line
  recipe.ingred_count = 0;
  recipe.max_ingreds = 64;
  recipe.ingredients = malloc(sizeof(struct Ingredient) * 64);
  //names = malloc(128 * 64);
  while(strcmp(fgets2(line, 256, file), "")){
    recipe.ingredients[recipe.ingred_count] = strToIng(line, &(names[recipe.ingred_count]));
    recipe.ingred_count++;
    if(recipe.ingred_count >= recipe.max_ingreds){
      recipe.max_ingreds += 64;
      recipe.ingredients = realloc(recipe.ingredients, sizeof(struct Ingredient) * recipe.max_ingreds);
      //names = realloc(recipe.ingredients, 128 * recipe.max_ingreds);
    }
  }

  // Skip everything after the ingredients and before the method
  while(strcmp(fgets2(line, 256, file), "Method."));

  // Read method one sentence at a time
  recipe.step_count = 0;
  recipe.max_steps = 64;
  recipe.steps = malloc(sizeof(struct Step) * 64);
  char c;
  // read until whitespace followed by newline peeked
  while('\n' != ungetc(getc(file), file)){
    // read next step
    readUntil(line, 256, '.', file);
    // parse line
    recipe.steps[recipe.step_count++] = strToStep(names, recipe.ingred_count, line);
    if(recipe.step_count >= recipe.max_steps){
      recipe.max_steps += 64;
      recipe.steps = realloc(recipe.steps, sizeof(struct Step) * recipe.max_steps);
    }
    // delete next character if it's a space or newline
    if(!isspace(c = getc(file))) ungetc(c, file);
  }

  // Skip all blank lines
  while(!strcmp(fgets2(line, 256, file), ""));

  // Parse "serves" line
  sscanf(line, "Serves %d.", &recipe.serves);

  // skip all blank characters
  while(isspace(c = fgetc(file)));
  ungetc(c, file);

  cleanupParses();

  return recipe;
}
