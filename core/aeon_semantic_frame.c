#include "aeon_semantic_frame.h"
#include <string.h>
#include <stdio.h>
static int valid_field(const char *s, uint32_t len)
{
    if (!s || len == 0 || len >= AEON_SEMANTIC_FIELD_MAX) return 0;
    return s[len] == '\0';
}
int aeon_semantic_frame_validate(const aeon_semantic_frame_t *f)
{
    uint32_t expected;
    if (!f || f->header.magic != AEON_SEMANTIC_FRAME_MAGIC ||
        f->header.version != AEON_SEMANTIC_FRAME_VERSION) return -1;
    if (!valid_field(f->subject, f->header.subject_len) ||
        !valid_field(f->predicate, f->header.predicate_len) ||
        !valid_field(f->object, f->header.object_len)) return -2;
    expected = (uint32_t)sizeof(f->header) +
               f->header.subject_len + 1u +
               f->header.predicate_len + 1u +
               f->header.object_len + 1u +
               (uint32_t)sizeof(f->auth) +
               (uint32_t)sizeof(f->friction) +
               (uint32_t)sizeof(f->attestation) +
               (uint32_t)sizeof(f->ipfs_cid);
    if (f->header.total_size != expected) return -3;
    return 0;
}
int aeon_semantic_frame_extract(const aeon_semantic_frame_t *f,
                                aeon_semantic_transmission_t *tx)
{
    if (!tx || aeon_semantic_frame_validate(f) != 0) return -1;
    memset(tx, 0, sizeof(*tx));
    (void)snprintf(tx->triple.subject, sizeof(tx->triple.subject), "%s", f->subject);
    (void)snprintf(tx->triple.predicate, sizeof(tx->triple.predicate), "%s", f->predicate);
    (void)snprintf(tx->triple.object, sizeof(tx->triple.object), "%s", f->object);
    memcpy(tx->digest, f->header.digest, sizeof(tx->digest));
    memcpy(&tx->auth, &f->auth, sizeof(tx->auth));
    memcpy(&tx->friction_nonce, &f->friction.challenge, sizeof(tx->friction_nonce));
    memcpy(tx->ipfs_cid, f->ipfs_cid, sizeof(tx->ipfs_cid));
    return 0;
}
