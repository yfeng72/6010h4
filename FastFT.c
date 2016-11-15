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
            Cpl *temp = x[i % size_x];
            x[i % size_x] = x[j % size_x];
            x[i % size_x] = temp;
            printf( "i = %d\n", i );
            printf( "j = %d\n", j );
            printf( "%f\n", temp->re );
            printf( "%f\n", temp->im );
        }
    }
}

/**
 * Performs Cooley-Tukey FFT algorithm on the input array
 */
void FFT( Cpl **W, Cpl **x, int size_x ) {
    printf( "Start of List\n" );
    for ( int i = 0; i < size_x; i++ ) {
        printf( "%f ", x[i]->re );
        printf( "%f\n", x[i]->im );
    }
    printf( "End of List\n" );
    int i = 0;
    int j = 0;
    int k = 0;
    int l = 0;
    printf( "start inverse\n" );
    InverseX( x, size_x );
    printf( "end inverse\n" );
    printf( "Start of List\n" );
    for ( int i = 0; i < size_x; i++ ) {
        printf( "%f ", x[i]->re );
        printf( "%f\n", x[i]->im );
    }
    printf( "End of List\n" );
    printf( "first for loop start\n" );
    for ( i = 0; i < log2( size_x ); i++ ) {
        l = 1 << i;
        printf( "l is %d", l );
        printf( "second for loop start\n" );
        for ( j = 0; j < size_x; j += 2 * l ) {
            printf( "third for loop start\n" );
            for ( k = 0; k < l; k++ ) {
                Cpl *product = multiply( x[j + k + l], W[size_x * k / l / 2] );
                Cpl *up = add( x[j + k], product );
                Cpl *down = minus( x[j + k], product );
                x[j + k] = up;
                printf( "%d modified\n", j + k );
                x[j + k + l] = down;
                printf( "%d modified\n", j + k +l );
            }
        }
    }
}

/**
 * Performs inverse FFT on the given input
 */
void InverseFFT( Cpl **W, Cpl **x, int size_x ) {
    int i, j, k;
    int l = size_x;
    Cpl *up;
    Cpl *down;
    for ( i = 0; i < log2( size_x ); i++ ) {
        l /= 2;
        for ( j = 0; j < size_x; j += 2 * l ) {
            for ( k = 0; k < l; k++ ) {
                up = add( x[j + k], x[j + k + l] );
                up->re /= 2.0;
                up->im /= 2.0;
                down = minus( x[j + k], x[j + k + l] );
                down->re /= 2.0;
                down->im /= 2.0;
                down = divide( down, W[size_x * k / 2 / l] );
                x[j + k] = up;
                x[j + k + l] = down;
            }
        }
    }
    InverseX( x, size_x );
}


void printrs( Cpl **x, int size_x ) {
    for ( int i = 0; i < size_x; i++ ) {
        printf( "i is %d\n", i );
        printf( "x[%d] is (%f,%f) \n", i, x[i]->re, x[i]->im );
    }
}