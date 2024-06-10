# Work Log

## Zoe Cater

### 2024-05-22

- Added some basic files (makefile, gitignore) and set up workspace.
- Spent ages implementing first half of recipe parsing (reading titles and ingredient lists).

### 2024-05-23

- Made ingredient parsing much more robust, less prone to breaking on multi-word ingredients
- Outlined the enums and structs needed for parsing the list of steps

### 2024-05-25

- Implemented the parsing of five commands using POSIX regexes as a proof of concept
- Realized the unfeasibility of the previous method and switched to using PCRE2 regexes
- Finished implementing most of the recipe parsing using PCRE2 regexes (still need to handle the last few lines and see about going back to the previous sections with the new parsing setup)

### 2024-05-26
- Handled the last few lines of parsing and went back to the previous sections with the new parsing setup (as promised)
- Split parser out into parser and types files
- Improved the worklog (by a lot)
- Future goals: Handling of subroutines, actually moving through parsed steps and processing them, continue making the readme bigger and better and odder

### 2024-05-27
- Simplified ingredient struct and separated out name data—it's only technically necessary for debug printing and during the parsing process, and as such doesn't need to be carted around everywhere.

### 2024-05-29
- Started working on actually executing code, using a big switch-case statement
- The Hello World Souffle completely functions for the first time
- Implemented input, push, pop, arithmetic, add_many, glyph and glyph_many, randomize (but it doesn't work), clean, and print

### 2024-05-30
- Continued working on code execution and removing magic numbers
- Implemented pushdown, looping, breaking, and early returns

### 2024-06-03
- Minor bugfixes

### 2024-06-05
- Added subroutine parsing (was awful)

### 2024-06-06
- Added subrouting execution

### 2024-06-07
- Minor bugfixes
- Added 'Broil' command, which is of our own design
- Implemented verbosity controls and improved the reverse shell code

### 2024-06-09
- Converted code obfuscator from Haskell (and sitting on a separate computer) to C (and part of our project)
- Split project into two parts: obfuscate and interpret
- Improved obfuscation capabilities
- Wrote documentation for both halves
- No upper limit on number of possible steps or ingredients anymore
- Improved error handling

### 2024-06-10
- Recorded video
- No upper limit on number of subroutines in a file

## Preston Thomsen

### 5/22/2024

- Ironed out the distribution of work and got our workspace ready to begin proper coding.
- Reread the documentation for Chef and refamiliarized myself with some old coding projects I did in C to try and remember the language

### 5/23/2024

- Implemented a dynamic stack system into C with stack.c and stack.h after much consideration over the merits of dynamic and static stack systems

### 5/24/2024

- Implemented the unique commands for stacks that chef utilizes (Randomizing the stack and pushNSpacesDown)

### 5/26/2024

- Worked on debugging the many issues with the stack functions I implemented

### 5/27/2024

- Reworked other functions to actually function in the way they are supposed to (Fixed NSpacesDown to just push the top value down and not push a new value down for example)

### 5/28/2024

- Implemented the changes Zoe desired for stacks (With the Ingredient struct)

### 5/29/2024

- Worked to fix the randomize stack function which now does rearrange the stack but it is not random.

### 5/31/2024

- Began to work on the script for the video (basic distribution of talking points and important notes to touch on)

### 6/3/2024

- Fixed newly found bugs with nodes which Zoe informed me of

### 6/4/2024

- Decided on how I will do randomization of stacks and interfaced with Zoe (Helped her with minor debugging)

### 6/6/2024

- Worked on debugging my code because somehow the randomization did not work

### 6/7/2024

- FINALLY FIXED RANDOMIZATION (with like two words of advice from Zoe)
- Finished up the script for the presentation

### 6/9-10/2024

- Did the presentation with Zoe!
