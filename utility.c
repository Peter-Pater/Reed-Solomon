#include<utility.h>

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
    // strrev(buf);
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