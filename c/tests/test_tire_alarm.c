/*
 * Test doubles by hand. What a jest.fn()/vi.fn() or Moq does for you, in plain C:
 *   stub = a function returning canned data (read from its context)
 *   spy  = a function recording how it was called (into its context)
 *
 * Other C techniques, for when function pointers aren't an option (e.g. testing code that
 * calls a library directly):
 *   link-time substitution: link a fake .c instead of the real one (one test binary per fake;
 *     this Makefile's one-binary-per-test-file layout makes that easy to add)
 *   CMock: generates mocks from headers (Unity's sister project, needs Ruby)
 */
#include "unity.h"
#include "runner.h"
#include "tire_alarm.h"

#include <string.h>

/* --- stub: returns whatever psi its context points to ------------------------------------ */

static double stub_read_psi(void *context)
{
    return *(const double *)context;
}

static Sensor sensor_reading(double *psi)
{
    return (Sensor){.read_psi = stub_read_psi, .context = psi};
}

/* --- spy: remembers every alert ----------------------------------------------------------- */

typedef struct {
    int calls;
    char last_message[64];
} AlertSpy;

static void spy_alert(void *context, const char *message)
{
    AlertSpy *spy = context;
    spy->calls++;
    strncpy(spy->last_message, message, sizeof spy->last_message - 1);
}

static AlertSpy spy;
static Notifier notifier;

void setUp(void)
{
    spy = (AlertSpy){0}; /* fresh spy per test: no state leaks between tests */
    notifier = (Notifier){.alert = spy_alert, .context = &spy};
}

void tearDown(void) {}

/* --- tests -------------------------------------------------------------------------------- */

static void test_normal_pressure_does_not_alert(void)
{
    double psi = 19.0;
    TEST_ASSERT_EQUAL_INT(0, tire_alarm_check(sensor_reading(&psi), notifier));
    TEST_ASSERT_EQUAL_INT(0, spy.calls);
}

static void test_low_pressure_alerts_once(void)
{
    double psi = 16.9;
    TEST_ASSERT_EQUAL_INT(1, tire_alarm_check(sensor_reading(&psi), notifier));
    TEST_ASSERT_EQUAL_INT(1, spy.calls);
    TEST_ASSERT_EQUAL_STRING("pressure too low", spy.last_message);
}

static void test_high_pressure_alerts(void)
{
    double psi = 21.1;
    tire_alarm_check(sensor_reading(&psi), notifier);
    TEST_ASSERT_EQUAL_STRING("pressure too high", spy.last_message);
}

static void test_limits_are_inclusive(void)
{
    double low = TIRE_PSI_LOW, high = TIRE_PSI_HIGH;
    tire_alarm_check(sensor_reading(&low), notifier);
    tire_alarm_check(sensor_reading(&high), notifier);
    TEST_ASSERT_EQUAL_INT(0, spy.calls);
}

int main(int argc, char **argv)
{
    if (UnityParseOptions(argc, argv) != 0) {
        return 1;
    }
    UNITY_BEGIN();
    RUN(test_normal_pressure_does_not_alert);
    RUN(test_low_pressure_alerts_once);
    RUN(test_high_pressure_alerts);
    RUN(test_limits_are_inclusive);
    return UNITY_END();
}
