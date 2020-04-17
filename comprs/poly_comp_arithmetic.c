#include "poly_comp_arithmetic.h"

// poly multiplication with integer: p = p * x, where p is a polynomial and x is an integer
struct Polynomial *gf_poly_scale(struct Polynomial *p, long x, struct Tables *tables)
{
    long arr[p->poly_size];
    for (long i = 0; i < p->poly_size; i++)
    {
        *(arr + i) = 0;
    }
    struct Polynomial *ret_val = newPolynomial(arr, p->poly_size);
    for (long i = 0; i < p->poly_size; i++)
    {
        *(ret_val->poly_arr + i) = gf_mul_MR_BCH_8bits_LUT(*(p->poly_arr + i), x, tables);
    }
    return ret_val;
}

// poly addition
struct Polynomial *gf_poly_add(struct Polynomial *p, struct Polynomial *q)
{
    long ret_size = 0;
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

    ret_val->poly_arr = malloc(ret_size * sizeof(long));

    if (ret_val->poly_arr == NULL)
    {
        free(ret_val);
        return NULL;
    }

    for (long i = 0; i < p->poly_size; i++)
    {
        *(ret_val->poly_arr + i + ret_size - p->poly_size) = *(p->poly_arr + i);
    }

    for (long i = 0; i < q->poly_size; i++)
    {
        *(ret_val->poly_arr + i + ret_size - q->poly_size) ^= *(q->poly_arr + i);
    }

    return ret_val;
}

// poly multiplication
struct Polynomial *gf_poly_mul(struct Polynomial *p, struct Polynomial *q, struct Tables *tables)
{
    struct Polynomial *ret_val = malloc(sizeof(struct Polynomial));
    if (ret_val == NULL)
    {
        return NULL;
    }
    ret_val->poly_size = (p->poly_size + q->poly_size - 1);

    ret_val->poly_arr = malloc(ret_val->poly_size * sizeof(long));

    for (long i = 0; i < ret_val->poly_size; i++)
    {
        *(ret_val->poly_arr + i) = 0;
    }

    if (ret_val->poly_arr == NULL)
    {
        free(ret_val);
        return NULL;
    }

    for (long j = 0; j < q->poly_size; j++)
    {
        for (long i = 0; i < p->poly_size; i++)
        {
            ret_val->poly_arr[i+j] = gf_add_BCH_8bits(ret_val->poly_arr[i+j],
            gf_mul_MR_BCH_8bits_LUT(*(p->poly_arr + i), *(q->poly_arr + j), tables));
        }
    }

    return ret_val;
}

// Evaluation: given x and the coefficients of a polynomial, convert it into a numeric number (integer)
long gf_poly_eval(struct Polynomial *p, long x, struct Tables *tables)
{
    long y = *(p->poly_arr);
    for (long i = 1; i < p->poly_size; i++)
    {
        y = gf_mul_MR_BCH_8bits_LUT(y, x, tables) ^ *(p->poly_arr + i);
    }
    return y;
}

// rs generator: given number of symbols, generate an unreducible generator polynomial
struct Polynomial *rs_generator_poly(long nsym, struct Tables *table){
    long arr_g[1] = {1};
    struct Polynomial *g = newPolynomial(arr_g, 1);
    for (long i = 0; i < nsym; i++){
        long arr_temp[2] = {1, gf_pow_MR_BCH_8bits_LUT(2, i, table)};
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
    long *msg_out = malloc(dividend->poly_size * sizeof(long));
    memcpy(msg_out, dividend->poly_arr, dividend->poly_size * sizeof(long));
    for (long i = 0; i < dividend->poly_size - divisor->poly_size + 1; i++){
        long coef = msg_out[i];
        // check that coef is not 0
        if (coef){
            for (long j = 1; j < divisor->poly_size; j++){
                if (*(divisor->poly_arr + j)){
                    msg_out[i + j] ^= gf_mul_MR_BCH_8bits_LUT(*(divisor->poly_arr + j), coef, table);
                }
            }
        }
    }
    // separtor is the length of remainder (TODO: what if the length of remainder is 0?)
    long separator = divisor->poly_size - 1;

    long *quo = malloc((dividend->poly_size - separator) * sizeof(long));
    memcpy(quo, msg_out, (dividend->poly_size - separator) * sizeof(long));
    // qoutient = newPolynomial(quo, dividend->poly_size - separator);
    qoutient->poly_size = dividend->poly_size - separator;
    qoutient->poly_arr = quo;

    long *remain = malloc(sizeof(long) * separator);
    memcpy(remain, msg_out + dividend->poly_size - separator, separator * sizeof(long));
    // remainder = newPolynomial(remain, separator);
    remainder->poly_size = separator;
    remainder->poly_arr = remain;
    free(msg_out);
}
