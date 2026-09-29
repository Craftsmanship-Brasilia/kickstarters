#include "tire_alarm.h"

int tire_alarm_check(Sensor sensor, Notifier notifier)
{
    double psi = sensor.read_psi(sensor.context);

    if (psi < TIRE_PSI_LOW) {
        notifier.alert(notifier.context, "pressure too low");
        return 1;
    }
    if (psi > TIRE_PSI_HIGH) {
        notifier.alert(notifier.context, "pressure too high");
        return 1;
    }
    return 0;
}
