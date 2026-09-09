#ifndef BATTERY_MONITOR_H
#define BATTERY_MONITOR_H

#include <stdint.h>

typedef struct {
    uint8_t ignition_on;
    uint8_t charging_active;
    uint8_t battery_low;
    uint16_t battery_voltage_mv;
} battery_monitor_state_t;

void battery_monitor_init(battery_monitor_state_t *state);
void battery_monitor_update(battery_monitor_state_t *state, uint8_t ignition_signal, uint16_t measured_mv);

#endif
