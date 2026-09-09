#include "battery_monitor.h"

void battery_monitor_init(battery_monitor_state_t *state)
{
    if (state == 0) {
        return;
    }

    state->ignition_on = 0;
    state->charging_active = 0;
    state->battery_low = 0;
    state->battery_voltage_mv = 0;
}

void battery_monitor_update(battery_monitor_state_t *state, uint8_t ignition_signal, uint16_t measured_mv)
{
    if (state == 0) {
        return;
    }

    state->ignition_on = ignition_signal;
    state->battery_voltage_mv = measured_mv;
    state->charging_active = ignition_signal;
    state->battery_low = (measured_mv < 3000U) ? 1U : 0U;
}
