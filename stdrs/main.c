#include <stdio.h>
#include <time.h>
#include "stdrscoding.h"

void evaluation(int bits, int n, int k, int num_err) {
    srand(time(0));
    int prime_polynomial = 0;
    if (bits == 8) {
        prime_polynomial = 285; //100011101
    } else if (bits == 16) {
        prime_polynomial = 66525; //10000001111011101
    }
    struct Tables *tables = newTables(prime_polynomial, bits);
    struct Polynomial *rand_message_poly = randPolynomial(k);
    printf("The Original Message:\n");
    printPolynomial(rand_message_poly);
    printPolynomialAsMessage(rand_message_poly, k);
    printf("\n");

    struct Polynomial *encoded_message_poly = rs_encode_msg(rand_message_poly, n - k, tables);
    printf("The Encoded Message:\n");
    printPolynomial(encoded_message_poly);
    printPolynomialAsMessage(encoded_message_poly, k);
    printf("\n");
    
    struct Polynomial *corrupted_message_poly = corruptPolynomial(encoded_message_poly, num_err);
    printf("The Corrupted Message:\n");
    printPolynomial(corrupted_message_poly);
    printPolynomialAsMessage(corrupted_message_poly, k);
    if (isEqualPolynomial(encoded_message_poly, corrupted_message_poly))
    {
        printf("The encoded message is not corrupted!\n");
    } else 
    {
        printf("The encoded message is corrupted!\n");
    }
    printf("\n");
    
    struct Polynomial *corrected_message_poly = rs_correct_msg(corrupted_message_poly, n - k, tables, bits);
    if (corrected_message_poly == NULL)
    {
        printf("Too many errors to correct!\n");
    }
    else
    {
        printf("The Corrected Message:\n");
        printPolynomial(corrected_message_poly);
        printPolynomialAsMessage(corrected_message_poly, k);
        if (isEqualPolynomial(encoded_message_poly, corrected_message_poly))
        {
            printf("The message is corrected!\n");
        } else 
        {
            printf("The message is not corrected!\n");
        }
        delPolynomial(corrected_message_poly);
    }

    delTables(tables);
    delPolynomial(rand_message_poly);
    delPolynomial(encoded_message_poly);
    delPolynomial(corrupted_message_poly);
}

//sample size unit is MB
void evalLargeSample(int bits, int n, int k, int num_err, int sample_size) 
{
    srand(time(0));
    int l = 32;
    int r = 126;
    int prime_polynomial = 0;
    if (bits == 8) {
        prime_polynomial = 285; //100011101
    } else if (bits == 16) {
        prime_polynomial = 66525; //10000001111011101
    }
    struct Tables *tables = newTables(prime_polynomial, bits);
    int sample_size_int = sample_size * 1024 * 1024;
    struct Polynomial *rand_message_poly = randPolynomial(sample_size_int);
    // printPolynomialAsMessage(rand_message_poly, sample_size_int);
    int num_chunks = sample_size_int / k;
    printf("The input message can be divided into %d chunks\n", num_chunks);
    int num_errs = num_chunks * num_err;
    int encoded_poly_size = num_chunks * n;
    // printf("debug\n");
    int *err_pos = malloc(encoded_poly_size * sizeof(int));
    // int err_pos[encoded_poly_size];
    // printf("debug!\n");
    for (int k = 0; k < encoded_poly_size; k++)
    {
        *(err_pos + k) = 0;
    }
    int m = num_errs;
    while (m > 0)
    {
        int rand_index = rand() % encoded_poly_size;
        if (*(err_pos + rand_index) == 0)
        {
            *(err_pos + rand_index) = 1;
            m--;
        }
    }
    printf("There are totally %d errors in this message\n", num_errs);
    printf("The encoded polynomial size is %d\n", encoded_poly_size);
    printf("\n");
    for (int i = 0; i < num_chunks; i++)
    {
        printf("chunk %d: ", i);
        long *original_message = malloc(k * sizeof(long));
        long *corrupted_message = malloc(n * sizeof(long));
        for (int j = i * k; j < (i + 1) * k; j++)
        {
            *(original_message + j - (i * k)) = *(rand_message_poly->poly_arr + j);
        }
        struct Polynomial *test_original_poly = newPolynomial(original_message, k);
        // printPolynomial(test_original_poly);
        struct Polynomial *test_encoded_poly = rs_encode_msg(test_original_poly, n - k, tables);
        // printPolynomial(test_encoded_poly);
        for (int c = i * n; c < (i + 1) * n; c++)
        {
            if (*(err_pos + c) == 1)
            {
                *(corrupted_message + c - (i * n)) = l + (rand() % (r - l + 1));
            }
            else
            {
                *(corrupted_message + c - (i * n)) = *(test_encoded_poly->poly_arr + c - (i * n));
            }
        }
        struct Polynomial *test_corrupted_poly = newPolynomial(corrupted_message, n);
        // printPolynomial(test_corrupted_poly);
        // if (isEqualPolynomial(test_encoded_poly, test_corrupted_poly))
        // {
        //     printf("The encoded message is not corrupted!\n");
        // } else 
        // {
        //     printf("The encoded message is corrupted!\n");
        // }
        struct Polynomial *test_corrected_poly = rs_correct_msg(test_corrupted_poly, n - k, tables, bits);
        if (test_corrected_poly == NULL)
        {
            printf("The message is failed to be corrected!\n");
        }
        else
        {
            // printf("The Corrected Message:\n");
            // printPolynomial(test_corrected_poly);
            // printPolynomialAsMessage(test_corrected_poly, k);
            if (isEqualPolynomial(test_encoded_poly, test_corrected_poly))
            {
                printf("The message is corrected!\n");
            } else 
            {
                printf("The message is not corrected!\n");
            }
            delPolynomial(test_corrected_poly);
        }
        delPolynomial(test_original_poly);
        delPolynomial(test_encoded_poly);
        delPolynomial(test_corrupted_poly);
        free(original_message);
        free(corrupted_message);
        printf("\n");
    }
    free(err_pos);
}

int main(int argc, char *argv[])
{
    if (argc != 6) {
        printf("6 arguments expected.\n");
        exit(1);
    }
    int bits = atoi(argv[1]);
    int n = atoi(argv[2]);
    int k = atoi(argv[3]);
    int num_err = atoi(argv[4]);
    int sample_size = atoi(argv[5]);
    printf("bits : %d\n", bits);
    printf("n : %d\n", n);
    printf("k : %d\n", k);
    printf("num_err : %d\n", num_err);
    printf("sampleSize : %d MB\n", sample_size);
    printf("\n");
    // evaluation(bits, n, k, num_err);
    // printf("\n");
    evalLargeSample(bits, n, k, num_err, sample_size);
    return 0;
}
