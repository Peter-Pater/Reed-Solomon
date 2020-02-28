#include<stdio.h>
#include<arithmetic.h>
#include<utility.h>

int main()
{
  int a = 5;
  int b = 6;
  int result = gf_sub_BCH_8bits(a, b);
  printf("the result is %d\n", result);

  for (int i = 0; i < 256; i++)
  {
    printBinary(i);
  }

  return 0;
}

