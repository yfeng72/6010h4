//
//  FastFT.c
//  CSE6010A4
//
//  Created by Yuzhou Li on 11/2/16.
//  Copyright © 2016 Yuzhou Li. All rights reserved.
//

#include "FastFT.h"
#include "Complex.h"
#include "stdlib.h"
#include "math.h"

Cpl* initialize_W (int size_x){
    
    Cpl* W;
    W = (Cpl *) malloc(sizeof(Cpl)*size_x);
    for (int i=0;i<size_x;i++){
        W[i].real = cos(2* M_PI/(double)(size_x)*i);
        W[i].image = -sin(2*M_PI/(double)(size_x)*i);
    }
    return W;
}

void InverseX(Cpl *x, int size_x){
    Cpl temp;
    unsigned short i;
    unsigned short j;
    unsigned short k;
    double t;
    for (i=0;i<size_x;i++){
        k=i;
        j=0;
        t=log2(size_x);
        printf("1f1f1f1f%f\n",t);
        while(t>0){
            t--;
            printf("2f2f2f2f%f\n",t);
            j=j<<1;
            j|= (k&1);
            k = k>>1;
        }
        printf("end while\n");
        if (j>1){
            temp = x[i];
            x[i] = x[j];
            x[j] = temp;
        }
    }
}

void FFT(Cpl *W, Cpl *x, int size_x){
    int i,j,k,l;
    i=0;
    j=0;
    k=0;
    l=0;
    Cpl up,down,product;
    printf("start inver\n");
    InverseX(x, size_x);
    printf("end inver\n");
    printf("first for loop start\n");
    for (i=0;i<log2(size_x);i++){
        l=1<<i;
        printf("l is %d",l);
        printf("second for loop start\n");
        for(j=0;j<size_x;j+=2*l){
            printf("third for loop start\n");
            for (k=0;k<l;k++){
                product = multiply(x[j+k+l], W[size_x*k/l/2]);
                up = add(x[j+k],product);
                down = minus(x[j+k],product);
                x[j+k]=up;
                x[j+k+l]=down;
            }
        }
    }
}

void InverseFFT(Cpl *W,Cpl *x,int size_x){
    int i,j,k;
    int l = size_x;
    Cpl up,down;
    for (i=0;i<log2(size_x);i++){
        l/=2;
        for (j=0;j<size_x;j+=2*l){
            for(k=0;k<l;k++){
                up = add(x[j+k],x[j+k+l]);
                up.real /=2;
                up.image /= 2;
                down = minus(x[j+k],x[j+k+l]);
                down.real /=2;
                down.image /= 2;
                down = divide(down,W[size_x*k/2/l]);
                x[j+k]=up;
                x[j+k+l]=down;
            }
        }
    }
    InverseX(x,size_x);
}


void printrs(Cpl *x, int size_x){
    for(int i=0;i<size_x;i++){
        printf("i is %d\n",i);
        printf("x[%d] is (%f,%f) \n",i,x[i].real,x[i].image);
    }
}