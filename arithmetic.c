#include <stdio.h>
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
int gf_mul_MR_BCH(int x, int y, int prime_polynomial, int bits){
    int result = 0;
    while (y > 0){
        if (y & 1){
            result = gf_add_BCH_8bits(result, x);
        }
        y = y >> 1;
        x = x << 1;
        int temp = pow(2, bits);
        if (prime_polynomial > 0 && (x & temp)){
            x = x ^ prime_polynomial;
        }
    }
    return result;
}

// multiplication using look up table
int gf_mul_MR_BCH_8bits_LUT(int x, int y, struct Tables *tables)
{
    if (x == 0 || y == 0)
    {
        return 0;
    }
    return *(tables->gf_exp + *(tables->gf_log + x) + *(tables->gf_log + y));
}

// division using look up table
int gf_div_MR_BCH_8bits_LUT(int x, int y, struct Tables *tables)
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
    int log_size = tables->gf_log_size - 1;
    return *(tables->gf_exp + (*(tables->gf_log + x) + log_size - *(tables->gf_log + y)) % log_size);
}

// power using look up table
int gf_pow_MR_BCH_8bits_LUT(int x, int power, struct Tables *tables)
{
    int log_size = tables->gf_log_size - 1;
    return *(tables->gf_exp + (*(tables->gf_log + x) * power) % log_size);
}

// inverse using look up table
int gf_inverse_MR_BCH_8bits_LUT(int x, struct Tables *tables)
{
    int log_size = tables->gf_log_size - 1;
    return *(tables->gf_exp + log_size - *(tables->gf_log + x));
}

// poly multiplication with integer: p = p * x, where p is a polynomial and x is an integer
struct Polynomial *gf_poly_scale(struct Polynomial *p, int x, struct Tables *tables)
{
    printPolynomial(p);
    for (int i = 0; i < p->poly_size; i++)
    {
        *(p->poly_arr + i) = gf_mul_MR_BCH_8bits_LUT(*(p->poly_arr + i), x, tables);
    }
    return p;
}

// poly addition
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

    ret_val->poly_arr = malloc(ret_size * sizeof(int));

    if (ret_val->poly_arr == NULL)
    {
        free(ret_val);
        return NULL;
    }

    for (int i = 0; i < p->poly_size; i++)
    {
        *(ret_val->poly_arr + i + ret_size - p->poly_size) = *(p->poly_arr + i);
    }

    for (int i = 0; i < q->poly_size; i++)
    {
        *(ret_val->poly_arr + i + ret_size - q->poly_size) ^= *(q->poly_arr + i);
    }

    return ret_val;
}

// poly multiplication
struct Polynomial *gf_poly_mul(struct Polynomial *p, struct Polynomial *q, struct Tables *tables)
{
    // printPolynomial(p);
    // printPolynomial(q);
    struct Polynomial *ret_val = malloc(sizeof(struct Polynomial));
    if (ret_val == NULL)
    {
        return NULL;
    }
    ret_val->poly_size = (p->poly_size + q->poly_size - 1);

    ret_val->poly_arr = malloc(ret_val->poly_size * sizeof(int));

    for (int i = 0; i < ret_val->poly_size; i++)
    {
        *(ret_val->poly_arr + i) = 0;
    }

    if (ret_val->poly_arr == NULL)
    {
        free(ret_val);
        return NULL;
    }

    for (int j = 0; j < q->poly_size; j++)
    {
        for (int i = 0; i < p->poly_size; i++)
        {
            ret_val->poly_arr[i+j] = gf_add_BCH_8bits(ret_val->poly_arr[i+j],
            gf_mul_MR_BCH_8bits_LUT(*(p->poly_arr + i), *(q->poly_arr + j), tables));
        }
    }

    return ret_val;
}

// Evaluation: given x and the coefficients of a polynomial, convert it into a numeric number (integer)
int gf_poly_eval(struct Polynomial *p, int x, struct Tables *tables)
{
    int y = *(p->poly_arr);
    for (int i = 1; i < p->poly_size; i++)
    {
        y = gf_mul_MR_BCH_8bits_LUT(y, x, tables) ^ *(p->poly_arr + i);
    }
    return y;
}

// rs generator: given number of symbols, generate an unreducible generator polynomial
struct Polynomial *rs_generator_poly(int nsym, struct Tables *table){
    int arr_g[1] = {1};
    struct Polynomial *g = newPolynomial(arr_g, 1);
    for (int i = 0; i < nsym; i++){
        int arr_temp[2] = {1, gf_pow_MR_BCH_8bits_LUT(2, i, table)};
        struct Polynomial *temp = newPolynomial(arr_temp, 2);
        struct Polynomial *old_g = g;
        g = gf_poly_mul(g, temp, table);
        delPolynomial(temp);
        delPolynomial(old_g);
    }
    return g;
}

// polynomial division, given a dividend and a divisor, fill the given pointer for qoutient and remainder.
void gf_poly_div(struct Polynomial *qoutient, struct Polynomial *remainder, struct Polynomial *dividend, struct Polynomial *divisor, struct Tables *table){
    int *msg_out = malloc(dividend->poly_size * sizeof(int));
    memcpy(msg_out, dividend->poly_arr, dividend->poly_size * sizeof(int));
    for (int i = 0; i < dividend->poly_size - divisor->poly_size + 1; i++){
        int coef = msg_out[i];
        // check that coef is not 0
        if (coef){
            for (int j = 1; j < divisor->poly_size; j++){
                if (*(divisor->poly_arr + j)){
                    msg_out[i + j] ^= gf_mul_MR_BCH_8bits_LUT(*(divisor->poly_arr + j), coef, table);
                }
            }
        }
    }
    // separtor is the length of remainder (TODO: what if the length of remainder is 0?)
    int separator = divisor->poly_size - 1;

    int *quo = malloc((dividend->poly_size - separator) * sizeof(int));
    memcpy(quo, msg_out, (dividend->poly_size - separator) * sizeof(int));
    // qoutient = newPolynomial(quo, dividend->poly_size - separator);
    qoutient->poly_size = dividend->poly_size - separator;
    qoutient->poly_arr = quo;

    int *remain = malloc(sizeof(int) * separator);
    memcpy(remain, msg_out + dividend->poly_size - separator, separator * sizeof(int));
    // remainder = newPolynomial(remain, separator);
    remainder->poly_size = separator;
    remainder->poly_arr = remain;
    free(msg_out);
}

