#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

void printBinary(int);

void reverse_string(char *);

int bit_length(int);

int carry_less_long_div(int, int);

int gf_mul_MR_BCH(int, int, int, int);

struct Tables
{
    int *gf_exp;
    size_t gf_exp_size;
    int *gf_log;
    size_t gf_log_size;
};

struct Tables *newTables(int, size_t);

void delTables(struct Tables *);

void printTables(struct Tables *);

struct Polynomial
{
    size_t poly_size;
    int *poly_arr;
};

struct Polynomial *newPolynomial(int *, size_t);

void delPolynomial(struct Polynomial *);

struct Polynomial* reversePolynomial(struct Polynomial *);

void printPolynomial(struct Polynomial *poly);

struct DynamicArray
{
    size_t arr_size;
    size_t capacity;
    int *data;
};

struct DynamicArray *newDynamicArray(size_t);

void delDynamicArray(struct DynamicArray *);

void push_back(struct DynamicArray *, int);

void printDynamicArray(struct DynamicArray *);
