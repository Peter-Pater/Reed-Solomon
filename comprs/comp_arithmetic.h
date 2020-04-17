#include "arithmetic.h"

long gf_mul_comp(long x, long y, struct Tables *tables);

long gf_div_comp(long x, long y, struct Tables *tables);

long gf_inverse_comp(long x, struct Tables *tables);

long gf_pow_comp(long x, long power, struct Tables *tables);

void ext_euclid(long a, long b, long* d, long* s, long* t, struct Tables *tables);
