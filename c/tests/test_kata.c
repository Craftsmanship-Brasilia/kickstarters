#include "unity.h"
#include "runner.h"
#include "kata.h"

/* Unity calls these around every test */
void setUp(void) {}
void tearDown(void) {}

static void test_setup_works(void)
{
    TEST_ASSERT_EQUAL_STRING("kata", kata_name());
}

int main(int argc, char **argv)
{
    if (UnityParseOptions(argc, argv) != 0) {
        return 1;
    }
    UNITY_BEGIN();
    RUN(test_setup_works);
    return UNITY_END();
}
