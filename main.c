#include <stdio.h>
#include "arithmetic.h"

void test(){
    printf("check add/sub:\n");
    int a = 5;
    int b = 6;
    int r1 = gf_sub_BCH_8bits(a, b);
    printf("the result is %d\n", r1);
    printBinary(r1);
    printf("\n");

    printf("check multiply without modular reduction:\n");
    int c = 137; //10001001
    int d = 42; //00101010
    int r2 = gf_mul_BCH_8bits(c, d);
    printf("the result is %d\n", r2);
    printBinary(r2);
    printf("\n");

    int prime_polynomial = 285; //100011101

    printf("check multiply with modular reduction:\n");
    int e = 137; //10001001
    int f = 42; //00101010
    int r3 = gf_mul_MR_BCH_8bits(e, f, prime_polynomial);
    printf("the result is %d\n", r3);
    printBinary(r3);
    printf("\n");

    struct Tables *tables = newTables(prime_polynomial, 256);
    // printTables(tables);

    printf("check multiply with LUT:\n");
    int g = 137; //10001001
    int h = 42; //00101010
    int r4 = gf_mul_MR_BCH_8bits_LUT(g, h, tables);
    printf("the result is %d\n", r4);
    printBinary(r4);
    printf("\n");

    printf("check division with LUT:\n");
    int a1 = 195; //11000011
    int a2 = 42; //00101010
    int r5 = gf_div_MR_BCH_8bits_LUT(a1, a2, tables);
    printf("the result is %d\n", r5);
    printBinary(r5);
    printf("\n");

    printf("check polynomial operations:\n");
    printf("check scale operation:\n");
    int arr1[4] = {137, 1, 4, 1};
    struct Polynomial *poly1 = newPolynomial(arr1, 4);
    struct Polynomial * ret_val = gf_poly_scale(poly1, 42, tables);
    printPolynomial(ret_val);
    delPolynomial(poly1);
    printf("\n");

    printf("check add operation:\n");
    int arr2[4] = {3, 1, 4, 1};
    int arr3[3] = {1, 2, 2};
    struct Polynomial *poly2 = newPolynomial(arr2, 4);
    struct Polynomial *poly3 = newPolynomial(arr3, 3);
    struct Polynomial * ret_val1 = gf_poly_add(poly2, poly3);
    printPolynomial(ret_val1);
    delPolynomial(poly2);
    delPolynomial(poly3);
    delPolynomial(ret_val1);
    printf("\n");

    printf("check multiply operation & evaluation:\n");
    int arr4[4] = {1, 1, 1, 1};
    int arr5[3] = {1, 1, 1};
    struct Polynomial *poly4 = newPolynomial(arr4, 4);
    struct Polynomial *poly5 = newPolynomial(arr5, 3);
    struct Polynomial * ret_val2 = gf_poly_mul(poly4, poly5, tables);
    printPolynomial(ret_val2);
    printf("The evaluation with x = 2 is: %d\n", gf_poly_eval(ret_val2, 2, tables));
    delPolynomial(poly4);
    delPolynomial(poly5);
    delPolynomial(ret_val2);

    delTables(tables);
}

int main()
{
    test();
    return 0;
}
