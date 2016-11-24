//
// Created by Yu Feng on 11/15/2016.
//
#include <stdlib.h>
#include "ThreadedFFT.h"

void *partialFFT( void *arg ) {
    Input *input = (Input *) arg;
    FFT( input->W, input->list, input->length );
    return NULL;
}

void *merge( void *arg ) {
    Merge *input = (Merge *) arg;
    if ( input->add ) {
        for ( int i = 0; i < input->length; i++ ) {
            Cpl *product = multiply( input->secondHalf[i], input->halfW[i] );
            Cpl *sum = add( input->firstHalf[i], product );
            input->result[i] = sum;
            free( product );
        }
    } else {
        for ( int i = 0; i < input->length; i++ ) {
            Cpl *product = multiply( input->secondHalf[i], input->halfW[i] );
            Cpl *sum = minus( input->firstHalf[i], product );
            input->result[i] = sum;
            free( product );
        }
    }
    return NULL;
}