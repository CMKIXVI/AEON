#include "aeon_semantic_gate.h"
#include <string.h>
int aeon_semantic_gate_check(const aeon_semantic_frame_t *f,
                             const aeon_did_registry_t *registry,
                             aeon_semantic_gate_reason_t *reason)
{
    aeon_semantic_transmission_t tx;
    uint8_t digest[32];

    if (!f || !registry || !reason) return -1;
    *reason = AEON_SEMANTIC_DENY_MALFORMED;

    if (aeon_semantic_frame_extract(f, &tx) != 0) return 0;

    if (aeon_semantic_hash(&tx.triple, digest) != 0) {
        *reason = AEON_SEMANTIC_DENY_DIGEST;
        return 0;
    }

    if (memcmp(digest, f->header.digest, sizeof(digest)) != 0) {
        *reason = AEON_SEMANTIC_DENY_DIGEST;
        return 0;
    }

    if (aeon_friction_bind_transmission(&tx, &f->friction,
                                        &f->attestation) != 0) {
        *reason = AEON_SEMANTIC_DENY_IDENTITY;
        return 0;
    }

    if (aeon_semantic_verify_transmission(registry, &tx) != 0) {
        *reason = AEON_SEMANTIC_DENY_IDENTITY;
        return 0;
    }

    *reason = AEON_SEMANTIC_ALLOW;
    return 0;
}
