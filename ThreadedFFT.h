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
    int length;
} Input;

void *partialFFT( void *arg ) {
    Input *input = (Input *) arg;
    Cpl **W = initialize_W( input->length );

    pthread_exit( NULL );
    return NULL;
}

#endif //INC_6010H4_THREADEDFFT_H
