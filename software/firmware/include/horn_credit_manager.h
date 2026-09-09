#ifndef HORN_CREDIT_MANAGER_H
#define HORN_CREDIT_MANAGER_H

#include <stdint.h>

#define CREDIT_DURATION_MS 250U
#define MAX_CREDITS 10000U

typedef struct {
    uint32_t local_credits;
    uint32_t cloud_credits;
    uint32_t reed_count;
    uint32_t last_press_ms;
    uint8_t horn_active;
} horn_credit_state_t;

void horn_credit_manager_init(horn_credit_state_t *state);
uint32_t horn_credit_manager_duration_to_credits(uint32_t duration_ms);
uint8_t horn_credit_manager_consume(horn_credit_state_t *state, uint32_t duration_ms);
void horn_credit_manager_update_cloud_balance(horn_credit_state_t *state, uint32_t remote_credits);

#endif
