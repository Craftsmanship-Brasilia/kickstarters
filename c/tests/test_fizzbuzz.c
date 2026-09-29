#include "unity.h"
#include "runner.h"
#include "fizzbuzz.h"

#include <limits.h>

static char buf[FIZZBUZZ_SIZE];

/* A fresh, garbage-filled buffer per test: proves fizzbuzz always terminates the string */
void setUp(void)
{
    for (size_t i = 0; i < sizeof buf; i++) {
        buf[i] = '#';
    }
}

void tearDown(void) {}

/* The TDD steps, in the order they were written: one small test per new rule */

static void test_plain_number_is_echoed(void)
{
    TEST_ASSERT_EQUAL_STRING("1", fizzbuzz(1, buf, sizeof buf));
}

static void test_multiple_of_3_is_fizz(void)
{
    TEST_ASSERT_EQUAL_STRING("Fizz", fizzbuzz(3, buf, sizeof buf));
}

static void test_multiple_of_5_is_buzz(void)
{
    TEST_ASSERT_EQUAL_STRING("Buzz", fizzbuzz(5, buf, sizeof buf));
}

static void test_multiple_of_15_is_fizzbuzz(void)
{
    TEST_ASSERT_EQUAL_STRING("FizzBuzz", fizzbuzz(15, buf, sizeof buf));
}

/*
 * C has no parameterized tests: a table + a loop does the same job.
 * The _MESSAGE variant says which row failed.
 */
static void test_first_fifteen(void)
{
    static const struct {
        int n;
        const char *expected;
    } cases[] = {
        {1, "1"},     {2, "2"},     {3, "Fizz"},  {4, "4"},     {5, "Buzz"},
        {6, "Fizz"},  {7, "7"},     {8, "8"},     {9, "Fizz"},  {10, "Buzz"},
        {11, "11"},   {12, "Fizz"}, {13, "13"},   {14, "14"},   {15, "FizzBuzz"},
    };

    for (size_t i = 0; i < sizeof cases / sizeof cases[0]; i++) {
        char msg[32];
        snprintf(msg, sizeof msg, "n = %d", cases[i].n);
        TEST_ASSERT_EQUAL_STRING_MESSAGE(cases[i].expected, fizzbuzz(cases[i].n, buf, sizeof buf), msg);
    }
}

/* Edge cases a C API has to decide on: tiny buffers and extreme ints */

static void test_small_buffer_truncates_but_stays_terminated(void)
{
    char tiny[5];
    TEST_ASSERT_EQUAL_STRING("Fizz", fizzbuzz(15, tiny, sizeof tiny));
}

static void test_int_min_fits_in_fizzbuzz_size(void)
{
    TEST_ASSERT_EQUAL_STRING("-2147483648", fizzbuzz(INT_MIN, buf, sizeof buf));
}

static void test_returns_the_callers_buffer(void)
{
    TEST_ASSERT_EQUAL_PTR(buf, fizzbuzz(7, buf, sizeof buf));
}

int main(int argc, char **argv)
{
    if (UnityParseOptions(argc, argv) != 0) {
        return 1;
    }
    UNITY_BEGIN();
    RUN(test_plain_number_is_echoed);
    RUN(test_multiple_of_3_is_fizz);
    RUN(test_multiple_of_5_is_buzz);
    RUN(test_multiple_of_15_is_fizzbuzz);
    RUN(test_first_fifteen);
    RUN(test_small_buffer_truncates_but_stays_terminated);
    RUN(test_int_min_fits_in_fizzbuzz_size);
    RUN(test_returns_the_callers_buffer);
    return UNITY_END();
}
