//
// Created by Yu Feng on 11/15/2016.
//

#ifndef INC_6010H4_THREADEDFFT_H
#define INC_6010H4_THREADEDFFT_H

#include <pthread.h>
#include "Complex.h"
#include "FastFT.h"
/**
 * Input struct for performing FFT on the first and second half
 */
typedef struct input_t {
    Cpl **list;
    Cpl **W;
    int length;
} Input;

/**
 * Input struct for merging two transformed half arrays
 */
typedef struct merge_t {
    Cpl **halfW;
    Cpl **firstHalf;
    Cpl **secondHalf;
    Cpl **result;
    int length;
    int add;
} Merge;

/**
 * Input struct for generating W^.5
 */
typedef struct half_w {
    Cpl **halfW;
    int length;
    int firstHalf;
} HalfW;

/**
 * Input struct for generating W
 */
typedef struct full_w {
    Cpl **W;
    int length;
    int firstHalf;
} FullW;

/**
 * This fills the W^.5 array with each thread generating half of the array
 * @param arg Input struct
 */
void *generateHalfW( void *arg );

/**
 * This fills the W array with each thread generating half of the array
 * @param arg Input struct
 */
void *generateW( void *arg );

/**
 * Performs FFT on set even/odd indexed items from the array
 * @param arg Input struct
 */
void *partialFFT( void *arg );

/**
 * Merges two transformed array into one transformed array
 * @param arg
 */
void *merge( void *arg );

/**
 * Performs FFT with 2 threads on the input
 * @param input  Input array
 * @param size   Size of Input array
 */
void threadedFFT( Cpl **input, int *size );

#endif //INC_6010H4_THREADEDFFT_H
