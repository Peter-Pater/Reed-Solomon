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
    for (int i = 0; i < syndrome_poly->poly_size; i++)
    {
        if (*(syndrome_poly->poly_arr+i) > 0)
        {
            return 1;
        }
    }
    return 0;
}

struct Polynomial *rs_find_errata_locator(struct Polynomial *e_pos, struct Tables *table)
{
    int e_loc[1] = {1};
    struct Polynomial *ret_val = newPolynomial(e_loc, 1);
    for (int i = 0; i < e_pos->poly_size; i++)
    {
        int temp[2] = {gf_pow_MR_BCH_8bits_LUT(2, *(e_pos->poly_arr + i), table), 0};
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

struct Polynomial *rs_find_error_locator(struct Polynomial *synd, int nsym, struct Polynomial *erase_loc, struct Tables *table)
{
    struct Polynomial *err_loc = NULL;
    struct Polynomial *old_loc = NULL;
    int erase_count = 0;
    if (erase_loc != NULL && erase_loc->poly_size != 0)
    {
        erase_count = erase_loc->poly_size;
        err_loc = newPolynomial(erase_loc->poly_arr, erase_loc->poly_size);
        old_loc = newPolynomial(erase_loc->poly_arr, erase_loc->poly_size);
        for (int i = 0; i < erase_loc->poly_size; i++)
        {
            *(err_loc->poly_arr + i) = *(erase_loc->poly_arr + i);
            *(old_loc->poly_arr + i) = *(erase_loc->poly_arr + i);
        }
    }  else
    {
        int temp_arr[1] = {1};
        err_loc = newPolynomial(temp_arr, 1);
        old_loc = newPolynomial(temp_arr, 1);
    }
    int synd_shift = 0;
    if (synd->poly_size > nsym)
    {
        synd_shift = synd->poly_size - nsym;
    }
    for (int i = 0; i < nsym - erase_count; i++)
    {
        int K = 0;
        if (erase_loc != NULL && erase_loc->poly_size != 0)
        {
            K = erase_count + i + synd_shift;
        } else
        {
            K = i + synd_shift;
        }
        int delta = *(synd->poly_arr + K);
        for (int j = 1; j < err_loc->poly_size; j++)
        {
            delta ^= gf_mul_MR_BCH_8bits_LUT(*(err_loc->poly_arr + err_loc->poly_size - j - 2), *(synd->poly_arr + K - j), table);
        }
        int temp_arr[old_loc->poly_size+1];        
        for (int k = 0; k < (old_loc->poly_size + 1); k++)
        {
            if (k == old_loc->poly_size)
            {
                *(temp_arr + k) = 0;
            } else
            {
                *(temp_arr + k) = *(old_loc->poly_arr + k);
            }
        }
        struct Polynomial *temp_loc = newPolynomial(temp_arr, old_loc->poly_size + 1);
        // printPolynomial(old_loc);
        old_loc = temp_loc;
        if (delta != 0)
        {
            if (old_loc->poly_size > err_loc->poly_size)
            {
                struct Polynomial* new_loc = gf_poly_scale(old_loc, delta, table);
                old_loc = gf_poly_scale(err_loc, gf_inverse_MR_BCH_8bits_LUT(delta, table), table);
                err_loc = new_loc;
            }
            err_loc = gf_poly_add(err_loc, gf_poly_scale(old_loc, delta, table));
        }
    }
    delPolynomial(old_loc);
    while (err_loc->poly_size != 0 && *(err_loc->poly_arr) == 0)
    {
        int temp_arr_1[err_loc->poly_size - 1];
        for (int k = 1; k < err_loc->poly_size; k++)
        {
            *(temp_arr_1 + k - 1) = *(err_loc->poly_arr + k);
        }
        struct Polynomial *new_err_loc = newPolynomial(temp_arr_1, err_loc->poly_size - 1);
        delPolynomial(err_loc);
        err_loc = new_err_loc;
    }
    int errs = err_loc->poly_size - 1;
    if ((errs - erase_count) * 2 + erase_count > nsym)
    {
        printf("Too many errors to correct\n");
        exit(1);
    }
    return err_loc;
}

struct Polynomial *rs_find_errors(struct Polynomial *err_loc, int nmess, struct Tables *table)
{
    printPolynomial(err_loc);
    int errs = err_loc->poly_size - 1;
    struct DynamicArray *err_pos_arr = newDynamicArray(10);
    printf("nmess is : %d\n", nmess);
    for (int i = 0; i < nmess; i++)
    {
        if (gf_poly_eval(err_loc, gf_pow_MR_BCH_8bits_LUT(2, i, table), table) == 0)
        {
            printf("here!!!\n");
            push_back(err_pos_arr, nmess - 1 - i);
        }
    }
    if (err_pos_arr->arr_size != errs)
    {
        printf("err_pos_arr_size is : %zu ; errs_size is : %d\n", err_pos_arr->arr_size, errs);
        printf("Too many (or few) errors found by Chien Search for the errata locator polynomial!\n");
        exit(1);
    }
    struct Polynomial *err_pos = newPolynomial(err_pos_arr->data, err_pos_arr->arr_size);
    delDynamicArray(err_pos_arr);
    return err_pos;
}
