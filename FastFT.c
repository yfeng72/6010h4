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

Cpl **initialize_HalfW( int size_x ) {
    Cpl **W = (Cpl **) calloc( (size_t) size_x, sizeof( Cpl * ) );
    for ( int i = 0; i < size_x; i++ ) {
        Cpl *pow = newComplex( 0.0, -2.0 * M_PI * (double) i / (double) size_x / 2.0 );
        Cpl *temp = expo( pow );
        free( pow );
        W[i] = temp;
    }
    return W;
}

void InverseX( Cpl **x, int size_x ) {
    int j;
    int k;
    double t;
    for ( int i = 0; i < size_x; i++ ) {
        k = i;
        j = 0;
        t = log2( size_x );
        while ( t > 0 ) {
            t--;
            j = j << 1;
            j |= ( k & 1 );
            k = k >> 1;
        }
        if ( j > 1 ) {
            Cpl *temp = x[i];
            x[i] = x[j];
            x[j] = temp;
        }
    }
}

/**
 * Performs Cooley-Tukey Radix-2 FFT algorithm on the input array
 */
void FFT( Cpl **W, Cpl **x, int size_x ) {
    recursiveFFT( W, x, size_x, size_x );
}

void recursiveFFT( Cpl **W, Cpl **x, int totalLength, int curLength ) {
    if ( curLength == 1 ) return;
    int halfLength = curLength >> 1;
    Cpl **firstHalf = calloc( (size_t) halfLength, sizeof( Cpl * ) );
    Cpl **secondHalf = calloc( (size_t) halfLength, sizeof( Cpl * ) );
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
        Cpl *product = multiply( secondHalf[i], W[i * totalLength / curLength] );
        Cpl *sum = add( cur, product );
        Cpl *diff = minus( cur, product );
        free( product );
        x[i] = sum;
        x[halfLength + i] = diff;
    }
    free( firstHalf );
    free( secondHalf );
}

/**
 * Performs inverse FFT on the given input
 */
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
        scaledOutput[i] = newComplex( x[i]->re / sqrt( (double) size_x ), x[i]->im / sqrt( (double) size_x ) );
    }
    return scaledOutput;
}

void printrs( Cpl **x, int size_x ) {
    for ( int i = 0; i < size_x; i++ ) {
        printf( "i is %d\n", i );
        printf( "x[%d] is (%f,%f) \n", i, x[i]->re, x[i]->im );
    }
}