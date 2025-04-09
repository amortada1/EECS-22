#include <stdio.h>

int main(void)
{
    int a[4]; // elements of first matrix
    int b[4]; // elements of second matrix
    int c[4]; // elements of result matrix
    /*
    layout:
    a[0]  a[1]
    a[2]  a[3]
    */

    // get user input for matrix a
    printf("Enter the first matrix(a) that will be multiplied:");
    scanf("%d", &a[0]);
    printf("a11 = %d\n", a[0]);
    scanf("%d", &a[1]);
    printf("a12 = %d\n", a[1]);
    scanf("%d", &a[2]);
    printf("a21 = %d\n", a[2]);
    scanf("%d", &a[3]);
    printf("a22 = %d\n\n", a[3]);
    
    // get user input for matrix b
    printf("Enter the second matrix(b) that will be multiplied:");
    scanf("%d", &b[0]);
    printf("b11 = %d\n", b[0]);
    scanf("%d", &b[1]);
    printf("b12 = %d\n", b[1]);
    scanf("%d", &b[2]);
    printf("b21 = %d\n", b[2]);
    scanf("%d", &b[3]);
    printf("b22 = %d\n\n", b[3]);

    // compute each element for matrix c
    c[0] = a[0]*b[0] + a[1]*b[2];
    c[1] = a[0]*b[1] + a[1]*b[3];
    c[2] = a[2]*b[0] + a[3]*b[2];
    c[3] = a[2]*b[1] + a[3]*b[3];

    // output result
    printf("Result:\n");
    printf("%d %d\n", c[0], c[1]);
    printf("%d %d\n", c[2], c[3]);

    return 0;
}