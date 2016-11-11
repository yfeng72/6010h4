//
//  main.c
//  CSE6010A4
//
//  Created by Yuzhou Li on 10/31/16.
//  Copyright © 2016 Yuzhou Li. All rights reserved.
//

#include <stdio.h>
#include "stdlib.h"
#include "string.h"
#include "Complex.h"
#include "FastFT.h"

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


int main( int argc, const char *argv[] ) {

    /*
    Cpl a1 = newComplex(5, 6);
    Cpl a2 = newComplex(3, 7);
    Cpl a3 = newComplex(8, 12);
    Cpl a4 = newComplex(15, 4);
    int sizeofx = 4;
    printf("start init\n");
    Cpl* W = initialize_W(sizeofx);
    printf("end init\n");
    Cpl x[4];
    x[0] = a1;
    x[1] = a2;
    x[2] = a3;
    x[3] = a4;
    printf("start fft\n");
    InverseFFT(W, x, sizeofx);
    printf("end init\n");
    printrs(x,sizeofx);
    */
    int size_x = 0;
    Cpl **x = readCSVtoX( "100complex.csv", &size_x );
    printf( "The size of x is %d\n", size_x );

    printf( "Create W start\n" );
    Cpl **W = initialize_W( size_x );
    printrs( W, size_x );
    printf( "W close\n" );
    printf( "FFT start\n" );
    FFT( W, x, size_x );
    printf( "FFT end\n" );
    printrs( x, size_x );
    printf( "printrs end\n" );

    return 0;
}







