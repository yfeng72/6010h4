//
//  FastFT.c
//  CSE6010A4
//
//  Created by Yuzhou Li on 11/2/16.
//  Copyright © 2016 Yuzhou Li. All rights reserved.
//

#include <time.h>
#include "FastFT.h"
#include "stdlib.h"
#include "math.h"
#define M_PI (3.14159265358979323846)

/**
 * Initialize a list of Exp(-2*PI*I*k/N) for all k in 0..N
 * This expedites FFT calculation process as it eliminates repeated
 * complex exponential calculation
 */
Cpl **initialize_W( int size_x ) {
    Cpl **W = (Cpl **) calloc( (size_t) size_x, sizeof( Cpl * ) );
    for ( int i = 0; i < size_x; i++ ) {
        Cpl *pow = newComplex( 0.0, -2.0 * M_PI * (double) i / (double) size_x );
        Cpl *temp = expo( pow );
        free( pow );
        W[i] = temp;
    }
    return W;
}


void FFT( Cpl **W, Cpl **x, int size_x ) {
    recursiveFFT( W, x, size_x, size_x );
}

void recursiveFFT( Cpl **W, Cpl **x, int totalLength, int curLength ) {
    //Base Case
    if ( curLength == 1 ) return;
    //Splits array in half, calculate FFT on both half and then merge
    int halfLength = curLength >> 1;
    Cpl **firstHalf = malloc( halfLength * sizeof( Cpl * ) );
    Cpl **secondHalf = malloc( halfLength * sizeof( Cpl * ) );
    for ( int i = 0; i < halfLength; i++ ) {
        firstHalf[i] = newComplex( x[i << 1]->re, x[i << 1]->im );
        secondHalf[i] = newComplex( x[( i << 1 ) | 1]->re, x[( i << 1 ) | 1]->im );
    }
    recursiveFFT( W, firstHalf, totalLength, halfLength );
    recursiveFFT( W, secondHalf, totalLength, halfLength );
    for ( int i = 0; i < halfLength; i++ ) {
        Cpl *cur = firstHalf[i];
        free( x[i] );
        free( x[halfLength + i] );
        Cpl *product = multiply( secondHalf[i], W[totalLength / curLength * i] );
        Cpl *sum = add( cur, product );
        Cpl *diff = minus( cur, product );
        free( product );
        x[i] = sum;
        x[halfLength + i] = diff;
    }
    free( firstHalf );
    free( secondHalf );
}

void InverseFFT( Cpl **W, Cpl **x, int size_x ) {
    for ( int i = 0; i < size_x; i++ ) {
        Cpl *cur = x[i];
        Cpl *conj = conjugate( cur );
        free( cur );
        x[i] = conj;
    }
    FFT( W, x, size_x );
    for ( int i = 0; i < size_x; i++ ) {
        Cpl *cur = x[i];
        Cpl *conj = conjugate( cur );
        Cpl *denom = newComplex( (double) size_x, 0.0 );
        Cpl *quotient = divide( conj, denom );
        x[i] = quotient;
        free( denom );
        free( conj );
        free( cur );
    }
}

Cpl **scale( Cpl **x, int size_x ) {
    Cpl **scaledOutput = calloc( (size_t) size_x, sizeof( Cpl * ) );
    for ( int i = 0; i < size_x; i++ ) {
        scaledOutput[i] = newComplex( x[i]->re / sqrt( (double) size_x ), -1.0 * x[i]->im / sqrt( (double) size_x ) );
    }
    return scaledOutput;
}