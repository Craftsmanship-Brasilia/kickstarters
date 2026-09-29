/*
 * A tour of Unity: the assertions worth knowing, messages, ignoring tests, and writing
 * your own assertion helpers. The full list is in vendor/unity/unity.h.
 * One test is ignored on purpose (shows up as "1 Ignored", not a failure).
 */
#include "unity.h"
#include "runner.h"

#include <math.h>
#include <stdint.h>
#include <stdio.h>

void setUp(void) {}
void tearDown(void) {}

typedef struct {
    int x;
    int y;
} Point;

/* --- your own assertions -------------------------------------------------------------------
 * A plain helper function would report ITS line on failure. Wrapping it in a macro that passes
 * __LINE__ through, and using Unity's UNITY_TEST_ASSERT_* (which take a line), reports the
 * caller's line instead: `tests/test_unity_features.c:NN` points at the test, not the helper.
 */
static void assert_point_equal(Point expected, Point actual, int line)
{
    char msg[64];
    snprintf(msg, sizeof msg, "expected (%d,%d) was (%d,%d)", expected.x, expected.y, actual.x, actual.y);
    UNITY_TEST_ASSERT_EQUAL_INT(expected.x, actual.x, line, msg);
    UNITY_TEST_ASSERT_EQUAL_INT(expected.y, actual.y, line, msg);
}
#define ASSERT_POINT_EQUAL(expected, actual) assert_point_equal((expected), (actual), __LINE__)

/* --- tests -------------------------------------------------------------------------------- */

static void test_booleans_and_null(void)
{
    int *nothing = NULL;
    TEST_ASSERT_TRUE(1 + 1 == 2);
    TEST_ASSERT_FALSE(1 > 2);
    TEST_ASSERT_NULL(nothing);
    TEST_ASSERT(2 > 1); /* same as TEST_ASSERT_TRUE */
}

static void test_integers(void)
{
    TEST_ASSERT_EQUAL_INT(42, 40 + 2);
    TEST_ASSERT_NOT_EQUAL_INT(0, 42);
    TEST_ASSERT_EQUAL_UINT32(4000000000u, 4000000000u); /* sized variants: INT8..INT64, UINT8..UINT64 */
    TEST_ASSERT_EQUAL_HEX8(0xFF, 255);                   /* failures print as 0xFF */
    TEST_ASSERT_GREATER_THAN(10, 11);                    /* read it as: 11 > 10 */
    TEST_ASSERT_LESS_OR_EQUAL(10, 10);
    TEST_ASSERT_INT_WITHIN(2, 100, 101); /* 101 is within 100 ± 2 */
    TEST_ASSERT_BITS(0x0F, 0x05, 0xA5);  /* only the masked bits are compared */
    TEST_ASSERT_BIT_HIGH(0, 0x01);
}

static void test_floating_point(void)
{
    /* Never compare floats with ==: 0.1 + 0.2 != 0.3 */
    TEST_ASSERT_DOUBLE_WITHIN(1e-9, 0.3, 0.1 + 0.2);
    TEST_ASSERT_EQUAL_DOUBLE(0.3, 0.1 + 0.2); /* uses a tiny relative tolerance */
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 3.14f, 3.14159f);
    TEST_ASSERT_DOUBLE_IS_NAN(NAN);
    TEST_ASSERT_DOUBLE_IS_INF(INFINITY);
}

static void test_strings_and_characters(void)
{
    TEST_ASSERT_EQUAL_STRING("kata", "kata");
    TEST_ASSERT_EQUAL_STRING_LEN("kata", "katalyst", 4); /* only the first 4 chars */
    TEST_ASSERT_EQUAL_CHAR('k', "kata"[0]);
    TEST_ASSERT_EQUAL_STRING_ARRAY(((const char *[]){"a", "b"}), ((const char *[]){"a", "b"}), 2);
}

static void test_arrays(void)
{
    int expected[] = {1, 2, 3};
    int actual[] = {1, 2, 3};
    int zeros[4] = {0};

    TEST_ASSERT_EQUAL_INT_ARRAY(expected, actual, 3); /* failure says which element differs */
    TEST_ASSERT_EACH_EQUAL_INT(0, zeros, 4);          /* every element equals one value */
}

static void test_memory_and_structs(void)
{
    Point a = {1, 2};
    Point b = {1, 2};

    /* Byte comparison: fine for simple structs, but padding bytes and pointers make it fragile.
       Prefer a custom assertion (next test) for anything bigger */
    TEST_ASSERT_EQUAL_MEMORY(&a, &b, sizeof a);
    TEST_ASSERT_EQUAL_PTR(&a, &a); /* same address, not same contents */
}

static void test_custom_assertion(void)
{
    ASSERT_POINT_EQUAL(((Point){1, 2}), ((Point){.x = 1, .y = 2}));
}

static void test_messages_explain_failures(void)
{
    int rolls = 20;
    /* Every assertion has a _MESSAGE variant; the message is printed only on failure */
    TEST_ASSERT_EQUAL_INT_MESSAGE(20, rolls, "a game without strikes has 20 rolls");
    TEST_MESSAGE("TEST_MESSAGE prints info in the output without failing");
}

static void test_ignored_on_purpose(void)
{
    /* Placeholder for the next test on your TDD list: counted as "Ignored", not failed.
       TEST_FAIL_MESSAGE("todo") is the red alternative */
    TEST_IGNORE_MESSAGE("next: strike bonus");
}

static void test_pass_ends_the_test_early(void)
{
    TEST_PASS_MESSAGE("nothing after this runs");
    TEST_FAIL_MESSAGE("never reached");
}

int main(int argc, char **argv)
{
    if (UnityParseOptions(argc, argv) != 0) {
        return 1;
    }
    UNITY_BEGIN();
    RUN(test_booleans_and_null);
    RUN(test_integers);
    RUN(test_floating_point);
    RUN(test_strings_and_characters);
    RUN(test_arrays);
    RUN(test_memory_and_structs);
    RUN(test_custom_assertion);
    RUN(test_messages_explain_failures);
    RUN(test_ignored_on_purpose);
    RUN(test_pass_ends_the_test_early);
    return UNITY_END();
}
