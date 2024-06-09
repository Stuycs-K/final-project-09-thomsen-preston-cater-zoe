#include <stdio.h> // printf, fgets
#include <string.h> // strlen, strcmp
#include <stdlib.h> // exit
#include "obfuscate.h"

#define PRINT 0
#define EXECUTE 1

int assignChars(char *input){
  int count = 0;
  for(char *i = input; *i; i++){
    char *j = input;
    while(*i != *j) j++; // find spot where both have same value
    if(i==j) ings[count++].c = *i; // compare addresses not values
    if(count == 100){
      fprintf(stderr, "More than 100 distinct characters used -- too many. Please use fewer distinct characters.\n");
      exit(1);
    }
  }
  return count;
}

int main(int argc, char *argv[]){
  int command;
  if(argc == 3 && !strcmp(argv[1], "-p")) command = PRINT;
  else if(argc == 3 && !strcmp(argv[1], "-x")) command = EXECUTE;
  else{
    fprintf(stderr, "Incorrect usage. Provide either the flag -p (to print) or -x (to execute) followed by a string to encode.\n");
    exit(1);
  }
  char *input = argv[2];
  int count = assignChars(input);

  printf("Arbitary Tarts.\n");
  if(command == PRINT) printf("\nPrints the arbitrary string '%s'.\n", input);
  if(command == EXECUTE) printf("\nCalls the arbitrary string '%s' as a shell command.\n", input);
  
  printf("\nIngredients.\n");
  for(int i = 0; i < count; i++)
    printf("%d %s%s\n", (int) ings[i].c, ings[i].measure, ings[i].name);
  
  printf("\nMethod.\n");
  for(int i = strlen(input)-1; i >= 0; i--){
    struct Ingredient *search = ings;
    while(input[i] != search->c) search++;
    printf("Put %s into mixing bowl.\n", search->name);
  }
  
  printf("Liquefy contents of mixing bowl.\nPour contents of the mixing bowl into the baking dish.");
  if(command == PRINT) printf("\n\nServes 1.\n");
  if(command == EXECUTE) printf("\nBroil contents of baking dish.\n\nServes 0.\n");
}
