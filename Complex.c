//
//  Complex->c
//  CSE6010A4
//
//  Created by Yuzhou Li on 10/31/16
//  Copyright © 2016 Yuzhou Li. All rights reserved
//

#include <stdlib.h>
#include "Complex.h"
#include "math.h"


Cpl *newComplex( double a, double b ) {
    Cpl *N = (Cpl *) malloc( sizeof( Cpl ) );
    N->re = a;
    N->im = b;
    printf( "New Complex (%f,%f)\n", a, b );
    return N;
}

Cpl *add( Cpl *a, Cpl *b ) {
    Cpl *N = (Cpl *) malloc( sizeof( Cpl ) );
    N->re = a->re + b->re;
    N->im = a->im + b->im;
    printf( "##Add## New Complex (%f,%f)\n", N->re, N->im );
    return N;
}

Cpl *minus( Cpl *a, Cpl *b ) {
    Cpl *N = (Cpl *) malloc( sizeof( Cpl ) );
    N->re = a->re - b->re;
    N->im = a->im - b->im;
    printf( "##Minus## New Complex (%f,%f)\n", N->re, N->im );
    return N;
}

Cpl *multiply( Cpl *a, Cpl *b ) {
    Cpl *N = (Cpl *) malloc( sizeof( Cpl ) );
    N->re = a->re * b->re - a->im * b->im;
    N->im = a->im * b->re + a->re * b->im;
    printf( "##Multiply## New Complex (%f,%f)\n", N->re, N->im );
    return N;
}

Cpl *divide( Cpl *a, Cpl *b ) {
    Cpl *N = (Cpl *) malloc( sizeof( Cpl ) );
    N->re = 0;
    N->im = 0;
    if ( !( b->re * b->re + b->im * b->im ) ) {
        printf( "##Divide## N/0 Error\n" );
        //printf("##Divide## New Complex (%f,%f)\n",N->re,N->im);
        return N; //(0,0)
    } else {
        N->re = ( a->re * b->re + a->im * b->im ) / ( b->re * b->re + b->im * b->im );
        N->im = ( a->im * b->re - a->re * b->im ) / ( b->re * b->re + b->im * b->im );
        printf( "##Divide## New Complex (%f,%f)\n", N->re, N->im );
        return N;
    }
}

Cpl *expo( Cpl *a ) {
    Cpl *N = (Cpl *) malloc( sizeof( Cpl ) );
    double coefficient = exp( a->re );
    N->re = coefficient * cos( a->im );
    N->im = coefficient * cos( a->im );
    printf( "##Exp## New Complex(%f,%f)\n", N->re, N->im );
    return N;
}














