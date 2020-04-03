#include "arithmetic.h"

// carry-less addition (use XOR)
int gf_add_BCH_8bits(int x, int y)
{
    return x ^ y;
}

// carry-less substraction (use XOR)
int gf_sub_BCH_8bits(int x, int y)
{
    return x ^ y;
}

// multiplication
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
int gf_mul_MR_BCH_8bits(int x, int y, int prime_polynomial){
    int result = 0;
    while (y > 0){
        if (y & 1){
            result = gf_add_BCH_8bits(result, x);
        }
        y = y >> 1;
        x = x << 1;
        if (prime_polynomial > 0 && (x & 256)){
            x = x ^ prime_polynomial;
        }
    }
    return result;
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

struct Polynomial *gf_poly_scale(struct Polynomial *p, int x, struct Tables *tables)
{
    printPolynomial(p);
    for (int i = 0; i < p->poly_size; i++)
    {
        *(p->ploy_arr + i) = gf_mul_MR_BCH_8bits_LUT(*(p->ploy_arr + i), x, tables);
    }
    return p;
}

struct Polynomial *gf_poly_add(struct Polynomial *p, struct Polynomial *q)
{
    int ret_size = 0;
    if (p->poly_size > q->poly_size)
    {
        ret_size = p->poly_size;
    } else {
        ret_size = q->poly_size;
    }
    struct Polynomial *ret_val = malloc(sizeof(struct Polynomial));
    if (ret_val == NULL)
    {
        return NULL;
    }
    ret_val->poly_size = ret_size;

    ret_val->ploy_arr = malloc(ret_size * sizeof(int));

    if (ret_val->ploy_arr == NULL)
    {
        free(ret_val);
        return NULL;
    }

    for (int i = 0; i < p->poly_size; i++)
    {
        *(ret_val->ploy_arr + i + ret_size - p->poly_size) = *(p->ploy_arr + i);
    }

    for (int i = 0; i < q->poly_size; i++)
    {
        *(ret_val->ploy_arr + i + ret_size - q->poly_size) ^= *(q->ploy_arr + i);
    }

    return ret_val;
}

struct Polynomial *gf_poly_mul(struct Polynomial *p, struct Polynomial *q, struct Tables *tables)
{
    printPolynomial(p);
    printPolynomial(q);
    struct Polynomial *ret_val = malloc(sizeof(struct Polynomial));
    if (ret_val == NULL)
    {
        return NULL;
    }
    ret_val->poly_size = (p->poly_size + q->poly_size - 1);

    ret_val->ploy_arr = malloc(ret_val->poly_size * sizeof(int));

    for (int i = 0; i < ret_val->poly_size; i++)
    {
        *(ret_val->ploy_arr + i) = 0;
    }

    if (ret_val->ploy_arr == NULL)
    {
        free(ret_val);
        return NULL;
    }

    for (int j = 0; j < q->poly_size; j++)
    {
        for (int i = 0; i < p->poly_size; i++)
        {
            ret_val->ploy_arr[i+j] = gf_add_BCH_8bits(ret_val->ploy_arr[i+j],
            gf_mul_MR_BCH_8bits_LUT(*(p->ploy_arr + i), *(q->ploy_arr + j), tables));
        }
    }

    return ret_val;
}

int gf_poly_eval(struct Polynomial *p, int x, struct Tables *tables)
{
    int y = *(p->ploy_arr);
    for (int i = 1; i < p->poly_size; i++)
    {
        y = gf_mul_MR_BCH_8bits_LUT(y, x, tables) ^ *(p->ploy_arr + i);
    }
    return y;
}
