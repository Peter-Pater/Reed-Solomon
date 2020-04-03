#include <stdio.h>
#include "utility.h"

int gf_add_BCH_8bits(int, int);

int gf_sub_BCH_8bits(int, int);

int gf_mul_BCH_8bits(int, int);

int table_generator(int);

int gf_mul_MR_BCH_8bits_LUT(int, int, struct Tables *);

int gf_div_MR_BCH_8bits_LUT(int, int, struct Tables *);

int gf_pow_MR_BCH_8bits_LUT(int, int, struct Tables *);

struct Polynomial *gf_poly_scale(struct Polynomial *, int, struct Tables *);

struct Polynomial *gf_poly_add(struct Polynomial *, struct Polynomial *);

struct Polynomial *gf_poly_mul(struct Polynomial *, struct Polynomial *, struct Tables *);

int gf_poly_eval(struct Polynomial *, int, struct Tables *);

struct Polynomial *rs_generator_poly(int, struct Tables *);

void gf_poly_div(struct Polynomial *qoutient, struct Polynomial *remainder, struct Polynomial *dividend, struct Polynomial *divisor, struct Tables *table);

int *rs_encode_msg(int *msg_in, int msg_size, int nsym, struct Tables *table);
