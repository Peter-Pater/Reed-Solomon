#include <stdio.h>
#include "arithmetic.h"
#include "utility.h"

int main()
{

  // check add/sub:
  int a = 5;
  int b = 6;
  int result = gf_sub_BCH_8bits(a, b);
  printf("the result is %d\n", result);


  // check multiply:
  int c = 137; //10001001
  int d = 42; //00101010
  int r1 = gf_mul_BCH_8bits(c, d);
  printf("the result is %d\n", r1);
  printBinary(r1);

  // for (int i = 0; i < 256; i++)
  // {
  //   printBinary(i);
  // }

  return 0;
}

