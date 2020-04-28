#include <stdio.h>
#include <time.h>
#include "stdrscoding.h"

void stdrs_test(){
    int bits = 8;
    int prime_polynomial_8 = 285; //100011101
    int prime_polynomial_16 = 66525; //10000001111011101
    long prime_polynomial_32 = 4299161607; //100000000010000000000000000000111

    clock_t start, end;
    double time_taken;

    start = clock();
    struct Tables *tables = newTables(prime_polynomial_8, bits);
    end = clock();
    time_taken = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("time taken for precomputing the table is %lf\n", time_taken);

    //Example1
    printf("Example1:\n");
    int n = 255;
    int k = 240;
    char message[240] = {'h', 'e', 'l', 'l', 'o', ' ', 'w', 'o', 'r', 'l', 'd', '\n',
                         'h', 'e', 'l', 'l', 'o', ' ', 'w', 'o', 'r', 'l', 'd', '\n',
                         'h', 'e', 'l', 'l', 'o', ' ', 'w', 'o', 'r', 'l', 'd', '\n',
                         'h', 'e', 'l', 'l', 'o', ' ', 'w', 'o', 'r', 'l', 'd', '\n',
                         'h', 'e', 'l', 'l', 'o', ' ', 'w', 'o', 'r', 'l', 'd', '\n',
                         'h', 'e', 'l', 'l', 'o', ' ', 'w', 'o', 'r', 'l', 'd', '\n',
                         'h', 'e', 'l', 'l', 'o', ' ', 'w', 'o', 'r', 'l', 'd', '\n',
                         'h', 'e', 'l', 'l', 'o', ' ', 'w', 'o', 'r', 'l', 'd', '\n',
                         'h', 'e', 'l', 'l', 'o', ' ', 'w', 'o', 'r', 'l', 'd', '\n',
                         'h', 'e', 'l', 'l', 'o', ' ', 'w', 'o', 'r', 'l', 'd', '\n',
                         'h', 'e', 'l', 'l', 'o', ' ', 'w', 'o', 'r', 'l', 'd', '\n',
                         'h', 'e', 'l', 'l', 'o', ' ', 'w', 'o', 'r', 'l', 'd', '\n',
                         'h', 'e', 'l', 'l', 'o', ' ', 'w', 'o', 'r', 'l', 'd', '\n',
                         'h', 'e', 'l', 'l', 'o', ' ', 'w', 'o', 'r', 'l', 'd', '\n',
                         'h', 'e', 'l', 'l', 'o', ' ', 'w', 'o', 'r', 'l', 'd', '\n',
                         'h', 'e', 'l', 'l', 'o', ' ', 'w', 'o', 'r', 'l', 'd', '\n',
                         'h', 'e', 'l', 'l', 'o', ' ', 'w', 'o', 'r', 'l', 'd', '\n',
                         'h', 'e', 'l', 'l', 'o', ' ', 'w', 'o', 'r', 'l', 'd', '\n',
                         'h', 'e', 'l', 'l', 'o', ' ', 'w', 'o', 'r', 'l', 'd', '\n',
                         'h', 'e', 'l', 'l', 'o', ' ', 'w', 'o', 'r', 'l', 'd', '\n'
                          };

    // char message[120] = {'h', 'e', 'l', 'l', 'o', ' ', 'w', 'o', 'r', 'l', 'd', '\n',
    //                     'h', 'e', 'l', 'l', 'o', ' ', 'w', 'o', 'r', 'l', 'd', '\n',
    //                     'h', 'e', 'l', 'l', 'o', ' ', 'w', 'o', 'r', 'l', 'd', '\n',
    //                     'h', 'e', 'l', 'l', 'o', ' ', 'w', 'o', 'r', 'l', 'd', '\n',
    //                     'h', 'e', 'l', 'l', 'o', ' ', 'w', 'o', 'r', 'l', 'd', '\n',
    //                     'h', 'e', 'l', 'l', 'o', ' ', 'w', 'o', 'r', 'l', 'd', '\n',
    //                     'h', 'e', 'l', 'l', 'o', ' ', 'w', 'o', 'r', 'l', 'd', '\n',
    //                     'h', 'e', 'l', 'l', 'o', ' ', 'w', 'o', 'r', 'l', 'd', '\n',
    //                     'h', 'e', 'l', 'l', 'o', ' ', 'w', 'o', 'r', 'l', 'd', '\n',
    //                     'h', 'e', 'l', 'l', 'o', ' ', 'w', 'o', 'r', 'l', 'd', '\n'
    //                     };

    // char message[9600];
    // for (int i = 0; i < 9600; i++){
    //     message[i] = 'h';
    // }

    long message_arr[k];
    for (int i = 0; i < k; i++)
    {
        *(message_arr + i) = *(message + i);
    }

    struct Polynomial *mesecc_poly = newPolynomial(message_arr, k);

    // time encoding
    // start = clock();
    // for (int i = 0; i < 10000000 / k; i++){
    //     struct Polynomial *temp;
    //     temp = rs_encode_msg(mesecc_poly, n - k, tables);
    //     delPolynomial(temp);
    // }
    // end = clock();
    // time_taken = ((double) (end - start)) / CLOCKS_PER_SEC;
    // printf("time taken for encoding %lf\n", time_taken);

    struct Polynomial *encoded_mesecc_poly = rs_encode_msg(mesecc_poly, n - k, tables);
    // printf("Original Encoded Message:\n");
    // printPolynomial(encoded_mesecc_poly);
    // printf("\n");

    // Tampering 6 characters of the message:
    int num_errors = 6;
    for (int i = 0; i < num_errors; i++){
        *(encoded_mesecc_poly->poly_arr + i) = 0;
    }

    printf("Corrupted Encoded Message:\n");
    printPolynomial(encoded_mesecc_poly);
    printf("\n");

    // time decoding
    // start = clock();
    // for (int i = 0; i < 10000000 / k; i++){
    //     struct Polynomial *temp;
    //     temp = rs_correct_msg(encoded_mesecc_poly, n - k, tables, bits);
    //     delPolynomial(temp);
    // }
    // end = clock();
    // time_taken = ((double) (end - start)) / CLOCKS_PER_SEC;
    // printf("time taken for decoding %lf\n", time_taken);

    struct Polynomial *corrected_message_poly = rs_correct_msg(encoded_mesecc_poly, n - k, tables, bits);
    printf("Repaired Message:\n");
    printPolynomial(corrected_message_poly);
    printPolynomialAsMessage(corrected_message_poly, k);

    delTables(tables);
    delPolynomial(mesecc_poly);
    delPolynomial(encoded_mesecc_poly);
    delPolynomial(corrected_message_poly);

    // printf("character : %c\n", 126);
}

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

    delTables(tables);
    delPolynomial(rand_message_poly);
    delPolynomial(encoded_message_poly);
    delPolynomial(corrupted_message_poly);
    delPolynomial(corrected_message_poly);
}

int main(int argc, char *argv[])
{
    // stdrs_test();
    if (argc != 5) {
        printf("5 arguments expected.\n");
        exit(1);
    }
    int bits = atoi(argv[1]);
    int n = atoi(argv[2]);
    int k = atoi(argv[3]);
    int num_err = atoi(argv[4]);
    printf("bits : %d\n", bits);
    printf("n : %d\n", n);
    printf("k : %d\n", k);
    printf("num_err : %d\n", num_err);
    printf("\n");
    evaluation(bits, n, k, num_err);
    return 0;
}
