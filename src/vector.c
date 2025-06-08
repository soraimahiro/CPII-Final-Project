#include "vector.h"

vector initVector() {
    vector vec;
    vec.SIZE = 0;
    memset(vec.array, 0, sizeof(vec.array));
    return vec;
}

void pushbackVector(vector* vec, int32_t val) {
    if (vec == NULL || vec->SIZE >= 256) {
        return;
    }
    vec->array[vec->SIZE] = val;
    vec->SIZE++;
}

void popbackVector(vector* vec) {
    if (vec == NULL || vec->SIZE == 0) {
        return;
    }
    vec->SIZE--;
    vec->array[vec->SIZE] = 0;
}

int getVectorTop(vector* vec, int32_t* value){
    if (vec == NULL || vec->SIZE == 0) {
        return -1;
    }
    *value = vec->array[vec->SIZE - 1];
    return 0;
}

void clearVector(vector* vec) {
    if (vec == NULL) {
        return;
    }
    vec->SIZE = 0;
    memset(vec->array, 0, sizeof(vec->array));
}

void eraseVector(vector* vec, int index) {
    if (vec == NULL || index < 0 || index >= vec->SIZE) {
        return;
    }
    for (uint32_t i = index; i < vec->SIZE - 1; i++) {
        vec->array[i] = vec->array[i + 1];
    }
    vec->SIZE--;
    vec->array[vec->SIZE] = 0;
}
