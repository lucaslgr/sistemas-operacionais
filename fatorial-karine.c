#include "./lib/bignum.c"



int main()
{
    int n;
    bignum bignum_n, bignum_result, bignum_partial_result;
    printf("Digite um valor para ser calculado o fatorial: ");
    scanf("%d", &n);

    if (n > 0)
    {
        /*
        fat=1;
        n=num;
        while(n>0)
        {
            fat*=n;  
            n--;
        }
        */

        int_to_bignum(n, &bignum_n);
        int_to_bignum(1, &bignum_result);

        while (n > 0)
        {
            bignum_partial_result = bignum_result;

            multiply_bignum(&bignum_partial_result, &bignum_n, &bignum_result);

            print_bignum(&bignum_result);
            n--;
            int_to_bignum(n, &bignum_n);
        }
        printf("Fatorial -- ");
        print_bignum(&bignum_result);
    }
    else
        printf("O calculo do fatorial se aplica somene aos numeros naturais.\n");
    return 0;
}
