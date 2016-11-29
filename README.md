# 6010h4

# Authors

Yu Feng, Yuzhou Li

# Compilation Instructions

gcc Complex.c Complex.h FastFT.c FastFT.h ThreadedFFT.c ThreadedFFT.h main.c -o main.o -lm -std=c99 -lpthread -O2

# Running Instructions

To run the benchmark program:
./main.o -benchmark

To run FFT, populate the input.csv with the input array, the first line being the number of terms
Then run ./main.o without arguments, the results will be printed to console output

To run inverse FFT, populate the input.csv with the input array, the first line being the number of terms
Then run ./main.o -inverse, the results will be printed to console output

# Input Format

Inputs are located in input.csv, the first line should be the number of terms
Number of terms MUST be an integer power of 2
Each line in the CSV file after the first line represents a complex number entry
The complex number (a + bi) is represented with "a,b" in the CSV file
There should NOT be a newline character at the end of the file