//
//  Complex.c
//  CSE6010A4
//
//  Created by Yuzhou Li on 10/31/16.
//  Copyright © 2016 Yuzhou Li. All rights reserved.
//

#include "Complex.h"
#include "math.h"
#include "stdlib.h"
#define e exp(1)

Cpl newComplex(double a, double b){
    Cpl N;
    N.real =a ;
    N.image = b;
    printf("New Complex (%f,%f)\n",a,b);
    return N;
}

Cpl add(Cpl a, Cpl b){
    Cpl N;
    N.real = a.real+b.real;
    N.image = a.image+b.image;
    printf("##Add## New Complex (%f,%f)\n",N.real,N.image);
    return N;
}
Cpl minus(Cpl a,Cpl b){
    Cpl N;
    N.real = a.real-b.real;
    N.image = a.image-b.image;
    printf("##Minus## New Complex (%f,%f)\n",N.real,N.image);
    return N;
}

Cpl multiply(Cpl a, Cpl b){
    Cpl N;
    N.real = a.real*b.real-a.image*b.image;
    N.image = a.image*b.real+a.real*b.image;
    printf("##Multiply## New Complex (%f,%f)\n",N.real,N.image);
    return N;
}
Cpl divide (Cpl a,Cpl b){
    Cpl N;
    if ((b.real*b.real+b.image*b.image)==0){
        printf("##Divide## N/0 Error\n");
        //printf("##Divide## New Complex (%f,%f)\n",N.real,N.image);
        return N; //(0,0)
    }else{
        N.real = (a.real*b.real+a.image*b.image)/(b.real*b.real+b.image*b.image);
        N.image = (a.image*b.real-a.real*b.image)/(b.real*b.real+b.image*b.image);
        printf("##Divide## New Complex (%f,%f)\n",N.real,N.image);
        return N;
    }
}

Cpl Euler(Cpl a){
    Cpl N;
    N.real = cos(a.image);
    N.image = sin(a.image);
    N.real = exp(a.real) * N.real;
    N.image = exp(a.real) * N.image;
    printf("##Exp## New Complex(%f,%f)\n",N.real,N.image);
    return N;
}













