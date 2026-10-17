#ifndef AEON_FRICTION_H
#define AEON_FRICTION_H

#include <stdint.h>
#include <stddef.h>
#include "aeon_semantic.h"

#define AEON_FRICTION_CONTEXT_BYTES 32

typedef struct {
    uint64_t challenge;
    uint8_t context_hash[AEON_FRICTION_CONTEXT_BYTES];
    uint8_t issued;
    uint8_t satisfied;
} aeon_friction_challenge_t;

typedef struct {
    uint64_t challenge;
    uint8_t context_hash[AEON_FRICTION_CONTEXT_BYTES];
    uint8_t verifier_signature[64];
    uint8_t verifier_public_key[32];
    uint8_t valid;
} aeon_human_attestation_t;

int aeon_friction_issue(aeon_friction_challenge_t *, uint64_t,
                        const uint8_t context_hash[32]);
int aeon_friction_verify(const aeon_friction_challenge_t *,
                         const aeon_human_attestation_t *);
int aeon_friction_bind_transmission(aeon_semantic_transmission_t *,
                                     const aeon_friction_challenge_t *,
                                     const aeon_human_attestation_t *);

#endif
