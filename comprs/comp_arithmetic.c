#include <stdio.h>
#include "comp_arithmetic.h"

long gf_mul_comp(long x, long y, struct Tables *tables){
    long a1 = x >> 8; //3. 00000011
    // printf("higher order byte of x: %ld\n", a1);
    long a0 = x & 0xff; //221, 11011101
    // printf("lower order byte of x: %ld\n", a0);

    long b1 = y >> 8; //35, 00100011
    // printf("higher order byte of y: %ld\n", a1);
    long b0 = y & 0xff; //11001101
    // printf("lower order byte of y: %ld\n", a0);

    // long p_arr[3] = {1, 1, 32};
    // struct Polynomial *p = newPolynomial(p_arr, 3);
    //
    // long ret_arr[3] = {gf_mul_MR_BCH_8bits_LUT(a1, b1, tables), gf_mul_MR_BCH_8bits_LUT(a1, b0, tables) ^ gf_mul_MR_BCH_8bits_LUT(a0, b1, tables), gf_mul_MR_BCH_8bits_LUT(a0, b0, tables)};
    // struct Polynomial *ret = newPolynomial(ret_arr, 3);

    long r1 = gf_mul_MR_BCH_8bits_LUT(a1, b0, tables) ^ gf_mul_MR_BCH_8bits_LUT(a0, b1, tables) ^ gf_mul_MR_BCH_8bits_LUT(a1, b1, tables);
    long r0 = gf_mul_MR_BCH_8bits_LUT(a0, b0, tables) ^ gf_mul_MR_BCH_8bits_LUT(gf_mul_MR_BCH_8bits_LUT(32, a1, tables), b1, tables);


    // printf("Higher order byte of result is: %ld\n", r1);
    // printf("Lower order byte of result is: %ld\n", r0); // 44799
    // printf("end result is: %ld\n", (r1 << 8) + r0);

    // return (*(ret->poly_arr) << 8) + *(ret->poly_arr + 1);
    return (r1 << 8) + r0;
}

long gf_div_comp(long x, long y, struct Tables *tables){
    long result = -1;
    for (long i = 0; i < 65535; i++){
        if (gf_mul_comp(i, y, tables) == x){
            result = i;
            break;
        }
    }
    if (result != -1){
        return result;
    }else{
        printf("Error! Qoutient not found!\n");
        return 0;
    }
}

long gf_inverse_comp(long x, struct Tables *tables){
    long result = -1;
    for (long i = 0; i < 65536; i++){
        if (gf_mul_comp(i, x, tables) == 1){
            result = i;
            break;
        }
    }
    // printf("%ld\n", result);
    // printf("correct inverse : %ld\n\n", gf_inverse_MR_BCH_8bits_LUT(x, tables));
    if (result != -1){
        return result;
    }else{
        printf("Error! Qoutient not found!\n");
        return 0;
    }
}

long gf_pow_comp(long x, long power, struct Tables *tables){
    if(power <= 0){
        power = 65535 + power;
    }
    long result = x;
    for (int i = 0; i < power - 1; i++){
        result = gf_mul_comp(result, x, tables);
    }
    // printf("%ld\n", result);
    // printf("%ld\n\n", gf_pow_MR_BCH_8bits_LUT(x, power, tables));
    // if (result == gf_pow_MR_BCH_8bits_LUT(x, power, tables)){
    //     ;;
    // }else{
    //     printf("False\n");
    // }
    return result;
}

void ext_euclid(long a, long b, long* d, long* s, long* t, struct Tables *tables){
    if (b == 0){
        *d = a;
        *s = 1;
        *t = 0;
    }else{
        long q = gf_div_comp(a, b, tables);
        long r = a ^ gf_mul_comp(b, q, tables);
        // long *d1;
        // long *s1;
        // long *t1;
        printf("q: %ld\n", q);
        printf("b: %ld\n", b);
        printf("r: %ld\n\n", r);
        ext_euclid(b, r, d, s, t, tables);
        long temp_s = *s;
        *s = *t; *t = temp_s ^ gf_mul_comp(q, (*t), tables);
    }
}
