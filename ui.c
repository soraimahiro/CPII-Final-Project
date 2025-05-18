#include "ui.h"

#include <stdio.h>

void get_init_info(int8_t* mode) {
    printf("Enter play mode: ");
    scanf("%hhd", mode);
    printf("Mode: %hhd\n", *mode);
}
