//
//  Complex.h
//  CSE6010A4
//
//  Created by Yuzhou Li on 10/31/16.
//  Copyright © 2016 Yuzhou Li. All rights reserved.
//

#ifndef Complex_h
#define Complex_h

#include <stdio.h>

typedef struct complex_t {
    double re;
    double im;
} Cpl;

Cpl *newComplex( double a, double b );

Cpl *add( Cpl *a, Cpl *b );

Cpl *minus( Cpl *a, Cpl *b );

Cpl *multiply( Cpl *a, Cpl *b );

Cpl *divide( Cpl *a, Cpl *b );

Cpl *expo( Cpl *a );

Cpl *conjugate( Cpl *a );

#endif /* Complex_h */
