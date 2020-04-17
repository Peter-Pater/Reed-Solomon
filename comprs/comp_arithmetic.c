#include <stdio.h>
#include "comp_arithmetic.h"

long gf_mul_comp(long x, long y, struct Tables *tables){
    long a1 = x >> 8; //3. 00000011
    // printf("higher order byte of x: %ld\n", a1);
    long a0 = x & 0xff; //221, 11011101
    // printf("lower order byte of x: %ld\n", a0);

    long b1 = y >> 8; //35, 00100011
    // printf("higher order byte of y: %ld\n", a1);
    long b0 = y & 0xff; //11001101
    // printf("lower order byte of y: %ld\n", a0);

    long r1 = gf_mul_MR_BCH_8bits_LUT(a1, b0, tables) ^ gf_mul_MR_BCH_8bits_LUT(a0, b1, tables) ^ gf_mul_MR_BCH_8bits_LUT(gf_mul_MR_BCH_8bits_LUT(32, a1, tables), b1, tables);
    long r0 = gf_mul_MR_BCH_8bits_LUT(a0, b0, tables) ^ gf_mul_MR_BCH_8bits_LUT(gf_mul_MR_BCH_8bits_LUT(32, a1, tables), b1, tables);
    printf("Higher order byte of result is: %ld\n", r1);
    printf("Lower order byte of result is: %ld\n", r0); // 44799
    // printf("end result is: %ld\n", (r1 << 8) + r0);
    return (r1 << 8) + r0;
}

long gf_mul_div(long x, long y, struct Tables *tables){
    return 0;
}