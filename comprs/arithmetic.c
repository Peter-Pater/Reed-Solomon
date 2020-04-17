#include <stdio.h>
#include "arithmetic.h"

// carry-less addition (use XOR)
long gf_add_BCH_8bits(long x, long y)
{
    return x ^ y;
}

// carry-less substraction (use XOR)
long gf_sub_BCH_8bits(long x, long y)
{
    return x ^ y;
}

// multiplication
long gf_mul_BCH_8bits(long x, long y)
{
    long result = 0;
    long pos = 0;
    while ((y >> pos) > 0)
    {
        if (y & (1 << pos))
        {
            result ^= x << pos;
        }
        pos += 1;
    }
    return result;
}

// carry-less multiplication
// int gf_mul_MR_BCH_8bits(int x, int y, int prime_polynomial)
// {
//     int result = gf_mul_BCH_8bits(x, y);
//
//     if (prime_polynomial > 0)
//     {
//         result = carry_less_long_div(result, prime_polynomial);
//     }
//
//     return result;
// }

// carry-less multiplication using the Russian Peasant Multiplication algorithm
long gf_mul_MR_BCH(long x, long y, long prime_polynomial, long bits){
    long result = 0;
    while (y > 0){
        if (y & 1){
            result = gf_add_BCH_8bits(result, x);
        }
        y = y >> 1;
        x = x << 1;
        long temp = pow(2, bits);
        if (prime_polynomial > 0 && (x & temp)){
            x = x ^ prime_polynomial;
        }
    }
    return result;
}

// multiplication using look up table
long gf_mul_MR_BCH_8bits_LUT(long x, long y, struct Tables *tables)
{
    if (x == 0 || y == 0)
    {
        return 0;
    }
    return *(tables->gf_exp + *(tables->gf_log + x) + *(tables->gf_log + y));
}

// division using look up table
long gf_div_MR_BCH_8bits_LUT(long x, long y, struct Tables *tables)
{
    if (y == 0)
    {
        printf("Error: divisor is zero!\n");
        exit(1);
    }
    if (x == 0)
    {
        return 0;
    }
    long log_size = tables->gf_log_size - 1;
    return *(tables->gf_exp + (*(tables->gf_log + x) + log_size - *(tables->gf_log + y)) % log_size);
}

// power using look up table
long gf_pow_MR_BCH_8bits_LUT(long x, long power, struct Tables *tables)
{
    long log_size = tables->gf_log_size - 1;
    long result;
    if (power >= 0){
        result =  *(tables->gf_exp + (*(tables->gf_log + x) * power) % log_size);
    }else{
        // printf("%ld\n", (*(tables->gf_log + x) * power) % log_size + log_size);
        result =  *(tables->gf_exp + (*(tables->gf_log + x) * power) % log_size + log_size);
    }

    // long result2 = x;
    // if (power > 0){
    //     for (int i = 0; i < power - 1; i++){
    //         result2 = gf_mul_MR_BCH_8bits_LUT(result2, x, tables);
    //     }
    // }else{
    //     power = power + 255;
    //     for (int i = 0; i < power - 1; i++){
    //         result2 = gf_mul_MR_BCH_8bits_LUT(result2, x, tables);
    //     }
    // }

    // printf("power: %ld\n", power);
    // printf("result 1: %ld\n", result);
    // printf("result 1: %ld\n\n", result2);
    return result;
}

// inverse using look up table
long gf_inverse_MR_BCH_8bits_LUT(long x, struct Tables *tables)
{
    long log_size = tables->gf_log_size - 1;
    return *(tables->gf_exp + log_size - *(tables->gf_log + x));
}
