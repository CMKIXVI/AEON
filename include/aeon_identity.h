#ifndef AEON_IDENTITY_H
#define AEON_IDENTITY_H
#include <stdint.h>
#include <stddef.h>

#define AEON_DID_MAX 256
#define AEON_IPFS_CID_MAX 128
#define AEON_VC_HASH_BYTES 32
#define AEON_SEMANTIC_FIELD_MAX 256

typedef struct {
    char did[AEON_DID_MAX];
    uint8_t public_key[32];
    uint8_t public_key_len;
    uint8_t human_verified;
    uint8_t active;
} aeon_did_record_t;

typedef struct {
    aeon_did_record_t *records;
    size_t count;
    size_t capacity;
} aeon_did_registry_t;

typedef struct {
    uint8_t public_key[32];
    uint8_t signature[64];
    uint8_t vc_hash[AEON_VC_HASH_BYTES];
    char did[AEON_DID_MAX];
    uint8_t human_verified;
} aeon_did_auth_payload_t;

int aeon_did_registry_init(aeon_did_registry_t *, aeon_did_record_t *, size_t);
int aeon_did_registry_add(aeon_did_registry_t *, const aeon_did_record_t *);
const aeon_did_record_t *aeon_did_registry_find(
    const aeon_did_registry_t *, const char *);
int aeon_did_auth_matches_registry(
    const aeon_did_registry_t *, const aeon_did_auth_payload_t *);

#endif
