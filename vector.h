#ifndef _VECTOR_H
#define _VECTOR_H
#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct _vector {
    int32_t array[256];
    uint32_t SIZE;
} vector;
vector initVector();
void pushbackVector(vector* vec, int32_t val);
void popbackVector(vector* vec);
void clearVector(vector* vec);
void eraseVector(vector* vec, int index);
#endif