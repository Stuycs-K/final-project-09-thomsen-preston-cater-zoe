#include <stdio.h> // printf
#include <stdlib.h>
#include <string.h>
#include "parser.h" // parse
#include "types.h" // Recipe, debug printing
#include "stack.h"

int main(int argc, char *argv[]) {
    char *filename = argv[1];
    //We will have status changes come after the file which is being modified so that it is easier to incorporate new things as we get to it

    // Parse file as Recipe
    struct Recipe recipe = parse(filename);

    // Print Recipe details for debugging purposes
    printf("%s: %d servings\n", recipe.title, recipe.serves);
    for(int i=0; i<recipe.ingred_count; i++){
        printIngredient(recipe.ingredients[i]);
    }
    printf("\n");
    printStepHeaders();
    for(int i=0; i<recipe.step_count; i++){
        printStep(recipe.ingredients, recipe.steps[i]);
    }
}
