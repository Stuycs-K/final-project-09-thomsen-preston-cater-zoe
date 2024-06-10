#include <stdio.h> // fprintf
#include <stdlib.h> // exit
#include <string.h> // strlen, strcspn, strncmp
#include <ctype.h>  // isspace

// Throw an error
void error(const char *msg){
  fprintf(stderr, "%s\n", msg);
  exit(1);
}

// Checks if a string begins with a given prefix
int hasPrefix(const char *str, const char *pre){
  return strncmp(pre, str, strlen(pre)) == 0;
}

// Like fgets, but reads until a character is found
char *readUntil(char *str, int n, char c, FILE *stream){
  char *curr = str;
  while (n-- > 0 && (*curr = getc(stream)) != c && *curr != EOF) curr++;
  *curr = '\0';
  return str;
}

// Like fgets, but removes the trailing newline
char *fgets2(char *restrict s, int n, FILE *restrict stream){
  if(!fgets(s, n, stream)) return NULL;
  s[strcspn(s, "\n")] = '\0';
  return s;
}

// Remove spaces from the start of a string
char *trimSpaces(char *curr){
  while(curr && isspace(*curr)) curr++;
  return curr;
}

