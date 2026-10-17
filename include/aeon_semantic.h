#ifndef AEON_SEMANTIC_H
#define AEON_SEMANTIC_H
#include <stdint.h>
#include <stddef.h>
#include "aeon_identity.h"
typedef struct {
    char subject[AEON_SEMANTIC_FIELD_MAX];
    char predicate[AEON_SEMANTIC_FIELD_MAX];
    char object[AEON_SEMANTIC_FIELD_MAX];
} aeon_semantic_triple_t;
typedef struct {
    aeon_semantic_triple_t triple;
    uint8_t digest[32];
    aeon_did_auth_payload_t auth;
    uint64_t friction_nonce;
    uint8_t friction_verified;
    char ipfs_cid[AEON_IPFS_CID_MAX];
} aeon_semantic_transmission_t;
int aeon_semantic_canonicalize(const aeon_semantic_triple_t *, uint8_t *, size_t, size_t *);
int aeon_semantic_hash(const aeon_semantic_triple_t *, uint8_t digest[32]);
int aeon_semantic_verify_transmission(const aeon_did_registry_t *, const aeon_semantic_transmission_t *);
#endif
