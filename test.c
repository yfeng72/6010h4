
#include <stdio.h>
#include <math.h>

#define n 128 //采样点
#define PI 3.14159265358979323846
#define f1 20
#define f2 30
#define fs 100 //采样频率

int main ()
{	
	double func[n] = {}; 
	double t = (double)1/fs;
	printf("%f\n", t);
    for (int i=0;i<n;i++){
    	func[i] = cos(2*PI*f1*i*t)+ sin(2*PI*f2*i*t);
    }

    FILE *file = fopen("record.csv","w");
    printf("----------start recording--------------\n");
    fprintf(file, "%d\n", n);
    for(int i = 0; i < n; i++) {
        printf("%f", func[i]);
        fprintf(file,"%f,%d\n",func[i], 0);
        printf("\n");
    }
    return 0;
}