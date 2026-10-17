#ifndef AEON_KERNEL_DEVICE_H
#define AEON_KERNEL_DEVICE_H
#include <stdint.h>
#include "aeon_device.h"
#include "aeon_process_gate.h"
typedef struct { aeon_process_gate_t process_gate; uint32_t registered; uint32_t major; uint32_t minor; } aeon_identity_device_t;
typedef struct { uint32_t command; uint32_t size; void *user_buffer; } aeon_ioctl_request_t;
#define AEON_DEVICE_IOCTL_SUBMIT AEON_IDENTITY_IOCTL_SUBMIT
#define AEON_DEVICE_IOCTL_QUERY AEON_IDENTITY_IOCTL_QUERY
int aeon_identity_device_register(aeon_identity_device_t *);
int aeon_identity_device_unregister(aeon_identity_device_t *);
int aeon_identity_device_ioctl(aeon_identity_device_t *, const aeon_did_registry_t *, const aeon_ioctl_request_t *, void *result);
#endif
