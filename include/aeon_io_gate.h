#ifndef AEON_IO_GATE_H
#define AEON_IO_GATE_H
#include <stdint.h>
#include "aeon_syscall.h"
typedef enum { AEON_IO_IPC = 1, AEON_IO_NETWORK_TX = 2, AEON_IO_NETWORK_RX = 3 } aeon_io_class_t;
typedef enum { AEON_IO_DENY = 0, AEON_IO_ALLOW = 1 } aeon_io_decision_t;
typedef struct {
    aeon_io_class_t io_class;
    uint32_t process_id;
    uint32_t flags;
    const aeon_submit_request_t *request;
} aeon_io_gate_request_t;
int aeon_io_gate_check(const aeon_did_registry_t *, const aeon_io_gate_request_t *, aeon_io_decision_t *);
#endif
