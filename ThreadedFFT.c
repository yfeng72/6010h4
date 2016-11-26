//
// Created by Yu Feng on 11/15/2016.
//
#include <stdlib.h>
#include <math.h>
#include "ThreadedFFT.h"

void *partialFFT( void *arg ) {
    Input *input = (Input *) arg;
    FFT( input->W, input->list, input->length );
    return NULL;
}
void *generateHalfW( void *arg ) {
    HalfW *input = (HalfW *) arg;
    int halfLength = input->length >> 1;
    int fullLength = input->length;
    Cpl **W = input->halfW;
    if ( input->firstHalf ) {
        for ( int i = 0; i < halfLength; i++ ) {
            Cpl *pow = newComplex( 0.0, -2.0 * M_PI * (double) i / (double) fullLength / 2.0 );
            Cpl *temp = expo( pow );
            free( pow );
            W[i] = temp;
        }
    } else {
        for ( int i = halfLength; i < fullLength; i++ ) {
            Cpl *pow = newComplex( 0.0, -2.0 * M_PI * (double) i / (double) fullLength / 2.0 );
            Cpl *temp = expo( pow );
            free( pow );
            W[i - halfLength] = temp;
        }
    }
    return NULL;
}

void *generateW( void *arg ) {
    FullW *input = (FullW *) arg;
    int halfLength = input->length >> 1;
    int fullLength = input->length;
    Cpl **W = input->W;
    if ( input->firstHalf ) {
        for ( int i = 0; i < halfLength; i++ ) {
            Cpl *pow = newComplex( 0.0, -2.0 * M_PI * (double) i / (double) fullLength );
            Cpl *temp = expo( pow );
            free( pow );
            W[i] = temp;
        }
    } else {
        for ( int i = halfLength; i < fullLength; i++ ) {
            Cpl *pow = newComplex( 0.0, -2.0 * M_PI * (double) i / (double) fullLength );
            Cpl *temp = expo( pow );
            free( pow );
            W[i - halfLength] = temp;
        }
    }
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

void threadedFFT( Cpl **input, int *size ) {
    //Initialize variables and threads
    int halfSize = *size >> 1;
    Cpl **halfPoint = input + halfSize;
    Cpl **firstHalf = (Cpl **) malloc( halfSize * sizeof( Cpl * ) );
    Cpl **secondHalf = (Cpl **) malloc( halfSize * sizeof( Cpl * ) );
    for ( int i = 0; i < halfSize; i += 1 ) {
        firstHalf[i] = input[i << 1];
        secondHalf[i] = input[( i << 1 ) | 1];
    }
    pthread_t thread1;
    pthread_t thread2;
    //Generate W with the 2 threads
    Cpl **W = malloc( halfSize * sizeof( Cpl * ) );
    FullW *first = (FullW *) malloc( sizeof( FullW ) );
    FullW *second = (FullW *) malloc( sizeof( FullW ) );
    first->length = halfSize;
    second->length = halfSize;
    first->firstHalf = 1;
    second->firstHalf = 0;
    first->W = W;
    second->W = W + ( halfSize >> 1 );
    pthread_create( &thread1, NULL, generateW, (void *) first );
    pthread_create( &thread2, NULL, generateW, (void *) second );
    pthread_join( thread1, NULL );
    pthread_join( thread2, NULL );
    //Perform FFT on both halves of the array
    Input *fh = (Input *) malloc( sizeof( Input ) );
    Input *sh = (Input *) malloc( sizeof( Input ) );
    fh->length = halfSize;
    fh->list = firstHalf;
    fh->W = W;
    sh->length = halfSize;
    sh->list = secondHalf;
    sh->W = W;
    pthread_create( &thread1, NULL, partialFFT, (void *) fh );
    pthread_create( &thread2, NULL, partialFFT, (void *) sh );
    pthread_join( thread1, NULL );
    pthread_join( thread2, NULL );
    //Generate W^.5 with the two threads
    Cpl **halfW = (Cpl **) malloc( halfSize * sizeof( Cpl * ) );
    HalfW *f = (HalfW *) malloc( sizeof( HalfW ) );
    HalfW *s = (HalfW *) malloc( sizeof( HalfW ) );
    f->length = halfSize;
    s->length = halfSize;
    f->firstHalf = 1;
    s->firstHalf = 0;
    f->halfW = halfW;
    s->halfW = halfW + ( halfSize >> 1 );
    pthread_create( &thread1, NULL, generateHalfW, (void *) f );
    pthread_create( &thread2, NULL, generateHalfW, (void *) s );
    pthread_join( thread1, NULL );
    pthread_join( thread2, NULL );
    //Merge both part of the transformed array to one array
    Merge *firstPart = (Merge *) malloc( sizeof( Merge ) );
    Merge *secondPart = (Merge *) malloc( sizeof( Merge ) );
    firstPart->length = halfSize;
    firstPart->firstHalf = firstHalf;
    firstPart->secondHalf = secondHalf;
    firstPart->result = input;
    firstPart->add = 1;
    firstPart->halfW = halfW;
    secondPart->length = halfSize;
    secondPart->firstHalf = firstHalf;
    secondPart->secondHalf = secondHalf;
    secondPart->result = halfPoint;
    secondPart->add = 0;
    secondPart->halfW = halfW;
    pthread_create( &thread1, NULL, merge, (void *) firstPart );
    pthread_create( &thread2, NULL, merge, (void *) secondPart );
    pthread_join( thread1, NULL );
    pthread_join( thread2, NULL );
}