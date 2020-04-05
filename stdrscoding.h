#include <stdio.h>
#include "arithmetic.h"

struct Polynomial *rs_encode_msg(struct Polynomial *, int, struct Tables *);

struct Polynomial *rs_calc_syndromes(struct Polynomial *, int, struct Tables *);

int rs_check(struct Polynomial *, int, struct Tables *);
