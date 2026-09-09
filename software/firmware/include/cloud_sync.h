#ifndef CLOUD_SYNC_H
#define CLOUD_SYNC_H

#include <stdint.h>

typedef struct {
    uint32_t local_balance;
    uint32_t cloud_balance;
    uint8_t gsm_connected;
    uint8_t pending_sync;
} cloud_sync_state_t;

void cloud_sync_init(cloud_sync_state_t *state);
void cloud_sync_send_usage(cloud_sync_state_t *state, uint32_t used_credits);
void cloud_sync_fetch_balance(cloud_sync_state_t *state, uint32_t remote_balance);

#endif
