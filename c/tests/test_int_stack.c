#include "unity.h"
#include "runner.h"
#include "int_stack.h"

static IntStack *stack;

/* setUp/tearDown own the resource: tearDown runs even when an assertion fails,
   so a red test doesn't also leak (run `make memcheck` to check) */
void setUp(void)
{
    stack = int_stack_create();
    TEST_ASSERT_NOT_NULL(stack);
}

void tearDown(void)
{
    int_stack_destroy(stack);
}

static void test_new_stack_is_empty(void)
{
    TEST_ASSERT_EQUAL_size_t(0, int_stack_size(stack));
}

static void test_pop_returns_last_pushed(void)
{
    int_stack_push(stack, 1);
    int_stack_push(stack, 2);

    int value = 0;
    TEST_ASSERT_TRUE(int_stack_pop(stack, &value)); /* &value: "here's where to put the result" */
    TEST_ASSERT_EQUAL_INT(2, value);
    TEST_ASSERT_EQUAL_size_t(1, int_stack_size(stack));
}

static void test_pop_on_empty_fails_and_leaves_output_alone(void)
{
    int value = 42;
    TEST_ASSERT_FALSE(int_stack_pop(stack, &value));
    TEST_ASSERT_EQUAL_INT(42, value);
}

static void test_peek_does_not_remove(void)
{
    int_stack_push(stack, 7);

    int value = 0;
    TEST_ASSERT_TRUE(int_stack_peek(stack, &value));
    TEST_ASSERT_EQUAL_INT(7, value);
    TEST_ASSERT_EQUAL_size_t(1, int_stack_size(stack));
}

static void test_grows_past_initial_capacity(void)
{
    for (int i = 0; i < 100; i++) {
        TEST_ASSERT_TRUE(int_stack_push(stack, i));
    }

    int value = 0;
    int_stack_pop(stack, &value);
    TEST_ASSERT_EQUAL_INT(99, value);
    TEST_ASSERT_EQUAL_size_t(99, int_stack_size(stack));
}

static void test_destroy_accepts_null(void)
{
    int_stack_destroy(NULL); /* like free(NULL): makes cleanup code simpler */
}

int main(int argc, char **argv)
{
    if (UnityParseOptions(argc, argv) != 0) {
        return 1;
    }
    UNITY_BEGIN();
    RUN(test_new_stack_is_empty);
    RUN(test_pop_returns_last_pushed);
    RUN(test_pop_on_empty_fails_and_leaves_output_alone);
    RUN(test_peek_does_not_remove);
    RUN(test_grows_past_initial_capacity);
    RUN(test_destroy_accepts_null);
    return UNITY_END();
}
