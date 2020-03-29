#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void printBinary(int);

void reverse_string(char *);

int bit_length(int);

int carry_less_long_div(int, int);

struct Polynomial
{
    size_t poly_size;
    int *ploy_arr;
};

struct Polynomial *newPolynomial(int *, size_t);

void delPolynomial(struct Polynomial *);

void printPolynomial(struct Polynomial *);
