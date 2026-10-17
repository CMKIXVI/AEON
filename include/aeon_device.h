#ifndef AEON_DEVICE_H
#define AEON_DEVICE_H
#include <stddef.h>
#include <stdint.h>
#include "aeon_io_gate.h"
#define AEON_IDENTITY_DEVICE "/dev/aeon_identity"
#define AEON_IDENTITY_IOCTL_SUBMIT 0xAE010001u
#define AEON_IDENTITY_IOCTL_QUERY 0xAE010002u
typedef struct { uint32_t abi_version; uint32_t process_id; uint32_t flags; uint32_t io_class; aeon_semantic_transmission_t transmission; } aeon_ioctl_submit_t;
typedef struct { uint32_t abi_version; uint32_t process_id; uint32_t authenticated; uint32_t policy_generation; } aeon_ioctl_query_t;
typedef struct { uint32_t process_id; uint32_t authenticated; uint64_t accepted; uint64_t denied; } aeon_process_identity_state_t;
#endif
