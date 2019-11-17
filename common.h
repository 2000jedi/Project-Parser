//
// A common header library
//

#pragma once

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <memory.h>

#define HASH_LEN 1001
#define TUPLE_LEN 10
#define STR_LEN 100
#define TABLE_MAX 10
#define NODE_LEN 100

// Hashing Function
#define h(x) (11*x[0] + 2*x[1] + 5*x[2] + 3*x[3] + 7*x[4]) % HASH_LEN

typedef char Relation[TUPLE_LEN][STR_LEN];

#define Free(x) SaveFree((void **) &x)

extern void SaveFree(void **ptr);

extern void str_concat(char **str, char c);

extern char* strdup(const char* c);

extern char* char2str(char c);
