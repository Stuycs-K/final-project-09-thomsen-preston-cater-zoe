#include <stdio.h> // printf
#include <stdlib.h>
#include <string.h>
#include <time.h> // for the randomization function
#include "parser.h" // Recipe, printIngredient, parse
#include "stack.h"
#include "execute.h"

int main(int argc, char *argv[]) {

    srand(time(NULL));

    //We will have status changes come after the file which is being modified so that it is easier to incorporate new things as we get to it
    if(argc<2){
      fprintf(stderr, "No input file provided.\n");
      exit(1);
    }
    char *filename = argv[1];

    int verbose = 0;
    if(argc >= 3 && !strcmp(argv[2], "-v")) verbose = 1;

    // Parse file as Recipe
    char names[64][128];
    struct Recipe recipes[5];
    int recipe_count;
    FILE *file = fopen(filename, "r");

    for(int j = 0; !feof(file); j++){
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
    }

    fclose(file);

    if(verbose) printf("\n---executing:---\n");
    execute(recipe_count, recipes);

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
