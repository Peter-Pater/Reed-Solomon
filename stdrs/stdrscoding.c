#include <stdio.h>
#include "stdrscoding.h"

// Standard Encode
struct Polynomial *rs_encode_msg(struct Polynomial *msg_in, long nsym, struct Tables *table){
    struct Polynomial *gen = rs_generator_poly(nsym, table);
    struct Polynomial *qoutient = malloc(sizeof(struct Polynomial));
    struct Polynomial *remainder = malloc(sizeof(struct Polynomial));

    // printf("the generated polynomial is: \n");
    // printPolynomial(gen);

    long *padded_msg_in = malloc((msg_in->poly_size + gen->poly_size - 1) * sizeof(long));
    memcpy(padded_msg_in, msg_in->poly_arr, msg_in->poly_size * sizeof(long));

    for (long i = msg_in->poly_size; i < msg_in->poly_size + gen->poly_size - 1; i++){
        *(padded_msg_in + i) = 0;
    }

    struct Polynomial *dividend = newPolynomial(padded_msg_in, msg_in->poly_size + gen->poly_size - 1);
    gf_poly_div(qoutient, remainder, dividend, gen, table);

    memcpy(padded_msg_in + msg_in->poly_size, remainder->poly_arr, remainder->poly_size * sizeof(long));
    struct Polynomial *ret_val = newPolynomial(padded_msg_in, msg_in->poly_size + remainder->poly_size);
    delPolynomial(gen);
    delPolynomial(qoutient);
    delPolynomial(remainder);
    delPolynomial(dividend);
    return ret_val;
}

// Syndrome calculation
struct Polynomial *rs_calc_syndromes(struct Polynomial *encoded_msg, long nsym, struct Tables *table)
{
    long arr[nsym + 1];
    arr[0] = 0;
    struct Polynomial *synd = newPolynomial(arr, nsym + 1);
    for (long i = 1; i < nsym + 1; i++)
    {
        *(synd->poly_arr + i) = gf_poly_eval(encoded_msg, gf_pow_MR_BCH_8bits_LUT(2, i - 1, table), table);
    }
    return synd;
}

// Automatically check if a message is corrupted or not
long rs_check(struct Polynomial *syndrome_poly, long nsym, struct Tables *table)
{
    for (long i = 0; i < syndrome_poly->poly_size; i++)
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
    long e_loc[1] = {1};
    struct Polynomial *ret_val = newPolynomial(e_loc, 1);
    for (long i = 0; i < e_pos->poly_size; i++)
    {
        long temp[2] = {gf_pow_MR_BCH_8bits_LUT(2, *(e_pos->poly_arr + i), table), 0};
        struct Polynomial *temp_poly1 = newPolynomial(temp, 2);
        long temp_arr[1] = {1};
        struct Polynomial *temp_poly2 = newPolynomial(temp_arr, 1);
        ret_val = gf_poly_mul(ret_val, gf_poly_add(temp_poly1, temp_poly2), table);
    }
    return ret_val;
}

struct Polynomial *rs_find_error_evaluator(struct Polynomial *synd, struct Polynomial *err_loc, long nsym, struct Tables *table)
{
    struct Polynomial *qoutient = malloc(sizeof(struct Polynomial));
    struct Polynomial *remainder = malloc(sizeof(struct Polynomial));
    long temp[nsym+2];
    for (long i = 0; i < nsym + 2; i++)
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

// Error fixation 4/9: the modulo function in C is different from python, and syndrome lacked [0] as prefix
struct Polynomial *rs_correct_errata(struct Polynomial *msg_in, struct Polynomial * synd, struct Polynomial *err_pos, struct Tables *table, int bits)
{
    struct Polynomial *coef_pos = newPolynomial(err_pos->poly_arr, err_pos->poly_size);
    for (long i = 0; i < err_pos->poly_size; i++)
    {
        *(coef_pos->poly_arr + i) = msg_in->poly_size - 1 - *(err_pos->poly_arr + i);
    }
    struct Polynomial *err_loc = rs_find_errata_locator(coef_pos, table);
    struct Polynomial *err_eval = rs_find_error_evaluator(reversePolynomial(synd), err_loc, err_loc->poly_size - 1, table);

    // printPolynomial(coef_pos);
    struct Polynomial *X = newPolynomial(coef_pos->poly_arr, coef_pos->poly_size);
    for (long i = 0; i < coef_pos->poly_size; i++)
    {
        *(X->poly_arr + i) = gf_pow_MR_BCH_8bits_LUT(2, *(coef_pos->poly_arr + i) - (long) pow(2.0, (double) bits) + 1, table);
    }
    // printPolynomial(X);

    struct Polynomial *E = newPolynomial(msg_in->poly_arr, msg_in->poly_size);
    for (long i = 0; i < E->poly_size; i++)
    {
        *(E->poly_arr + i) = 0;
    }
    long Xlength = X->poly_size;
    for (long i = 0; i < Xlength; i++)
    {
        long Xi_inv = gf_inverse_MR_BCH_8bits_LUT(*(X->poly_arr + i), table);
        // printf("X_inv: %ld\n", Xi_inv);
        struct DynamicArray *err_loc_prime_tmp = newDynamicArray(10);
        for (long j = 0; j < Xlength; j++)
        {
            if (j != i)
            {
                push_back(err_loc_prime_tmp, gf_sub_BCH_8bits(1, gf_mul_MR_BCH_8bits_LUT(Xi_inv, *(X->poly_arr + j), table)));
            }
        }
        // printDynamicArray(err_loc_prime_tmp);
        long err_loc_prime = 1;
        for (long k = 0; k < err_loc_prime_tmp->arr_size; k++)
        {
            err_loc_prime = gf_mul_MR_BCH_8bits_LUT(err_loc_prime, *(err_loc_prime_tmp->data + k), table);
        }
        // printf("err_loc_prime : %ld\n", err_loc_prime);
        delDynamicArray(err_loc_prime_tmp);
        long y = gf_poly_eval(err_eval, Xi_inv, table);
        // printf("y: %ld\n", y);
        y = gf_mul_MR_BCH_8bits_LUT(gf_pow_MR_BCH_8bits_LUT(*(X->poly_arr + i), 1, table), y, table);
        // printf("y: %ld\n", y);
        if (err_loc_prime == 0)
        {
            printf("Could not find error magnitude!!!\n");
            exit(1);
        }
        long magnitude = gf_div_MR_BCH_8bits_LUT(y, err_loc_prime, table);
        *(E->poly_arr + *(err_pos->poly_arr + i)) = magnitude;
    }
    // printPolynomial(E);
    msg_in = gf_poly_add(msg_in, E);
    return msg_in;
}

struct Polynomial *rs_find_error_locator(struct Polynomial *synd, long nsym, struct Polynomial *erase_loc, struct Tables *table)
{
    struct Polynomial *err_loc = NULL;
    struct Polynomial *old_loc = NULL;
    long erase_count = 0;
    if (erase_loc != NULL && erase_loc->poly_size != 0)
    {
        erase_count = erase_loc->poly_size;
        err_loc = newPolynomial(erase_loc->poly_arr, erase_loc->poly_size);
        old_loc = newPolynomial(erase_loc->poly_arr, erase_loc->poly_size);
        for (long i = 0; i < erase_loc->poly_size; i++)
        {
            *(err_loc->poly_arr + i) = *(erase_loc->poly_arr + i);
            *(old_loc->poly_arr + i) = *(erase_loc->poly_arr + i);
        }
    }  else
    {
        long temp_arr[1] = {1};
        err_loc = newPolynomial(temp_arr, 1);
        old_loc = newPolynomial(temp_arr, 1);
    }
    long synd_shift = 0;
    if (synd->poly_size > nsym)
    {
        synd_shift = synd->poly_size - nsym;
    }
    // printf("erase count is %ld\n", erase_count);
    // printf("nsym is %ld\n", nsym);
    for (long i = 0; i < nsym - erase_count; i++)
    {
        long K = 0;
        if (erase_loc != NULL && erase_loc->poly_size != 0)
        {
            K = erase_count + i + synd_shift;
        } else
        {
            // printf("here + %ld\n", i);
            K = i + synd_shift;
        }
        long delta = *(synd->poly_arr + K);
        for (long j = 1; j < err_loc->poly_size; j++)
        {
            delta ^= gf_mul_MR_BCH_8bits_LUT(*(err_loc->poly_arr + err_loc->poly_size - j - 1), *(synd->poly_arr + K - j), table);
        }
        long temp_arr[old_loc->poly_size+1];
        for (long k = 0; k < (old_loc->poly_size + 1); k++)
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
        long temp_arr_1[err_loc->poly_size - 1];
        for (long k = 1; k < err_loc->poly_size; k++)
        {
            *(temp_arr_1 + k - 1) = *(err_loc->poly_arr + k);
        }
        struct Polynomial *new_err_loc = newPolynomial(temp_arr_1, err_loc->poly_size - 1);
        delPolynomial(err_loc);
        err_loc = new_err_loc;
    }
    long errs = err_loc->poly_size - 1;
    if ((errs - erase_count) * 2 + erase_count > nsym)
    {
        // printf("Too many errors to correct, erase count: %ld, err count: %ld\n", erase_count, errs);
        return NULL;
    }
    return err_loc;
}

struct Polynomial *rs_find_errors(struct Polynomial *err_loc, long nmess, struct Tables *table)
{
    // printPolynomial(err_loc);
    long errs = err_loc->poly_size - 1;
    struct DynamicArray *err_pos_arr = newDynamicArray(10);
    for (long i = 0; i < nmess; i++)
    {
        if (gf_poly_eval(err_loc, gf_pow_MR_BCH_8bits_LUT(2, i, table), table) == 0)
        {
            // printf("here!!!\n");
            push_back(err_pos_arr, nmess - 1 - i);
        }
    }
    if (err_pos_arr->arr_size != errs)
    {
        // printf("Too many (or few) errors found by Chien Search for the errata locator polynomial! Err count: %ld\n", err_pos_arr->arr_size);
        // exit(1);
        return NULL;
    }
    struct Polynomial *err_pos = newPolynomial(err_pos_arr->data, err_pos_arr->arr_size);
    delDynamicArray(err_pos_arr);
    return err_pos;
}

// Syndrome calculation with rs_forney_syndromes
struct Polynomial *rs_forney_syndromes(struct Polynomial *synd, struct Polynomial *erase_pos, long nmess, struct Tables *table)
{
    struct Polynomial *erase_pos_reversed;
    if (erase_pos != NULL)
    {
        erase_pos_reversed = newPolynomial(erase_pos->poly_arr, erase_pos->poly_size);
        for (long i = 0; i < erase_pos->poly_size; i++)
        {
            *(erase_pos_reversed->poly_arr + i) = nmess - 1 - *(erase_pos->poly_arr + i);
        }
    }
    long fsynd_arr[synd->poly_size - 1];
    for (long i = 0; i < synd->poly_size - 1; i++){
        *(fsynd_arr + i) = *(synd->poly_arr + i + 1);
    }
    struct Polynomial *fsynd = newPolynomial(fsynd_arr, synd->poly_size - 1);
    if (erase_pos != NULL)
    {
        for (long i = 0; i < erase_pos->poly_size; i++){
            long x = gf_pow_MR_BCH_8bits_LUT(2, *(erase_pos_reversed->poly_arr + i), table);
            for (long j = 0; j < fsynd->poly_size - 1; j++){
                *(fsynd->poly_arr + j) = gf_mul_MR_BCH_8bits_LUT(*(fsynd->poly_arr + j), x, table) ^ *(fsynd->poly_arr + j + 1);
            }
        }
    }
    if (erase_pos != NULL)
    {
        delPolynomial(erase_pos_reversed);
    }
    return fsynd;
}

struct Polynomial *rs_correct_msg(struct Polynomial *msg_in, long nsym, struct Tables *table, int bits)
{
    if (msg_in->poly_size > table->gf_log_size - 1)
    {
        printf("Message is too long (when max is 255)\n");
        exit(1);
    }
    struct Polynomial *msg_out = newPolynomial(msg_in->poly_arr, msg_in->poly_size);
    // if (erase_pos != NULL)
    // {
    //     for (int i = 0; i < erase_pos->poly_size; i++)
    //     {
    //         *(msg_out->poly_arr + *(erase_pos->poly_arr + i)) = 0;
    //     }
    // }
    // if (erase_pos != NULL && erase_pos->poly_size > nsym)
    // {
    //     printf("Too many erasures to correct\n");
    //     exit(1);
    // }
    struct Polynomial *synd = rs_calc_syndromes(msg_out, nsym, table);
    if (rs_check(synd, nsym, table) == 0)
    {
        printf("no errors!!!\n");
        return msg_out;
    }
    // struct Polynomial *fsynd = rs_forney_syndromes(synd, erase_pos, msg_out->poly_size, table); // error incurred
    struct Polynomial *err_loc = rs_find_error_locator(synd, nsym, NULL, table);
    if (err_loc == NULL) {
        return NULL;
    }
    struct Polynomial *err_pos = rs_find_errors(reversePolynomial(err_loc), msg_out->poly_size, table);
    // if (err_pos->poly_size == 0)
    // {
    //     printf("Could not locate error\n");
    //     exit(1);
    // }
    if (err_pos == NULL)
    {
        return NULL;
    }
    // if (erase_pos != NULL)
    // {
    //     int pos_arr[0 + erase_pos->poly_size + err_pos->poly_size];
    //     for (int j = 0 ; j < erase_pos->poly_size + err_pos->poly_size; j++)
    //     {
    //         if (j < erase_pos->poly_size)
    //         {
    //             *(pos_arr + j) = *(erase_pos->poly_arr + j);
    //         } else
    //         {
    //             *(pos_arr + j) = *(err_pos->poly_arr + j - erase_pos->poly_size);
    //         }
    //     }
    //     struct Polynomial *pos_poly = newPolynomial(pos_arr, erase_pos->poly_size + err_pos->poly_size);
    //     msg_out = rs_correct_errata(msg_out, synd, pos_poly, table);
    // } else {
    msg_out = rs_correct_errata(msg_out, synd, err_pos, table, bits);
    // }
    synd = rs_calc_syndromes(msg_out, nsym, table);
    if (rs_check(synd, nsym, table) == 1)
    {
        printf("Could not correct message\n");
        exit(1);
    }
    return msg_out;
}
