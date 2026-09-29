#include "unity.h"
#include "runner.h"
#include "modern_c.h"

void setUp(void) {}
void tearDown(void) {}

static void test_designated_initializers_zero_everything_not_named(void)
{
    Point p = {.y = 2}; /* C89: Point p = {0, 2}; (positional, breaks if fields are reordered) */
    TEST_ASSERT_EQUAL_INT(0, p.x);
    TEST_ASSERT_EQUAL_INT(2, p.y);

    int squares[5] = {[1] = 1, [4] = 16}; /* array designators too */
    TEST_ASSERT_EQUAL_INT_ARRAY(((int[]){0, 1, 0, 0, 16}), squares, 5);
}

static void test_structs_are_values(void)
{
    Point a = {1, 2};
    Point b = a; /* a copy, not a reference */
    b.x = 100;

    TEST_ASSERT_EQUAL_INT(1, a.x);
    TEST_ASSERT_EQUAL_INT(100, b.x);
    TEST_ASSERT_EQUAL_INT(11, point_add(a, (Point){.x = 10}).x); /* compound literal as argument */
}

static void test_anonymous_union_members(void)
{
    Shape circle = {.kind = SHAPE_CIRCLE, .radius = 2};
    Shape rect = {.kind = SHAPE_RECT, .w = 3, .h = 4};

    TEST_ASSERT_EQUAL_INT(12, shape_area_approx(circle));
    TEST_ASSERT_EQUAL_INT(12, shape_area_approx(rect));
}

static void test_inline_function_evaluates_argument_once(void)
{
    int i = 3;
    TEST_ASSERT_EQUAL_INT(9, square(i++));
    TEST_ASSERT_EQUAL_INT(4, i); /* the SQUARE(i++) macro would give 5 (and undefined behaviour) */
}

static void test_generic_selects_by_type(void)
{
    TEST_ASSERT_EQUAL_STRING("int", describe(42));
    TEST_ASSERT_EQUAL_STRING("double", describe(4.2));
    TEST_ASSERT_EQUAL_STRING("string", describe("42"));
}

static void test_fixed_width_integers(void)
{
    TEST_ASSERT_EQUAL_size_t(1, sizeof(int8_t));
    TEST_ASSERT_EQUAL_size_t(8, sizeof(int64_t));
    TEST_ASSERT_EQUAL_UINT8(255, (uint8_t)-1); /* unsigned wrap-around is defined; signed overflow is not */
}

static void test_flexible_array_member(void)
{
    IntList *list = int_list_of(3, (int[]){1, 2, 3});
    TEST_ASSERT_NOT_NULL(list);

    TEST_ASSERT_EQUAL_INT(6, int_list_sum(list));
    int_list_free(list); /* forget this and `make memcheck` / `make asan` fail */
}

static void test_restrict_arrays(void)
{
    int out[3];
    add_arrays(3, out, (int[]){1, 2, 3}, (int[]){10, 20, 30});
    TEST_ASSERT_EQUAL_INT_ARRAY(((int[]){11, 22, 33}), out, 3);
}

int main(int argc, char **argv)
{
    if (UnityParseOptions(argc, argv) != 0) {
        return 1;
    }
    UNITY_BEGIN();
    RUN(test_designated_initializers_zero_everything_not_named);
    RUN(test_structs_are_values);
    RUN(test_anonymous_union_members);
    RUN(test_inline_function_evaluates_argument_once);
    RUN(test_generic_selects_by_type);
    RUN(test_fixed_width_integers);
    RUN(test_flexible_array_member);
    RUN(test_restrict_arrays);
    return UNITY_END();
}
