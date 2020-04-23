#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

void printBinary(long);

void reverse_string(char *);

long bit_length(long);

long carry_less_long_div(long, long);

long gf_mul_MR_BCH(long, long, long, long);

struct Tables
{
    long *gf_exp;
    size_t gf_exp_size;
    long *gf_log;
    size_t gf_log_size;
};

struct Tables *newTables(long, long);

void delTables(struct Tables *);

void printTables(struct Tables *);

struct Polynomial
{
    size_t poly_size;
    long *poly_arr;
};

struct Polynomial *randPolynomial(size_t);

struct Polynomial *newPolynomial(long *, size_t);

void delPolynomial(struct Polynomial *);

struct Polynomial* corruptPolynomial(struct Polynomial *, int);

struct Polynomial* reversePolynomial(struct Polynomial *);

void printPolynomial(struct Polynomial *);

void printPolynomialAsMessage(struct Polynomial *, long);

struct DynamicArray
{
    size_t arr_size;
    size_t capacity;
    long *data;
};

struct DynamicArray *newDynamicArray(size_t);

void delDynamicArray(struct DynamicArray *);

void push_back(struct DynamicArray *, long);

void printDynamicArray(struct DynamicArray *);
