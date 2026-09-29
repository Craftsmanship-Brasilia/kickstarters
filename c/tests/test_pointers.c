/*
 * Learning tests: executable notes on pointers, arrays and strings.
 * No production code here; each test pins down one rule of the language.
 */
#include "unity.h"
#include "runner.h"

#include <string.h>

void setUp(void) {}
void tearDown(void) {}

static void swap(int *a, int *b)
{
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

/* `const int array[10]` as a parameter means exactly `const int *array`; the 10 is ignored.
   gcc flags `sizeof array` on such a parameter (-Wsizeof-array-argument), so we spell it as a pointer */
static size_t size_of_parameter(const int *array)
{
    return sizeof array;
}

static void test_a_pointer_holds_an_address(void)
{
    int x = 1;
    int *p = &x; /* & = "address of" */
    *p = 2;      /* * = "the thing at" (dereference) */
    TEST_ASSERT_EQUAL_INT(2, x);
}

static void test_pass_a_pointer_to_modify_the_callers_variable(void)
{
    int a = 1, b = 2;
    swap(&a, &b); /* C only passes by value; passing the address is how you get "ref" */
    TEST_ASSERT_EQUAL_INT(2, a);
    TEST_ASSERT_EQUAL_INT(1, b);
}

static void test_arrays_decay_to_pointers(void)
{
    int numbers[10] = {0};
    TEST_ASSERT_EQUAL_size_t(10 * sizeof(int), sizeof numbers);           /* here: the whole array */
    TEST_ASSERT_EQUAL_size_t(sizeof(int *), size_of_parameter(numbers)); /* in a function: a pointer */
    /* so functions taking arrays also need the length: f(const int *items, size_t count) */
}

static void test_pointer_arithmetic_moves_by_element(void)
{
    int numbers[] = {10, 20, 30};
    int *p = numbers; /* same as &numbers[0] */

    TEST_ASSERT_EQUAL_INT(20, *(p + 1)); /* p + 1 moves sizeof(int) bytes */
    TEST_ASSERT_EQUAL_INT(30, p[2]);     /* p[i] is just *(p + i) */
}

static void test_strings_are_char_arrays_ending_in_nul(void)
{
    char word[] = "kata"; /* array copy of the literal: 5 chars incl. '\0', writable */
    TEST_ASSERT_EQUAL_size_t(5, sizeof word);
    TEST_ASSERT_EQUAL_size_t(4, strlen(word));
    TEST_ASSERT_EQUAL_CHAR('\0', word[4]);

    word[0] = 'K';
    TEST_ASSERT_EQUAL_STRING("Kata", word);
    /* char *lit = "kata"; lit[0] = 'K';  compiles (literals are char[] in C), but the literal is
       read-only memory: undefined behaviour, usually a crash. Point at literals with const char *. */
}

static void test_compare_strings_with_strcmp_not_equals(void)
{
    char a[] = "kata";
    char b[] = "kata";
    const char *pa = a, *pb = b;
    TEST_ASSERT_TRUE(pa != pb);             /* == compares addresses (`a == b` is even a -Warray-compare error) */
    TEST_ASSERT_EQUAL_INT(0, strcmp(a, b)); /* strcmp compares contents (0 = equal) */
}

static void test_const_placement(void)
{
    int x = 1, y = 2;

    const int *read_only_target = &x; /* can't change *p, can point elsewhere */
    read_only_target = &y;

    int *const fixed_pointer = &x; /* can change *p, can't point elsewhere */
    *fixed_pointer = 3;

    /* read right-to-left: "fixed_pointer is a const pointer to int" */
    TEST_ASSERT_EQUAL_INT(2, *read_only_target);
    TEST_ASSERT_EQUAL_INT(3, x);
}

static void test_null_is_a_pointer_to_nothing(void)
{
    int *p = NULL;
    TEST_ASSERT_NULL(p); /* dereferencing it is undefined behaviour: UBSan/ASan catch it */
}

int main(int argc, char **argv)
{
    if (UnityParseOptions(argc, argv) != 0) {
        return 1;
    }
    UNITY_BEGIN();
    RUN(test_a_pointer_holds_an_address);
    RUN(test_pass_a_pointer_to_modify_the_callers_variable);
    RUN(test_arrays_decay_to_pointers);
    RUN(test_pointer_arithmetic_moves_by_element);
    RUN(test_strings_are_char_arrays_ending_in_nul);
    RUN(test_compare_strings_with_strcmp_not_equals);
    RUN(test_const_placement);
    RUN(test_null_is_a_pointer_to_nothing);
    return UNITY_END();
}
