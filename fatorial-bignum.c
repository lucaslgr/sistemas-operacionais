#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "./lib/bignum.c"

//*Calcula fatorial recursivamente
bignum calc_fatorial(bignum bignum_n){
    bignum bignum_zero, bignum_one,bignum_two;
    int_to_bignum(0, &bignum_zero);
    int_to_bignum(1, &bignum_one);
    int_to_bignum(2, &bignum_two);

    bignum bignum_n_copy;
    bignum_n_copy = bignum_n;

    if(compare_bignum(&bignum_zero, &bignum_n) == 0 || compare_bignum(&bignum_one, &bignum_n) == 0){
        return bignum_one;
    } else{
        bignum bignum_n_decremented;
        subtract_bignum(&bignum_n_copy, &bignum_one, &bignum_n_decremented);

        bignum partial_result = calc_fatorial(bignum_n_decremented);
        
        bignum bignum_result;

        multiply_bignum(&partial_result, &bignum_n, &bignum_result);

        return bignum_result;
    }
}

//!OBS: Primeiro argumento do argv é o numero a se calcular o fatorial
int main(int argc, char *argv[]){
    //===================================<CÁLCULO DO FATORIAL COM BIGNUM>===================================
    //pritnf("Informe o numero que deseja calcular o fatorial: ");
    //scanf("%d", &num);

    bignum bignum_n;
    int num;

    //Convertendo de char para inteiro
    num = atoi(argv[1]);
    //Convertendo de inteiro para bignum
    int_to_bignum(num,&bignum_n);

    printf("Valor informado foi: %d \n\r", num);

    //INT_MAX = 2^(32-1) = 2147483647
    printf("O limite máximo do valor de um inteiro nessa máquina é: %d\r\n", INT_MAX);

    //printf("Resultado: %d\n\r", fat);
    
    printf("Resultado: \r\n");

    bignum result = calc_fatorial(bignum_n);
    print_bignum(&result);

    return 0;
}