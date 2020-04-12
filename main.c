#include <stdio.h>
#include <time.h>
#include "stdrscoding.h"

void test(){
    // printf("check add/sub:\n");
    // int a = 5;
    // int b = 6;
    // int r1 = gf_sub_BCH_8bits(a, b);
    // printf("the result is %d\n", r1);
    // printBinary(r1);
    // printf("\n");
    //
    // printf("check multiply without modular reduction:\n");
    // int c = 137; //10001001
    // int d = 42; //00101010
    // int r2 = gf_mul_BCH_8bits(c, d);
    // printf("the result is %d\n", r2);
    // printBinary(r2);
    // printf("\n");
    //
    int bits = 16;
    int prime_polynomial = 285; //100011101
    int prime_polynomial_16 = 66525; //10000001111011101
    long prime_polynomial_32 = 4299161607; //100000000010000000000000000000111

    clock_t start, end;
    double time_taken;
    //
    // printf("check multiply with modular reduction:\n");
    // int e = 137; //10001001
    // int f = 42; //00101010
    // int r3 = gf_mul_MR_BCH(e, f, prime_polynomial, 8);
    // printf("the result is %d\n", r3);
    // printBinary(r3);
    // printf("\n");

    start = clock();
    struct Tables *tables = newTables(prime_polynomial_16, bits);
    end = clock();
    time_taken = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("time taken for precomputing the table is %lf\n", time_taken);
    // printTables(tables);
    //
    // printf("check multiply with LUT:\n");
    // int g = 137; //10001001
    // int h = 42; //00101010
    // int r4 = gf_mul_MR_BCH_8bits_LUT(g, h, tables);
    // printf("the result is %d\n", r4);
    // printBinary(r4);
    // printf("\n");
    //
    // printf("check division with LUT:\n");
    // int a1 = 195; //11000011
    // int a2 = 42; //00101010
    // int r5 = gf_div_MR_BCH_8bits_LUT(a1, a2, tables);
    // printf("the result is %d\n", r5);
    // printBinary(r5);
    // printf("\n");
    //
    // printf("check polynomial operations:\n");
    // printf("check scale operation:\n");
    // int arr1[4] = {137, 1, 4, 1};
    // struct Polynomial *poly1 = newPolynomial(arr1, 4);
    // struct Polynomial * ret_val = gf_poly_scale(poly1, 42, tables);
    // printPolynomial(ret_val);
    // delPolynomial(poly1);
    // printf("\n");
    //
    // printf("check add operation:\n");
    // int arr2[4] = {3, 1, 4, 1};
    // int arr3[3] = {1, 2, 2};
    // struct Polynomial *poly2 = newPolynomial(arr2, 4);
    // struct Polynomial *poly3 = newPolynomial(arr3, 3);
    // struct Polynomial * ret_val1 = gf_poly_add(poly2, poly3);
    // printPolynomial(ret_val1);
    // delPolynomial(poly2);
    // delPolynomial(poly3);
    // delPolynomial(ret_val1);
    // printf("\n");
    //
    // printf("check multiply operation & evaluation:\n");
    // int arr4[4] = {1, 1, 1, 1};
    // int arr5[3] = {1, 1, 1};
    // struct Polynomial *poly4 = newPolynomial(arr4, 4);
    // struct Polynomial *poly5 = newPolynomial(arr5, 3);
    // struct Polynomial * ret_val2 = gf_poly_mul(poly4, poly5, tables);
    // printPolynomial(ret_val2);
    // printf("The evaluation with x = 2 is: %d\n", gf_poly_eval(ret_val2, 2, tables));
    // delPolynomial(poly4);
    // delPolynomial(poly5);
    // delPolynomial(ret_val2);
    // printf("\n");
    //
    // printf("check rs_generator\n");
    // int nsym = 4;
    // struct Polynomial *gen = rs_generator_poly(nsym, tables);
    // printPolynomial(gen);
    // delPolynomial(gen);
    // printf("\n");
    //
    // printf("check encoding\n");
    // nsym = 10;
    // int msg_in[16] = {0x40, 0xd2, 0x75, 0x47, 0x76, 0x17, 0x32, 0x06, 0x27, 0x26, 0x96, 0xc6, 0xc6, 0x96, 0x70, 0xec};
    // struct Polynomial *msg_in_poly = newPolynomial(msg_in, 16);
    // printf("msg in is: \n");
    // printPolynomial(msg_in_poly);
    // printf("\n");
    //
    // struct Polynomial *encoded_msg = rs_encode_msg(msg_in_poly, nsym, tables);
    //
    // printf("encoded msg is: \n");
    // printPolynomial(encoded_msg);
    // printf("\n");
    // // remainder:
    // // 188 42 144 19 107 175 239 253 75 224
    // // 0xbc 0x2a 0x90 0x13 0x6b 0xaf 0xef 0xfd 0x4b 0xe0
    //
    // printf("check Syndrome Calculation\n");
    // printf("not corrupted message\n");
    // struct Polynomial *syndrome_poly = rs_calc_syndromes(encoded_msg, nsym, tables);
    // printPolynomial(syndrome_poly);
    // printf("the check result is %d\n", rs_check(syndrome_poly, nsym, tables));
    // delPolynomial(syndrome_poly);
    //
    // printf("\ncorrupted message\n");
    // *(encoded_msg->poly_arr) = 0;
    // struct Polynomial *syndrome_poly1 = rs_calc_syndromes(encoded_msg, nsym, tables);
    // printPolynomial(syndrome_poly1);
    // printf("the check result is %d\n", rs_check(syndrome_poly1, nsym, tables));
    //
    // //check erasure correction
    // printf("\ncheck erasure correction\n");
    // int err_pos_arr[1] = {0};
    // struct Polynomial *err_pos = newPolynomial(err_pos_arr, 1);
    //
    // printf("the msg before erasure correction is :\n");
    // printPolynomial(encoded_msg);
    //
    // printf("the msg after erasure correction is :\n");
    // encoded_msg = rs_correct_errata(encoded_msg, syndrome_poly1, err_pos, tables);
    // printPolynomial(encoded_msg);
    //
    // delPolynomial(syndrome_poly1);
    // delPolynomial(err_pos);
    // delPolynomial(msg_in_poly);
    //
    // //check error correction
    // printf("\ncheck error correction\n");
    // *(encoded_msg->poly_arr) = 6;
    // *(encoded_msg->poly_arr + 10) = 7;
    // *(encoded_msg->poly_arr + 20) = 8;
    // printf("the corrupted msg before error correction is :\n");
    // printPolynomial(encoded_msg);
    // struct Polynomial *syndrome_poly2 = rs_calc_syndromes(encoded_msg, nsym, tables);
    // int *erase_pos_arr;
    // struct Polynomial *erase_pos = newPolynomial(erase_pos_arr, 0);
    // struct Polynomial *fsynd = rs_forney_syndromes(syndrome_poly2, erase_pos, encoded_msg->poly_size, tables);
    // // printPolynomial(syndrome_poly2);
    // // printPolynomial(fsynd);
    //
    // // this function should be able to find both error and erasure, set erasure position to NULL due to the given test cases
    // struct Polynomial *err_loc = rs_find_error_locator(fsynd, nsym, NULL, tables);
    // printf("find error position(s):\n");
    // struct Polynomial *pos = rs_find_errors(reversePolynomial(err_loc), encoded_msg->poly_size, tables);
    // printPolynomial(pos);
    //
    // encoded_msg = rs_correct_errata(encoded_msg, syndrome_poly2, pos, tables);
    //
    // printf("the msg after error correction is :\n");
    // printPolynomial(encoded_msg);
    //
    // delPolynomial(encoded_msg);

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

    // printf("Corrupted Encoded Message:\n");
    // printPolynomial(encoded_mesecc_poly);
    // printf("\n");

    // int erase_pos_arr_0[2] = {0, 2};
    // struct Polynomial *erase_pos_poly = newPolynomial(erase_pos_arr_0, 3);

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

int main()
{
    test();
    return 0;
}
