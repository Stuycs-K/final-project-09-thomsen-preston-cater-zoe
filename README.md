[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-24ddc0f5d75046c5622901739e7c5dd533143b0c8e959d652212380cedb1ea36.svg)](https://classroom.github.com/a/ecp4su41)
# The Caterers

### P Thomsen & Z Cater  (Period 10)

## Overview

>  "Chef is a programming language in which programs look like recipes."  
> — [*David Morgan-Mar*](https://www.dangermouse.net/esoteric/chef.html)

Chef is an *esoteric programming language*[^1] following the concept of *multiple coding*[^2], in which programs are intentionally designed so as to function as both valid programs and valid recipes. Chef is stack-based and utilizes three basic concepts:
1. Ingredients, which have numerical values and can be either dry, liquid, or unknown (*e.g.* 72 g haricot beans, 25 ml water, 6 human eyeballs)
2. An ordered set of infinite mixing bowls, which are the stacks primarily used for general processing and which hold various ingredients within them
3. An ordered set of infinite baking pans, which are used almost solely for data output—once items have been poured into the pans, they are not able to be removed

Together with cooking- and baking-themed commands, these three elements form a Turing-complete language[^3] that is *also* capable of describing a great number of recipes[^4]. Features such as the "Take *ingredient* from refrigerator" "Mix [the [*nth*] mixing bowl] well" directives even allow for user input and randomness, respectively, allowing us to imagine a world in which one could write and play a cooking simulator in Chef.

Our final project attempts to create a Chef interpreter in C, with the possibility of creating an obfuscator to transform simple scripts into Chef [add more to this later]

## Instructions

1. Clone the repository and ensure that gcc, make, and libpcre2-dev are installed (they are both, as far as I can tell, already installed on the CS-NFS, which is good)
    - gcc is necessary for compiling the C code
    - make is necessary for running the makefile to direct gcc what to actually compile
    - libpcre2-dev is necessary for the regular expressions used in parsing the input files, which were as far as I can tell completely unfeasible to reasonably implement using POSIX regular expressions and far too small-scale to be worth utilizing lex and yacc for
2. Run `make compile` to compile the project, followed by `./main 'recipes/souffle.chef'` or any other Chef program to execute the interpreter. Alternatively, run `make ARGS='recipes/souffle.chef'` or any other Chef program to run the project and compile if necessary. [add more later as more features are implemented]
3. Clean up using `make clean` at any time to delete the executable and any `.o` files


[^1]: Per [Wikipedia](https://en.wikipedia.org/wiki/Esoteric_programming_language): "An *esoteric programming language* (sometimes shortened to *esolang*) is a programming language designed to test the boundaries of computer programming language design, as a proof of concept, as software art, as a hacking interface to another language (particularly functional programming or procedural programming languages), or as a joke. The use of the word esoteric distinguishes them from languages that working developers use to write software … Usability is rarely a goal for designers of esoteric programming languages; often their design leads to quite the opposite."
[^2]: [Mateas, Michael and Nick Montfort. “A Box, Darkly: Obfuscation, Weird Languages, and Code Aesthetics.” In Proceedings of the 6th Digital Arts and Culture Conference, IT University of Copenhagen, 1-3 Dec 2005, pp. 144-153.](https://nickm.com/cis/a_box_darkly.pdf)
[^3]: At least I assume it's Turing complete. I found [this program by Wesley Janssen, Joost Rijneveld and Mathijs Vos](https://github.com/joostrijneveld/Chef-Interpreter/blob/master/ChefInterpreter/TuringsTastyTortillas.chef) which claims to be a working Turing Machine (and it certainly seems to be) and the fact that I cannot spot any way to solve the halting problem with it certainly seem exciting, but I can find no actual analysis and do not trust my own intuition.
[^4]: On the other hand, I know for a *fact* that Chef cannot represent all possible recipes, because there is no way to encode my grandmother's skillet chops and rice recipe. The step "Lie about the presence of onions" is unencodable due to the simple fact that Chef requres all instructions involving arbitrary verbs to be loops and have a matching "until *present participle*" command, while the command to lie about the presence of onions can never complete.
