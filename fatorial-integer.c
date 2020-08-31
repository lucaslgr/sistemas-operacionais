#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

//*Calcula fatorial recursivamente
int calc_fatorial(int num){
    return (num == 0 || num == 1)? 1 : (calc_fatorial(num - 1)*num);
}

//!OBS: Primeiro argumento do argv é o numero a se calcular o fatorial
int main(int argc, char *argv[]){
    //===================================<CÁLCULO DO FATORIAL>===================================
    //pritnf("Informe o numero que deseja calcular o fatorial: ");
    //scanf("%d", &num);

    int num = 0;
    //int *count_overflow = 0;
    num = atoi(argv[1]);

    printf("Valor informado foi: %d \n\r", num);

    //INT_MAX = 2^(32-1) = 2147483647
    printf("O limite máximo do valor de um inteiro nessa máquina é: %d\r\n", INT_MAX);

    //printf("Resultado: %d\n\r", fat);
    printf("Resultado: %d\r\n", calc_fatorial (num));

    //Checando se houver o overflow
    //if(fat <= 0){
    //    printf("Ocorreu um overflow...\r\n");
    //}

    return 0;
}