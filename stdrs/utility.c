#include "math.h"
#include "utility.h"

void printBinary(long n)
{
    long num = n;

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
    printf("%ld = %s\n", num, buf);
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

long bit_length(long n)
{
    long bits = 0;
    while (n >> bits)
    {
        bits++;
    }
    return bits;
}

long carry_less_long_div(long dividend, long divisor)
{
    long len_dividend = bit_length(dividend);
    long len_divisor = bit_length(divisor);

    // printf("dividend length is %ld\n", len_dividend);
    // printf("divisor length is %ld\n", len_divisor);

    if (len_dividend < len_divisor)
    {
        return dividend;
    }

    long pos = len_dividend - len_divisor;
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
long table_generator(long prime_polynomial)
{
    long *gf_exp = malloc(512 * sizeof(long));
    long *gf_log = malloc(256 * sizeof(long));
    if (gf_exp == NULL || gf_log == NULL)
    {
        return -1;
    }

    long x = 1;
    for (long i = 0; i < 255; i++)
    {
        *(gf_exp + i) = x;
        *(gf_log + x) = i;
        x = gf_mul_MR_BCH(x, 2, prime_polynomial, 8);
    }
    for (long i = 255; i < 512; i++)
    {
        *(gf_exp + i) = *(gf_exp + i - 255);
    }
    for (long i = 0; i < 512; i++)
    {
        printf("gf_exp[%ld] = %ld\n", i, *(gf_exp + i));
    }
    for (long i = 0; i < 256; i++)
    {
        printf("gf_log[%ld] = %ld\n", i, *(gf_log + i));
    }
    free(gf_exp);
    free(gf_log);
    return 0;
}

// precompute the logarithm and anti-log tables for faster computation
struct Tables *newTables(long prime_polynomial, long bits)
{
    size_t sz = (long) pow(2.0, (double) bits);
    // printf("%zu\n", sz);
    struct Tables *retVal = malloc(sizeof(struct Tables));
    if (retVal == NULL)
    {
        return NULL;
    }

    retVal->gf_exp = malloc(2 * sz * sizeof(long));
    retVal->gf_log = malloc(sz * sizeof(long));

    // printf("here\n");
    if (retVal->gf_exp == NULL || retVal->gf_log == NULL)
    {
        free (retVal);
        return NULL;
    }

    retVal->gf_exp_size = 2 * sz;
    retVal->gf_log_size = sz;
    // printf("here\n");

    long x = 1;
    for (long i = 0; i < sz - 1; i++)
    {
        *(retVal->gf_exp + i) = x;
        *(retVal->gf_log + x) = i;
        x = gf_mul_MR_BCH(x, 2, prime_polynomial, bits);
        // printf("%ld\n", x);
    }
    for (long i = sz - 1; i < 2 * sz; i++)
    {
        *(retVal->gf_exp + i) = *(retVal->gf_exp + i - (1 * sz - 1));
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
    for (long i = 0; i < tables->gf_exp_size; i++)
    {
        printf("gf_exp[%ld] = %ld\n", i, *(tables->gf_exp + i));
    }
    printf("\ngf_log_table:\n");
    for (long i = 0; i < (tables->gf_log_size - 1); i++)
    {
        printf("gf_log[%ld] = %ld\n", i, *(tables->gf_log + i));
    }
}

struct Polynomial *randPolynomial(size_t sz)
{
    struct Polynomial *ret_val = malloc(sizeof(struct Polynomial));
    if (ret_val == NULL)
    {
        return NULL;
    }

    ret_val->poly_arr = malloc(sz * sizeof(long));

    if (ret_val->poly_arr == NULL)
    {
        free (ret_val);
        return NULL;
    }

    ret_val->poly_size = sz;

    int l = 32;
    int r = 126;
    for (long i = 0; i < sz; i++)
    {
        *(ret_val->poly_arr + i) = l + (rand() % (r - l + 1));
    }

    return ret_val;
}

struct Polynomial *newPolynomial(long *arr, size_t sz)
{
    struct Polynomial *ret_val = malloc(sizeof(struct Polynomial));
    if (ret_val == NULL)
    {
        return NULL;
    }

    ret_val->poly_arr = malloc(sz * sizeof(long));

    if (ret_val->poly_arr == NULL)
    {
        free (ret_val);
        return NULL;
    }

    ret_val->poly_size = sz;

    for (long i = 0; i < sz; i++)
    {
        *(ret_val->poly_arr + i) = *(arr + i);
    }

    return ret_val;
}

void delPolynomial(struct Polynomial *poly)
{
    if (poly != NULL)
    {
        free(poly->poly_arr);
        free(poly);
    }
}

struct Polynomial* corruptPolynomial(struct Polynomial *poly, int num_err)
{
    int l = 32;
    int r = 126;
    struct Polynomial *ret_val = newPolynomial(poly->poly_arr, poly->poly_size);
    for (long i = 0; i < poly->poly_size; i++)
    {
        *(ret_val->poly_arr + i) = *(poly->poly_arr + i);
    }
    // int err_index_arr[num_err];
    while (num_err > 0)
    {
        // *(err_index_arr + j) = rand() % (poly->poly_size);
        int rand_index = rand() % (poly->poly_size);
        if (*(ret_val->poly_arr + rand_index) == *(poly->poly_arr + rand_index))
        {
            int rand_num = l + (rand() % (r - l + 1));
            if (rand_num != *(poly->poly_arr + rand_index))
            {
                *(ret_val->poly_arr + rand_index) = l + (rand() % (r - l + 1));
                num_err--;
            }
        }
    }
    return ret_val;
}

struct Polynomial* reversePolynomial(struct Polynomial *poly)
{
    struct Polynomial *ret_val = newPolynomial(poly->poly_arr, poly->poly_size);
    for (long i = poly->poly_size - 1; i >= 0; i--)
    {
        *(ret_val->poly_arr + i) = *(poly->poly_arr + poly->poly_size - i - 1);
    }
    return ret_val;
}

int isEqualPolynomial(struct Polynomial *poly1, struct Polynomial *poly2)
{
    if (poly1->poly_size != poly2->poly_size)
    {
        return 0;
    }
    for (int i = 0; i < poly1->poly_size; i++)
    {
        if (*(poly1->poly_arr + i) != *(poly2->poly_arr + i))
        {
            return 0;
        }
    }
    return 1;
}

void printPolynomial(struct Polynomial *poly)
{
    printf("the size of poly is %lu\n", poly->poly_size);
    printf("the coefficients of poly is: \n");
    for (long i = 0; i < poly->poly_size; i++)
    {
        printf("%ld ", *(poly->poly_arr+i));
    }
    printf("\n");
}

void printPolynomialAsMessage(struct Polynomial *poly, long k)
{
    // printf("the size of poly is %lu\n", poly->poly_size);
    printf("The message is: \n");
    for (long i = 0; i < k; i++)
    {
        printf("%c", (int) *(poly->poly_arr+i));
    }
    printf("\n");
}

struct DynamicArray *newDynamicArray(size_t capacity)
{
    struct DynamicArray *ret_val = malloc(sizeof(struct DynamicArray));
    ret_val->data = NULL;
    ret_val->arr_size = 0;
    ret_val->capacity = capacity;
    return ret_val;
}

void delDynamicArray(struct DynamicArray * array)
{
    if (array != NULL)
    {
        free(array->data);
        free(array);
    }
}

void push_back(struct DynamicArray * array, long val)
{
    if (array->arr_size == 0)
    {
        array->data = malloc(sizeof(long) * array->capacity);
    }
    if (array->arr_size + 1 == array->capacity)
    {
        array->capacity *= 2;
        array->data = realloc(array->data, sizeof(long) * array->capacity);
    }
    *(array->data + array->arr_size) = val;
    array->arr_size++;
}

void printDynamicArray(struct DynamicArray * array)
{
    printf("the size of the arr is %zu\n", array->arr_size);
    printf("the capacity of the arr is %zu\n", array->capacity);
    for (long i = 0; i < array->arr_size; i++)
    {
        printf("%ld ", array->data[i]);
    }
    printf("\n");
}
