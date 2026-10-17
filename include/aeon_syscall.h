#ifndef AEON_SYSCALL_H
#define AEON_SYSCALL_H
#include <stdint.h>
#include "aeon_semantic.h"
#define AEON_IDENTITY_ABI_VERSION 1u
#define AEON_IOC_SUBMIT 0xAE010001u
typedef struct { uint32_t abi_version; uint32_t flags; aeon_semantic_transmission_t transmission; } aeon_submit_request_t;
enum { AEON_SUBMIT_REQUIRE_HUMAN = 1u << 0, AEON_SUBMIT_REQUIRE_DID = 1u << 1, AEON_SUBMIT_REQUIRE_VC = 1u << 2, AEON_SUBMIT_REQUIRE_IPFS = 1u << 3 };
int aeon_submit_semantic(const aeon_did_registry_t *, const aeon_submit_request_t *);
#endif
