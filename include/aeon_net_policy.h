#ifndef AEON_NET_POLICY_H
#define AEON_NET_POLICY_H
#include <stdint.h>
#include "aeon_kernel_device.h"
#include "aeon_semantic_gate.h"
typedef enum { AEON_NET_HOOK_PRE_TX=1, AEON_NET_HOOK_PRE_RX=2 } aeon_net_hook_t;
typedef struct { uint32_t process_id; uint32_t protocol; uint32_t payload_len; uint8_t semantic_candidate; uint8_t authenticated; const aeon_semantic_frame_t *semantic_frame; } aeon_net_event_t;
typedef enum { AEON_NET_DROP=0, AEON_NET_ACCEPT=1, AEON_NET_DEFER_IDENTITY=2 } aeon_net_action_t;
int aeon_net_policy_decide(const aeon_net_event_t *, aeon_net_hook_t, const aeon_did_registry_t *, aeon_net_action_t *);
#endif
