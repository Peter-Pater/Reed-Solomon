#include "arithmetic.h"

int gf_add_BCH_8bits(int x, int y)
{
    return x ^ y;
}

int gf_sub_BCH_8bits(int x, int y)
{
    return x ^ y;
}

int gf_mul_BCH_8bits(int x, int y)
{
    int result = 0;
    int pos = 0;
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

int gf_mul_MR_BCH_8bits(int x, int y, int prime_polynomial)
{
    int result = gf_mul_BCH_8bits(x, y);

    if (prime_polynomial > 0)
    {
        result = carry_less_long_div(result, prime_polynomial);
    }
    
    return result;
}
