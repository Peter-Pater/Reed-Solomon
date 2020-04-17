#include "poly_comp_arithmetic.h"

struct Polynomial *rs_encode_msg(struct Polynomial *, long, struct Tables *);

struct Polynomial *rs_calc_syndromes(struct Polynomial *, long, struct Tables *);

long rs_check(struct Polynomial *, long, struct Tables *);

struct Polynomial *rs_find_errata_locator(struct Polynomial *, struct Tables *);

struct Polynomial *rs_find_error_evaluator(struct Polynomial *, struct Polynomial *, long, struct Tables *);

struct Polynomial *rs_correct_errata(struct Polynomial *, struct Polynomial *, struct Polynomial *, struct Tables *, int bits);

struct Polynomial *rs_find_error_locator(struct Polynomial *, long, struct Polynomial *, struct Tables *);

struct Polynomial *rs_find_errors(struct Polynomial *, long, struct Tables *);

struct Polynomial *rs_forney_syndromes(struct Polynomial *, struct Polynomial *, long, struct Tables *);

// struct Polynomial * rs_correct_msg(struct Polynomial *, long, struct Polynomial *, struct Tables *);
struct Polynomial *rs_correct_msg(struct Polynomial *, long, struct Tables *, int bits);
