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

int table_generator(int prime_polynomial)
{
    int *gf_exp = malloc(512 * sizeof(int));
    int *gf_log = malloc(256 * sizeof(int));
    if (gf_exp == NULL || gf_log == NULL)
    {
        return -1;
    }

    int x = 1;
    for (int i = 0; i < 255; i++)
    {
        *(gf_exp + i) = x;
        *(gf_log + x) = i;
        x = gf_mul_MR_BCH_8bits(x, 2, prime_polynomial);
    }
    for (int i = 255; i < 512; i++)
    {
        *(gf_exp + i) = *(gf_exp + i - 255);
    }
    for (int i = 0; i < 512; i++)
    {
        printf("gf_exp[%d] = %d\n", i, *(gf_exp + i));
    }
    for (int i = 0; i < 256; i++)
    {
        printf("gf_log[%d] = %d\n", i, *(gf_log + i));
    }
    free(gf_exp);
    free(gf_log);
    return 0;
}

struct Tables *newTables(int prime_polynomial, size_t sz)
{
    struct Tables *retVal = malloc(sizeof(struct Tables));
    if (retVal == NULL)
    {
        return NULL;
    }

    retVal->gf_exp = malloc(2 * sz * sizeof(int));
    retVal->gf_log = malloc(sz * sizeof(int));
    if (retVal->gf_exp == NULL || retVal->gf_log == NULL) 
    {
        free (retVal);
        return NULL;
    }

    retVal->gf_exp_size = 2 * sz;
    retVal->gf_log_size = sz;

    int x = 1;
    for (int i = 0; i < 255; i++)
    {
        *(retVal->gf_exp + i) = x;
        *(retVal->gf_log + x) = i;
        x = gf_mul_MR_BCH_8bits(x, 2, prime_polynomial);
    }
    for (int i = 255; i < 512; i++)
    {
        *(retVal->gf_exp + i) = *(retVal->gf_exp + i - 255);
    }

    return retVal;

}

void delTables(struct Tables *tables)
{
    if (tables != NULL)
    {
        free(tables->gf_exp);
        free(tables->gf_log);
        free(tables);
    }
}

void printTables(struct Tables *tables)
{
    printf("gf_exp_table:\n");
    for (int i = 0; i < 512; i++)
    {
        printf("gf_exp[%d] = %d\n", i, *(tables->gf_exp + i));
    }
    printf("\ngf_log_table:\n");
    for (int i = 0; i < 256; i++)
    {
        printf("gf_log[%d] = %d\n", i, *(tables->gf_log + i));
    }
}

int gf_mul_MR_BCH_8bits_LUT(int x, int y, struct Tables *tables)
{
    if (x == 0 || y == 0)
    {
        return 0;
    }
    return *(tables->gf_exp + *(tables->gf_log + x) + *(tables->gf_log + y));
}

int gf_div_MR_BCH_8bits_LUT(int x, int y, struct Tables *tables)
{
    if (y == 0)
    {
        exit(1);
    }
    if (x == 0)
    {
        return 0;
    }
    return *(tables->gf_exp + (*(tables->gf_log + x) + 255 - *(tables->gf_log + y)) % 255);
}

int gf_pow_MR_BCH_8bits_LUT(int x, int power, struct Tables *tables)
{
    return *(tables->gf_exp + (*(tables->gf_log + x) * power) % 255);
}

int gf_inverse_MR_BCH_8bits_LUT(int x, struct Tables *tables)
{
    return *(tables->gf_exp + 255 - *(tables->gf_log + x));
}


