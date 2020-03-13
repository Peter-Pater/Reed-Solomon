#include <stdio.h>
#include "utility.h"

int gf_add_BCH_8bits(int, int);

int gf_sub_BCH_8bits(int, int);

int gf_mul_BCH_8bits(int, int);

int gf_mul_MR_BCH_8bits(int, int, int);

int table_generator(int);

struct Tables
{
    int *gf_exp;
    size_t gf_exp_size;
    int *gf_log;
    size_t gf_log_size;
};

struct Tables *newTables(int, size_t);
void delTables(struct Tables *);

