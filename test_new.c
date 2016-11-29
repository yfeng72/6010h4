#include <stdio.h>
#include <stdlib.h>
#include "Complex.h"
#include "math.h"


void output (Cpl **input, int size, char *fileName) {

	FILE *file = fopen(fileName,"w");
    printf("----------start recording--------------\n");
    fprintf(file, "%d\n", size);
    for(int i = 0; i < size - 1 ; i++) {
        fprintf(file,"%f,%f\n",input[i]->re,input[i]->im);
    }
    fprintf(file,"%f,%f",input[size - 1]->re,input[size - 1]->im);
    file.close();
}
