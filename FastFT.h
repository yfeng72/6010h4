//
//  FastFT.h
//  CSE6010A4
//
//  Created by Yuzhou Li on 11/2/16.
//  Copyright © 2016 Yuzhou Li. All rights reserved.
//

#ifndef FastFT_h
#define FastFT_h

#include <stdio.h>
#include "Complex.h"

Cpl **initialize_W( int size_x );

/**
 * Performs Cooley-Tukey Radix-2 FFT algorithm on the input array
 */
void FFT( Cpl **W, Cpl **x, int size_x );

/**
 * Helper function for performing FFT recursively
 */
void recursiveFFT( Cpl **W, Cpl **x, int totalLength, int curLength );

/**
 * Performs inverse FFT on the given input
 */
void InverseFFT( Cpl **W, Cpl **x, int size_x );

Cpl **scale( Cpl **x, int size_x );

#endif /* FastFT_h */
