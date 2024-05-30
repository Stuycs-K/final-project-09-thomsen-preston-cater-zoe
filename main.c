#include <stdio.h> // printf
#include <stdlib.h>
#include <string.h>
#include <time.h> // for the randomization function
#include "parser.h" // Recipe, printIngredient, parse
#include "stack.h"
#include "execute.h"

int main(int argc, char *argv[]) {

    //We will have status changes come after the file which is being modified so that it is easier to incorporate new things as we get to it
    char *filename = argv[1];

    // Parse file as Recipe
    char names[64][128];
    struct Recipe recipe = parse(filename, names);

    //Print Recipe details for debugging purposes
    printf("%s\n", recipe.title);
    for(int i=0; i<recipe.ingred_count; i++){
        printIngredient(recipe.ingredients[i], names[i]);
    }
    printStepHeaders();
    for(int i=0; i<recipe.step_count; i++){
        printStep(names, recipe.steps[i]);
    }

    printf("\n");
    execute(recipe);

    printf("\n---\n\n");

    /*
    // Testing stack implementation
    srand(time(NULL)); //Prepare for randomization shennanigans.

    struct Stack* tester = createStack();
    struct Ingredient holdingOne = {DRY, 11111};
    struct Ingredient holdingTwo = {DRY, 22222};
    struct Ingredient holdingThree = {DRY, 33333};
    struct Ingredient holdingFour = {DRY, 44444};

    push(tester, holdingOne);
    push(tester, holdingTwo);
    push(tester, holdingThree);
    push(tester, holdingFour);
    printf("%ld \n", peek(tester).count);
    pop(tester);
    printf("%ld \n", peek(tester).count);
    pop(tester);
    printf("%ld \n", peek(tester).count);
    pop(tester);
    printf("%ld \n", peek(tester).count);
    pop(tester);
    deleteStack(tester);


    tester = createStack();
    push(tester, holdingOne);
    push(tester, holdingTwo);
    push(tester, holdingThree);
    push(tester, holdingFour);
    pushNSpacesDown(tester, 1);
    printf("\n");
    printf("%ld \n", peek(tester).count);
    pop(tester);
    printf("%ld \n", peek(tester).count);
    pop(tester);
    printf("%ld \n", peek(tester).count);
    pop(tester);
    printf("%ld \n", peek(tester).count);
    pop(tester);
    deleteStack(tester);

    // Now test randomizaiton function
    tester = createStack();
    push(tester, holdingOne);
    push(tester, holdingTwo);
    push(tester, holdingThree);
    push(tester, holdingFour);
    randomizeStack(tester);
    printf("\n");
    printf("%ld \n", peek(tester).count);
    pop(tester);
    printf("%ld \n", peek(tester).count);
    pop(tester);
    printf("%ld \n", peek(tester).count);
    pop(tester);
    printf("%ld \n", peek(tester).count);
    pop(tester);
    deleteStack(tester);
    */
}
