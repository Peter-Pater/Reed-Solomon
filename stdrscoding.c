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
