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

#ifdef DEBUG
#define DEBUG_PRINT_VEC(vec, fmt, ...) do { \
    fprintf(stderr, "[DEBUG] " fmt " (size=%u): [", ##__VA_ARGS__, (vec)->SIZE); \
    for (int32_t vec_idx = 0; vec_idx < (vec)->SIZE; vec_idx++) { \
        fprintf(stderr, "%d", (vec)->array[vec_idx]); \
        if (vec_idx < (vec)->SIZE - 1) fprintf(stderr, ", "); \
    } \
    fprintf(stderr, "]\n"); \
} while(0)
#else
#define DEBUG_PRINT_VEC(vec, fmt, ...) ((void)0)
#endif

typedef struct _vector {
    int32_t array[256];
    uint32_t SIZE;
} vector;
vector initVector();
void pushbackVector(vector* vec, int32_t val);
void popbackVector(vector* vec);
int getVectorTop(vector* vec, int32_t* value);
void clearVector(vector* vec);
void eraseVector(vector* vec, int index);
#endif
