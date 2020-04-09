#include <stdio.h>
#include "arithmetic.h"

struct Polynomial *rs_encode_msg(struct Polynomial *, int, struct Tables *);

struct Polynomial *rs_calc_syndromes(struct Polynomial *, int, struct Tables *);

int rs_check(struct Polynomial *, int, struct Tables *);

struct Polynomial *rs_find_errata_locator(struct Polynomial *, struct Tables *);

struct Polynomial *rs_find_error_evaluator(struct Polynomial *, struct Polynomial *, int, struct Tables *);

struct Polynomial *rs_correct_errata(struct Polynomial *, struct Polynomial *, struct Polynomial *, struct Tables *);

struct Polynomial *rs_find_error_locator(struct Polynomial *, int, struct Polynomial *, struct Tables *);

struct Polynomial *rs_find_errors(struct Polynomial *, int, struct Tables *);

struct Polynomial *rs_forney_syndromes(struct Polynomial *synd, struct Polynomial *err_pos, int nmess, struct Tables *table);
