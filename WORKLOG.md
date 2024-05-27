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

## Preston Thomsen

### 5/22/2024

- Ironed out the distribution of work and got our workspace ready to begin proper coding.
- Reread the documentation for Chef and refamiliarized myself with some old coding projects I did in C to try and remember the language

### 5/23/2024

- Implemented a dynamic stack system into C with stack.c and stack.h after much consideration over the merits of dynamic and static stack systems

### 5/24/2024
