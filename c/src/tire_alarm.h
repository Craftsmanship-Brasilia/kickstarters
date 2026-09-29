#ifndef TIRE_ALARM_H
#define TIRE_ALARM_H

/*
 * Dependency injection in C: an "interface" is a struct holding a function pointer plus a
 * void *context (the "this" the function receives). Production passes the real sensor;
 * tests pass stubs and spies (tests/test_tire_alarm.c). No mocking library needed.
 */

typedef struct {
    double (*read_psi)(void *context);
    void *context;
} Sensor;

typedef struct {
    void (*alert)(void *context, const char *message);
    void *context;
} Notifier;

#define TIRE_PSI_LOW 17.0
#define TIRE_PSI_HIGH 21.0

/* Reads the sensor once and alerts when the pressure is outside [LOW, HIGH]. Returns 1 if it alerted */
int tire_alarm_check(Sensor sensor, Notifier notifier);

#endif
