#ifndef AEON_EBPF_BOUNDARY_H
#define AEON_EBPF_BOUNDARY_H
#include <stdint.h>
#include "aeon_device.h"
typedef struct { uint32_t process_id; uint32_t io_class; uint32_t payload_len; uint8_t semantic_candidate; } aeon_ebpf_event_t;
typedef struct { uint32_t allow; uint32_t reason; } aeon_ebpf_decision_t;
enum { AEON_EBPF_REASON_NONE=0, AEON_EBPF_REASON_NOT_SEMANTIC=1, AEON_EBPF_REASON_IDENTITY_GATE=2, AEON_EBPF_REASON_POLICY=3 };
int aeon_ebpf_classify(const aeon_ebpf_event_t *, aeon_ebpf_decision_t *);
#endif
