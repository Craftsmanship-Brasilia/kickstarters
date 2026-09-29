#include "fizzbuzz.h"

#include <stdbool.h>
#include <stdio.h>

static bool divisible(int n, int by)
{
    return n % by == 0;
}

const char *fizzbuzz(int n, char *out, size_t size)
{
    if (divisible(n, 15)) {
        snprintf(out, size, "FizzBuzz");
    } else if (divisible(n, 3)) {
        snprintf(out, size, "Fizz");
    } else if (divisible(n, 5)) {
        snprintf(out, size, "Buzz");
    } else {
        snprintf(out, size, "%d", n);
    }
    return out;
}
