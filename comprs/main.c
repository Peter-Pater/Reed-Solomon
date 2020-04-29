#include <stdio.h>
#include <time.h>
#include "comp_rscoding.h"

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
    
    struct Polynomial *corrected_message_poly = rs_correct_msg(corrupted_message_poly, n - k, tables, 2 * bits);
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
    evaluation(bits, n, k, num_err);
    return 0;
}
