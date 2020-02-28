#include<arithmetic.h>

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
