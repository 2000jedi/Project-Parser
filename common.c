#include "common.h"

// Free a pointer and set its value to NULL, avoiding memory leak
void SaveFree(void** ptr) {
  free(*ptr);
  *ptr = NULL;
}

// Concat a character to a string
void str_concat(char **str, char c) {
  size_t len = strlen(*str);
  char *result = (char *) calloc(sizeof(char), len + 2);
  strcpy(result, *str);
  result[len] = c;
  result[len + 1] = '\0';
  free(*str);
  *str = result;
}

// Utility to convert a char to a string with one character
char *char2str(char c) {
  char *y = malloc(sizeof(char) * 2);
  y[0] = c;
  y[1] = 0;
  return y;
}

// Equal to strdup in C++
char *strdup(const char *s) {
  char *d = malloc(strlen(s) + 1);
  if (d == NULL) return NULL;
  strcpy(d, s);
  return d;
}
