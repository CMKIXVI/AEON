#ifndef AEON_KERNEL_ADAPTER_H
#define AEON_KERNEL_ADAPTER_H
#include <stdint.h>
#include "aeon_net_policy.h"
typedef struct { uint32_t attached; uint32_t tx_events; uint32_t rx_events; uint32_t deferred_identity; uint32_t dropped; } aeon_kernel_hook_stats_t;
int aeon_kernel_hooks_attach(aeon_kernel_hook_stats_t *);
int aeon_kernel_hooks_detach(aeon_kernel_hook_stats_t *);
int aeon_kernel_hook_event(aeon_kernel_hook_stats_t *, const aeon_net_event_t *, aeon_net_hook_t, const aeon_did_registry_t *, aeon_net_action_t *);
#endif
