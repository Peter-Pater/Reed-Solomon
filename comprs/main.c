#include <stdio.h>
#include <time.h>
#include "comp_rscoding.h"

void sample(){
    int bits = 8;
    int prime_polynomial = 285; //100011101
    // int prime_polynomial_16 = 66525; //10000001111011101
    // long prime_polynomial_32 = 4299161607; //100000000010000000000000000000111

    // generate a pre-computed table with 8 bits
    struct Tables *tables = newTables(prime_polynomial, bits);

    // RS(12000, 10000)
    int n = 12000;
    int k = 10000;

    long message_arr[k];

    for (int i = 0; i < k; i++)
    {
        *(message_arr + i) = i;
    }

    // The message need to be stored as a polynomial
    struct Polynomial *mesecc_poly = newPolynomial(message_arr, k);

    // encode
    struct Polynomial *encoded_mesecc_poly = rs_encode_msg(mesecc_poly, n - k, tables);
    printf("Original Encoded Message:\n");
    printPolynomial(encoded_mesecc_poly);
    printf("\n");

    // Tampering num_errors characters of the message:
    int num_errors = 1000;
    for (int i = 0; i < num_errors; i++){
        *(encoded_mesecc_poly->poly_arr + i * 10) = 6666;
    }

    printf("Corrupted Encoded Message:\n");
    printPolynomial(encoded_mesecc_poly);
    printf("\n");

    // decode
    struct Polynomial *corrected_message_poly = rs_correct_msg(encoded_mesecc_poly, n - k, tables, 16);
    printf("Repaired Message:\n");
    printPolynomial(corrected_message_poly);

    // clean up
    delTables(tables);
    delPolynomial(encoded_mesecc_poly);
}

int main()
{
    sample();
    return 0;
}

// obsolete tests
void comprs_test(){
    // int prime_polynomial_16 = 102231; //10000001111011101
    // 66525, 69643, 69765, 79555, 80075, 80967, 83211, 94317, 95361, 99439, 101303, 101615, 101959, 102231
    // struct Tables *tables_16 = newTables(prime_polynomial_16, 16);

    // just choose two numbers in field GF(2^16)
    long g1 = 989; // 0000001111011101
    long g2 = 9165; // 0010001111001101
    printf("g1 = %ld, g2 = %ld\n", g1, g2);

    // // compute with big table
    // printf("The result using regular 16-bits table = %ld\n", gf_mul_MR_BCH_8bits_LUT(g1, g2, tables_16));
    // delTables(tables_16);

    // now use 8-bit table
    // first half and second half:
    int prime_polynomial = 285; //100011101
    struct Tables *tables = newTables(prime_polynomial, 8);
    printf("result is: %ld\n", gf_mul_comp(g1, g2, tables));
    // printf("result is: %ld\n", gf_mul_MR_BCH_8bits_LUT(g1, g2, tables));

    // inverse * 44799 = 1
    long inverse = 0;
    for (long i = 0; i < 65535; i++){
        if (gf_mul_comp(i, 44799, tables) == 1){
            inverse = i;
            printf("inverse is: %ld\n", i);
        }
    }
    printf("qoutient is: %ld\n", gf_mul_comp(inverse, 23333, tables));

    // 23333 / 44799 = 5417
    for (long i = 0; i < 65535; i++){
        if (gf_mul_comp(i, 44799, tables) == 23333){
            printf("qoutient is: %ld\n", i);
        }
    }
    printf("qoutient is: %ld\n", gf_div_comp(23333, 44799, tables));

    long a = 2;
    long b = 8;
    printf("Exponential result is %ld\n", gf_pow_comp(a, b, tables));

    // // find inverse
    // long a = 44799;
    // long b = 65824;
    // // long b = 65535;
    // long *d = malloc(sizeof(long));
    // long *s = malloc(sizeof(long));
    // long *t = malloc(sizeof(long));
    // ext_euclid(a, b, d, s, t, tables);
    // printf("%ld, %ld, %ld\n", *d, *s, *t); // d = gcd(a, b), as + bt = d
}
