#include <stdio.h>
#include "utility.h"

long gf_add_BCH_8bits(long, long);

long gf_sub_BCH_8bits(long, long);

long gf_mul_BCH_8bits(long, long);

long table_generator(long);

long gf_mul_MR_BCH_8bits_LUT(long, long, struct Tables *);

long gf_div_MR_BCH_8bits_LUT(long, long, struct Tables *);

long gf_pow_MR_BCH_8bits_LUT(long, long, struct Tables *);

long gf_inverse_MR_BCH_8bits_LUT(long, struct Tables *);

struct Polynomial *gf_poly_scale(struct Polynomial *, long, struct Tables *);

struct Polynomial *gf_poly_add(struct Polynomial *, struct Polynomial *);

struct Polynomial *gf_poly_mul(struct Polynomial *, struct Polynomial *, struct Tables *);

long gf_poly_eval(struct Polynomial *, long, struct Tables *);

struct Polynomial *rs_generator_poly(long, struct Tables *);

void gf_poly_div(struct Polynomial *, struct Polynomial *, struct Polynomial *, struct Polynomial *, struct Tables *);
