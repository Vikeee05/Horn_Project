#include "horn_credit_manager.h"

void horn_credit_manager_init(horn_credit_state_t *state)
{
    if (state == 0) {
        return;
    }

    state->local_credits = 0;
    state->cloud_credits = 0;
    state->reed_count = 0;
    state->last_press_ms = 0;
    state->horn_active = 0;
}

uint32_t horn_credit_manager_duration_to_credits(uint32_t duration_ms)
{
    if (duration_ms == 0) {
        return 0;
    }

    return duration_ms / CREDIT_DURATION_MS;
}

uint8_t horn_credit_manager_consume(horn_credit_state_t *state, uint32_t duration_ms)
{
    uint32_t consumed_credits;

    if (state == 0) {
        return 0;
    }

    consumed_credits = horn_credit_manager_duration_to_credits(duration_ms);

    if (consumed_credits == 0) {
        return 0;
    }

    if (state->local_credits < consumed_credits) {
        return 0;
    }

    state->local_credits -= consumed_credits;
    state->reed_count += consumed_credits;
    return 1;
}

void horn_credit_manager_update_cloud_balance(horn_credit_state_t *state, uint32_t remote_credits)
{
    if (state == 0) {
        return;
    }

    state->cloud_credits = remote_credits;
    if (state->cloud_credits > MAX_CREDITS) {
        state->cloud_credits = MAX_CREDITS;
    }
}
