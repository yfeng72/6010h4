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

Cpl* initialize_W (int size_x);
void InverseX(Cpl *x, int size_x);
void FFT(Cpl *W, Cpl *x, int size_x);
void InverseFFT(Cpl *W,Cpl *x,int size_x);
void printrs(Cpl *x, int size_x);

#endif /* FastFT_h */
