#ifndef AEON_UEFI_NETWORK_H
#define AEON_UEFI_NETWORK_H

#include <stddef.h>
#include <stdint.h>

typedef struct aeon_uefi_nic_context {
    void *pci_io;
    uintptr_t bar0;
    uintptr_t bar1;
    uintptr_t mmio_base;
    uint16_t vendor_id;
    uint16_t device_id;
    unsigned initialized;
} aeon_uefi_nic_context_t;

int aeon_uefi_network_probe(aeon_uefi_nic_context_t *ctx);
int aeon_uefi_network_tls_post(aeon_uefi_nic_context_t *ctx,
                               const char *host,
                               const char *path,
                               const char *payload,
                               char *response,
                               size_t response_capacity);

#endif
