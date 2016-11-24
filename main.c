//
//  main.c
//  CSE6010A4
//
//  Created by Yuzhou Li on 10/31/16.
//  Copyright © 2016 Yuzhou Li. All rights reserved.
//

#include <stdio.h>
#include <pthread.h>
#include "stdlib.h"
#include "string.h"
#include "Complex.h"
#include "FastFT.h"
#include "ThreadedFFT.h"

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
    for ( int i = 0; i <numberOfTerms; i++ ) {
        x[i] = newComplex( 0.0, 0.0 );
    }
    *size = numberOfTerms;

    int count = 0;
    while ( fgets( line, sizeof( line ), file )) {
        char *num1 = strtok( line, "," );
        double re = atof( num1 );
        char *num2 = strtok( NULL, "," );
        double im = atof( num2 );
        x[count] = newComplex( re, im );

        count++;
    }
    /*
    for(int k=0;k<100;k++){
        printf("Test the readcsv (%f,%f)\n",x[k].re,x[k].im);
    }
    */
    return x;
}

void threadedFFT( Cpl **input, int *size ) {
    int halfSize = *size >> 1;
    Cpl **halfPoint = input + halfSize;
    printf( "half point is %f, %f\n", halfPoint[0]->re, halfPoint[0]->im );
    Cpl **firstHalf = (Cpl **) malloc( halfSize * sizeof( Cpl * ) );
    Cpl **secondHalf = (Cpl **) malloc( halfSize * sizeof( Cpl * ) );
    for ( int i = 0; i < halfSize; i += 1 ) {
        firstHalf[i] = input[i << 1];
        secondHalf[i] = input[( i << 1 ) | 1];
    }
    Cpl **W = initialize_W( halfSize );
    Cpl **halfW = initialize_HalfW( halfSize );
    Input *fh = (Input *) malloc( sizeof( Input ) );
    Input *sh = (Input *) malloc( sizeof( Input ) );
    fh->length = halfSize;
    fh->list = firstHalf;
    fh->W = W;
    sh->length = halfSize;
    sh->list = secondHalf;
    sh->W = W;
    pthread_t thread1;
    pthread_t thread2;
    pthread_create( &thread1, NULL, partialFFT, (void *) fh );
    pthread_create( &thread2, NULL, partialFFT, (void *) sh );
    pthread_join( thread1, NULL );
    pthread_join( thread2, NULL );
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

int main( int argc, const char *argv[] ) {

    /*
    Cpl a1 = newComplex(5, 6);
    Cpl a2 = newComplex(3, 7);
    Cpl a3 = newComplex(8, 12);
    Cpl a4 = newComplex(15, 4);
    int sizeofx = 4;
    printf("start init\n");
    Cpl* halfW = initialize_W(sizeofx);
    printf("end init\n");
    Cpl x[4];
    x[0] = a1;
    x[1] = a2;
    x[2] = a3;
    x[3] = a4;
    printf("start fft\n");
    InverseFFT(halfW, x, sizeofx);
    printf("end init\n");
    printrs(x,sizeofx);
    */
    int size_x = 0;
    Cpl **x = readCSVtoX( "100complex.csv", &size_x );
    Cpl **y = readCSVtoX( "100complex.csv", &size_x );
    printf( "The size of x is %d\n", size_x );
    clock_t beforeRun = clock();
    threadedFFT( x, &size_x );
    clock_t afterRun = clock();
    long long threadedRT = (long long) afterRun - beforeRun;
    printf( "FFT end\n" );
    printf( "printrs end\n" );
    beforeRun = clock();
    Cpl **W = initialize_W( size_x );
    FFT( W, y, size_x );
    afterRun = clock();
    long long singleThreadRT = (long long) afterRun - beforeRun;
    printf( "FFT end\n" );
    printf( "printrs end\n" );
    InverseFFT( W, y, size_x );
    InverseFFT( W, x, size_x );
    printf( "ifft\n" );
    printf( "threaded time %lld\n", threadedRT );
    printf( "single thread time %lld\n", singleThreadRT );
    return 0;
}







