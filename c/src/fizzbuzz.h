#ifndef FIZZBUZZ_H
#define FIZZBUZZ_H

#include <stddef.h>

/* Big enough for "FizzBuzz" and any int ("-2147483648"), plus '\0' */
#define FIZZBUZZ_SIZE 16

/*
 * Writes the FizzBuzz word for n into out (at most size bytes, always '\0'-terminated)
 * and returns out, so it can be used inline: puts(fizzbuzz(3, buf, sizeof buf)).
 * The caller owns the memory: nothing is allocated, nothing needs freeing.
 */
const char *fizzbuzz(int n, char *out, size_t size);

#endif
