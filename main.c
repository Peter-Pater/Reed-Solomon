#include <stdio.h>
#include "arithmetic.h"
#include "utility.h"

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

  // check multiply with modular reduction:
  int e = 137; //10001001
  int f = 42; //00101010
  int prime_polynomial = 285; //100011101
  int r3 = gf_mul_MR_BCH_8bits(e, f, prime_polynomial);
  printf("the result is %d\n", r3);
  printBinary(r3);

  // table_generator(prime_polynomial);
  struct Tables *tables = newTables(prime_polynomial, 256);
  // int *exponents = tables->gf_exp;
  for (int i = 0; i < 512; i++)
  {
      printf("gf_exp[%d] = %d\n", i, *(tables->gf_exp + i));
  }

  delTables(tables);

  return 0;
}

