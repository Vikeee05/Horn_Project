#include "cloud_sync.h"

void cloud_sync_init(cloud_sync_state_t *state)
{
    if (state == 0) {
        return;
    }

    state->local_balance = 0;
    state->cloud_balance = 0;
    state->gsm_connected = 0;
    state->pending_sync = 0;
}

void cloud_sync_send_usage(cloud_sync_state_t *state, uint32_t used_credits)
{
    if (state == 0) {
        return;
    }

    if (state->gsm_connected == 0) {
        state->pending_sync = 1;
        return;
    }

    state->local_balance += used_credits;
    state->pending_sync = 0;
}

void cloud_sync_fetch_balance(cloud_sync_state_t *state, uint32_t remote_balance)
{
    if (state == 0) {
        return;
    }

    state->cloud_balance = remote_balance;
    state->local_balance = remote_balance;
}
