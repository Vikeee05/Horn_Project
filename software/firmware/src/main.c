#include <stdio.h>
#include <inttypes.h>
#include "horn_credit_manager.h"
#include "cloud_sync.h"
#include "battery_monitor.h"
#include "board.h"

int main(void)
{
    board_init();

    horn_credit_state_t horn_state;
    cloud_sync_state_t sync_state;
    battery_monitor_state_t battery_state;

    horn_credit_manager_init(&horn_state);
    cloud_sync_init(&sync_state);
    battery_monitor_init(&battery_state);

    horn_state.local_credits = 120U;
    battery_state.ignition_on = 0U;
    battery_state.battery_voltage_mv = 3700U;

    printf("Horn billing firmware started.\n");
    printf("Initial local credits: %" PRIu32 "\n", horn_state.local_credits);

    /* Example: horn press for 1250 ms => 5 credits */
    if (horn_credit_manager_consume(&horn_state, 1250U)) {
        printf("Horn usage consumed: 5 credits\n");
    } else {
        printf("Not enough credits available\n");
    }

    printf("Remaining credits: %" PRIu32 "\n", horn_state.local_credits);
    printf("Battery charging state: %s\n", battery_state.ignition_on ? "Charging" : "Battery powered");

    for (;;) {
        board_led_on();
        board_delay_ms(500U);
        board_led_off();
        board_delay_ms(500U);
        printf("LED blink heartbeat\n");
    }

    return 0;
}
