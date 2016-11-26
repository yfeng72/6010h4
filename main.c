//
//  main.c
//  CSE6010A4
//
//  Created by Yuzhou Li on 10/31/16.
//  Copyright © 2016 Yuzhou Li. All rights reserved.
//

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <math.h>
#include "Complex.h"
#include "String.h"
#include "FastFT.h"
#include "ThreadedFFT.h"

/**
 * Read the CSV File
 * @return the complex numbers
 */
Cpl **readCSVtoX( const char *add, int *size ) {
    FILE *file = fopen( add, "r" );
    if ( file == NULL) {
        exit( 2 );
    }
    char line[1024];
    fgets( line, sizeof( line ), file );
    int numberOfTerms = atoi( line );
    printf( "The series has %d terms\n", numberOfTerms );
    Cpl **x = (Cpl **) malloc( sizeof( Cpl * ) * numberOfTerms );
    *size = numberOfTerms;

    int count = 0;
    while ( fgets( line, sizeof( line ), file )) {
        char *num1 = strtok( line, "," );
        double re = atof( num1 );
        char *num2 = strtok(NULL, "," );
        double im = atof( num2 );
        x[count] = newComplex( re, im );

        count++;
    }
    return x;
}

int main( int argc, char **argv ) {
    if (( argc > 1 ) && !strcmp( argv[1], "-benchmark" )) {
        int size_x = 0;
        Cpl **x = readCSVtoX( "testFile.csv", &size_x );
        Cpl **y = readCSVtoX( "testFile.csv", &size_x );
        printf( "The size of x is %d\n", size_x );
        printf( "Start Threaded FFT\n" );
        clock_t beforeRun = clock();
        threadedFFT( x, &size_x );
        clock_t afterRun = clock();
        double threadedRT = (double) afterRun - beforeRun;
        printf( "Threaded FFT Complete\n" );
        beforeRun = clock();
        printf( "Start Single-threaded FFT\n" );
        Cpl **W = initialize_W( size_x );
        FFT( W, y, size_x );
        afterRun = clock();
        double singleThreadRT = (double) afterRun - beforeRun;
        printf( "Single-threaded FFT Complete\n" );
        threadedRT /= ((double) CLOCKS_PER_SEC / 1000.0 );
        singleThreadRT /= ((double) CLOCKS_PER_SEC / 1000.0 );
        printf( "Threaded Rum Time %f ms\n", threadedRT );
        printf( "Single Thread Rum Time %f ms\n", singleThreadRT );
        return 0;
    } else if ( argc > 1 && !strcmp( argv[1], "-inverse" ) ) {
        int size_x = 0;
        Cpl **x = readCSVtoX( "input.csv", &size_x );
        if ( size_x & ( size_x - 1 ) ) {
            printf( "Number of terms must be power of 2" );
            return 1;
        }
        printf( "The size of x is %d\n", size_x );
        printf( "Start Inverse FFT\n" );
        Cpl **W = initialize_W( size_x );
        InverseFFT( W, x, size_x );
        printf( "Inverse FFT Complete\n" );
        printf( "Results:\n" );
        for ( int i = 0; i < size_x; i++ ) {
            printf( "%f", x[i]->re);
            if ( x[i]->im >= 0.0 ) {
                printf( " + %fi\n", x[i]->im );
            } else {
                printf( " - %fi\n", fabs( x[i]->im ) );
            }
        }
        return 0;
    } else {
        int size_x = 0;
        Cpl **x = readCSVtoX( "input.csv", &size_x );
        printf( "The size of x is %d\n", size_x );
        if ( size_x & ( size_x - 1 ) ) {
            printf( "Number of terms must be power of 2" );
            return 1;
        }
        printf( "Start Threaded FFT\n" );
        threadedFFT( x, &size_x );
        printf( "Threaded FFT Complete\n" );
        printf( "Results:\n" );
        for ( int i = 0; i < size_x; i++ ) {
            printf( "%f", x[i]->re );
            if ( x[i]->im >= 0.0 ) {
                printf( " + %fi\n", x[i]->im );
            } else {
                printf( " - %fi\n", -1.0 * x[i]->im );
            }
        }
        Cpl **scaledX = scale( x, size_x );
        printf( "Scaled Results:\n" );
        for ( int i = 0; i < size_x; i++ ) {
            printf( "%f", scaledX[i]->re );
            if ( scaledX[i]->im >= 0.0 ) {
                printf( " + %fi\n", scaledX[i]->im );
            } else {
                printf( " - %fi\n", fabs( scaledX[i]->im ) );
            }
        }
        return 0;
    }
}







