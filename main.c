#include <stdio.h> // printf
#include "parser.h" // parse
#include "types.h" // Recipe, debug printing, 128
#include "stack.h"
#include "execute.h"

int main(int argc, char *argv[]) {
    char *filename = argv[1];
    //We will have status changes come after the file which is being modified so that it is easier to incorporate new things as we get to it

    // Parse file as Recipe
    // The array of names is used only for debugging purposes: it's not relevant after parsing, and we can just delete it later
    char names[64][128];
    struct Recipe recipe = parse(filename, names);

    // Print Recipe details for debugging purposes
    printf("%s: %d servings\n", recipe.title, recipe.serves);
    for(int i=0; i<recipe.ingred_count; i++){
        printIngredient(recipe.ingredients[i], names[i]);
    }
    printf("\n");
    printStepHeaders();
    for(int i=0; i<recipe.step_count; i++){
        printStep(names, recipe.steps[i]);
    }

    printf("\n");
    execute(recipe);
}
