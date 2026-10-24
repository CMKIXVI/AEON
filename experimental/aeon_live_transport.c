/*
 * AEON live network transport integration boundary.
 *
 * This file records the supplied PCIe NIC + lwIP + BearSSL design, isolated
 * from the host-testable core until the external UEFI/lwIP/BearSSL dependency
 * boundary is implemented and hardware-tested.
 */

#if defined(AEON_UEFI_LIVE_NETWORK)
#include <lwip/netif.h>
#include <lwip/tcp.h>
#include <bearssl.h>

#include <string.h>

#include "aeon_uefi_network.h"

static aeon_uefi_nic_context_t g_nic;

int aeon_live_network_init(void)
{
    return aeon_uefi_network_probe(&g_nic);
}

int aeon_live_network_tls_post(const char *host,
                               const char *path,
                               const char *payload,
                               char *response,
                               size_t response_capacity)
{
    /* The actual NIC driver, TCP transport, certificate store, SNI,
       BearSSL engine BIO and HTTP parser must be supplied by the platform
       backend before this symbol is enabled. */
    return aeon_uefi_network_tls_post(&g_nic, host, path, payload,
                                      response, response_capacity);
}
#endif
