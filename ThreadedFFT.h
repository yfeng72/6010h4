//
// Created by Yu Feng on 11/15/2016.
//

#ifndef INC_6010H4_THREADEDFFT_H
#define INC_6010H4_THREADEDFFT_H

#include <pthread.h>
#include "Complex.h"
#include "FastFT.h"

typedef struct input_t {
    Cpl **list;
    Cpl **W;
    int length;
} Input;

typedef struct merge_t {
    Cpl **halfW;
    Cpl **firstHalf;
    Cpl **secondHalf;
    Cpl **result;
    int length;
    int add;
} Merge;


void *partialFFT( void *arg );

void *merge( void *arg );

#endif //INC_6010H4_THREADEDFFT_H
