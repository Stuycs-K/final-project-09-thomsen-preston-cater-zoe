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
    char *filename = argv[1];

    // Parse file as Recipe
    char names[64][128];
    struct Recipe recipes[5];
    int recipe_count;
    FILE *file = fopen(filename, "r");
    for(recipe_count = 0; !feof(file); recipe_count++) recipes[recipe_count] = parse(file, names);
    fclose(file);
    //struct Recipe recipe = recipes[0];

    //Print Recipe details for debugging purposes
    for(int j=0; j<recipe_count; j++){
      printf("\n=== %s ===\n", recipes[j].title);
      for(int i=0; i<recipes[j].ingred_count; i++){
          printIngredient(recipes[j].ingredients[i], names[i]);
      }
      printf("\n");
      printStepHeaders();
      for(int i=0; i<recipes[j].step_count; i++){
          printStep(names, recipes[j].steps[i]);
      }
    }

    printf("\n");
    execute(recipes[0]);

    printf("\n---\n\n");

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
