#include <stdio.h> // printf
#include <stdlib.h>
#include <string.h>
#include <time.h> // for the randomization function
#include "parser.h" // Recipe, printIngredient, parse
#include "stack.h"

int main(int argc, char *argv[]) {

    srand(time(NULL)); //Prepare for randomization shennanigans.

    // char *filename = argv[1];
    // //We will have status changes come after the file which is being modified so that it is easier to incorporate new things as we get to it

    // // Parse file as Recipe
    // struct Recipe recipe = parse(filename);

    // // Print Recipe details for debugging purposes
    // printf("%s\n", recipe.title);
    // for(int i=0; i<recipe.ingred_count; i++){
    //     printIngredient(recipe.ingredients[i]);
    // }

    // Testing stack implementation
    struct Stack* tester = createStack();
    struct Ingredient holdingOne = {DRY, 11111};
    struct Ingredient holdingTwo = {DRY, 22222};
    struct Ingredient holdingThree = {DRY, 33333};
    struct Ingredient holdingFour = {DRY, 44444};

    push(tester, holdingOne);
    push(tester, holdingTwo);
    push(tester, holdingThree);
    push(tester, holdingFour);
    printf("%f \n", peek(tester).count);
    pop(tester);
    printf("%f \n", peek(tester).count);
    pop(tester);
    printf("%f \n", peek(tester).count);
    pop(tester);
    printf("%f \n", peek(tester).count);
    pop(tester);

    // push(tester, 11.11);
    // push(tester, 22.22);
    // push(tester, 33.33);
    // push(tester, 44.44);
    // pushNspacesDown(tester, 55.55, 2);
    // printf("%f \n", peek(tester));
    // pop(tester);
    // printf("%f \n", peek(tester));
    // pop(tester);
    // printf("%f \n", peek(tester));
    // pop(tester);
    // printf("%f \n", peek(tester));
    // pop(tester);
    // printf("%f \n", peek(tester));
    // pop(tester);

    // Now test randomizaiton function
    // push(tester, 11.11);
    // push(tester, 22.22);
    // push(tester, 33.33);
    // push(tester, 44.44);
    // randomizeStack(tester);
    // printf("%f \n", peek(tester));
    // pop(tester);
    // printf("%f \n", peek(tester));
    // pop(tester);
    // printf("%f \n", peek(tester));
    // pop(tester);
    // printf("%f \n", peek(tester));
    // pop(tester);

}
