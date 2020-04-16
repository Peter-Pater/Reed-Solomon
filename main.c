#include <stdio.h>
#include <time.h>
// #include "stdrscoding.h"
#include "comprs_arithmetic.h"

void stdrs_test(){
    int bits = 8;
    int prime_polynomial = 285; //100011101
    int prime_polynomial_16 = 66525; //10000001111011101
    long prime_polynomial_32 = 4299161607; //100000000010000000000000000000111

    clock_t start, end;
    double time_taken;

    start = clock();
    struct Tables *tables = newTables(prime_polynomial, bits);
    end = clock();
    time_taken = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("time taken for precomputing the table is %lf\n", time_taken);

    //Example1
    printf("Example1:\n");
    int n = 250;
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
    int num_errors = 5;
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
    // printPolynomial(corrected_message_poly);
    printPolynomialAsMessage(corrected_message_poly, k);

    delTables(tables);
    delPolynomial(encoded_mesecc_poly);
}

void comprs_test(){
    int prime_polynomial_16 = 69643; //10000001111011101
    // 66525, 69643, 69765, 79555, 80075, 80967, 83211, 94317, 95361, 99439, 101303, 101615, 101959, 102231
    struct Tables *tables_16 = newTables(prime_polynomial_16, 16);

    // just choose two numbers in field GF(2^16)
    long g1 = 989; // 0000001111011101
    long g2 = 9165; // 0010001111001101

    // compute with big table
    printf("g1 = %ld, g2 = %ld\n", g1, g2);
    printf("The result using regular 16-bits table = %ld\n", gf_mul_MR_BCH_8bits_LUT(g1, g2, tables_16));
    delTables(tables_16);

    // now use 8-bit table
    // first half and second half:
    int prime_polynomial = 285; //100011101
    struct Tables *tables_8 = newTables(prime_polynomial, 8);
    printf("%ld\n", gf_mul_comp(g1, g2, tables_8));

    // long a1 = g1 >> 8; //3. 00000011
    // printf("%ld\n", a1);
    // long a0 = g1 & 0xff; //221, 11011101
    // printf("%ld\n", a0);
    //
    // long b1 = g2 >> 8; //35, 00100011
    // printf("%ld\n", b1);
    // long b0 = g2 & 0xff; //11001101
    // printf("%ld\n", b0);
    //
    // long r1 = gf_mul_MR_BCH_8bits_LUT(a1, b0, tables_8) ^ gf_mul_MR_BCH_8bits_LUT(a0, b1, tables_8) ^ gf_mul_MR_BCH_8bits_LUT(gf_mul_MR_BCH_8bits_LUT(32, a1, tables_8), b1, tables_8);
    // long r0 = gf_mul_MR_BCH_8bits_LUT(a0, b0, tables_8) ^ gf_mul_MR_BCH_8bits_LUT(gf_mul_MR_BCH_8bits_LUT(32, a1, tables_8), b1, tables_8);
    // printf("Higher order byte is: %ld\n", r1);
    // printf("Lower order byte is: %ld\n", r0); // 44799
    // printf("end result is: %ld\n", (r1 << 8) + r0);
}

int main()
{
    // stdrs_test();
    comprs_test();
    return 0;
}
