#include <stdio.h>
#include "arithmetic.h"

int main()
{
  // check add/sub:
  int a = 5;
  int b = 6;
  int r1 = gf_sub_BCH_8bits(a, b);
  printf("the result is %d\n", r1);
  printBinary(r1);

  // check multiply without modular reduction:
  int c = 137; //10001001
  int d = 42; //00101010
  int r2 = gf_mul_BCH_8bits(c, d);
  printf("the result is %d\n", r2);
  printBinary(r2);

  int prime_polynomial = 285; //100011101

  // check multiply with modular reduction:
  int e = 137; //10001001
  int f = 42; //00101010
  int r3 = gf_mul_MR_BCH_8bits(e, f, prime_polynomial);
  printf("the result is %d\n", r3);
  printBinary(r3);

  struct Tables *tables = newTables(prime_polynomial, 256);
  printTables(tables);

  // check multiply with LUT:
  int g = 137; //10001001
  int h = 42; //00101010
  int r4 = gf_mul_MR_BCH_8bits_LUT(g, h, tables);
  printf("the result is %d\n", r4);
  printBinary(r4);

  // check division with LUT:
  int a1 = 195; //11000011
  int a2 = 42; //00101010
  int r5 = gf_div_MR_BCH_8bits_LUT(a1, a2, tables);
  printf("the result is %d\n", r5);
  printBinary(r5);

  // check polynomial operations
  int arr1[4] = {137, 1, 4, 1};
  int arr2[3] = {1, 2, -1};
  struct Polynomial *poly1 = newPolynomial(arr1, 4);

  struct Polynomial * retval = gf_poly_scale(poly1, 42, tables);

  printPolynomial(retval);

  delPolynomial(poly1);

  delTables(tables);

  return 0;
}

