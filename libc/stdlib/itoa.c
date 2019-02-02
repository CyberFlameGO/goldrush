#include <stddef.h>
#include <stdlib.h>

char *itoa(int val, char *buf, int base)
{
    // itoa() takes a number and outputs it in the specified base.
    // The number of digits produced can be expressed as ceil(log_base(n)).
    unsigned long u = (unsigned) val;

    int offset = 0;

    // It is often desirable that base 10 numbers should not be printed
    // in the native two's complement that computers use.
    if (val < 0 && base == 10)
    {
        u = (unsigned) -val;
    }

    // Divide by the base repeatedly and take the remainder. This takes the
    // representation in reverse order.
    do
    {
        int remainder = u % base;

        buf[offset++] = (remainder < 10) ? remainder + '0' : remainder - 10 + 'a';
    } while (u /= base);

    if (val < 0)
    {
        buf[offset++] = '-';
    }
    buf[offset] = '\0';

    for (int i = offset - 1, j = 0; j < i; i--, j++)
    {
        char tmp = buf[i];
        buf[i] = buf[j];
        buf[j] = tmp;
    }

    return buf;
}