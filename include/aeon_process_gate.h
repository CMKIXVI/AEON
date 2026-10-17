#ifndef AEON_PROCESS_GATE_H
#define AEON_PROCESS_GATE_H
#include <stddef.h>
#include <stdint.h>
#include "aeon_device.h"
#define AEON_MAX_PROCESS_STATES 128u
typedef struct { aeon_process_identity_state_t states[AEON_MAX_PROCESS_STATES]; uint32_t policy_generation; } aeon_process_gate_t;
int aeon_process_gate_init(aeon_process_gate_t *);
int aeon_process_gate_submit(aeon_process_gate_t *, const aeon_did_registry_t *, const aeon_ioctl_submit_t *, aeon_io_decision_t *);
int aeon_process_gate_query(const aeon_process_gate_t *, uint32_t, aeon_ioctl_query_t *);
#endif
