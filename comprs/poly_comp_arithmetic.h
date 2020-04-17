#include "comp_arithmetic.h"

struct Polynomial *gf_poly_scale(struct Polynomial *, long, struct Tables *);

struct Polynomial *gf_poly_add(struct Polynomial *, struct Polynomial *);

struct Polynomial *gf_poly_mul(struct Polynomial *, struct Polynomial *, struct Tables *);

long gf_poly_eval(struct Polynomial *, long, struct Tables *);

struct Polynomial *rs_generator_poly(long, struct Tables *);

void gf_poly_div(struct Polynomial *, struct Polynomial *, struct Polynomial *, struct Polynomial *, struct Tables *);
