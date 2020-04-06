#include <stdio.h>
#include "stdrscoding.h"

// Standard Encode
struct Polynomial *rs_encode_msg(struct Polynomial *msg_in, int nsym, struct Tables *table){
    struct Polynomial *gen = rs_generator_poly(nsym, table);
    struct Polynomial *qoutient = malloc(sizeof(struct Polynomial));
    struct Polynomial *remainder = malloc(sizeof(struct Polynomial));

    printf("the generated polynomial is: \n");
    printPolynomial(gen);

    int *padded_msg_in = malloc((msg_in->poly_size + gen->poly_size - 1) * sizeof(int));
    memcpy(padded_msg_in, msg_in->poly_arr, msg_in->poly_size * sizeof(int));

    for (int i = msg_in->poly_size; i < msg_in->poly_size + gen->poly_size - 1; i++){
        *(padded_msg_in + i) = 0;
    }

    struct Polynomial *dividend = newPolynomial(padded_msg_in, msg_in->poly_size + gen->poly_size - 1);
    gf_poly_div(qoutient, remainder, dividend, gen, table);

    memcpy(padded_msg_in + msg_in->poly_size, remainder->poly_arr, remainder->poly_size * sizeof(int));
    struct Polynomial *ret_val = newPolynomial(padded_msg_in, msg_in->poly_size + remainder->poly_size);
    delPolynomial(gen);
    delPolynomial(qoutient);
    delPolynomial(remainder);
    delPolynomial(dividend);
    return ret_val;
}

// Syndrome calculation
struct Polynomial *rs_calc_syndromes(struct Polynomial *encoded_msg, int nsym, struct Tables *table)
{
    int arr[nsym];
    for (int i = 0; i < nsym; i++)
    {
        arr[i] = 0;
    }
    struct Polynomial *synd = newPolynomial(arr, nsym);
    for (int i = 0; i < nsym; i++)
    {
        *(synd->poly_arr + i) = gf_poly_eval(encoded_msg, gf_pow_MR_BCH_8bits_LUT(2, i, table), table);
    }
    return synd;
}

// Automatically check if a message is corrupted or not
int rs_check(struct Polynomial *syndrome_poly, int nsym, struct Tables *table)
{
    int max = 0;
    for (int i = 0; i < syndrome_poly->poly_size; i++)
    {
        if (*(syndrome_poly->poly_arr+i) > max)
        {
            max = *(syndrome_poly->poly_arr+i);
        }
    }
    if (max == 0)
    {
        return 0;
    } else {
        return 1;
    }
}

struct Polynomial *rs_find_errata_locator(struct Polynomial *e_pos, struct Tables *table)
{
    int e_loc[1] = {1};
    struct Polynomial *ret_val = newPolynomial(e_loc, 1);
    for (int i = 0; i < e_pos->poly_size; i++)
    {
        int temp[2] = {gf_pow_MR_BCH_8bits_LUT(2, i, table), 0};
        struct Polynomial *temp_poly1 = newPolynomial(temp, 2);
        int temp_arr[1] = {1};
        struct Polynomial *temp_poly2 = newPolynomial(temp_arr, 1);
        ret_val = gf_poly_mul(ret_val, gf_poly_add(temp_poly2, temp_poly1), table);
    }
    return ret_val;
}

struct Polynomial *rs_find_error_evaluator(struct Polynomial *synd, struct Polynomial *err_loc, int nsym, struct Tables *table)
{
    struct Polynomial *qoutient = malloc(sizeof(struct Polynomial));
    struct Polynomial *remainder = malloc(sizeof(struct Polynomial));
    int temp[nsym+2];
    for (int i = 0; i < nsym + 2; i++)
    {
        if (i == 0)
        {
            temp[i] = 1;
        } else
        {
            temp[i] = 0;
        }
    }
    struct Polynomial *temp_poly = newPolynomial(temp, nsym+2);
    gf_poly_div(qoutient, remainder, gf_poly_mul(synd, err_loc, table), temp_poly, table);
    delPolynomial(qoutient);
    delPolynomial(temp_poly);
    return remainder;
}

struct Polynomial *rs_correct_errata(struct Polynomial *msg_in, struct Polynomial * synd, struct Polynomial *err_pos, struct Tables *table)
{
    struct Polynomial *coef_pos = newPolynomial(err_pos->poly_arr, err_pos->poly_size);
    for (int i = 0; i < err_pos->poly_size; i++)
    {
        *(coef_pos->poly_arr + i) = msg_in->poly_size - 1 - *(err_pos->poly_arr + i);
    }
    struct Polynomial *err_loc = rs_find_errata_locator(coef_pos, table);
    struct Polynomial *err_eval = reversePolynomial(rs_find_error_evaluator(reversePolynomial(synd), err_loc, err_loc->poly_size - 1, table));

    struct Polynomial *X = newPolynomial(coef_pos->poly_arr, coef_pos->poly_size);
    for (int i = 0; i < coef_pos->poly_size; i++)
    {
        *(X->poly_arr + i) = gf_pow_MR_BCH_8bits_LUT(2, *(coef_pos->poly_arr + i) - 255, table);
    }
    struct Polynomial *E = newPolynomial(msg_in->poly_arr, msg_in->poly_size);
    for (int i = 0; i < E->poly_size; i++)
    {
        *(E->poly_arr + i) = 0;
    }
    int Xlength = X->poly_size;
    for (int i = 0; i < X->poly_size; i++)
    {
        int Xi_inv = gf_inverse_MR_BCH_8bits_LUT(*(X->poly_arr + i), table);
        struct DynamicArray *err_loc_prime_tmp = newDynamicArray(10);
        for (int j = 0; j < Xlength; j++)
        {
            if (j != i)
            {
                push_back(err_loc_prime_tmp, gf_sub_BCH_8bits(1, gf_mul_MR_BCH_8bits_LUT(Xi_inv, *(X->poly_arr + j), table)));
            }
        }
        int err_loc_prime = 1;
        for (int k = 0; k < err_loc_prime_tmp->arr_size; k++)
        {
            err_loc_prime = gf_mul_MR_BCH_8bits_LUT(err_loc_prime, *(err_loc_prime_tmp->data + k), table);
        }
        delDynamicArray(err_loc_prime_tmp);

        int y = gf_mul_MR_BCH_8bits_LUT(gf_pow_MR_BCH_8bits_LUT(*(X->poly_arr + i), 1, table), gf_poly_eval(reversePolynomial(err_eval), Xi_inv, table), table);
        if (err_loc_prime == 0)
        {
            printf("Could not find error magnitude!!!\n");
            exit(1);
        }
        int magnitude = gf_div_MR_BCH_8bits_LUT(y, err_loc_prime, table);
        *(E->poly_arr + *(err_pos->poly_arr + i)) = magnitude;
    }
    msg_in = gf_poly_add(msg_in, E);
    return msg_in;
}