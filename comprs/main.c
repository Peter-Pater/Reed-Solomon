#include <stdio.h>
#include <time.h>
#include "comp_rscoding.h"

void evalSmallSample(int base_bits, int composite_bits, int n, int k, int num_err) {
    srand(time(0));
    int prime_polynomial = 0;
    if (base_bits == 8) {
        prime_polynomial = 285; //100011101
    } else if (base_bits == 16) {
        prime_polynomial = 66525; //10000001111011101
    }
    struct Tables *tables = newTables(prime_polynomial, base_bits);
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
    
    struct Polynomial *corrected_message_poly = rs_correct_msg(corrupted_message_poly, n - k, tables, composite_bits);
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
void evalLargeSample(int base_bits, int composite_bits, int n, int k, int num_err, int sample_size) 
{
    clock_t start, end;
    double time_taken;

    start = clock();
    int corrected_chunks = 0;
    srand(time(0));
    int l = 32;
    int r = 126;
    int prime_polynomial = 0;
    if (base_bits == 8) {
        prime_polynomial = 285; //100011101
    } else if (base_bits == 16) {
        prime_polynomial = 66525; //10000001111011101
    }
    struct Tables *tables = newTables(prime_polynomial, base_bits);
    int sample_size_int = sample_size * 1024 * 1024;
    struct Polynomial *rand_message_poly = randPolynomial(sample_size_int);
    // printPolynomialAsMessage(rand_message_poly, sample_size_int);
    int num_chunks = sample_size_int / k;
    printf("The input message can be divided into %d chunks\n", num_chunks);
    int num_errs = num_chunks * num_err;
    int encoded_poly_size = num_chunks * n;
    int *err_pos = malloc(encoded_poly_size * sizeof(int));
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
        struct Polynomial *test_corrected_poly = rs_correct_msg(test_corrupted_poly, n - k, tables, composite_bits);
        if (test_corrected_poly == NULL)
        {
            printf("The message is failed to be corrected!\n");
        }
        else
        {
            if (isEqualPolynomial(test_encoded_poly, test_corrected_poly))
            {
                corrected_chunks++;
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
    end = clock();
    time_taken = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("Time taken for %d MB data is %lfs\n", sample_size, time_taken);
    printf("The corrected rate for %f%% of err is %f%%\n", 
        (double)(num_err * 100) / k, (double)(corrected_chunks * 100) / num_chunks);
}

int main(int argc, char *argv[])
{
    if (argc != 7) {
        printf("7 arguments expected.\n");
        exit(1);
    }
    int base_bits = atoi(argv[1]);
    int composite_bits = atoi(argv[2]);
    int n = atoi(argv[3]);
    int k = atoi(argv[4]);
    int num_err = atoi(argv[5]);
    int sample_size = atoi(argv[6]);
    printf("base_bits : %d\n", base_bits);
    printf("composite_bits : %d\n", composite_bits);
    printf("n : %d\n", n);
    printf("k : %d\n", k);
    printf("num_err : %d\n", num_err);
    printf("sampleSize : %d MB\n", sample_size);
    printf("\n");
    // evalSmallSample(base_bits, composite_bits, n, k, num_err);
    evalLargeSample(base_bits, composite_bits, n, k, num_err, sample_size);
    return 0;
}
