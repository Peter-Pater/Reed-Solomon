#include "utility.h"

void printBinary(int n)
{
    int num = n;

    char *buf = (char*)malloc((n + 1) * sizeof(char));

    if (NULL == buf)
    {
        return;
    }

    char *p = buf;

    do
    {
        if (n % 2 == 0)
        {
            *p = '0';
        } else
        {
            *p = '1';
        }
        p++;
        n /= 2;
    } while (n > 0);
    *p = '\0';

    reverse_string(buf);
    printf("%d = %s\n", num, buf);
    free(buf);

}

void reverse_string(char *str)
{
    if (str == 0 || *str == 0)
    {
        return;
    }

    char *s = str;
    char *e = s + strlen(str) - 1;
    char t;

    while (e > s)
    {
        t = *s;
        *s = *e;
        *e = t;

        ++s;
        --e;
    }

}

int bit_length(int n)
{
    int bits = 0;
    while (n >> bits)
    {
        bits++;
    }
    return bits;
}

int carry_less_long_div(int dividend, int divisor)
{
    int len_dividend = bit_length(dividend);
    int len_divisor = bit_length(divisor);

    // printf("dividend length is %d\n", len_dividend);
    // printf("divisor length is %d\n", len_divisor);

    if (len_dividend < len_divisor)
    {
        return dividend;
    }

    int pos = len_dividend - len_divisor;
    while (pos >= 0)
    {
        if (dividend & (1 << (pos + len_divisor - 1)))
        {
            dividend ^= divisor << pos;
        }
        pos--;
    }

    return dividend;
}

// precompute the logarithm and anti-log tables for faster computation (NOT USED)
int table_generator(int prime_polynomial)
{
    int *gf_exp = malloc(512 * sizeof(int));
    int *gf_log = malloc(256 * sizeof(int));
    if (gf_exp == NULL || gf_log == NULL)
    {
        return -1;
    }

    int x = 1;
    for (int i = 0; i < 255; i++)
    {
        *(gf_exp + i) = x;
        *(gf_log + x) = i;
        x = gf_mul_MR_BCH_8bits(x, 2, prime_polynomial);
    }
    for (int i = 255; i < 512; i++)
    {
        *(gf_exp + i) = *(gf_exp + i - 255);
    }
    for (int i = 0; i < 512; i++)
    {
        printf("gf_exp[%d] = %d\n", i, *(gf_exp + i));
    }
    for (int i = 0; i < 256; i++)
    {
        printf("gf_log[%d] = %d\n", i, *(gf_log + i));
    }
    free(gf_exp);
    free(gf_log);
    return 0;
}

// precompute the logarithm and anti-log tables for faster computation
struct Tables *newTables(int prime_polynomial, size_t sz)
{
    struct Tables *retVal = malloc(sizeof(struct Tables));
    if (retVal == NULL)
    {
        return NULL;
    }

    retVal->gf_exp = malloc(2 * sz * sizeof(int));
    retVal->gf_log = malloc(sz * sizeof(int));
    if (retVal->gf_exp == NULL || retVal->gf_log == NULL)
    {
        free (retVal);
        return NULL;
    }

    retVal->gf_exp_size = 2 * sz;
    retVal->gf_log_size = sz;

    int x = 1;
    for (int i = 0; i < 255; i++)
    {
        *(retVal->gf_exp + i) = x;
        *(retVal->gf_log + x) = i;
        x = gf_mul_MR_BCH_8bits(x, 2, prime_polynomial);
    }
    for (int i = 255; i < 512; i++)
    {
        *(retVal->gf_exp + i) = *(retVal->gf_exp + i - 255);
    }

    return retVal;

}

// free tables
void delTables(struct Tables *tables)
{
    if (tables != NULL)
    {
        free(tables->gf_exp);
        free(tables->gf_log);
        free(tables);
    }
}

void printTables(struct Tables *tables)
{
    printf("gf_exp_table:\n");
    for (int i = 0; i < 512; i++)
    {
        printf("gf_exp[%d] = %d\n", i, *(tables->gf_exp + i));
    }
    printf("\ngf_log_table:\n");
    for (int i = 0; i < 256; i++)
    {
        printf("gf_log[%d] = %d\n", i, *(tables->gf_log + i));
    }
}

struct Polynomial *newPolynomial(int *arr, size_t sz)
{
    struct Polynomial *ret_val = malloc(sizeof(struct Polynomial));
    if (ret_val == NULL)
    {
        return NULL;
    }

    ret_val->ploy_arr = malloc(sz * sizeof(int));

    if (ret_val->ploy_arr == NULL)
    {
        free (ret_val);
        return NULL;
    }

    ret_val->poly_size = sz;

    for (int i = 0; i < sz; i++)
    {
        *(ret_val->ploy_arr + i) = *(arr + i);
    }

    return ret_val;
}

void delPolynomial(struct Polynomial *poly)
{
    if (poly != NULL)
    {
        free(poly->ploy_arr);
        free(poly);
    }
}

void printPolynomial(struct Polynomial *poly)
{
    printf("the size of poly1 is %lu\n", poly->poly_size);
    printf("the coefficients of poly1 is: ");
    for (int i = 0; i < poly->poly_size; i++)
    {
        printf("%d ", *(poly->ploy_arr+i));
    }
    printf("\n");
}
