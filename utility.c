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

struct Polynomial *newPolynomial(int *arr, size_t sz)
{
    struct Polynomial *retVal = malloc(sizeof(struct Polynomial));
    if (retVal == NULL)
    {
        return NULL;
    }

    retVal->ploy_arr = malloc(sz * sizeof(int));

    if (retVal->ploy_arr == NULL) 
    {
        free (retVal);
        return NULL;
    }

    retVal->poly_size = sz;

    for (int i = 0; i < sz; i++)
    {
        *(retVal->ploy_arr + i) = *(arr + i);
    }

    return retVal;
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
