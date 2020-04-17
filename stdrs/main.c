#include <stdio.h>
#include <time.h>
#include "stdrscoding.h"

void stdrs_test(){
    int bits = 16;
    int prime_polynomial = 285; //100011101
    int prime_polynomial_16 = 66525; //10000001111011101
    long prime_polynomial_32 = 4299161607; //100000000010000000000000000000111

    clock_t start, end;
    double time_taken;

    start = clock();
    struct Tables *tables = newTables(prime_polynomial_16, bits);
    // printTables(tables);
    end = clock();
    time_taken = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("time taken for precomputing the table is %lf\n", time_taken);

    //Example1
    printf("Example1:\n");
    int n = 10;
    int k = 5;
    int message[5] = {1000, 1000, 2000, 2000, 3000};
    // char message[5] = {'h', 'e', 'l', 'l', 'o', ' ', 'w', 'o', 'r', 'l', 'd', '\n',
    //                      'h', 'e', 'l', 'l', 'o', ' ', 'w', 'o', 'r', 'l', 'd', '\n',
    //                      'h', 'e', 'l', 'l', 'o', ' ', 'w', 'o', 'r', 'l', 'd', '\n',
    //                      'h', 'e', 'l', 'l', 'o', ' ', 'w', 'o', 'r', 'l', 'd', '\n',
    //                      'h', 'e', 'l', 'l', 'o', ' ', 'w', 'o', 'r', 'l', 'd', '\n',
    //                      'h', 'e', 'l', 'l', 'o', ' ', 'w', 'o', 'r', 'l', 'd', '\n',
    //                      'h', 'e', 'l', 'l', 'o', ' ', 'w', 'o', 'r', 'l', 'd', '\n',
    //                      'h', 'e', 'l', 'l', 'o', ' ', 'w', 'o', 'r', 'l', 'd', '\n',
    //                      'h', 'e', 'l', 'l', 'o', ' ', 'w', 'o', 'r', 'l', 'd', '\n',
    //                      'h', 'e', 'l', 'l', 'o', ' ', 'w', 'o', 'r', 'l', 'd', '\n',
    //                      'h', 'e', 'l', 'l', 'o', ' ', 'w', 'o', 'r', 'l', 'd', '\n',
    //                      'h', 'e', 'l', 'l', 'o', ' ', 'w', 'o', 'r', 'l', 'd', '\n',
    //                      'h', 'e', 'l', 'l', 'o', ' ', 'w', 'o', 'r', 'l', 'd', '\n',
    //                      'h', 'e', 'l', 'l', 'o', ' ', 'w', 'o', 'r', 'l', 'd', '\n',
    //                      'h', 'e', 'l', 'l', 'o', ' ', 'w', 'o', 'r', 'l', 'd', '\n',
    //                      'h', 'e', 'l', 'l', 'o', ' ', 'w', 'o', 'r', 'l', 'd', '\n',
    //                      'h', 'e', 'l', 'l', 'o', ' ', 'w', 'o', 'r', 'l', 'd', '\n',
    //                      'h', 'e', 'l', 'l', 'o', ' ', 'w', 'o', 'r', 'l', 'd', '\n',
    //                      'h', 'e', 'l', 'l', 'o', ' ', 'w', 'o', 'r', 'l', 'd', '\n',
    //                      'h', 'e', 'l', 'l', 'o', ' ', 'w', 'o', 'r', 'l', 'd', '\n'
    //                       };

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
    printf("Original Encoded Message:\n");
    printPolynomial(encoded_mesecc_poly);
    printf("\n");

    // Tampering 6 characters of the message:
    int num_errors = 2;
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
    // printPolynomialAsMessage(corrected_message_poly, k);

    delTables(tables);
    delPolynomial(encoded_mesecc_poly);
}

int main()
{
    stdrs_test();
    return 0;
}
