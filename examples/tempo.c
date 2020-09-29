#include <time.h>
#include <stdio.h>
#include <unistd.h>

int main() {
    clock_t start, end;
    start = clock();
        // função a ser medida
        for(int i = 0; i < 100000; i++)
            printf(".");
    end = clock();
    double tmp = (double) (end - start); 
    tmp = tmp / (double) CLOCKS_PER_SEC;
    printf("\nTempo decorrido %f\n", tmp);

    return 0;
}
