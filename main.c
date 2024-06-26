#include <stdio.h> // printf
#include <stdlib.h>
#include <string.h>
#include <time.h> // for the randomization function
#include "parser.h" // Recipe, printIngredient, parse
#include "stack.h"
#include "execute.h"
#include "utils.h"

int main(int argc, char *argv[]) {

    srand(time(NULL));

    //We will have status changes come after the file which is being modified so that it is easier to incorporate new things as we get to it
    if(argc<2 || argc==3 && strcmp(argv[2], "-v") || argc>=4) error("Invalid arguments. A file path to read from or a - indicating data from the standard input is required, and an optional -v (verbose) flag may follow.");

    char *filename = argv[1];
    int verbose = argc == 3 && !strcmp(argv[2], "-v") ? 1 : 0;

    // Parse file as Recipe
    char **names;
    struct Recipe *recipes = malloc(0);
    int recipe_count = 0;
    FILE *file = !strcmp(filename, "-") ? stdin : fopen(filename, "r");
    if(!file) error("Error opening file.");

    for(int j = 0; !feof(file); j++){
      names = malloc(sizeof(char*) * 64);
      recipes = realloc(recipes, sizeof(struct Recipe) * (recipe_count+1));
      recipes[recipe_count++] = parse(file, names);
      if(verbose){
        printf("\n=== %s (serves %d) ===\n", recipes[j].title, recipes[j].serves);
        for(int i=0; i<recipes[j].ingred_count; i++){
          printIngredient(recipes[j].ingredients[i], names[i]);
        }
        printf("\n");
        printStepHeaders();
        for(int i=0; i<recipes[j].step_count; i++){
            printStep(names, recipes[j].steps[i]);
        }
      }
      for(int i=0; i<recipes[j].ingred_count; i++) free(names[i]);
    free(names);
    }

    if(strcmp(filename, "-")) fclose(file);

    if(verbose) printf("\n---executing:---\n");
    execute(recipe_count, recipes);


    free(recipes[0].ingredients); // i DO NOT understand
    for(int i=0; i<recipe_count; i++){
      free(recipes[i].steps);
    }
    free(recipes);

    printf("\n");

    // Testing stack implementation
    // struct Stack* tester = createStack();
    // struct Ingredient holdingOne = {DRY, 11111};
    // struct Ingredient holdingTwo = {DRY, 22222};
    // struct Ingredient holdingThree = {DRY, 33333};
    // struct Ingredient holdingFour = {DRY, 44444};

    // push(tester, holdingOne);
    // push(tester, holdingTwo);
    // push(tester, holdingThree);
    // push(tester, holdingFour);
    // printf("%ld \n", peek(tester).count);
    // pop(tester);
    // printf("%ld \n", peek(tester).count);
    // pop(tester);
    // printf("%ld \n", peek(tester).count);
    // pop(tester);
    // printf("%ld \n", peek(tester).count);
    // pop(tester);
    // deleteStack(tester);


    // tester = createStack();
    // push(tester, holdingOne);
    // push(tester, holdingTwo);
    // push(tester, holdingThree);
    // push(tester, holdingFour);
    // pushNSpacesDown(tester, 1);
    // printf("\n");
    // printf("%ld \n", peek(tester).count);
    // pop(tester);
    // printf("%ld \n", peek(tester).count);
    // pop(tester);
    // printf("%ld \n", peek(tester).count);
    // pop(tester);
    // printf("%ld \n", peek(tester).count);
    // pop(tester);
    // deleteStack(tester);

    // // Now test randomizaiton function
    // tester = createStack();
    // push(tester, holdingOne);
    // push(tester, holdingTwo);
    // push(tester, holdingThree);
    // push(tester, holdingFour);
    // randomizeStack(tester);
    // printf("\n");
    // printf("%ld \n", peek(tester).count);
    // pop(tester);
    // printf("%ld \n", peek(tester).count);
    // pop(tester);
    // printf("%ld \n", peek(tester).count);
    // pop(tester);
    // printf("%ld \n", peek(tester).count);
    // pop(tester);
    // deleteStack(tester);
}
