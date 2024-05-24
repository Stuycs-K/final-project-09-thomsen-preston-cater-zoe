#include <stdio.h> // printf
#include <stdlib.h>
#include <string.h>
#include <time.h> // for the randomization function
#include "parser.h" // Recipe, printIngredient, parse
#include "stack.h"

int main(int argc, char *argv[]) {

    srand(time(NULL)); //Prepare for randomization shennanigans.

    char *filename = argv[1];
    //We will have status changes come after the file which is being modified so that it is easier to incorporate new things as we get to it

    // Parse file as Recipe
    struct Recipe recipe = parse(filename);

    // Print Recipe details for debugging purposes
    printf("%s\n", recipe.title);
    for(int i=0; i<recipe.ingred_count; i++){
        printIngredient(recipe.ingredients[i]);
    }
}
