/* main.c: Main program file */

#include <stdio.h>
#include "sqrt.h"

int main(void)
{
    double n, s;

    do
    {
        printf("Enter a positive real number N: ");
        scanf("%lf", &n);
    } while (n < 0.0);

    s = SquareRoot(n);

    printf("The square root of %g is %g", n, s);
    
}