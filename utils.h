#ifndef UTIL_H
#define UTIL_H

void error(const char *msg);

int hasPrefix(const char *str, const char *pre);
char *readUntil(char *str, int n, char c, FILE *stream);
char *fgets2(char *restrict s, int n, FILE *restrict stream);
char *trimSpaces(char *curr);

#endif
