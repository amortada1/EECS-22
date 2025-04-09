#include <stdio.h>

int main(void)
{
    int a, b;
    int sum;

    printf("Please enter an integer: ");
    scanf("%d", &a);
    printf("Please enter another integer: ");
    scanf("%d", &b);

    sum = a + b;

    printf("The sum of %d and %d is %d.\n", a, b, sum);

    return 0;
}